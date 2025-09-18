//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2025-09-15
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Path/PathHelper.h>
#include <NVLib/Parameters/Parameters.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "ArgReader.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVLib::Parameters * parameters);
void GenScenePoints(int rows, int cols, int blockSize, vector<Point3d>& output);
void ProjectPoints(const vector<Point3d>& scenePoints, const Vec2d & scale, Mat& cameraMatrix, const Vec3d& euler, const Vec3d& translation, vector<Point2d>& imagePoints);
Mat Euler2Rotation(const Vec3d& euler);
Mat RenderPoints(const vector<Point2d>& imagePoints, int width, int height);
void SaveImage(NVLib::PathHelper& pathHelper , const string& name, const Mat& image);
void SavePoints(NVLib::PathHelper& pathHelper, const vector<Point3d>& scenePoints, const vector<Point2d>& imagePoints1, const vector<Point2d>& imagePoints2);
void SaveMeta(NVLib::PathHelper& pathHelper, const Mat& cameraMatrix, const Size& imageSize, const Size& gridSize, int blockSize);
void SavePose(NVLib::PathHelper& pathHelper, const string& name, const Vec3d& euler, const Vec3d& translation);

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

    logger.Log(1, "Creating scene parameters");
    auto database = NVL_Utils::ArgReader::ReadString(parameters, "database");
    auto dataset = NVL_Utils::ArgReader::ReadString(parameters, "dataset");
    auto pathHelper = NVLib::PathHelper(database, dataset);

    logger.Log(1, "Generating scene points");
    auto gridSize = Size(6, 7); auto blockSize = 50;
    auto scenePoints = vector<Point3d>(); GenScenePoints(gridSize.height, gridSize.width, blockSize, scenePoints);

    logger.Log(1, "Creating the image size");
    auto imageSize = Size(640 * 2, 480 * 2);

    logger.Log(1, "Creating a camera matrix");
    auto focal = max(imageSize.width, imageSize.height) * 1.9;
    Mat cameraMatrix = (Mat_<double>(3, 3) << focal, 0, imageSize.width / 2, 0, focal, imageSize.height / 2, 0, 0, 1);

    logger.Log(1, "Create the first calibration plane image");
    auto wOffset = gridSize.width * (blockSize - 1);
    auto euler1 = Vec3d(0, -45, 0); auto translation1 = Vec3d(-50, -150, 1000);
    auto imagePoints1 = vector<Point2d>(); ProjectPoints(scenePoints, Vec2d(-1, 1), cameraMatrix, euler1, translation1, imagePoints1);

    logger.Log(1, "Create the second calibration plane image");
    auto euler2 = Vec3d(0, 45, 0); auto translation2 = Vec3d(0, -150, 1000);
    auto imagePoints2 = vector<Point2d>(); ProjectPoints(scenePoints, Vec2d(1, 1), cameraMatrix, euler2, translation2, imagePoints2);

    logger.Log(1, "Rendering the first image");
    auto combined = vector<Point2d>(imagePoints1); combined.insert(combined.end(), imagePoints2.begin(), imagePoints2.end());
    Mat sceneImage = RenderPoints(combined, imageSize.width, imageSize.height);

    logger.Log(1, "Save image");
    SaveImage(pathHelper, "scene.png", sceneImage);

    logger.Log(1, "Save Points");
    SavePoints(pathHelper, scenePoints, imagePoints1, imagePoints2);

    logger.Log(1, "Meta data");
    SaveMeta(pathHelper, cameraMatrix, imageSize, gridSize, blockSize);

    logger.Log(1, "Save Pose 1");
    SavePose(pathHelper, "pose1", euler1, translation1);

    logger.Log(1, "Save Pose 2");
    SavePose(pathHelper, "pose2", euler2, translation2);

    logger.StopApplication();
}

//--------------------------------------------------
// Generate Scene Points
//--------------------------------------------------

