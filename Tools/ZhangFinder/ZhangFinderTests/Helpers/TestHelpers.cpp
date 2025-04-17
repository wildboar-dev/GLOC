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
    Mat cameraMatrix = (Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);

    // Return the camera matrix
    return cameraMatrix;    
}

//--------------------------------------------------
// Save Logic
//--------------------------------------------------

/**
 * @brief Save the homography matrix to a file.
 * @param path The file path to save the homography matrix.
 * @param H The homography matrix to save.
 */
void TestHelpers::SaveHomography(const string& path, const Mat& H) 
{
    auto writer = ofstream(path);

    writer << H.at<double>(0, 0) << " " << H.at<double>(0, 1) << " " << H.at<double>(0, 2) << endl;
    writer << H.at<double>(1, 0) << " " << H.at<double>(1, 1) << " " << H.at<double>(1, 2) << endl;
    writer << H.at<double>(2, 0) << " " << H.at<double>(2, 1) << " " << H.at<double>(2, 2) << endl;

    writer.close();
}

/**
 * @brief Save the matrix to a file.
 * @param path The file path to save the matrix.
 * @param M The matrix to save.
 */
void TestHelpers::SaveMatrix(const string& path, const Mat& M) 
{
    auto writer = ofstream(path);

    for (int i = 0; i < M.rows; i++) 
    {
        for (int j = 0; j < M.cols; j++) 
        {
            writer << M.at<double>(i, j) << " ";
        }
        writer << endl;
    }

    writer.close();
}
