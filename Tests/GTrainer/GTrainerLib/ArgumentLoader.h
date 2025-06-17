//--------------------------------------------------
// Adds the functionality to load the associated arguments from disk
//
// @author: Wild Boar
//
// @date: 2025-06-17
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/Path/PathHelper.h>
#include "Arguments.h"

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class ArgumentLoader
	{
	public:
		static unique_ptr<Arguments> Load(NVLib::PathHelper * pathHelper);
	};
}