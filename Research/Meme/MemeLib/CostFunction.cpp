//--------------------------------------------------
// Implementation of class CostFunction
//
// @author: Wild Boar
//
// @date: 2025-09-09
//--------------------------------------------------

#include "CostFunction.h"
using namespace NVL_App;

//--------------------------------------------------
// Calculate
//--------------------------------------------------

/**
 * @brief Calculates the error between the scene points and the image points
 * @param points The points to calculate the error for
 * @param errors The individual errors
 * @return double Returns a double
 */
double CostFunction::CalculateError(Points * points, vector<double>& errors)
{
	Mat H_1 = findHomography(points->GetScenePoints(), points->GetImagePoints_1());
	Mat H_2 = findHomography(points->GetScenePoints(), points->GetImagePoints_2());

	auto errors_1 = vector<double>();
	auto errors_2 = vector<double>();

	auto score_1 = HomographyError(points->GetScenePoints(), points->GetImagePoints_1(), H_1, errors_1);
	auto score_2 = HomographyError(points->GetScenePoints(), points->GetImagePoints_2(), H_2, errors_2);

	errors.insert(errors.end(), errors_1.begin(), errors_1.end());
	errors.insert(errors.end(), errors_2.begin(), errors_2.end());

	return score_1 + score_2;
}

//--------------------------------------------------
// Helpers
//--------------------------------------------------

/**
 * @brief Calculate the homography error
 * @param scenePoints The scene points
 * @param image The image points
 * @param H The homography matrix
 * @param errors The individual errors
 * @return double Returns the total error
 */
double CostFunction::HomographyError(const vector<Point3d>& scenePoints, const vector<Point2d>& image, Mat& H, vector<double>& errors)
{
	auto totalError = 0.0;
	errors.clear();
	for (auto i = 0; i < (int)scenePoints.size(); ++i)
	{
		auto sp = Point2d(scenePoints[i].x, scenePoints[i].y);
		auto ip = image[i];

		auto projected = NVLib::Math2D::Transform(H, sp);
		auto error = norm(projected - ip);

		errors.push_back(error);
		totalError += error;
	}
	return totalError;
}	