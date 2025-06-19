//--------------------------------------------------
// Implementation code for the Engine
//
// @author: Wild Boar
//
// @date: 2025-06-17
//--------------------------------------------------

#include "Engine.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructor and Terminator
//--------------------------------------------------

/**
 * Main Constructor
 * @param logger The logger that we are using for the system
 * @param parameters The input parameters
 */
Engine::Engine(NVLib::Logger* logger, NVLib::Parameters* parameters) 
{
    _logger = logger; _parameters = parameters;

    _logger->Log(1, "Creating a path helper");
    auto database = ArgUtils::GetString(parameters, "database");
    auto dataset = ArgUtils::GetString(parameters, "dataset");
    _pathHelper = new NVLib::PathHelper(database, dataset);
}

/**
 * Main Terminator 
 */
Engine::~Engine() 
{
    delete _parameters; delete _pathHelper;
}

//--------------------------------------------------
// Execution Entry Point
//--------------------------------------------------

/**
 * Entry point function
 */
void Engine::Run()
{
    _logger->Log(1, "Loading the arguments from disk");
    auto arguments = ArgumentLoader::Load(_pathHelper);

    _logger->Log(1, "Generate a problem state");
    auto problemState = StateGenerator::Create(arguments.get());

    _logger->Log(1, "Camera Matrix:\n%s", (NVLib::Formatter() <<  problemState->GetCamera()).str().c_str());
    _logger->Log(1, "Distortion Matrix:\n%s", (NVLib::Formatter() <<  problemState->GetDistortion()).str().c_str());
    _logger->Log(1, "Pose Matrix:\n%s", (NVLib::Formatter() <<  problemState->GetPose()).str().c_str());

}
