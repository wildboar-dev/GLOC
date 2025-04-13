//--------------------------------------------------
// Add the logic to load point files from disk
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

#include "Points.h"

namespace NVL_App
{
	class PointLoader
	{
	public:
		static unique_ptr<Points> Load(const string& path);
	};
}