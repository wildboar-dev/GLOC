//--------------------------------------------------
// Loader - Source.cpp
//
// @author: Wild Boar
//
// @date: 2025-10-08
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include <NVLib/Path/PathHelper.h>
#include <NVLib/FileUtils.h>

#include <RealFinderLib/Logger.h>

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVL_App::Logger& logger);

//--------------------------------------------------
// Main entry point into the application
//--------------------------------------------------

/**
 * Main execution logic
 * @param logger The logger that we are using
 */
void Run(NVL_App::Logger& logger) 
{
    logger << NVL_App::Logger::Color(36) << "Call the the run method invoked" << NVL_App::Logger::Save();   
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
    // Create the logger
    auto logger = NVL_App::Logger();

    // Indicate that the application is starting
    logger << NVL_App::Logger::Color(32) << "Starting Loader Application" << NVL_App::Logger::Save();
 
    // Main execution logic
    try 
    {
        Run(logger);
    }
    catch (const cv::Exception& ex) 
    {
        logger << NVL_App::Logger::Color(31) << "OpenCV Exception: " << ex.what() << NVL_App::Logger::Save();
    }
    catch (const std::exception& ex) 
    {
        logger << NVL_App::Logger::Color(31) << "Standard Exception: " << ex.what() << NVL_App::Logger::Save();
    }
    catch (...) 
    {
        logger << NVL_App::Logger::Color(31) << "Unknown Exception occurred" << NVL_App::Logger::Save();
    }

    // Indicate that the application is closing
    logger << NVL_App::Logger::Color(32) << "Closing Loader Application" << NVL_App::Logger::Save();

    return EXIT_SUCCESS;
}