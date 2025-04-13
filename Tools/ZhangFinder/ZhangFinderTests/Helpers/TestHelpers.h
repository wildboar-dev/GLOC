//--------------------------------------------------
// Helper methods for unit testing
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

class TestHelpers
{
public:
    static Mat GetCamera(const vector<double> & params);
};
