//--------------------------------------------------
// Implementation code for the Engine
//
// @author: Wild Boar
//
// @date: 2025-04-16
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

    _logger->Log(1, "Retrieving the scene name");
    _sceneName = ArgUtils::GetString(parameters, "scene");
    _logger->Log(1, "Scene name: %s", _sceneName.c_str());
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
    _logger->Log(1, "Loading Arguments");
    auto argumentFile = stringstream(); argumentFile << _sceneName << ".xml";
    auto argumentPath = _pathHelper->GetPath("Config", argumentFile.str());
    auto arguments = ArgumentLoader::Load(argumentPath);

    _logger->Log(1, "Generating the camera matrix");
    Mat cameraMatrix = HelperUtils::GetCameraMatrix(arguments.get());

    _logger->Log(1, "Generating the scene points");
    auto scenePoints = HelperUtils::GetScenePoints(arguments.get());
    _logger->Log(1, "Scene Point Count: %i", scenePoints.size());

    _logger->Log(1, "Generating the board poses");
    Mat pose_1 = PoseHelper::FindPose(arguments.get(), 0);
    Mat pose_2 = PoseHelper::FindPose(arguments.get(), 1);

    _logger->Log(1, "Generating the boards");
    auto board_1 = Board(cameraMatrix, pose_1, scenePoints);
    auto board_2 = Board(cameraMatrix, pose_2, scenePoints);

    _logger->Log(1, "Generating the image");
    Mat image = Mat_<Vec3b>::zeros(arguments->GetImageSize());
    board_1.Render(image); board_2.Render(image);

    _logger->Log(1, "Writing the preview image");
    imwrite("preview.png", image);
}