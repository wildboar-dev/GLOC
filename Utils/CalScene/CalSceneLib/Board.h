//--------------------------------------------------
// Contains all the details associated with a board in this rendering
//
// @author: Wild Boar
//
// @date: 2025-04-16
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/PoseUtils.h>

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class Board
	{
	private:
		Mat _camera;
		Mat _pose;
		vector<Point3d> _scenePoints;
		vector<Point2d> _imagePoints;

	public:
		Board(Mat & camera, Mat & pose, vector<Point3d> & scenePoints);

		void Render(Mat& image);
		void Save(ostream& writer);

		inline Mat& GetCamera() { return _camera; }
		inline Mat& GetPose() { return _pose; }
		inline vector<Point3d>& GetScenePoints() { return _scenePoints; }
		inline vector<Point2d>& GetImagePoints() { return _imagePoints; }
	};
}