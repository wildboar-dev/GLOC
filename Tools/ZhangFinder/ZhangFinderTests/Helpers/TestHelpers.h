//--------------------------------------------------
// Helper methods for unit testing
//
// @author: Wild Boar
//
// @date: 2025-04-12
//--------------------------------------------------

#pragma once

#include <fstream>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class TestHelpers
{
public:
    static Mat GetCamera(const vector<double> & params);
    static void SaveHomography(const string& path, const Mat& H);
    static void SaveMatrix(const string& path, const Mat& M);
};