/**
 * Generate a grid of points in the scene
 * @param rows The number of rows of points
 * @param cols The number of columns of points
 * @param blockSize The size of each block in the grid
 * @param output The output vector of points
 */
void GenScenePoints(int rows, int cols, int blockSize, vector<Point3d>& output) 
{
    output.clear();
    for (int r = 0; r < rows; r++) 
    {
        for (int c = 0; c < cols; c++) 
        {
            output.push_back(Point3d(c * blockSize, r * blockSize, 0));
        }
    }
}

//--------------------------------------------------
// Project Points
//--------------------------------------------------

/**
 * Project the 3D points into the image plane
 * @param scenePoints The input 3D scene points
 * @param cameraMatrix The camera matrix
 * @param euler The euler angles for rotation
 * @param translation The translation vector
 * @param imagePoints The resultant image points
 */
void ProjectPoints(const vector<Point3d>& scenePoints, const Vec2d & scale, Mat& cameraMatrix, const Vec3d& euler, const Vec3d& translation, vector<Point2d>& imagePoints) 
{
    imagePoints.clear();
    Mat R = Euler2Rotation(euler);
    Mat t = (Mat_<double>(3, 1) << translation[0], translation[1], translation[2]);

    for (const auto& point : scenePoints) 
    {
        Mat pt = (Mat_<double>(3, 1) << point.x * scale[0], point.y * scale[1], point.z);
        Mat pt_cam = R * pt + t;

        double x = pt_cam.at<double>(0, 0) / pt_cam.at<double>(2, 0);
        double y = pt_cam.at<double>(1, 0) / pt_cam.at<double>(2, 0);

        double u = cameraMatrix.at<double>(0, 0) * x + cameraMatrix.at<double>(0, 2);
        double v = cameraMatrix.at<double>(1, 1) * y + cameraMatrix.at<double>(1, 2);

        imagePoints.push_back(Point2d(u, v));
    }
}

/**
 * Convert euler angles to a rotation matrix
 * @param euler The input euler angles
 */
Mat Euler2Rotation(const Vec3d& euler) 
{
    Mat R;
    // Convert euler angles (in degrees) to radians
    double rx = euler[0] * CV_PI / 180.0;
    double ry = euler[1] * CV_PI / 180.0;
    double rz = euler[2] * CV_PI / 180.0;

    // Compute rotation matrices around each axis
    Mat Rx = (Mat_<double>(3, 3) << 1, 0, 0,
                                    0, cos(rx), -sin(rx),
                                    0, sin(rx), cos(rx));
    Mat Ry = (Mat_<double>(3, 3) << cos(ry), 0, sin(ry),
                                    0, 1, 0,
                                    -sin(ry), 0, cos(ry));
    Mat Rz = (Mat_<double>(3, 3) << cos(rz), -sin(rz), 0,
                                    sin(rz), cos(rz), 0,
                                    0, 0, 1);

    // Combine rotation matrices
    R = Rz * Ry * Rx;
    return R;
}

//--------------------------------------------------
// Render Points and Save Image
//--------------------------------------------------

/**
 * Render the projected points onto an image
 * @param imagePoints The input image points
 * @param width The width of the output image
 * @param height The height of the output image
 * @return The rendered image
 */
Mat RenderPoints(const vector<Point2d>& imagePoints, int width, int height) 
{
    Mat image = Mat::zeros(height, width, CV_8UC3);
    for (const auto& pt : imagePoints) 
    {
        if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) 
        {
            circle(image, pt, 5, Scalar(0, 255, 0), -1);
        }
    }
    return image;
}

/**
 * Save the image to disk
 * @param pathHelper The path helper for generating paths
 * @param name The name of the image file
 * @param image The image to save
 */
void SaveImage(NVLib::PathHelper& pathHelper , const string& name, const Mat& image) 
{
    auto imageFolder = pathHelper.GetPath("Image");
    if (!NVLib::FileUtils::Exists(imageFolder)) 
    {
        NVLib::FileUtils::AddFolder(imageFolder);
    }

    auto imagePath = pathHelper.GetPath("Image", name);

    imwrite(imagePath, image);
}

