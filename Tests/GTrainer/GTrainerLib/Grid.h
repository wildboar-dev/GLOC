//--------------------------------------------------
// Generate a grid representing a virtual calibration object
//
// @author: Wild Boar
//
// @date: 2025-06-19
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/Math3D.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Arguments.h"
#include "ProblemState.h"

namespace NVL_App
{
	class Grid
	{
	private:
		vector<Point3d> _scenePoints;
		vector<Point2d> _imagePoints;
		bool _isValid;
	public:
		Grid(Arguments * arguments, ProblemState * problemState);

		bool IsValid();
		void Render(Mat& image);
		Mat GetErrors();

		inline vector<Point3d>& GetScenePoints() { return _scenePoints; }
		inline vector<Point2d>& GetImagePoints() { return _imagePoints; }
		inline bool GetIsValid() { return _isValid; }

	private:
		void MakeVanillaScenePoints(Arguments * arguments, vector<Point3d>& scenePoints);
		bool ValidatePoints(Arguments * arguments);
	};
}
