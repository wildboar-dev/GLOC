//--------------------------------------------------
// Implementation of class HelperUtils
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#include "HelperUtils.h"
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
unique_ptr<Points> HelperUtils::ApplyDistortion(Mat& camera, Parameters * parameters, Points * points)
{
	throw runtime_error("Not implemented");
}
