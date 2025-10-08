//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2025-10-08
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
    logger << NVL_App::Logger::Color(32) << "Starting RealFinder Application" << NVL_App::Logger::Save();

    try
    {
        auto parameters = NVL_App::ArgUtils::Load("RealFinder", argc, argv);
        NVL_App::Engine(&logger, parameters).Run();
    }
    catch (runtime_error exception)
    {
        logger << NVL_App::Logger::Color(31) << "Error: " << exception.what() << NVL_App::Logger::Save();
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        logger << NVL_App::Logger::Color(31) << "Error: " << exception.c_str() << NVL_App::Logger::Save();
        exit(EXIT_FAILURE);
    }

    logger << NVL_App::Logger::Color(32) << "Stopping RealFinder Application" << NVL_App::Logger::Save();

    return EXIT_SUCCESS;
}
