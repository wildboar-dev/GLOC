//--------------------------------------------------
// Implementation of class HelperUtils
//
// @author: Wild Boar
//
// @date: 2025-09-09
//--------------------------------------------------

#include "HelperUtils.h"
using namespace NVL_App;

//--------------------------------------------------
// Undistort
//--------------------------------------------------

/**
 * @brief Undistort the associated pointset that has been provided
 * @param cameraMatrix The camera matrix to use for undistortion
 * @param distCoeffs The distortion coefficients to use for undistortion
 * @param points The input points to be undistorted
 * @return unique_ptr<Points> Returns a unique_ptr<Points>
 */
unique_ptr<Points> HelperUtils::Undistort(Mat& cameraMatrix, const Vec4d& distCoeffs, Points * points)
{
	auto points_1 = vector<Point2d>(); auto points_2 = vector<Point2d>();

	auto& ipoints_1 = points->GetImagePoints_1();
	auto& ipoints_2 = points->GetImagePoints_2();

	undistortPoints(ipoints_1, points_1, cameraMatrix, distCoeffs);
	undistortPoints(ipoints_2, points_2, cameraMatrix, distCoeffs);

	return make_unique<Points>(points->GetScenePoints(), points_1, points_2);
}

//--------------------------------------------------
// Render
//--------------------------------------------------

/**
 * @brief Render the error space for the given points
 * @param cameraMatrix The camera matrix to use for rendering
 * @param inputPoints The input points to be rendered
 * @param imageSize The size of the output image
 * @param RangeK1 The range of the K1
 * @param RangeK2 The range of the K2
 * @return Mat Returns a Mat
 */
Mat HelperUtils::RenderKSpace(Mat& cameraMatrix, Points * inputPoints, const Size& imageSize, const Range& RangeK1, const Range& RangeK2)
{
	Mat image = Mat_<float>::zeros(imageSize);
	auto link = (float *)image.data;

	auto stepK1 = (RangeK1.end - RangeK1.start) / (float)imageSize.width;
	auto stepK2 = (RangeK2.end - RangeK2.start) / (float)imageSize.height;

	for (auto row = 0; row < image.rows; row++) 
	{
		for (auto column = 0; column < image.cols; column++) 
		{
			auto k1 = RangeK1.start + column * stepK1;
			auto k2 = RangeK2.start + row * stepK2;

			auto undistortedPoints = HelperUtils::Undistort(cameraMatrix, Vec4d(k1, k2, 0.0, 0.0), inputPoints);
			auto errors = vector<double>();
			auto score = CostFunction::CalculateError(undistortedPoints.get(), errors);

			link[row * image.cols + column] = (float)score;
		}
	}

	return image;
}

/**
 * @brief Render the path of the given points
 * @param imageSize The size of the output image
 * @param points The points to be rendered
 * @param RangeK1 The range of the K1
 * @param RangeK2 The range of the K2
 * @return Mat Returns a Mat
 */
Mat HelperUtils::RenderPath(const Size& imageSize, const vector<Point2d>& points, const Range& RangeK1, const Range& RangeK2)
{
	Mat image = Mat::zeros(imageSize, CV_8UC3);

	auto stepK1 = (float)imageSize.width / (RangeK1.end - RangeK1.start);
	auto stepK2 = (float)imageSize.height / (RangeK2.end - RangeK2.start);

	for (size_t i = 1; i < points.size() - 1; i++)
	{
		auto k_1_0 = (points[i - 1].x - RangeK1.start) * stepK1;
		auto k_2_0 = (points[i - 1].y - RangeK2.start) * stepK2;
		auto k_1_1 = (points[i].x - RangeK1.start) * stepK1;
		auto k_2_1 = (points[i].y - RangeK2.start) * stepK2;

		line(image, Point2d(k_1_0, k_2_0), Point2d(k_1_1, k_2_1), Scalar(0, 255, 0), 2);
	}

	return image;
}