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
#include <NVLib/Math2D.h>
#include <NVLib/Math3D.h>
#include <NVLib/PoseUtils.h>
#include <NVLib/FileUtils.h>

#include <RealFinderLib/Logger.h>
#include <RealFinderLib/MetaLoader.h>
#include <RealFinderLib/PointLoader.h>

#include <OptLib/Common/CostBase.h>

#include "LoadUtils.h"

//--------------------------------------------------
// Defines a Cost Class
//--------------------------------------------------



//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVL_App::Logger& logger);
unique_ptr<NVLib::PathHelper> CreatePathHelper();
Mat FindPose(Mat& camera, Mat& H);
Mat FindPose(Mat& camera, Mat& distortion, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);
Vec2d FindRepoError(Mat& camera, Mat& pose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);
Vec2d FindHError(Mat& H, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);
unique_ptr<NVL_App::Points> GetIdealPoints(NVL_App::Points * points, Mat& H_1, Mat& H_2);
void WriteTsaiPoints(const string& filePath, NVL_App::Points * points, Mat& relativePose);
Mat RefinePose(Mat& camera, Mat& initialPose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints);

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
    auto points = NVL_App::PointLoader::Load(pathHelper->GetPath("Distortion","points.txt"));

    /////////////////////////////////////////////////

    logger << NVL_App::Logger::Color(36) << "Find the homography for the first board" << NVL_App::Logger::Save();
    auto H_1 = findHomography(points->GetScenePoints(), points->GetImagePoints_1());
    auto error_1 = FindHError(H_1, points->GetScenePoints(), points->GetImagePoints_1());
    logger << NVL_App::Logger::Color(36) << "H found with error: [" << error_1[0] << "," << error_1[1] << "]" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(36) << "Finding the pose" << NVL_App::Logger::Save();
    auto pose_1 = FindPose(meta->GetCameraMatrix(), H_1);
    cout << "Pose: " << pose_1 << endl;

    logger << NVL_App::Logger::Color(36) << "Finding the reprojection error" << NVL_App::Logger::Save();
    auto repo_error_1 = FindRepoError(meta->GetCameraMatrix(), pose_1, points->GetScenePoints(), points->GetImagePoints_1());
    logger << NVL_App::Logger::Color(36) << "Reprojection error: " << repo_error_1[0] << " +/- " << repo_error_1[1] << NVL_App::Logger::Save();

    /////////////////////////////////////////////////

    logger << NVL_App::Logger::Color(36) << "Find the homography for the second board" << NVL_App::Logger::Save();
    auto H_2 = findHomography(points->GetScenePoints(), points->GetImagePoints_2());
    auto error_2 = FindHError(H_2, points->GetScenePoints(), points->GetImagePoints_2());
    logger << NVL_App::Logger::Color(36) << "H found with error: [" << error_2[0] << "," << error_2[1] << "]" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(36) << "Finding the pose" << NVL_App::Logger::Save();
    auto pose_2 = FindPose(meta->GetCameraMatrix(), H_2);
    cout << "Pose: " << pose_2 << endl;

    logger << NVL_App::Logger::Color(36) << "Finding the reprojection error" << NVL_App::Logger::Save();
    auto repo_error_2 = FindRepoError(meta->GetCameraMatrix(), pose_2, points->GetScenePoints(), points->GetImagePoints_2());
    logger << NVL_App::Logger::Color(36) << "Reprojection error: " << repo_error_2[0] << " +/- " << repo_error_2[1] << NVL_App::Logger::Save();

    /////////////////////////////////////////////////

    logger << NVL_App::Logger::Color(36) << "Saving the result to disk" << NVL_App::Logger::Save();
    auto writer = FileStorage(pathHelper->GetPath("Distortion","pose.xml"), FileStorage::WRITE | FileStorage::FORMAT_XML);
    writer << "Pose_1" << pose_1;
    writer << "Pose_2" << pose_2;
    writer << "H_1" << H_1;
    writer << "H_2" << H_2;
    writer.release();

    /////////////////////////////////////////////////

    logger << NVL_App::Logger::Color(32) << "Finding Relative pose" << NVL_App::Logger::Save();
    auto relative = (Mat)(pose_1.inv() * pose_2);
    cout << "Relative Pose: " << relative << endl;
    auto idealPoints = GetIdealPoints(points.get(), H_1, H_2);
    WriteTsaiPoints(pathHelper->GetPath("Distortion","tsai_points.txt"), idealPoints.get(), relative);

    //-----------------------------------> Test
    logger << NVL_App::Logger::Color(36) << "Confirm the reformatted scene points" << NVL_App::Logger::Save();

    auto s = vector<Point3d>(); auto p = vector<Point2d>();

    for (auto i = 0; i < idealPoints->GetScenePoints().size(); i++) 
    {
        p.push_back(idealPoints->GetImagePoints_2()[i]);

        auto scenePoint = (Mat)(relative * (Mat_<double>(4,1) << points->GetScenePoints()[i].x, points->GetScenePoints()[i].y, points->GetScenePoints()[i].z, 1));
        auto slink = (double *)scenePoint.data;
        s.push_back(Point3d(slink[0] / slink[3], slink[1] / slink[3], slink[2] / slink[3]));
    }

    auto repo_error_3 = FindRepoError(meta->GetCameraMatrix(), pose_1, s, p);
    logger << NVL_App::Logger::Color(36) << "Reprojection error: " << repo_error_3[0] << " +/- " << repo_error_3[1] << NVL_App::Logger::Save();
}

