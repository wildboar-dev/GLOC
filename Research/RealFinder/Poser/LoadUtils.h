//--------------------------------------------------
// Utilities for loading data from disk
//
// @author: Wild Boar
//
// @date: 2025-10-13
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include <NVLib/Path/PathHelper.h>
#include "DParams.h"

namespace NVL_App
{
	class LoadUtils
	{
	public:
		static unique_ptr<DParams> LoadDParams(NVLib::PathHelper * pathHelper);
	};
}
