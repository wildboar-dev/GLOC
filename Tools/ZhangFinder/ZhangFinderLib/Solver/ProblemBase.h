//--------------------------------------------------
// A base class for the definition of a refinement problem
//
// @author: Wild Boar
//
// @date: 2023-12-29
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_AI
{
	class ProblemBase
	{
	public:
		ProblemBase() {}
		virtual int GetDataSize() = 0;
		virtual double Evaluate(Mat& parameters, Mat& errors) = 0;
	};
}