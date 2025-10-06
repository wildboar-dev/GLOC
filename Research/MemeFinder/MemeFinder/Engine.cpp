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
    //---------------------------------------------------
    // HARDCODED PARAMETERS
    //---------------------------------------------------
    const double K1 = -0.2, K2 = 0.7;
    const int dIndex_1 = 0, dIndex_2 = 1;
    //---------------------------------------------------

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
    auto scene = NVL_App::Parameters(Point2d(cx, cy)); scene.SetValue(dIndex_1, K1); scene.SetValue(dIndex_2, K2);

    logger << NVL_App::Logger::Color(34) << "Applying the parameters to the basePoints" << NVL_App::Logger::Save();
    Mat camera = meta->GetCameraMatrix().clone(); Mat distortion = scene.GetDistortion();
    auto points = NVL_App::Utilities::ApplyDistortion(camera, pose_1.get(), pose_2.get(), &scene, basePoints.get());
    
    logger << NVL_App::Logger::Color(34) << "Determining the initial error score" << NVL_App::Logger::Save();
    auto errors = vector<double>(); auto initialScore = CostFunction::CalculateError(points.get(), errors);
    logger << NVL_App::Logger::Color(34) << "Initial Score: " << initialScore << NVL_App::Logger::Save();
    
    logger << NVL_App::Logger::Color(34) << "Running optimization" << NVL_App::Logger::Save();
    auto x0 = VectorXd::Zero((int)scene.GetIndices().size());
    auto costFunction = DCostFunction(meta->GetCameraMatrix(), scene.GetIndices(), points.get());

    auto tracker = NVL_App::Tracker();
    //auto result = GradientDescent::Solve(&costFunction, x0, 1000, 1e-10, &tracker);
    //auto result = PSearch::Solve(&costFunction, x0, 100, 1e-2, 1e4, &tracker);
    auto result = FPSearch::Solve(&costFunction, x0, 100, 1e-5, 1e4, 1e-21, &tracker);

    auto finalScore = costFunction.Evaluate(result);
    logger << NVL_App::Logger::Color(34) << "Final Score: " << finalScore << NVL_App::Logger::Save();
    logger << NVL_App::Logger::Color(34) << "Result: " << result.transpose() << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Saving the optimization path" << NVL_App::Logger::Save();
    SavePath("path.txt", tracker);
}

//--------------------------------------------------
// Execution Entry Point
//--------------------------------------------------

/**
 * Saves the path to a file
 * @param fileName The file name to save to
 * @param tracker The tracker that contains the path
 */
void Engine::SavePath(const string & fileName, NVL_App::Tracker & tracker) 
{
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "Error opening file for writing: " << fileName << endl;
        return;
    }

    // Write the path points to the file
    for (const auto & point : tracker.GetPath()) {
        outFile << point(0) << " " << point(1) << endl;
    }

    outFile.close();
}
