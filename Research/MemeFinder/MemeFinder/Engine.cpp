//--------------------------------------------------
// Implementation code for the Engine
//
// @author: Wild Boar
//
// @date: 2025-09-15
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
Engine::Engine(NVLib::Parameters* parameters) 
{
    _parameters = parameters;

    auto database = NVL_App::ArgUtils::GetString(parameters, "database");
    auto dataset = NVL_App::ArgUtils::GetString(parameters, "dataset");
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
 * @param logger The logger that we are using for the system
 */
void Engine::Run(NVL_App::Logger & logger)
{
    logger << NVL_App::Logger::Color(34) << "Loading Points" << NVL_App::Logger::Save();
    auto basePoints = NVL_App::PointLoader::Load(_pathHelper->GetPath("Point","point.txt"));
    logger << "Loaded " << basePoints->PointCount() << " basePoints" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Loading Meta" << NVL_App::Logger::Save();
    auto meta = NVL_App::MetaLoader::Load(_pathHelper->GetPath("Meta","meta.xml"));

    logger << NVL_App::Logger::Color(34) << "Loading Pose" << NVL_App::Logger::Save();
    auto pose_1 = NVL_App::PoseLoader::LoadPose(_pathHelper->GetPath("Meta","pose1.xml"));
    auto pose_2 = NVL_App::PoseLoader::LoadPose(_pathHelper->GetPath("Meta","pose2.xml"));

    logger << NVL_App::Logger::Color(34) << "Create a parameters" << NVL_App::Logger::Save();
    auto cx = meta->GetCameraMatrix().at<double>(0, 2); auto cy = meta->GetCameraMatrix().at<double>(1, 2);
    auto scene = NVL_App::Parameters(Point2d(cx, cy)); scene.SetValue(0, 0.1); scene.SetValue(2, 0.1);

    logger << NVL_App::Logger::Color(34) << "Applying the parameters to the basePoints" << NVL_App::Logger::Save();
    Mat camera = meta->GetCameraMatrix().clone(); Mat distortion = scene.GetDistortion();
    auto points = NVL_App::Utilities::ApplyDistortion(camera, pose_1.get(), pose_2.get(), &scene, basePoints.get());
    
    logger << NVL_App::Logger::Color(34) << "Determining the initial error score" << NVL_App::Logger::Save();
    auto errors = vector<double>(); auto initialScore = CostFunction::CalculateError(points.get(), errors);
    logger << NVL_App::Logger::Color(34) << "Initial Score: " << initialScore << NVL_App::Logger::Save();
    
    logger << NVL_App::Logger::Color(34) << "Running optimization" << NVL_App::Logger::Save();
    auto x0 = VectorXd::Zero((int)scene.GetIndices().size());
    auto costFunction = DCostFunction(meta->GetCameraMatrix(), scene.GetIndices(), points.get());
    auto solver = LMSolver(&costFunction); VectorXd result = x0; double finalCost = solver.Solve(result);
    auto finalScore = costFunction.Evaluate(result);
    logger << NVL_App::Logger::Color(34) << "Final Score: " << finalScore << NVL_App::Logger::Save();

    // _logger->Log(1, "Result: %f %f", result[0], result[1]);
    // _logger->Log(1, "True Values: %f %f", scene->GetValueDelta(scene->GetIndices()[0]), scene->GetValueDelta(scene->GetIndices()[1]));
}
