//--------------------------------------------------
// Generate problem states for the system to try and learn
//
// @author: Wild Boar
//
// @date: 2025-06-19
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/RandomUtils.h>
#include <NVLib/PoseUtils.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "ProblemState.h"
#include "Arguments.h"

namespace NVL_App
{
	class StateGenerator
	{
	public:
		static unique_ptr<ProblemState> Create(Arguments * arguments);
	private:
		static Mat GenerateCamera(Arguments * arguments);
		static Mat GenerateDistortion(Arguments * arguments);
		static Mat GeneratePose(Arguments * arguments);
	};
}