//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2025-09-01
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Path/PathHelper.h>
#include <NVLib/Parameters/Parameters.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include <dynet/training.h>
#include <dynet/expr.h>
#include <dynet/io.h>
#include <dynet/model.h>
using namespace dynet;

#include "ArgReader.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run();

//--------------------------------------------------
// Execution Logic
//--------------------------------------------------

/**
 * Main entry point into the application
 */
void Run() 
{
    auto logger = NVLib::Logger(1);

    logger.StartApplication();

    logger.Log(1,"Creating DyNet Model");
    auto parameters = dynet::ParameterCollection();
    auto l_1 = parameters.add_parameters({ 3, 4 }); // Input 4 -> 3 Hidden 
    auto l_2 = parameters.add_parameters({ 3, 3 }); // Hidden 3 -> 3 Hidden
    auto l_3 = parameters.add_parameters({ 3, 3 }); // Hidden 3 -> 3 Output
    auto l_4 = parameters.add_parameters({ 3 }); // Output 3


    logger.StopApplication();
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
    dynet::initialize(argc, argv);

    try
    {
        Run();
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

    return EXIT_SUCCESS;
}
