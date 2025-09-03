//--------------------------------------------------
// Defines a basic engine for a vanilla C++ project.
//
// @author: Wild Boar
//
// @date: 2025-04-12
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Math2D.h>
#include <NVLib/PoseUtils.h>
#include <NVLib/Formatter.h>
#include <NVLib/Path/PathHelper.h>

#include "ArgUtils.h"
#include "PointLoader.h"
#include "Meta.h"
#include "Calibration.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
unique_ptr<NVL_App::Meta> LoadMeta(NVLib::PathHelper* pathHelper, const string& elementName);
unique_ptr<NVL_App::Calibration> LoadCalibration(NVLib::PathHelper* pathHelper, const string& elementName);
unique_ptr<NVL_App::Calibration> Meta2Calibration(NVL_App::Meta* meta); 
Vec2d GetScore(NVL_App::Calibration * calibration, NVL_App::Points * points);
void ProjectPoints(Mat& camera, Mat& pose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);
void FindDifferences(vector<Point2d>& expected, vector<Point2d>& actual, vector<double>& errors);
void SaveResult(NVLib::PathHelper * pathHelper, const string& saveName, const Vec2d& truth_error, const Vec2d& measured_error);

//--------------------------------------------------
// Main Execution Logic
//--------------------------------------------------

/**
 * Execute the main logic of the application
 * @param logger The logger instance
 * @param parameters The parameters for execution   
 */
void Execute(NVLib::Logger * logger, NVLib::Parameters * parameters)
{
    logger->Log(1, "Creating a path helper");
    auto database = NVL_App::ArgUtils::GetString(parameters, "database");
    auto dataset = NVL_App::ArgUtils::GetString(parameters, "dataset");
    auto pathHelper = NVLib::PathHelper(database, dataset);

    logger->Log(1, "Retrieving the element name...");
    auto elementName = NVL_App::ArgUtils::GetString(parameters, "element");
    logger->Log(1, "Element Name: %s", elementName.c_str());

    logger->Log(1, "Retrieving the save file name...");
    auto saveFileName = NVL_App::ArgUtils::GetString(parameters, "save");
    logger->Log(1, "Save File Name: %s", saveFileName.c_str());

    logger->Log(1, "Load test points...");
    auto pointFile = stringstream(); pointFile << elementName << ".txt";
    auto pointPath = pathHelper.GetPath("Point", pointFile.str());
    auto points = NVL_App::PointLoader::Load(pointPath);
    logger->Log(1, "Number of points Loaded: %i", points->GetScenePoints().size());

    logger->Log(1, "Load meta information...");
    auto meta = LoadMeta(&pathHelper, elementName);
    logger->Log(1, "Focal: %f", meta->GetFocal());

    logger->Log(1, "Converting the meta data into calibration");
    auto truth_calibration = Meta2Calibration(meta.get());

    logger->Log(1, "Load calibration information...");
    auto measured_calibration = LoadCalibration(&pathHelper, elementName);
    logger->Log(1, "Load Success");

    logger->Log(1, "Calculate the error with the truth calibration");
    auto truth_error = GetScore(truth_calibration.get(), points.get());
    logger->Log(1, "Error: %f +/- %f", truth_error[0], truth_error[1]);

    logger->Log(1, "Calculate the error with the measured calibration");
    auto measured_error = GetScore(measured_calibration.get(), points.get()); 
    logger->Log(1, "Error: %f +/- %f", measured_error[0], measured_error[1]);

    logger->Log(1, "Saving the result to the disk");
    SaveResult(&pathHelper, saveFileName, truth_error, measured_error);
    logger->Log(1, "Save Success");
}

//--------------------------------------------------
// Loader Helpers
//--------------------------------------------------

/**
 * @brief Load the meta information from the disk
 * @param pathHelper The path helper to use
 * @param elementName The name of the element
 * @return unique_ptr<Meta> Returns a unique_ptr<Meta>
 */
unique_ptr<NVL_App::Meta> LoadMeta(NVLib::PathHelper* pathHelper, const string& elementName) 
{
    auto fileName = stringstream(); fileName << elementName << ".xml";
    auto path = pathHelper->GetPath("Meta", fileName.str());

    auto reader = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::READ);
    if (!reader.isOpened()) throw runtime_error("Unable to open: " + path);

    double focal; reader["focal"] >> focal;
    Size imageSize; reader["imageSize"] >> imageSize;
    Vec3d rvec_1; reader["rvec_1"] >> rvec_1;
    Vec3d tvec_1; reader["tvec_1"] >> tvec_1;
    Vec3d rvec_2; reader["rvec_2"] >> rvec_2;
    Vec3d tvec_2; reader["tvec_2"] >> tvec_2;
    double blockSize; reader["blockSize"] >> blockSize;
    int gridSize; reader["gridSize"] >> gridSize;
  
    reader.release();

    return make_unique<NVL_App::Meta>(focal, imageSize, rvec_1, tvec_1, rvec_2, tvec_2, blockSize, gridSize);
} 

/**
 * @brief Load the calibration information from the disk
 * @param pathHelper The path helper to use
 * @param elementName The name of the element
 * @return unique_ptr<Calibration> Returns a unique_ptr<Calibration>
 */
