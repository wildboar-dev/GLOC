//--------------------------------------------------
// The definition of a problem state entity
//
// @author: Wild Boar
//
// @date: 2025-06-18
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class ProblemState
	{
	private:
		Mat _camera;
		Mat _distortion;
		Mat _pose;
	public:
		ProblemState(Mat& camera, Mat& distortion, Mat& pose) :
			_camera(camera), _distortion(distortion), _pose(pose) {}

		inline Mat& GetCamera() { return _camera; }
		inline Mat& GetDistortion() { return _distortion; }
		inline Mat& GetPose() { return _pose; }
	};
}