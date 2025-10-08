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

#include <RealFinderLib/Logger.h>

#include "LoadUtils.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVL_App::Logger& logger);
unique_ptr<NVLib::PathHelper> CreatePathHelper();
Size GetImageSize(NVLib::PathHelper * pathHelper);

//--------------------------------------------------
// Main entry point into the application
//--------------------------------------------------

/**
 * Main execution logic
 * @param logger The logger that we are using
 */
void Run(NVL_App::Logger& logger) 
{
    logger << NVL_App::Logger::Color(36) << "Generating a path helper" << NVL_App::Logger::Save();   
    auto pathHelper = CreatePathHelper();

    logger << NVL_App::Logger::Color(36) << "Loading board settings" << NVL_App::Logger::Save();
    auto settings = NVL_App::LoadUtils::LoadBoardSettings(pathHelper.get());

    logger << NVL_App::Logger::Color(36) << "Getting the image size" << NVL_App::Logger::Save();   
    auto imageSize = GetImageSize(pathHelper.get());
    logger << NVL_App::Logger::Color(36) << "Image Size: " << imageSize.width << "x" << imageSize.height << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(36) << "Loading grids" << NVL_App::Logger::Save();
    auto grid_1 = NVL_App::LoadUtils::LoadGrid(pathHelper.get(), settings.get(), 0, 0);
    auto grid_2 = NVL_App::LoadUtils::LoadGrid(pathHelper.get(), settings.get(), 0, 1);

    logger << NVL_App::Logger::Color(36) << "Loading calibrations" << NVL_App::Logger::Save();
    auto calibration = NVL_App::LoadUtils::LoadCalibration(pathHelper.get(), 0);
 }

//--------------------------------------------------
// Helper Size
//--------------------------------------------------

/**
 * Get the image size that we are dealing with
 * @param pathHelper That we are finding
 * @return The size of the image that we are getting
*/
Size GetImageSize(NVLib::PathHelper * pathHelper) 
{
    // Get the image path
    auto path = pathHelper->GetPath("Tool_Output", "Camera_0_Image_0.jpg");

    // Load the image
    Mat image = imread(path); if (image.empty()) throw runtime_error("Unable to find: " + path);

    // Return 
    return image.size();
}

//--------------------------------------------------
// Create a path helper
//--------------------------------------------------

/**
 * Create the path helper
 * @return The path helper
 */
unique_ptr<NVLib::PathHelper> CreatePathHelper() 
{
    // Load the configuration file
    FileStorage fs("config.xml", FileStorage::READ);
    if (!fs.isOpened()) throw runtime_error("Failed to open configuration file: config.xml");

    // Read the database and dataset
    string databasePath, datasetName;
    fs["database"] >> databasePath;
    fs["dataset"] >> datasetName;
    fs.release();

    // Create the path helper
    return make_unique<NVLib::PathHelper>(databasePath, datasetName);
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
