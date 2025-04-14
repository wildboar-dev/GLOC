//--------------------------------------------------
// Defines a basic engine for a vanilla C++ project.
//
// @author: Wild Boar
//
// @date: 2025-04-12
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Path/PathHelper.h>

#include <ZhangFinderLib/ArgUtils.h>
#include <ZhangFinderLib/PointLoader.h>
#include <ZhangFinderLib/ZhangUtils.h>
#include <ZhangFinderLib/ZhangProblem.h>
#include <ZhangFinderLib/Solver/LMFinder.h>

namespace NVL_App
{
	class Engine
	{
	private:
		NVLib::Parameters * _parameters;
		NVLib::Logger* _logger;
		NVLib::PathHelper * _pathHelper;
		string _elementName;
	public:
		Engine(NVLib::Logger* logger, NVLib::Parameters * parameters);
		~Engine();

		void Run();
	private:
		string GetPointPath();
	};
}
