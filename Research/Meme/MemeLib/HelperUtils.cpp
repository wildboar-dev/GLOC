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
	throw runtime_error("Not implemented");
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
Mat HelperUtils::RenderKSpace(Mat& cameraMatrix, const Points& inputPoints, const Size& imageSize, const Range& RangeK1, const Range& RangeK2)
{
	throw runtime_error("Not implemented");
}