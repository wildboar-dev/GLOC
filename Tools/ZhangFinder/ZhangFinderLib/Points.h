//--------------------------------------------------
// Holds the points that have been imported into the process
//
// @author: Wild Boar
//
// @date: 2025-04-12
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class Points
	{
	private:
		vector<Point3d> _scenePoints;
		vector<Point2d> _imagePoints_1;
		vector<Point2d> _imagePoints_2;
	public:
		Points(vector<Point3d>& scenePoints, vector<Point2d>& imagePoints_1, vector<Point2d>& imagePoints_2) :
			_scenePoints(scenePoints), _imagePoints_1(imagePoints_1), _imagePoints_2(imagePoints_2) {}

		inline vector<Point3d>& GetScenePoints() { return _scenePoints; }
		inline vector<Point2d>& GetImagePoints_1() { return _imagePoints_1; }
		inline vector<Point2d>& GetImagePoints_2() { return _imagePoints_2; }
	};
}