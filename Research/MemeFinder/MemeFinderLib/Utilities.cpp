//--------------------------------------------------
// Implementation of class HelperUtils
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#include "Utilities.h"
using namespace NVL_App;

//--------------------------------------------------
// Distortion Removal
//--------------------------------------------------

/**
 * @brief Applies distortion from the image
 * @param camera The camera matrix
 * @param parameters The input parameters
 * @param points The input points to distort
 * @return unique_ptr<Points> Returns a unique_ptr<Points>
 */
unique_ptr<Points> Utilities::ApplyDistortion(Mat& camera, Parameters * parameters, Points * points)
{
	Mat newCamera = camera.clone();
	newCamera.at<double>(0, 2) = parameters->GetCx();
	newCamera.at<double>(1, 2) = parameters->GetCy();

	Mat distortion = parameters->GetDistortion();

	return HelperUtils::Undistort(newCamera, distortion, points);
}
