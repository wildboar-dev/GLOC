//--------------------------------------------------
// Loads the given set of arguments from disk
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
	class ArgumentLoader
	{
	public:
		static unique_ptr<Arguments> Load(const string& path);
	};
}