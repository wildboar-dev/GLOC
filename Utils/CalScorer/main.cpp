//--------------------------------------------------
// Defines a basic engine for a vanilla C++ project.
//
// @author: Wild Boar
//
// @date: 2025-04-12
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Formatter.h>
#include <NVLib/Path/PathHelper.h>

#include "ArgUtils.h"
#include "PointLoader.h"
#include "Meta.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
unique_ptr<NVL_App::Meta> LoadMeta(NVLib::PathHelper* pathHelper, const string& elementName); 

//--------------------------------------------------
// Main Execution Logic
//--------------------------------------------------

/**
 * Execute the main logic of the application
 * @param logger The logger instance
 * @param parameters The parameters for execution   
 */
void Execute(NVLib::Logger * logger, NVLib::Parameters * parameters)
{
    logger->Log(1, "Creating a path helper");
    auto database = NVL_App::ArgUtils::GetString(parameters, "database");
    auto dataset = NVL_App::ArgUtils::GetString(parameters, "dataset");
    auto pathHelper = NVLib::PathHelper(database, dataset);

    logger->Log(1, "Retrieving the element name...");
    auto elementName = NVL_App::ArgUtils::GetString(parameters, "element");
    logger->Log(1, "Element Name: %s", elementName.c_str());

    logger->Log(1, "Retrieving the save file name...");
    auto saveFileName = NVL_App::ArgUtils::GetString(parameters, "save");
    logger->Log(1, "Save File Name: %s", saveFileName.c_str());

    logger->Log(1, "Load test points...");
    auto pointFile = stringstream(); pointFile << elementName << ".txt";
    auto pointPath = pathHelper.GetPath("Point", pointFile.str());
    auto points = NVL_App::PointLoader::Load(pointPath);
    logger->Log(1, "Number of points Loaded: %i", points->GetScenePoints().size());

    logger->Log(1, "Load meta information...");
    auto meta = LoadMeta(&pathHelper, elementName);
    logger->Log(1, "Focal: %f", meta->GetFocal());

}

//--------------------------------------------------
// Loader Helpers
//--------------------------------------------------

/**
 * @brief Load the meta information from the disk
 * @param pathHelper The path helper to use
 * @param elementName The name of the element
 * @return unique_ptr<Meta> Returns a unique_ptr<Meta>
 */
unique_ptr<NVL_App::Meta> LoadMeta(NVLib::PathHelper* pathHelper, const string& elementName) 
{
    auto fileName = stringstream(); fileName << elementName << ".xml";
    auto path = pathHelper->GetPath("Meta", fileName.str());

    auto reader = FileStorage(path, FileStorage::FORMAT_XML | FileStorage::READ);
    if (!reader.isOpened()) throw runtime_error("Unable to open: " + path);

    double focal; reader["focal"] >> focal;
    Size imageSize; reader["imageSize"] >> imageSize;
    Vec3d rvec_1; reader["rvec_1"] >> rvec_1;
    Vec3d tvec_1; reader["tvec_1"] >> tvec_1;
    Vec3d rvec_2; reader["rvec_2"] >> rvec_2;
    Vec3d tvec_2; reader["tvec_2"] >> tvec_2;
    double blockSize; reader["blockSize"] >> blockSize;
    int gridSize; reader["gridSize"] >> gridSize;
  
    reader.release();

    return make_unique<NVL_App::Meta>(focal, imageSize, rvec_1, tvec_1, rvec_2, tvec_2, blockSize, gridSize);
} 

//--------------------------------------------------
// Execution entry point
//--------------------------------------------------

/**
 * Main Method
 * @param argc The count of the incomming arguments
 * @param argv The number of incomming arguments
 */
int main(int argc, char ** argv) 
{
    auto logger = NVLib::Logger(2);
    logger.StartApplication();

    try
    {
        auto parameters = NVL_App::ArgUtils::Load("CalScorer", argc, argv);
        Execute(&logger, parameters);
    }
    catch (runtime_error exception)
    {
        logger.Log(1, "Error: %s", exception.what());
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        logger.Log(1, "Error: %s", exception.c_str());
        exit(EXIT_FAILURE);
    }

    logger.StopApplication();

    return EXIT_SUCCESS;
}
