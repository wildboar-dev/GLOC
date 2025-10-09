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
    auto result = FPSearch::Solve(&costFunction, x0, 100, 1e-2, 150, DBL_EPSILON);

    auto finalScore = costFunction.Evaluate(result);
    Log() << NVL_App::Logger::Color(34) << "Final Score: " << finalScore << NVL_App::Logger::Save();
    Log() << NVL_App::Logger::Color(34) << "Result: " << result.transpose() << NVL_App::Logger::Save();

    Log() << Logger::Color(34) << "Rendering the results" << Logger::Save();
    auto resultImage = HelperUtils::RenderResult(meta->GetCameraMatrix(), Vec4d(result[0], result[1], result[2], result[3]), points.get(), meta->GetImageSize());
    imwrite("points.png", resultImage);

    Log() << Logger::Color(32) << "Saving the distortion parameters to disk" << Logger::Save();
    Mat distortion = (Mat_<double>(4, 1) << result[0], result[1], result[2], result[3]);
    SaveResult(_pathHelper, meta->GetCameraMatrix(), distortion);

    Log() << Logger::Color(32) << "Saving the undistorted points to disk" << Logger::Save();
    SavePoints(_pathHelper, points.get(), meta->GetCameraMatrix(), distortion);
}

//--------------------------------------------------
// Saving Results
//--------------------------------------------------

/**
 * Save the resultant camera and distortion coefficients to disk
 * @param pathHelper The path helper that we are using
 * @param camera The camera matrix
 * @param distCoeffs The distortion coefficients
 */
void Engine::SaveResult(NVLib::PathHelper * pathHelper, Mat& camera, Mat& distCoeffs) 
{
    auto path = pathHelper->GetPath("Distortion");
    if (!NVLib::FileUtils::Exists(path)) NVLib::FileUtils::AddFolder(path);

    FileStorage fs(pathHelper->GetPath("Distortion", "result.xml"), FileStorage::WRITE);
    fs << "CameraMatrix" << camera;
    fs << "DistCoeffs" << distCoeffs;
    fs.release();
}

/**
 * Save the points to disk
 * @param pathHelper The path helper that we are using
 * @param points The points that we are saving
 * @param camera The camera matrix
 * @param distCoeffs The distortion coefficients
 */
void Engine::SavePoints(NVLib::PathHelper * pathHelper, Points * points, Mat& camera, Mat& distCoeffs) 
{
    // Check to see if a meta folder already exists
    auto folderPath = pathHelper->GetPath("Distortion");
    if (!NVLib::FileUtils::Exists(folderPath)) NVLib::FileUtils::AddFolder(folderPath);

    // Open up a FileStorage XML generate the meta file
    auto filePath = pathHelper->GetPath("Distortion", "points.txt");
    auto writer = ofstream(filePath);
    if (!writer.is_open()) throw runtime_error("Unable to open file: " + filePath);

    // Find the undistorted points
    auto distortion = Vec4d(distCoeffs.at<double>(0), distCoeffs.at<double>(1), distCoeffs.at<double>(2), distCoeffs.at<double>(3));
    auto upoints = HelperUtils::Undistort(camera, distortion, points);

    // Write the header
    writer << "X,Y,Z,u1,v1,u2,v2" << endl;

    // Write the points
    for (auto i = 0; i < upoints->PointCount(); i++) 
    {
        auto scenePoint = upoints->GetScenePoints()[i];
        auto imagePoint_1 = upoints->GetImagePoints_1()[i];
        auto imagePoint_2 = upoints->GetImagePoints_2()[i];
        writer << fixed << setprecision(11) << scenePoint.x << "," << scenePoint.y << "," << scenePoint.z << ",";
        writer << fixed << setprecision(11) << imagePoint_1.x << "," << imagePoint_1.y << ",";
        writer << fixed << setprecision(11) << imagePoint_2.x << "," << imagePoint_2.y << endl;
    }

    // Release the file
    writer.close();
}