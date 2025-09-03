//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2025-04-11
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/RandomUtils.h>
#include <NVLib/StringUtils.h>
#include <NVLib/Path/PathHelper.h>
#include <NVLib/Parameters/Parameters.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "ArgReader.h"
#include "Arguments.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVLib::Parameters * parameters);
unique_ptr<NVL_App::Arguments> CreateArguments();
Vec4d GetRandomDistortion();
void Save(const string& path, NVL_App::Arguments * arguments);
void CreateDB(NVLib::PathHelper& pathHelper);
void MakeFolder(const string& path);
int Random(int min, int max);   

//--------------------------------------------------
// Execution Logic
//--------------------------------------------------

/**
 * Main entry point into the application
 * @param parameters The input parameters
 */
void Run(NVLib::Parameters * parameters) 
{
    if (parameters == nullptr) return; auto logger = NVLib::Logger(1);

    NVLib::RandomUtils::TimeSeedRandomNumbers();

    logger.StartApplication();

    logger.Log(1, "Loading parameters");
    auto database = NVL_Utils::ArgReader::ReadString(parameters, "database");
    auto dataset = NVL_Utils::ArgReader::ReadString(parameters, "dataset");
    auto pathHelper = NVLib::PathHelper(database, dataset);

    logger.Log(1, "Creating database folders");
    CreateDB(pathHelper);

    logger.Log(1, "Creating Output Configuration");
    auto arguments = CreateArguments();

    logger.Log(1, "Saving Output Configuration");
    auto name = NVLib::StringUtils::GetDateTimeString();
    auto path = pathHelper.GetPath("Config", name + ".xml");
    logger.Log(1, "Saving to: %s", path.c_str());
    Save(path, arguments.get());

    logger.StopApplication();
}

//--------------------------------------------------
// Generate Data
//--------------------------------------------------

/**
 * Create the metadata for the application
 * @return The metadata object
 */
unique_ptr<NVL_App::Arguments> CreateArguments() 
{
    auto focal = 640.0 * (1.0 + NVLib::RandomUtils::GetInteger(0, 100) / 100.0);
    auto imageSize = Size(640, 480);

    double blockSize = 5.0;
    Vec2i gridSize = Vec2i(7, 5);
    Vec2d shiftXY = Vec2d(Random(10,30), Random(10,30));
    Vec2d RotYZ = Vec2d(Random(-10,10) / 1e3, Random(-10,10) / 1e3);
    double angle = Random(30,90);
    double distance = Random(100, 300);
    Vec2d focals = Vec2d(focal, focal);
    Point2d center = Point2d(imageSize.width / 2.0 + Random(-20,20), imageSize.height / 2.0 + Random(-20,20));
    int decimals = 8;
    auto distortion = GetRandomDistortion();

    return make_unique<NVL_App::Arguments>(blockSize, gridSize, shiftXY, RotYZ, angle, distance, focals, center, distortion, imageSize, decimals);
}

/**
 * Get a random distortion vector
 * @return The random distortion vector
 */
Vec4d GetRandomDistortion() 
{
    auto k1 = NVLib::RandomUtils::GetInteger(-100, 100) / 100.0;
    auto k2 = NVLib::RandomUtils::GetInteger(-100, 100) / 100.0;
    auto p1 = NVLib::RandomUtils::GetInteger(-100, 100) / 100.0;
    auto p2 = NVLib::RandomUtils::GetInteger(-100, 100) / 100.0;
    return Vec4d(k1, k2, p1, p2);
}

/**
 * Defines a random number generator helper
 * @param min The min value possible for the random number
 * @param max The max value possible for the random number
 */
int Random(int min, int max) 
{
    return NVLib::RandomUtils::GetInteger(min, max);    
}   

//--------------------------------------------------
// Saving Logic
//--------------------------------------------------

/**
 * Save the metadata to a file
 * @param path The path to save the metadata
 * @param metaData The metadata object
 */
void Save(const string& path, NVL_App::Arguments * arguments) 
{
    auto fs = FileStorage(path, FileStorage::WRITE | FileStorage::FORMAT_XML);
    if (!fs.isOpened()) throw runtime_error("Failed to open file for writing: " + path);

    fs << "block_size" << arguments->GetBlockSize();
    fs << "grid_size" << arguments->GetGridSize();
    fs << "shift_xy" << arguments->GetShiftXY();
    fs << "rot_yz" << arguments->GetRotYZ();
    fs << "angle" << arguments->GetAngle();
    fs << "distance" << arguments->GetDistance();
    fs << "focals" << arguments->GetFocals();
    fs << "center" << arguments->GetCenter();
    fs << "image_size" << arguments->GetImageSize();
    fs << "decimals" << arguments->GetDecimals();
    fs << "distortion" << arguments->GetDistortion();

    fs.release();
}

//--------------------------------------------------
// Database Creation
//--------------------------------------------------

/**
 * Make sure that the database folder structure exists
 * @param pathHelper The path helper object
 */
void CreateDB(NVLib::PathHelper& pathHelper) 
{
    MakeFolder(pathHelper.GetBasePath());
    MakeFolder(pathHelper.GetPath("Config"));
}
    
/**
 * Create a folder if it doesn't exist
 * @param path The path to the folder
 */
void MakeFolder(const string& path) 
{
    if (!filesystem::exists(path)) 
    {
        filesystem::create_directories(path);
    }
}

//--------------------------------------------------
// Entry Point
//--------------------------------------------------

/**
 * Main Method
 * @param argc The count of the incoming arguments
 * @param argv The number of incoming arguments
 * @return SUCCESS and FAILURE
 */
int main(int argc, char ** argv) 
{
    NVLib::Parameters * parameters = nullptr;

    try
    {
        parameters = NVL_Utils::ArgReader::GetParameters(argc, argv);
        Run(parameters);
    }
    catch (runtime_error exception)
    {
        cerr << "Error: " << exception.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        cerr << "Error: " << exception << endl;
        exit(EXIT_FAILURE);
    }

    if (parameters != nullptr) delete parameters;

    return EXIT_SUCCESS;
}