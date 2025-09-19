//--------------------------------------------------
// Utilities for loading and processing pose data
//
// @author: Wild Boar
//
// @date: 2025-09-19
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Pose.h"
using namespace NVLib;

namespace NVL_App
{
	class PoseLoader
	{
	public:
		static unique_ptr<Pose> LoadPose(const string& path);
	};
}
