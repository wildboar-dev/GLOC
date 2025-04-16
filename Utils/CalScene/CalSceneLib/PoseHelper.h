//--------------------------------------------------
// Generates a pose for the left and right boards, given the arguments
//
// @author: Wild Boar
//
// @date: 2025-04-16
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Arguments.h"

namespace NVL_App
{
	class PoseHelper
	{

		public:
			static Mat FindPose(Arguments * arguments, int boardIndex);

		private:
			static Mat GetRotX(double angle);
			static Mat GetRotY(double angle);
			static Mat GetRotZ(double angle);

			static Mat GetTranslation(Mat& rotation, Arguments * arguments);
	};
}
