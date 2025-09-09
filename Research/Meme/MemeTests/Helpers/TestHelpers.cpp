//--------------------------------------------------
// Implementation of class TestHelpers
//
// @author: Wild Boar
//
// @date: 2025-09-09
//--------------------------------------------------

#include "TestHelpers.h"

//--------------------------------------------------
// Construct a grid of points
//--------------------------------------------------

/**
 * @brief Build a grid of points in the XY plane
 * @param rows The number of rows
 * @param cols The number of columns
 * @param grid The output grid of points
 */
void TestHelpers::BuildGrid(int rows, int cols, vector<Point3d>& grid) 
{
    for (auto r = 0; r < rows; ++r) 
    {
        for (auto c = 0; c < cols; ++c) 
        {
            grid.push_back(Point3d(c, r, 0.0));
        }
    }
}

//--------------------------------------------------
// Construct a camera matrix
//--------------------------------------------------

/**
 * @brief Build a camera matrix
 * @param fx The focal length in x
 * @param fy The focal length in y
 * @param center The principal point
 * @return Mat Returns the camera matrix
 */
Mat TestHelpers::BuildCameraMatrix(double fx, double fy, const Point2d& center) 
{
    Mat cameraMatrix = Mat::zeros(3, 3, CV_64F);
    cameraMatrix.at<double>(0, 0) = fx;
    cameraMatrix.at<double>(1, 1) = fy;
    cameraMatrix.at<double>(0, 2) = center.x;
    cameraMatrix.at<double>(1, 2) = center.y;
    cameraMatrix.at<double>(2, 2) = 1.0;
    return cameraMatrix;
}

Mat TestHelpers::BuildTranslation(double x, double y, double z) 
{
    Mat translation = Mat::zeros(4, 4, CV_64F);
    translation.at<double>(0, 3) = x;
    translation.at<double>(1, 3) = y;
    translation.at<double>(2, 3) = z;
    translation.at<double>(3, 3) = 1.0;
    return translation;
}

//--------------------------------------------------
// Construct a full 3D to 2D point set
//--------------------------------------------------

/**
 * @brief Build a set of 2D points from a set of 3D points
 * @param scenePoints The input 3D points
 * @param cameraMatrix The camera matrix
 * @return unique_ptr<NVL_App::Points> Returns the 2D points
 */
unique_ptr<NVL_App::Points> TestHelpers::BuildTestPoints(const vector<Point3d>& scenePoints, const Mat& cameraMatrix) 
{
    auto rvec = Vec3d(0.0, 0.0, 0.0);
    auto tvec_1 = Vec3d(-50.0, 0.0, 200);
    auto tvec_2 = Vec3d(50.0, 0.0, 200);

    auto points_1 = vector<Point2d>();
    auto points_2 = vector<Point2d>();

    cv::projectPoints(scenePoints, rvec, tvec_1, cameraMatrix, noArray(), points_1);
    cv::projectPoints(scenePoints, rvec, tvec_2, cameraMatrix, noArray(), points_2);

    return make_unique<NVL_App::Points>(scenePoints, points_1, points_2);
}

//--------------------------------------------------
// Create a neww point set with distortion applied
//--------------------------------------------------

/**
 * @brief Apply distortion to a set of points
 * @param cameraMatrix The camera matrix
 * @param distortion The distortion coefficients
 * @param originalPoints The original points
 * @return unique_ptr<NVL_App::Points> Returns the distorted points
 */
unique_ptr<NVL_App::Points> TestHelpers::ApplyDistortion(const Mat& cameraMatrix, const Vec4d& distortion, NVL_App::Points* originalPoints) 
{
    auto rvec = Vec3d(0.0, 0.0, 0.0);
    auto tvec_1 = Vec3d(-50.0, 0.0, 200);
    auto tvec_2 = Vec3d(50.0, 0.0, 200);

    auto points_1 = vector<Point2d>();
    auto points_2 = vector<Point2d>();

    cv::projectPoints(originalPoints->GetScenePoints(), rvec, tvec_1, cameraMatrix, distortion, points_1);
    cv::projectPoints(originalPoints->GetScenePoints(), rvec, tvec_2, cameraMatrix, distortion, points_2);

    return make_unique<NVL_App::Points>(originalPoints->GetScenePoints(), points_1, points_2);
}
