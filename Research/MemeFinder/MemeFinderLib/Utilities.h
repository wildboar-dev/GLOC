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
#include <MemeLib/HelperUtils.h>

#include "Pose.h"
#include "Parameters.h"

namespace NVL_App
{
	class Utilities
	{
	public:
		static unique_ptr<Points> ApplyDistortion(Mat& camera, Pose * pose1, Pose * pose2, Parameters * parameters, Points * points);
	};
}
