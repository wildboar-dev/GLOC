//--------------------------------------------------
// A closed-form solver designed to extract the camera matrix from homographies
//
// @author: Wild Boar
//
// @date: 2025-04-17
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class ClosedSolver
	{
	private:
		vector<Mat> _homographies;
	public:
		ClosedSolver();
		void AddHomography(Mat & H);
		Mat FindK();
	private:
		void AddRowT1(Mat& V, Mat& H);
		void AddRowT2(Mat& V, Mat& H);
		void FindB(Mat& V);
		Mat ExtractK(Mat& B);
	};
}
