//--------------------------------------------------
// Attempts to get a cost value for a given set of points
//
// @author: Wild Boar
//
// @date: 2025-09-09
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/Math2D.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Points.h"

namespace NVL_App
{
	class CostFunction
	{
	public:
		static double CalculateError(Points * points, vector<double>& errors);
	private:
		static double HomographyError(const vector<Point3d>& scenePoints, const vector<Point2d>& image, Mat& H, vector<double>& errors);
	};
}
