//--------------------------------------------------
// Defines a set of utilities for helping the distortion estimation process
//
// @author: Wild Boar
//
// @date: 2025-09-09
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Points.h"

namespace NVL_App
{
	class HelperUtils
	{
	public:
		static unique_ptr<Points> Undistort(Mat& cameraMatrix, const Vec4d& distCoeffs, Points * points);
		static Mat RenderKSpace(Mat& cameraMatrix, const Points& inputPoints, const Size& imageSize, const Range& RangeK1, const Range& RangeK2);
	};
}