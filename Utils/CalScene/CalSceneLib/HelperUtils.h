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

#include <NVLib/FileUtils.h>
#include <NVLib/PoseUtils.h>
#include <NVLib/Path/PathHelper.h>

#include "Arguments.h"

namespace NVL_App
{
	class HelperUtils
	{
	public:
		static vector<Point3d> GetScenePoints(Arguments * arguments);
		static Mat GetCameraMatrix(Arguments * arguments);
		static void WriteMeta(const string& path, Arguments * arguments, Mat& pose_1, Mat& pose_2);
		static void CreateFolders(const string& databasePath, const string& folder);
		static void WritePoints(const string& paths, int precision, const vector<Point3d>& scenePoints, vector<Point2d>& imagePoints_1, vector<Point2d>& imagePoints_2);
	};
}