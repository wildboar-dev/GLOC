//--------------------------------------------------
// A set of utility methods for processing the data
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/Math2D.h>
#include <NVLib/Math3D.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Points.h"

namespace NVL_App
{
	class ZhangUtils
	{
	public:
		static Mat FindHomography(Points * points, int pointSetIndex);
		static double TestHomography(Mat& H, Points * points, int pointSetIndex);
		static Mat GetCameraMatrix(Mat& parameters);
		static Mat GetPose(Mat& H, Mat& K);
	};
}