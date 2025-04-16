//--------------------------------------------------
// A set of utilities that support the rendering of our synthetic calibration measurements
//
// @author: Wild Boar
//
// @date: 2025-04-16
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Arguments.h"

namespace NVL_App
{
	class HelperUtils
	{
	public:
		static vector<Point3d> GetScenePoints(Arguments * arguments);
		static Mat GetCameraMatrix(Arguments * arguments);
		static void WriteMeta(const string& path, Arguments * arguments);
	};
}