unique_ptr<NVL_App::Calibration> LoadCalibration(NVLib::PathHelper* pathHelper, const string& elementName) 
{
    auto fileName = stringstream(); fileName << elementName << ".xml";
    auto path = pathHelper->GetPath("Calib_Output", fileName.str());

    auto reader = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::READ);
    if (!reader.isOpened()) throw runtime_error("Unable to open: " + path);

    Mat camera; reader["K"] >> camera;
    Mat pose_1; reader["M_1"] >> pose_1;
    Mat pose_2; reader["M_2"] >> pose_2;

    if (camera.empty()) throw runtime_error("Camera matrix is empty");
    if (pose_1.empty()) throw runtime_error("Pose 1 matrix is empty");
    if (pose_2.empty()) throw runtime_error("Pose 2 matrix is empty");

    reader.release();

    return make_unique<NVL_App::Calibration>(camera, pose_1, pose_2);
}

//--------------------------------------------------
// Conversion Helpers
//--------------------------------------------------

/**
 * @brief Convert Meta to Calibration
 * @param meta The meta information
 * @return unique_ptr<Calibration> Returns a unique_ptr<Calibration>
 */
unique_ptr<NVL_App::Calibration> Meta2Calibration(NVL_App::Meta* meta) 
{
    auto camera = Mat(3, 3, CV_64F);
    camera.at<double>(0, 0) = meta->GetFocal();
    camera.at<double>(1, 1) = meta->GetFocal();
    camera.at<double>(0, 2) = meta->GetImageSize().width / 2;
    camera.at<double>(1, 2) = meta->GetImageSize().height / 2;
    camera.at<double>(2, 2) = 1.0;

    auto pose_1 = Mat(3, 4, CV_64F); auto plink_1 = (double *) pose_1.data;
    Rodrigues(meta->GetRvec_1(), pose_1.colRange(0, 3));
    plink_1[3] = meta->GetTvec_1()[0];
    plink_1[7] = meta->GetTvec_1()[1];
    plink_1[11] = meta->GetTvec_1()[2];

    auto pose_2 = Mat(3, 4, CV_64F); auto plink_2 = (double *) pose_2.data;
    Rodrigues(meta->GetRvec_2(), pose_2.colRange(0, 3));
    plink_2[3] = meta->GetTvec_2()[0];
    plink_2[7] = meta->GetTvec_2()[1];
    plink_2[11] = meta->GetTvec_2()[2];

    return make_unique<NVL_App::Calibration>(camera, pose_1, pose_2);
}

//--------------------------------------------------
// Get the associated score
//--------------------------------------------------

/**
 * @brief Get the score of the calibration
 * @param calibration The calibration information
 * @param points The points to use
 * @return Vec2d Returns a Vec2d with the score
 */
Vec2d GetScore(NVL_App::Calibration * calibration, NVL_App::Points * points) 
{
    auto errors = vector<double>();
    
    auto estimated_1 = vector<Point2d>(); ProjectPoints(calibration->GetCamera(), calibration->GetPose_1(), points->GetScenePoints(), estimated_1);
    auto estimated_2 = vector<Point2d>(); ProjectPoints(calibration->GetCamera(), calibration->GetPose_2(), points->GetScenePoints(), estimated_2);
    
    FindDifferences(estimated_1, points->GetImagePoints_1(), errors);
    FindDifferences(estimated_2, points->GetImagePoints_2(), errors);

    auto mean = Scalar(), stddev = Scalar(); meanStdDev(errors, mean, stddev); 

    return Vec2d(mean[0], stddev[0]);
}

/**
 * Project the the associated points
 * @param camera The camera that we are dealing with
 * @param pose The pose that we are dealing with
 * @param scenePoints The scene points that we are dealing with
 * @param imagePoints The image points that we are dealing with
 */
void ProjectPoints(Mat& camera, Mat& pose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints) 
{
    auto rvec = Vec3d(), tvec = Vec3d(); NVLib::PoseUtils::Pose2Vectors(pose, rvec, tvec);
    projectPoints(scenePoints, rvec, tvec, camera, noArray(), imagePoints);
}

/**
 * Find the differences between elements
 * @param expected The expected set of points
 * @param actual The actual that we are dealing we are dealing with
 * @param errors The errors that we are dealing with
 */
void FindDifferences(vector<Point2d> & expected, vector<Point2d>& actual, vector<double>& errors) 
{
    assert(expected.size() == actual.size());

    for (auto i = 0; i < expected.size(); i++) 
    {
        auto difference = expected[i] - actual[i];
        auto error = NVLib::Math2D::GetMagnitude(difference);
        errors.push_back(error);
    }
}

//--------------------------------------------------
// Execution entry point
//--------------------------------------------------

/**
 * Save the result to the disk
 * @param pathHelper The path helper to use
 * @param elementName The name of the element
 * @param truth_error The truth error
 * @param measured_error The measured error
 */
void SaveResult(NVLib::PathHelper * pathHelper, const string& saveName, const Vec2d& truth_error, const Vec2d& measured_error) 
{
    auto path = pathHelper->GetPath("Calib_Output", saveName);

    auto writer = ofstream(path, ios::app);
    if (!writer.is_open()) throw runtime_error("Unable to open: " + path);

    writer << truth_error[0] << " " << truth_error[1] << " " << measured_error[0] << " " << measured_error[1] << endl;

    writer.close();
}

//--------------------------------------------------
// Execution entry point
//--------------------------------------------------

/**
 * Main Method
 * @param argc The count of the incomming arguments
 * @param argv The number of incomming arguments
 */
int main(int argc, char ** argv) 
{
    auto logger = NVLib::Logger(2);
    logger.StartApplication();

    try
    {
        auto parameters = NVL_App::ArgUtils::Load("CalScorer", argc, argv);
        Execute(&logger, parameters);
    }
    catch (runtime_error exception)
    {
        logger.Log(1, "Error: %s", exception.what());
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        logger.Log(1, "Error: %s", exception.c_str());
        exit(EXIT_FAILURE);
    }

    logger.StopApplication();

    return EXIT_SUCCESS;
}