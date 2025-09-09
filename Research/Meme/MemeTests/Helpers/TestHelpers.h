//--------------------------------------------------
// A set of helper function for driving test methods
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

#include <MemeLib/Points.h>

class TestHelpers
{
public:
    static void BuildGrid(int rows, int cols, vector<Point3d>& grid);
    static Mat BuildCameraMatrix(double fx, double fy, const Point2d& center);
    static Mat BuildTranslation(double x, double y, double z);
    static unique_ptr<NVL_App::Points> BuildTestPoints(const vector<Point3d>& scenePoints, const Mat& cameraMatrix);
    static unique_ptr<NVL_App::Points> ApplyDistortion(const Mat& cameraMatrix, const Vec4d& distortion, NVL_App::Points* originalPoints);
};