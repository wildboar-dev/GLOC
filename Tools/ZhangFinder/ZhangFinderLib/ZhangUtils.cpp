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