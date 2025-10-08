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
