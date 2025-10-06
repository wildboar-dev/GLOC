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
	auto fx = cameraMatrix.at<double>(0, 0);
	auto fy = cameraMatrix.at<double>(1, 1);
	auto cx = cameraMatrix.at<double>(0, 2);
	auto cy = cameraMatrix.at<double>(1, 2);

	auto points_1 = vector<Point2d>(); auto points_2 = vector<Point2d>();

	auto& ipoints_1 = points->GetImagePoints_1();
	auto& ipoints_2 = points->GetImagePoints_2();

	// cout << "Using distortion: " << distCoeffs << endl;

	auto term = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 500, DBL_EPSILON);
	undistortPoints(ipoints_1, points_1, cameraMatrix, distCoeffs, Mat(), cameraMatrix, term);
	undistortPoints(ipoints_2, points_2, cameraMatrix, distCoeffs, Mat(), cameraMatrix, term);

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
 * @param indices The output indices of the minimum error found
 * @param RangeK1 The range of the K1
 * @param RangeK2 The range of the K2
 * @return Mat Returns a Mat
 */
Mat HelperUtils::RenderKSpace(Mat& cameraMatrix, Points * inputPoints, const Size& imageSize, vector<int>& indices,  const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2)
{
	Mat image = Mat_<float>::zeros(imageSize);
	auto link = (float *)image.data;

	auto stepK1 = (RangeK1.GetMax() - RangeK1.GetMin()) / (float)imageSize.width;
	auto stepK2 = (RangeK2.GetMax() - RangeK2.GetMin()) / (float)imageSize.height;

	for (auto row = 0; row < image.rows; row++) 
	{
		for (auto column = 0; column < image.cols; column++) 
		{
			double k1 = RangeK1.GetMin() + column * stepK1;
			double k2 = RangeK2.GetMin() + row * stepK2;

			auto distortion = Vec4d();
			distortion[indices[0]] = k1;	
			distortion[indices[1]] = k2;

			auto undistortedPoints = HelperUtils::Undistort(cameraMatrix, distortion, inputPoints);
			auto errors = vector<double>();
			auto score = CostFunction::CalculateError(undistortedPoints.get(), errors);
			auto scoref = min(9555.0f, (float)score);

			link[row * image.cols + column] = scoref;
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
Mat HelperUtils::RenderPath(const Size& imageSize, const vector<Point2d>& points, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2)
{
	Mat image = Mat::zeros(imageSize, CV_8UC3);

	auto stepK1 = (float)imageSize.width / (RangeK1.GetMax() - RangeK1.GetMin());
	auto stepK2 = (float)imageSize.height / (RangeK2.GetMax() - RangeK2.GetMin());

	for (size_t i = 1; i < points.size() - 1; i++)
	{
		auto k_1_0 = (points[i - 1].x - RangeK1.GetMin()) * stepK1;
		auto k_2_0 = (points[i - 1].y - RangeK2.GetMin()) * stepK2;
		auto k_1_1 = (points[i].x - RangeK1.GetMin()) * stepK1;
		auto k_2_1 = (points[i].y - RangeK2.GetMin()) * stepK2;

		line(image, Point2d(k_1_0, k_2_0), Point2d(k_1_1, k_2_1), Scalar(0, 255, 0), 2);
	}

	return image;
}