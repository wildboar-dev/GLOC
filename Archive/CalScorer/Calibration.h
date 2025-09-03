//--------------------------------------------------
// The list of arguments that serve as input for the application
//
// @author: Wild Boar
//
// @date: 2024-03-06
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class Calibration
	{
	private:
		Mat _camera;
		Mat _pose_1;
		Mat _pose_2;
	public:
		Calibration(Mat camera, Mat pose_1, Mat pose_2) : _camera(camera), _pose_1(pose_1), _pose_2(pose_2) {}

		inline Mat & GetCamera() { return _camera; }
		inline Mat & GetPose_1() { return _pose_1; }
		inline Mat & GetPose_2() { return _pose_2; }
	};
}