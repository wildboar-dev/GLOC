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
Engine::Engine(NVLib::Logger* logger, NVLib::Parameters* parameters) 
{
    _logger = logger; _parameters = parameters;

    _logger->Log(1, "Creating a path helper");
    auto database = ArgUtils::GetString(parameters, "database");
    auto dataset = ArgUtils::GetString(parameters, "dataset");
    _pathHelper = new NVLib::PathHelper(database, dataset);

    _logger->Log(1, "Loading Points");
    auto points = PointLoader::Load(_pathHelper->GetPath("Point","point.txt"));
    _logger->Log(1, "Loaded %d points", points->PointCount());

    _logger->Log(1, "Loading Meta");
    auto meta = MetaLoader::Load(_pathHelper->GetPath("Meta","meta.xml"));
    cout << "Camera Matrix: " << meta->GetCameraMatrix() << endl;

    _logger->Log(1, "Loading Pose");
    auto pose_1 = PoseLoader::LoadPose(_pathHelper->GetPath("Meta","pose1.xml"));
    auto pose_2 = PoseLoader::LoadPose(_pathHelper->GetPath("Meta","pose2.xml"));

    _logger->Log(1, "Determine a randomly selected scene");
    auto cx = meta->GetCameraMatrix().at<double>(0, 2);
    auto cy = meta->GetCameraMatrix().at<double>(1, 2);
    auto factory = ParameterFactory(Point2d(cx, cy));
    auto scene = factory.Generate();

    _logger->Log(1, "Creating distorted points");
    auto distortedPoints = Utilities::ApplyDistortion(meta->GetCameraMatrix(), pose_1.get(), pose_2.get(), scene.get(), points.get());

    _logger->Log(1, "Creating the cost function");
    auto costFunction = DCostFunction(meta->GetCameraMatrix(), scene->GetIndices(), distortedPoints.get());
    auto initialScore = costFunction.Evaluate(VectorXd::Zero((int)scene->GetIndices().size()));
    _logger->Log(1, "Initial Score: %f", initialScore);

    _logger->Log(1, "Running optimization");
    auto x0 = VectorXd::Zero((int)scene->GetIndices().size());
    //auto result = GradientDescent::Solve(&costFunction, x0, 1000, 1e-21, nullptr);
    auto solver = LMSolver(&costFunction);
    VectorXd result = x0; double finalCost = solver.Solve(result);

    _logger->Log(1, "Result: %f %f", result[0], result[1]);
    _logger->Log(1, "True Values: %f %f", scene->GetValueDelta(scene->GetIndices()[0]), scene->GetValueDelta(scene->GetIndices()[1]));

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
    // TODO: Execution Logic
}
