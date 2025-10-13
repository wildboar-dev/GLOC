//--------------------------------------------------
// Loader - Source.cpp
//
// @author: Wild Boar
//
// @date: 2025-10-08
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include <NVLib/Path/PathHelper.h>
#include <NVLib/PoseUtils.h>
#include <NVLib/FileUtils.h>

#include <RealFinderLib/Logger.h>
#include <RealFinderLib/MetaLoader.h>
#include <RealFinderLib/PointLoader.h>

#include "LoadUtils.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVL_App::Logger& logger);
unique_ptr<NVLib::PathHelper> CreatePathHelper();
Mat FindPose(Mat& camera, Mat& distortion, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);
Vec2d FindRepoError(Mat& camera, Mat& distortion, Mat& pose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);

//--------------------------------------------------
// Main entry point into the application
//--------------------------------------------------

/**
 * Main execution logic
 * @param logger The logger that we are using
 */
void Run(NVL_App::Logger& logger) 
{ 
    logger << NVL_App::Logger::Color(36) << "Creating a path helper" << NVL_App::Logger::Save();
    auto pathHelper = CreatePathHelper();
    
    logger << NVL_App::Logger::Color(36) << "Loading Meta" << NVL_App::Logger::Save();
    auto meta = NVL_App::MetaLoader::Load(pathHelper->GetPath("Meta","meta.xml"));

    logger << NVL_App::Logger::Color(36) << "Loading Distortion Parameters" << NVL_App::Logger::Save();
    auto dparams = NVL_App::LoadUtils::LoadDParams(pathHelper->GetPath("Distortion","result.xml"));

    logger << NVL_App::Logger::Color(36) << "Loading undistorted points" << NVL_App::Logger::Save();
    auto points = NVL_App::PointLoader::Load(pathHelper->GetPath("Points","points.txt"));

    logger << NVL_App::Logger::Color(36) << "Finding the pose of the first board" << NVL_App::Logger::Save();
    auto pose_1 = FindPose(meta->GetCameraMatrix(), dparams->GetDistortion(), points->GetScenePoints(), points->GetImagePoints_1());
    auto error_1 = FindRepoError(dparams->GetCamera(), dparams->GetDistortion(), pose_1, points->GetScenePoints(), points->GetImagePoints_1());
    logger << NVL_App::Logger::Color(36) << "Pose found with error: [" << error_1[0] << "," << error_1[1] << "]" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(36) << "Finding the pose of the second board" << NVL_App::Logger::Save();
    auto pose_2 = FindPose(meta->GetCameraMatrix(), dparams->GetDistortion(), points->GetScenePoints(), points->GetImagePoints_2());
    auto error_2 = FindRepoError(dparams->GetCamera(), dparams->GetDistortion(), pose_2, points->GetScenePoints(), points->GetImagePoints_2());
    logger << NVL_App::Logger::Color(36) << "Pose found with error: [" << error_2[0] << "," << error_2[1] << "]" << NVL_App::Logger::Save();     
}

//--------------------------------------------------
// Calculation Helpers
//--------------------------------------------------

/**
 * Find the pose of the calibration board
 * @param camera The camera matrix
 * @param distortion The distortion parameters
 * @param scenePoints The associated 3D scene Points
 * @param imagePoints The related image points
 */
Mat FindPose(Mat& camera, Mat& distortion, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints) 
{
    // Initialize the rotation and translation vectors
    auto rvec = Vec3d(), tvec = Vec3d();

    // Find the pose using solvePnP
    solvePnP(scenePoints, imagePoints, camera, distortion, rvec, tvec);

    // Return the result
    return NVLib::PoseUtils::Vectors2Pose(rvec, tvec);
}

/**
 * Find the reprojection error
 * @param camera The camera matrix
 * @param distortion The distortion parameters
 * @param pose The pose matrix
 * @param scenePoints The associated 3D scene Points
 * @param imagePoints The related image points
 */
Vec2d FindRepoError(Mat& camera, Mat& distortion, Mat& pose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints) 
{
    // Decompose the pose into rotation and translation
    auto rvec = Vec3d(), tvec = Vec3d(); NVLib::PoseUtils::Pose2Vectors(pose, rvec, tvec);

    // Add the logic to project points        
    auto actual = vector<Point2d>(); projectPoints(scenePoints, rvec, tvec, camera, distortion, actual);

    // Create a vector to hold errors
    auto errors = vector<double>();

    // Iterate through all points
    for (size_t i = 0; i < scenePoints.size(); i++) {

        // Compute the error
        errors.push_back(norm(actual[i] - imagePoints[i]));
    }

    // Compute the mean and standard deviation of the errors
    auto mean = Scalar(), stddev = Scalar(); meanStdDev(errors, mean, stddev);

    // Return the error
    return Vec2d(mean[0], stddev[0]);
}

//--------------------------------------------------
// Create a path helper
//--------------------------------------------------

/**
 * Create the path helper
 * @return The path helper
 */
unique_ptr<NVLib::PathHelper> CreatePathHelper() 
{
    // Load the configuration file
    FileStorage fs("config.xml", FileStorage::READ);
    if (!fs.isOpened()) throw runtime_error("Failed to open configuration file: config.xml");

    // Read the database and dataset
    string databasePath, datasetName;
    fs["database"] >> databasePath;
    fs["dataset"] >> datasetName;
    fs.release();

    // Create the path helper
    return make_unique<NVLib::PathHelper>(databasePath, datasetName);
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
    // Create the logger
    auto logger = NVL_App::Logger();

    // Indicate that the application is starting
    logger << NVL_App::Logger::Color(32) << "Starting Loader Application" << NVL_App::Logger::Save();
 
    // Main execution logic
    try 
    {
        Run(logger);
    }
    catch (const cv::Exception& ex) 
    {
        logger << NVL_App::Logger::Color(31) << "OpenCV Exception: " << ex.what() << NVL_App::Logger::Save();
    }
    catch (const std::exception& ex) 
    {
        logger << NVL_App::Logger::Color(31) << "Standard Exception: " << ex.what() << NVL_App::Logger::Save();
    }
    catch (...) 
    {
        logger << NVL_App::Logger::Color(31) << "Unknown Exception occurred" << NVL_App::Logger::Save();
    }

    // Indicate that the application is closing
    logger << NVL_App::Logger::Color(32) << "Closing Loader Application" << NVL_App::Logger::Save();

    return EXIT_SUCCESS;
}