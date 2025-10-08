//--------------------------------------------------
// Holds calibration parmeters
//
// @author: Wild Boar
//
// @date: 2025-10-08
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
		Mat _distortion;

	public:
		Calibration(Mat& camera, Mat& distortion) :
			_camera(camera), _distortion(distortion) {}

		inline Mat& GetCamera() { return _camera; }
		inline Mat& GetDistortion() { return _distortion; }
	};
}
