//--------------------------------------------------
// A utility for loading meta data
//
// @author: Wild Boar
//
// @date: 2025-09-16
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "MetaData.h"

namespace NVL_App
{
	class MetaLoader
	{
		public:
			static unique_ptr<MetaData> Load(const string& filePath);
	};
}
