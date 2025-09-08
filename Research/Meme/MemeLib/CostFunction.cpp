//--------------------------------------------------
// Implementation of class CostFunction
//
// @author: Wild Boar
//
// @date: 2025-09-09
//--------------------------------------------------

#include "CostFunction.h"
using namespace NVL_App;

//--------------------------------------------------
// Calculate
//--------------------------------------------------

/**
 * @brief Calculates the error between the scene points and the image points
 * @param gridSize The size of the image
 * @param points The points to calculate the error for
 * @param errors The individual errors
 * @return double Returns a double
 */
double CostFunction::CalculateError(const Size& gridSize, const Points& points, vector<double>& errors)
{
	throw runtime_error("Not implemented");
}
