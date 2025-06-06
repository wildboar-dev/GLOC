//--------------------------------------------------
// Defines a basic engine for a vanilla C++ project.
//
// @author: Wild Boar
//
// @date: 2025-04-16
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Path/PathHelper.h>

#include <CalSceneLib/ArgUtils.h>
#include <CalSceneLib/Board.h>
#include <CalSceneLib/PoseHelper.h>
#include <CalSceneLib/HelperUtils.h>
#include <CalSceneLib/ArgumentLoader.h>

namespace NVL_App
{
	class Engine
	{
	private:
		NVLib::Parameters * _parameters;
		NVLib::Logger* _logger;
		NVLib::PathHelper * _pathHelper;

		string _sceneName;
		string _database;
	public:
		Engine(NVLib::Logger* logger, NVLib::Parameters * parameters);
		~Engine();

		void Run();
	};
}
