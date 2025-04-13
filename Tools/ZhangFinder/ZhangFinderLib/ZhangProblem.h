//--------------------------------------------------
// Determine the correctness of camera matrix with respect to grid homographies
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Solver/ProblemBase.h"

namespace NVL_App
{
	class ZhangProblem : public NVL_AI::ProblemBase
	{
	private:
		Vec3d _h11;
		Vec3d _h12;
		Vec3d _h21;
		Vec3d _h22;
	public:
		ZhangProblem(Mat& H1, Mat& H2);

		virtual int GetDataSize();
		virtual double Evaluate(Mat& parameters, Mat& errors);

		Mat GetB(Mat& parameters);

		inline Vec3d& GetH11() { return _h11; }
		inline Vec3d& GetH12() { return _h12; }
		inline Vec3d& GetH21() { return _h21; }
		inline Vec3d& GetH22() { return _h22; }
	};
}
