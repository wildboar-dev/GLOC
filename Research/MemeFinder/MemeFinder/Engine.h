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

#include <NVLib/Logger.h>
#include <NVLib/Path/PathHelper.h>
#include <MemeFinderLib/ArgUtils.h>

namespace NVL_App
{
	class Engine
	{
	private:
		NVLib::Parameters * _parameters;
		NVLib::Logger* _logger;
		NVLib::PathHelper * _pathHelper;
	public:
		Engine(NVLib::Logger* logger, NVLib::Parameters * parameters);
		~Engine();

		void Run();
	};
}
