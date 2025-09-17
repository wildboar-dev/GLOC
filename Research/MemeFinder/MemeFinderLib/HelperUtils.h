//--------------------------------------------------
// A set of utilities for helping the application
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include <MemeLib/Points.h>

#include "Parameters.h"

namespace NVL_App
{
	class HelperUtils
	{
	public:
		static unique_ptr<Points> ApplyDistortion(Mat& camera, Parameters * parameters, Points * points);
	};
}