//--------------------------------------------------
// Calculation Helpers
//--------------------------------------------------

/**
 * Find the pose of the calibration board
 * @param camera The camera matrix
 * @param H The homograph that we are getting the pose from
 */
Mat FindPose(Mat& camera, Mat& H) 
{
    Mat M = camera.inv() * H;

    auto mlink = (double *) M.data;

    auto v_1 = Vec3d(mlink[0], mlink[3], mlink[6]);
    auto v_2 = Vec3d(mlink[1], mlink[4], mlink[7]);
    auto v_3 = v_2.cross(v_1);
    auto t = Vec3d(mlink[2], mlink[5], mlink[8]);
    auto mag = sqrt(v_1[0] * v_1[0] + v_1[1] * v_1[1] + v_1[2] * v_1[2]); 

    Mat P = Mat_<double>::eye(4,4); auto plink = (double *)P.data;
    plink[0] = v_1[0]; plink[1] = v_2[0]; plink[2] = v_3[0]; plink[3] = t[0];
    plink[4] = v_1[1]; plink[5] = v_2[1]; plink[6] = v_3[1]; plink[7] = t[1];
    plink[8] = v_1[2]; plink[9] = v_2[2]; plink[10] = v_3[2]; plink[11] = t[2];

    P *= (1.0 / mag); plink[15] = 1.0;

    return P;
}

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
 * @param pose The pose matrix
 * @param scenePoints The associated 3D scene Points
 * @param imagePoints The related image points
 */
Vec2d FindRepoError(Mat& camera, Mat& pose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints) 
{
    // Reduce the pose matrix
    Mat M = Mat_<double>(3,4); auto mlink = (double *)M.data; auto plink = (double *)pose.data;
    for (auto row = 0; row < 3; row++) for (auto col = 0; col < 4; col++) mlink[row * 4 + col] = plink[row * 4 + col];

    // Add the logic to project points        
    auto actual = vector<Point2d>(); 
    for (auto i = 0; i < scenePoints.size(); i++) 
    {
        Mat point_3d = (Mat_<double>(4,1) << scenePoints[i].x, scenePoints[i].y, scenePoints[i].z, 1);
        Mat upoint_3d = M * point_3d;
        Mat ppoint = camera * upoint_3d; auto rlink = (double *)ppoint.data;
        actual.push_back(Point2d(rlink[0] / rlink[2], rlink[1] / rlink[2]));
    }
 
    // Create a vector to hold errors
    auto errors = vector<double>();

    // Iterate through all points
    for (auto i = 0; i < scenePoints.size(); i++) 
    {
        auto error = norm(actual[i] - imagePoints[i]);
        errors.push_back(error);
    }

    // Compute the mean and standard deviation of the errors
    auto mean = Scalar(), stddev = Scalar(); meanStdDev(errors, mean, stddev);

    // Return the error
    return Vec2d(mean[0], stddev[0]);
}

