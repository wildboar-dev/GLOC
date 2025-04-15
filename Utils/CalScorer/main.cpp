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

    logger->Log(1, "Retrieving the element name");
    auto elementName = NVL_App::ArgUtils::GetString(parameters, "element");
    logger->Log(1, "Element Name: %s", elementName.c_str());

    logger->Log(1, "Retrieviing the save file name");
    auto saveFileName = NVL_App::ArgUtils::GetString(parameters, "save");
    logger->Log(1, "Save File Name: %s", saveFileName.c_str());
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
