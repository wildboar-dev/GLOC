//--------------------------------------------------
// Implementation of class ZhangUtils
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#include "ZhangUtils.h"
using namespace NVL_App;

//--------------------------------------------------
// Homography
//--------------------------------------------------

/**
 * @brief Determine the associated homography
 * @param points The points that are are finding the homography from
 * @param pointSetIndex The index of the point set to use
 * @return Mat Returns a Mat
 */
Mat ZhangUtils::FindHomography(Points * points, int pointSetIndex)
{
	auto sourcePoints = vector<Point2d>(points->GetScenePoints().size()); 
	for (auto i = 0; i < points->GetScenePoints().size(); i++)
	{
		sourcePoints[i].x  = points->GetScenePoints()[i].x;
		sourcePoints[i].y = points->GetScenePoints()[i].y;
	}

	auto & targetPoints = (pointSetIndex == 0) ? points->GetImagePoints_1() : points->GetImagePoints_2();

	return findHomography(sourcePoints, targetPoints);
}

/**
 * @brief Get the average fit score associated with the homography
 * @param H The homography that we are testing
 * @param points The points that are are finding the homography from
 * @param pointSetIndex The index of the point set to use
 * @return double Returns a double
 */
double ZhangUtils::TestHomography(Mat& H, Points * points, int pointSetIndex)
{
	auto sourcePoints = vector<Point2d>(points->GetScenePoints().size()); 
	for (auto i = 0; i < points->GetScenePoints().size(); i++)
	{
		sourcePoints[i].x  = points->GetScenePoints()[i].x;
		sourcePoints[i].y = points->GetScenePoints()[i].y;
	}

	auto & targetPoints = (pointSetIndex == 0) ? points->GetImagePoints_1() : points->GetImagePoints_2();

	auto error = vector<double>();

	for (auto i = 0; i < sourcePoints.size(); i++)
	{
		auto estimated = NVLib::Math2D::Transform(H, sourcePoints[i]);
		auto diff = targetPoints[i] - estimated;
		error.push_back(sqrt(diff.x * diff.x + diff.y * diff.y));
	}

	auto mean = Scalar(); meanStdDev(error, mean, noArray());

	return mean[0];
}

//--------------------------------------------------
// Homography
//--------------------------------------------------

/**
 * @brief Get the camera matrix from the parameters
 * @param parameters The input parameters
 * @return Mat Returns a Mat
 */
Mat ZhangUtils::GetCameraMatrix(Mat& parameters) 
{
	auto fx = ((double *) parameters.data)[0];
	auto fy = ((double *) parameters.data)[1];
	auto cx = ((double *) parameters.data)[2];
	auto cy = ((double *) parameters.data)[3];

	Mat cameraMatrix = (Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
	return cameraMatrix;
}

/**
 * @brief Get the pose of the camera
 * @param H The homography matrix
 * @param K The camera matrix
 * @return Mat Returns a Mat
 */
Mat ZhangUtils::GetPose(Mat& H, Mat& K) 
{
	Mat K_inv = K.inv();
	Mat M_1 = K_inv * H;

	Vec3d r1 = M_1.col(0);
	Vec3d r2 = M_1.col(1);
	Vec3d t = M_1.col(2);
	Vec3d r3 = r1.cross(r2);

	auto factor = NVLib::Math3D::GetMagnitude(r1);

	r1 = r1 / factor;
	r2 = r2 / factor;
	t = t / factor;
	r3 = r3 / factor;
	
	Mat pose = (Mat_<double>(4, 4) << r1[0], r1[1], r1[2], t[0],
		r2[0], r2[1], r2[2], t[1],
		r3[0], r3[1], r3[2], t[2],
		0, 0, 0, 1);

	return pose;
}