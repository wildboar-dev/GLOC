//--------------------------------------------------
// Defines a basic engine for a vanilla C++ project.
//
// @author: Wild Boar
//
// @date: 2025-10-08
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <RealFinderLib/Logger.h>

#include <NVLib/Path/PathHelper.h>

#include <RealFinderLib/ArgUtils.h>
#include <RealFinderLib/MetaLoader.h>
#include <RealFinderLib/PointLoader.h>
#include <RealFinderLib/CostFunction.h>
#include <RealFinderLib/DCostFunction.h>

#include <OptLib/PSearch/PSearch.h>
#include <OptLib/PSearch/FPSearch.h>
#include <OptLib/GradientDescent/GradientDescent.h>
#include <OptLib/LM/LMSolver.h>

namespace NVL_App
{
	class Engine
	{
	private:
		NVLib::Parameters * _parameters;
		NVL_App::Logger* _logger;
		NVLib::PathHelper * _pathHelper;
	public:
		Engine(NVL_App::Logger* logger, NVLib::Parameters * parameters);
		~Engine();

		void Run();

	private:
		NVL_App::Logger& Log() { return *_logger; }
	};
}
