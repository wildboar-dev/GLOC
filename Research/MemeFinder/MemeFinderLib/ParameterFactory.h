//--------------------------------------------------
// A factory for generating parameters for evaluation
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#pragma once

#include <iostream>
#include <unordered_set>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include <NVLib/RandomUtils.h>
#include "Parameters.h"

namespace NVL_App
{
	class ParameterFactory
	{
	private:
		Point2d _center;
	public:
		ParameterFactory(const Point2d& center);
		unique_ptr<Parameters> Generate();
	private:
		int SelectIndex(vector<int> ignore);
		pair<double, double> GetValueRange(int index);
		double GetRandomValue(pair<double, double>& range);
	};
}