//--------------------------------------------------
// Save Point Logic
//--------------------------------------------------

/**
 * Save the points to disk
 * @param pathHelper The path helper for generating paths
 * @param scenePoints The 3D scene points
 * @param imagePoints1 The first set of image points
 * @param imagePoints2 The second set of image points
 */
void SavePoints(NVLib::PathHelper& pathHelper, const vector<Point3d>& scenePoints, const vector<Point2d>& imagePoints1, const vector<Point2d>& imagePoints2) 
{
    auto pointsFolder = pathHelper.GetPath("Point");
    if (!NVLib::FileUtils::Exists(pointsFolder)) 
    {
        NVLib::FileUtils::AddFolder(pointsFolder);
    }

    auto pointsPath = pathHelper.GetPath("Point", "point.txt");
    ofstream file(pointsPath);
    if (!file.is_open()) 
    {
        throw runtime_error("Failed to open points file for writing: " + pointsPath);
    }

    file << "X,Y,Z,u1,v1,u2,v2" << endl;
    for (size_t i = 0; i < scenePoints.size(); i++) 
    {
        file << fixed << setprecision(8) << scenePoints[i].x << "," << scenePoints[i].y << "," << scenePoints[i].z << ","
             << imagePoints1[i].x << "," << imagePoints1[i].y << ","
             << imagePoints2[i].x << "," << imagePoints2[i].y << endl;
    }

    file.close();
}

//--------------------------------------------------
// Save Meta Data
//--------------------------------------------------

/**
 * Save the meta data to disk
 * @param pathHelper The path helper for generating paths
 * @param cameraMatrix The camera matrix
 * @param imageSize The size of the images
 * @param gridSize The size of the calibration grid
 * @param blockSize The size of each block in the grid
 */
void SaveMeta(NVLib::PathHelper& pathHelper, const Mat& cameraMatrix, const Size& imageSize, const Size& gridSize, int blockSize) 
{
    auto metaFolder = pathHelper.GetPath("Meta");
    if (!NVLib::FileUtils::Exists(metaFolder)) 
    {
        NVLib::FileUtils::AddFolder(metaFolder);
    }

    auto metaPath = pathHelper.GetPath("Meta", "meta.xml");

    auto reader = FileStorage(metaPath, FileStorage::WRITE | FileStorage::FORMAT_XML);
    if (!reader.isOpened())  throw runtime_error("Failed to open meta file for writing: " + metaPath);
    
    reader << "cameraMatrix" << cameraMatrix;
    reader << "imageSize" << imageSize;
    reader << "gridSize" << gridSize;
    reader << "blockSize" << blockSize;

    reader.release();
}

/**
 * Save the pose data to disk
 * @param pathHelper The path helper for generating paths
 * @param name The name of the pose file
 * @param euler The euler angles
 * @param translation The translation vector
 */
void SavePose(NVLib::PathHelper& pathHelper, const string& name, const Vec3d& euler, const Vec3d& translation) 
{
    auto metaFolder = pathHelper.GetPath("Meta");
    if (!NVLib::FileUtils::Exists(metaFolder)) 
    {
        NVLib::FileUtils::AddFolder(metaFolder);
    }

    auto filename = name + ".xml";

    auto metaPath = pathHelper.GetPath("Meta", filename);

    auto writer = FileStorage(metaPath, FileStorage::WRITE | FileStorage::FORMAT_XML);
    if (!writer.isOpened())  throw runtime_error("Failed to open pose file for writing: " + metaPath);

    // Convert Euler to rvec
    Mat R = Euler2Rotation(euler);
    Mat rvec; Rodrigues(R, rvec);

    writer << "rvec" << rvec;
    writer << "tvec" << translation;

    writer.release();
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
