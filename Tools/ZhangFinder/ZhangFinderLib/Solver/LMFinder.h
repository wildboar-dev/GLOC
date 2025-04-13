//--------------------------------------------------
// A Levenberg Marquardt solver
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

#include "cminpack.h"
#include "ProblemBase.h"

namespace NVL_AI
{
	class LMFinder
	{
	private:
		ProblemBase * _problem;
	public:
		LMFinder(ProblemBase * problem);
		~LMFinder();

		double Solve(Mat& parameters);
		double Evaluate(Mat& parameters, Mat& errors);
	private:
		static void Callback(int dataCount, int paramCount, double * params, double * errors, int * errorNumber, void * callback);
	};
}
