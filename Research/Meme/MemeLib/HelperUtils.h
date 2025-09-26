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
#include "CostFunction.h"

namespace NVL_App
{
	class HelperUtils
	{
	public:
		static unique_ptr<Points> Undistort(Mat& cameraMatrix, const Vec4d& distCoeffs, Points * points);
		static Mat RenderKSpace(Mat& cameraMatrix, Points * inputPoints, const Size& imageSize, vector<int>& indices, const Range& RangeK1 = Range(-1, 1), const Range& RangeK2 = Range(-1, 1));	
		static Mat RenderPath(const Size& imageSize, const vector<Point2d>& points, const Range& RangeK1 = Range(-1, 1), const Range& RangeK2 = Range(-1, 1));
	};
}