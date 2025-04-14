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
    Mat parameters = (Mat_<double>(4,1) << 500, 500, 500, 500);
    solver.Solve(parameters);

    

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