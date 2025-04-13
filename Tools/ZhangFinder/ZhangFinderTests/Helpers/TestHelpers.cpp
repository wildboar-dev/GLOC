//--------------------------------------------------
// Implementation of the test helpers
//
// @author: Wild Boar
//
// @date: 2025-04-12
//--------------------------------------------------

#include "TestHelpers.h"

//--------------------------------------------------
// Create a camera matrix
//--------------------------------------------------

/** 
* @brief Create a camera matrix from the given parameters.
* @param params A vector of camera parameters.
* @return A 3x3 camera matrix.
*/
Mat TestHelpers::GetCamera(const vector<double> & params) 
{
    // Get variables
    double fx = params[0];
    double fy = params[1];
    double cx = params[2];
    double cy = params[3];

    // Create the camera matrix
    Mat cameraMatrix = (Mat_<double>(3, 3) << fx, 0, cx,
                                               0, fy, cy,
                                               0, 0, 1);

    // Return the camera matrix
    return cameraMatrix;    
}