//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2024-03-05
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/FileUtils.h>
#include <NVLib/Path/PathHelper.h>
#include <NVLib/Parameters/Parameters.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "ArgReader.h"
#include "Arguments.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void ProcessEntry(NVLib::PathHelper * pathHelper, NVLib::Logger * logger, const string fileName);
void Run(NVLib::Parameters * parameters);
unique_ptr<NVL_App::Arguments> LoadArguments(NVLib::PathHelper * pathHelper, const string& fileName);
void FillScenePoints(NVL_App::Arguments * arguments, vector<Point3d>& points);
void FillImagePoints(NVL_App::Arguments * arguments, const vector<Point3d>& scenePoints, const Vec3d& rvec, const Vec3d& tvec, vector<Point2d>& points);
Mat BuildCameraMat(NVL_App::Arguments * arguments);
Mat MakeImage(NVL_App::Arguments * arguments, const vector<Point2d>& points);
void SavePoints(const string& path, const vector<Point3d>& scenePoints, const vector<Point2d>& imagePoints_1, const vector<Point2d>& imagePoints_2);
string GetImagePath(NVLib::PathHelper * pathHelper, const string& fileName, int index);
string GetPointsPath(NVLib::PathHelper * pathHelper, const string& fileName);

//--------------------------------------------------
// Execution Logic
//--------------------------------------------------

/**
 * Main entry point into the application
 * @param parameters The input parameters
 */
void Run(NVLib::Parameters * parameters) 
{
    if (parameters == nullptr) return; auto logger = NVLib::Logger(1);

    logger.StartApplication();

    logger.Log(1, "Create a path helper");
    auto database = NVL_Utils::ArgReader::ReadString(parameters, "database");
    auto dataset = NVL_Utils::ArgReader::ReadString(parameters, "dataset");
    auto pathHelper = NVLib::PathHelper(database, dataset);

    logger.Log(1, "Get all the XML meta files in the given folder");
    auto metaFolder = pathHelper.GetPath("Meta");
    auto files = vector<string>(); NVLib::FileUtils::GetFileList(metaFolder, files);

    logger.Log(1, "Process each of the files");
    for (auto file : files) 
    {
        auto fileName = NVLib::FileUtils::GetFileName(file);
        auto extension = NVLib::FileUtils::GetExtension(fileName);
        if (extension != "xml") continue;

        logger.Log(1, "Processing: %s", fileName.c_str());
        ProcessEntry(&pathHelper, &logger, fileName);
    }

    logger.StopApplication();
}

//--------------------------------------------------
// Data processing logic
//--------------------------------------------------

/**
 * Add the logic to process an associated entry
 * @param pathHelper The path helper that we are using
 * @param logger The logger that we are using
 * @param fileName The name of the file that we are processing
 */
void ProcessEntry(NVLib::PathHelper * pathHelper, NVLib::Logger * logger, const string fileName) 
{
    logger->Log(1, "Load the associated arguments from disk");
    auto arguments = LoadArguments(pathHelper, fileName);

    logger->Log(1, "Generate the associated 3D points");
    auto scenePoints = vector<Point3d>(); FillScenePoints(arguments.get(), scenePoints);
    
    logger->Log(1, "Generate the image 2D points");
    auto imagePoints_1 = vector<Point2d>(); FillImagePoints(arguments.get(), scenePoints, arguments->GetRvec_1(), arguments->GetTvec_1(),  imagePoints_1);
    auto imagePoints_2 = vector<Point2d>(); FillImagePoints(arguments.get(), scenePoints, arguments->GetRvec_2(), arguments->GetTvec_2(),  imagePoints_2);

    logger->Log(1, "Save the image");
    Mat image_1 = MakeImage(arguments.get(), imagePoints_1);
    Mat image_2 = MakeImage(arguments.get(), imagePoints_2);
    auto imagePath_1 = GetImagePath(pathHelper, fileName, 1);
    auto imagePath_2 = GetImagePath(pathHelper, fileName, 2);
    imwrite(imagePath_1, image_1); imwrite(imagePath_2, image_2);

    logger->Log(1, "Save the associated points");
    auto pointsPath = GetPointsPath(pathHelper, fileName);
    SavePoints(pointsPath, scenePoints, imagePoints_1, imagePoints_2);
}

//--------------------------------------------------
// Helper Methods
//--------------------------------------------------

/**
 * Retrieve the arguments associated with the application
 * @param pathHelper The helper for getting the path information
 * @param fileName The name of the file that we are processing
 * @return The arguments that we are getting for the application
*/
unique_ptr<NVL_App::Arguments> LoadArguments(NVLib::PathHelper* pathHelper, const string& fileName) 
{
    auto path = pathHelper->GetPath("Meta", fileName);

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

    return unique_ptr<NVL_App::Arguments>(new NVL_App::Arguments(focal, imageSize, rvec_1, tvec_1, rvec_2, tvec_2, blockSize, gridSize));
}

