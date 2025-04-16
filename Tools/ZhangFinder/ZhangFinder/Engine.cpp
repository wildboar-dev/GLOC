//--------------------------------------------------
// Implementation code for the Engine
//
// @author: Wild Boar
//
// @date: 2025-04-12
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

    _logger->Log(1, "Retrieving the element name");
    _elementName = ArgUtils::GetString(parameters, "element");
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
    _logger->Log(1, "Loading points");
    auto pointPath = GetPointPath();
    auto points = PointLoader::Load(pointPath);

    _logger->Log(1, "Create the Homographies");
    Mat H_1 = ZhangUtils::FindHomography(points.get(), 0);
    Mat H_2 = ZhangUtils::FindHomography(points.get(), 1);

    _logger->Log(1, "Create the Zhang Problem");
    auto problem = new ZhangProblem(H_1, H_2);

    _logger->Log(1, "Creating a problem solver engine");
    auto solver = NVL_AI::LMFinder(problem);

    _logger->Log(1, "Attempt to solve the problem");
    Mat parameters = (Mat_<double>(4,1) << 1000, 1000, 500, 500);
    solver.Solve(parameters);

    _logger->Log(1, "Generating the camera matrix");
    Mat K = ZhangUtils::GetCameraMatrix(parameters);
    //_logger->Log(1, (NVLib::Formatter() << "Camera Matrix: \n" << K).str().c_str());

    _logger->Log(1, "Get the first pose");
    Mat pose_1 = ZhangUtils::GetPose(H_1, K);
    //_logger->Log(1, (NVLib::Formatter() << "Pose 1: \n" << pose_1).str().c_str());

    _logger->Log(1, "Get the second pose");
    Mat pose_2 = ZhangUtils::GetPose(H_2, K);
    //_logger->Log(1, (NVLib::Formatter() << "Pose 2: \n" << pose_2).str().c_str());

    _logger->Log(1, "Testing the accuracy of the first board");
    auto error_1 = ZhangUtils::GetProjectError(K, pose_1, points.get(), 0);
    _logger->Log(1, (NVLib::Formatter() << "Error 1: " << error_1).str().c_str());

    _logger->Log(1, "Testing the accuracy of the second board");
    auto error_2 = ZhangUtils::GetProjectError(K, pose_2, points.get(), 1);
    _logger->Log(1, (NVLib::Formatter() << "Error 2: " << error_2).str().c_str());

    _logger->Log(1, "Writing the results to the file");
    WriteResults(_pathHelper, K, pose_1, pose_2);
}

//--------------------------------------------------
// Helper Methods
//--------------------------------------------------

/**
 * @brief Helper method to get the path to the points
 * @return The path to the points
 */
string Engine::GetPointPath() 
{
    auto filename = stringstream(); filename << _elementName << ".txt";
    auto path = _pathHelper->GetPath("Point", filename.str());
    return path;
}

/**
 * @brief Helper method to write the results to the file
 * @param pathHelper The path helper
 * @param K The camera matrix
 * @param M_1 The first pose
 * @param M_2 The second pose
 */
void Engine::WriteResults(NVLib::PathHelper * pathHelper, Mat& K, Mat& M_1, Mat& M_2) 
{
    auto basePath = pathHelper->GetPath("Calib_Output");
    if (!NVLib::FileUtils::Exists(basePath)) NVLib::FileUtils::AddFolder(basePath);

    auto filename = stringstream(); filename << _elementName << ".xml";
    auto path = pathHelper->GetPath("Calib_Output", filename.str());
    auto writer = FileStorage(path, FileStorage::WRITE | FileStorage::FORMAT_XML);

    writer << "K" << K;
    writer << "M_1" << M_1;
    writer << "M_2" << M_2;

    writer.release();
}