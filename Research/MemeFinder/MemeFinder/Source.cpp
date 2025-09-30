//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2025-09-15
//--------------------------------------------------

#include "Engine.h"

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
    auto logger = NVL_App::Logger();
 
    try
    {
        logger << NVL_App::Logger::Color(32) << "Starting Mapper Application" << NVL_App::Logger::Save();
        auto parameters = NVL_App::ArgUtils::Load("MemeFinder", argc, argv);
        NVL_App::Engine(parameters).Run(logger);
    }
    catch (const exception& ex) 
    {
        logger << NVL_App::Logger::Color(31) << "Error: " << ex.what() << NVL_App::Logger::Save();
        return EXIT_FAILURE;
    }

    logger << NVL_App::Logger::Color(32) << "Mapper Application Finished" << NVL_App::Logger::Save();

    return EXIT_SUCCESS;
}