/**
 * Add logic to generate scene points and put them into a points vector
 * @param arguments The arguments provided for the application
 * @param points The points that we are adding to the system
 */
void FillScenePoints(NVL_App::Arguments * arguments, vector<Point3d>& points) 
{
    for (auto row = 0; row < arguments->GetGridSize(); row++) 
    {
        for (auto column = 0; column < arguments->GetGridSize(); column++) 
        {
            auto X = column * arguments->GetBlockSize();
            auto Y = row * arguments->GetBlockSize();
            auto Z = 0;
            points.push_back(Point3d(X, Y, Z));
        }
    }
}

/**
 * Add the logic to fill associated image points
 * @param arguments The arguments that we are populating
 * @param scenePoints The given scene points for the application
 * @param rvec The rotation vector associated with the application
 * @param tvec The translation vector associated with the application
 * @param points The resultant image points associated with the application
 */
void FillImagePoints(NVL_App::Arguments * arguments, const vector<Point3d> & scenePoints, const Vec3d& rvec, const Vec3d& tvec, vector<Point2d> & points) 
{
    Mat camera = BuildCameraMat(arguments);
    Mat distortion = Mat_<double>::zeros(4,1);
    projectPoints(scenePoints, rvec, tvec, camera, distortion, points);
}

/**
 * Assemble the camera matrix
 * @param arguments The arguments that we are dealing with
*/
Mat BuildCameraMat(NVL_App::Arguments * arguments) 
{
    auto fx = arguments->GetFocal(); 
    auto fy = arguments->GetFocal();
    auto cx = arguments->GetImageSize().width * 0.5;
    auto cy = arguments->GetImageSize().height * 0.5;

    return (Mat_<double>(3,3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
}

/**
 * Generate an image that we are dealing with
 * @param arguments The arguments that we are dealing with
 * @param points The points that we are generating the images from
 * @return The image that it was made
*/
Mat MakeImage(NVL_App::Arguments * arguments, const vector<Point2d>& points) 
{
    Mat image = Mat_<Vec3b>::zeros(arguments->GetImageSize());

    for (auto& point : points) 
    {
        circle(image, point, 3, Scalar(0, 0, 255), FILLED);
    }

    return image;
}

/**
 * Save the point files to disk
 * @param path The folder that we are saving to
 * @param scenePoints The scene points that are being saved
 * @param imagePoints_1 The image points that are being saved
 * @param imagePoints_2 The image points that are being saved
 */
void SavePoints(const string& path, const vector<Point3d>& scenePoints, const vector<Point2d>& imagePoints_1, const vector<Point2d>& imagePoints_2) 
{
    auto writer = ofstream(path); if (!writer.is_open()) throw runtime_error("Unable to create file: " + path);
    if (scenePoints.size() != imagePoints_1.size() || scenePoints.size() != imagePoints_2.size()) throw runtime_error("There is a size mismatch between the scene points and image points");

    writer << scenePoints.size() << endl;

    for (auto point : scenePoints) writer << point.x << "," << point.y << "," << point.z << endl; 
    writer << endl;
    for (auto point : imagePoints_1) writer << point.x << "," << point.y << endl; 
    writer << endl;
    for (auto point : imagePoints_2) writer << point.x << "," << point.y << endl; 

    writer.close();
}

/**
 * Get the image path for the given file name
 * @param pathHelper The path helper that we are using
 * @param fileName The name of the file that we are processing
 * @param index The index of the image that we are processing
 * @return The path to the image
 */
string GetImagePath(NVLib::PathHelper * pathHelper, const string& fileName, int index) 
{
    auto name_no_ext = NVLib::FileUtils::GetNameWithoutExtension(fileName);
    auto imagePath = pathHelper->GetPath("Image", name_no_ext + "_" + to_string(index) + ".png");
    return imagePath;
}

/**
 * Get the points path for the given file name
 * @param pathHelper The path helper that we are using
 * @param fileName The name of the file that we are processing
 * @return The path to the points
 */
string GetPointsPath(NVLib::PathHelper * pathHelper, const string& fileName) 
{
    auto name_no_ext = NVLib::FileUtils::GetNameWithoutExtension(fileName);
    auto pointsPath = pathHelper->GetPath("Point", name_no_ext + ".txt");
    return pointsPath;
}

//--------------------------------------------------
// Entry Point
//--------------------------------------------------

/**
 * Main Method
 * @param argc The count of the incoming arguments
 * @param argv The number of incoming arguments
 * @return SUCCESS and FAILURE
 */
int main(int argc, char ** argv) 
{
    NVLib::Parameters * parameters = nullptr;

    try
    {
        parameters = NVL_Utils::ArgReader::GetParameters(argc, argv);
        Run(parameters);
    }
    catch (runtime_error exception)
    {
        cerr << "Error: " << exception.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        cerr << "Error: " << exception << endl;
        exit(EXIT_FAILURE);
    }

    if (parameters != nullptr) delete parameters;

    return EXIT_SUCCESS;
}