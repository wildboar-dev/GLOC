//--------------------------------------------------
// Implementation code for the Engine
//
// @author: Wild Boar
//
// @date: 2025-10-08
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
Engine::Engine(NVL_App::Logger* logger, NVLib::Parameters* parameters) : _logger(logger), _parameters(parameters)
{
    Log() << Logger::Color(36) << "Creating a path helper" << Logger::Save();
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
    Log() << Logger::Color(34) << "Loading Points" << Logger::Save();
    auto points = NVL_App::PointLoader::Load(_pathHelper->GetPath("Points","points.txt"));
    Log() << "Loaded " << points->PointCount() << " basePoints" << Logger::Save();

    Log() << Logger::Color(34) << "Loading Meta" << Logger::Save();
    auto meta = NVL_App::MetaLoader::Load(_pathHelper->GetPath("Meta","meta.xml"));

    Log() << "Setting up a basic cost function" << meta->GetCameraMatrix() << Logger::Save();
    auto x0 = VectorXd::Zero(4);
    auto costFunction = DCostFunction(meta->GetCameraMatrix(), {0,1,2,3}, points.get());

    Log() << Logger::Color(34) << "Determining the initial error score" << Logger::Save();
    auto errors = vector<double>(); auto initialScore = CostFunction::CalculateError(points.get(), errors);
    Log() << Logger::Color(34) << "Initial Score: " << initialScore << Logger::Save();

    Log() << Logger::Color(34) << "Refining the model" << Logger::Save();
    //auto result = GradientDescent::Solve(&costFunction, x0, 1000, 1e-10);
    //auto result = PSearch::Solve(&costFunction, x0, 100, 1e-2, 1e4);
    auto result = FPSearch::Solve(&costFunction, x0, 100, 1e-2, 100, DBL_EPSILON);

    auto finalScore = costFunction.Evaluate(result);
    Log() << NVL_App::Logger::Color(34) << "Final Score: " << finalScore << NVL_App::Logger::Save();
    Log() << NVL_App::Logger::Color(34) << "Result: " << result.transpose() << NVL_App::Logger::Save();

    Log() << Logger::Color(34) << "Rendering the results" << Logger::Save();
    auto resultImage = HelperUtils::RenderResult(meta->GetCameraMatrix(), Vec4d(result[0], result[1], result[2], result[3]), points.get(), meta->GetImageSize());
    imwrite("points.png", resultImage);
}
