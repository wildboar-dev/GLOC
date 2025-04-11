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
#include "MetaData.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVLib::Parameters * parameters);
unique_ptr<NVL_App::MetaData> CreateMetaData();
Vec3d GetRandomRotation();
Vec3d GetRandomTranslation();
void Save(const string& path, NVL_App::MetaData * metaData);

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

    logger.Log(1, "Creating metadata");
    auto metaData = CreateMetaData();

    logger.Log(1, "Saving metadata");
    auto name = NVLib::StringUtils::GetDateTimeString();
    auto path = pathHelper.GetPath("MetaData", name + ".xml");
    Save(path, metaData.get());

    logger.StopApplication();
}

//--------------------------------------------------
// Generate Data
//--------------------------------------------------

/**
 * Create the metadata for the application
 * @return The metadata object
 */
unique_ptr<NVL_App::MetaData> CreateMetaData() 
{
    auto focal = 640.0 * (1.0 + NVLib::RandomUtils::GetInteger(0, 100) / 100.0);
    auto imageSize = Size(640, 480);

    auto rvec_1 = GetRandomRotation();
    auto tvec_1 = GetRandomTranslation();
    auto rvec_2 = GetRandomRotation();
    auto tvec_2 = GetRandomTranslation();
    
    auto blockSize = 5;
    auto gridSize = 10;

    return make_unique<NVL_App::MetaData>(focal, imageSize, rvec_1, tvec_1, rvec_2, tvec_2, blockSize, gridSize);
}

/**
 * Get a random rotation vector
 * @return The random rotation vector
 */
Vec3d GetRandomRotation() 
{
    auto rx = NVLib::RandomUtils::GetInteger(0, 20) / 100.0;
    auto ry = NVLib::RandomUtils::GetInteger(0, 20) / 100.0;
    auto rz = NVLib::RandomUtils::GetInteger(80, 100) / 100.0;
    auto rmag_r = sqrt(rx * rx + ry * ry + rz * rz);
    auto rmag = M_PI_4 * NVLib::RandomUtils::GetInteger(0, 100) / 100.0;
    return Vec3d(rx, ry, rz) / (rmag / rmag_r);
}

/**
 * Get a random translation vector
 * @return The random translation vector
 */
Vec3d GetRandomTranslation() 
{
    auto tx = NVLib::RandomUtils::GetInteger(-100, 100);
    auto ty = NVLib::RandomUtils::GetInteger(-20, 20);
    auto tz = NVLib::RandomUtils::GetInteger(200, 500);
    return Vec3d(tx, ty, tz);
}

//--------------------------------------------------
// Saving Logic
//--------------------------------------------------

/**
 * Save the metadata to a file
 * @param path The path to save the metadata
 * @param metaData The metadata object
 */
void Save(const string& path, NVL_App::MetaData * metaData) 
{
    auto fs = FileStorage(path, FileStorage::WRITE | FileStorage::FORMAT_XML);
    if (!fs.isOpened()) throw runtime_error("Failed to open file for writing: " + path);
    
    fs << "focal" << metaData->GetFocal();
    fs << "imageSize" << metaData->GetImageSize();
    fs << "rvec_1" << metaData->GetRVec_1();
    fs << "tvec_1" << metaData->GetTVec_1();
    fs << "rvec_2" << metaData->GetRVec_2();
    fs << "tvec_2" << metaData->GetTVec_2();
    fs << "blockSize" << metaData->GetBlockSize();
    fs << "gridSize" << metaData->GetGridSize();

    fs.release();
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