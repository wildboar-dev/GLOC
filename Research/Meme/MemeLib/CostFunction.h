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

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Points.h"

namespace NVL_App
{
	class CostFunction
	{
	public:
		static double CalculateError(const Size& gridSize, Points * points, vector<double>& errors);
	};
}
