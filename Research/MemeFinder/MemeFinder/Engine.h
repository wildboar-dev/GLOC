//--------------------------------------------------
// Defines a basic engine for a vanilla C++ project.
//
// @author: Wild Boar
//
// @date: 2025-09-15
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/Path/PathHelper.h>

#include <MemeFinderLib/ArgUtils.h>
#include <MemeFinderLib/ParameterFactory.h>
#include <MemeFinderLib/DCostFunction.h>
#include <MemeFinderLib/Utilities.h>
#include <MemeFinderLib/PoseLoader.h>
#include <MemeFinderLib/Logger.h>

#include <MemeLib/Points.h>
#include <MemeLib/PointLoader.h>
#include <MemeLib/MetaLoader.h>

#include <OptLib/Helpers/Tracker.h>
#include <OptLib/PSearch/PSearch.h>
#include <OptLib/LM/LMSolver.h>

namespace NVL_App
{
	class Engine
	{
	private:
		NVLib::Parameters * _parameters;
		NVLib::PathHelper * _pathHelper;
	public:
		Engine(NVLib::Parameters * parameters);
		~Engine();

		void Run(NVL_App::Logger & logger);
	private:
		void SavePath(const string & fileName, NVL_App::Tracker & tracker);
	};
}