/**
 * Find the homography elements
 * @param H The H homography
 * @param scenePoints The scene points that we are dealing with
 * @param imagePoints The image that we are dealing with
 * @return The image that we are dealing with
 */
Vec2d FindHError(Mat& H, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints) 
{
    // Create a value for holding errors
    auto errors = vector<double>();

    // Process points
    for (auto i = 0; i < scenePoints.size(); i++) 
    {
        auto point_1 = NVLib::Math2D::Transform(H, Point2d(scenePoints[i].x, scenePoints[i].y));
        auto error = norm(point_1 - imagePoints[i]);
        errors.push_back(error);
    }

    // Compute the mean and standard deviation of the errors
    auto mean = Scalar(), stddev = Scalar(); meanStdDev(errors, mean, stddev);

    // Return the error
    return Vec2d(mean[0], stddev[0]);
}

//--------------------------------------------------
// Write Tsai Points
//--------------------------------------------------

/**
 * Get the ideal points for the Tsai calibration
 * @param points The points that we are using
 * @param H_1 The homography for board 1
 * @param H_2 The homography for board 2
 * @return The ideal points
 */
unique_ptr<NVL_App::Points> GetIdealPoints(NVL_App::Points * points, Mat& H_1, Mat& H_2) 
{
    // Create the vectors to hold the ideal points
    auto idealImagePoints_1 = vector<Point2d>();
    auto idealImagePoints_2 = vector<Point2d>();

    // Process each point
    for (auto i = 0; i < points->PointCount(); i++) 
    {
        idealImagePoints_1.push_back(NVLib::Math2D::Transform(H_1, Point2d(points->GetScenePoints()[i].x, points->GetScenePoints()[i].y)));
        idealImagePoints_2.push_back(NVLib::Math2D::Transform(H_2, Point2d(points->GetScenePoints()[i].x, points->GetScenePoints()[i].y)));
    }

    // Return the new points
    return make_unique<NVL_App::Points>(points->GetScenePoints(), idealImagePoints_1, idealImagePoints_2);
}

/**
 * Write the Tsai points to disk
 * @param filePath The file path that we are writing to
 * @param points The points that we are writing
 * @param relativePose The relative pose between the two boards
 */
void WriteTsaiPoints(const string& filePath, NVL_App::Points * points, Mat& relativePose) 
{
    // Open the file for writing
    ofstream writer(filePath);
    if (!writer.is_open()) throw runtime_error("Failed to open file for writing: " + filePath);

    // Write the points for the first board
    for (auto i = 0; i < points->PointCount(); i++) 
    {
        writer << points->GetScenePoints()[i].x << " " << points->GetScenePoints()[i].y << " " << points->GetScenePoints()[i].z << " "
               << points->GetImagePoints_1()[i].x << " " << points->GetImagePoints_1()[i].y << endl;
    }

    // Write the points for the second board
    for (auto i = 0; i < points->PointCount(); i++) 
    {
        auto scenePoint = (Mat)(relativePose * (Mat_<double>(4,1) << points->GetScenePoints()[i].x, points->GetScenePoints()[i].y, points->GetScenePoints()[i].z, 1));

        writer << scenePoint.at<double>(0) << " " << scenePoint.at<double>(1) << " " << scenePoint.at<double>(2) << " "
               << points->GetImagePoints_2()[i].x << " " << points->GetImagePoints_2()[i].y << endl;
    }

    // Close the file
    writer.close();
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
// Pose Refinement Logic
//--------------------------------------------------

/**
 * Refine the pose using bundle adjustment
 * @param camera The camera matrix
 * @param initialPose The initial pose that we are refining
 * @param scenePoints The associated 3D scene Points
 * @param imagePoints The related image points
 */
Mat RefinePose(Mat& camera, Mat& initialPose, vector<Point3d>& scenePoints, vector<Point2d>& imagePoints)
{
    throw runtime_error("Not implemented");
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