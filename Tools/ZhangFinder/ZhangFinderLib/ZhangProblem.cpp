//--------------------------------------------------
// Implementation of class ZhangProblem
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#include "ZhangProblem.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param H1 The homography of the first point
 * @param H2 The homography of the second point
 */
ZhangProblem::ZhangProblem(Mat& H1, Mat& H2)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Data Size
//--------------------------------------------------

/**
 * @brief Returns the number of data points that we are evaluating
 * @return int Returns a int
 */
int ZhangProblem::GetDataSize()
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Evaluate
//--------------------------------------------------

/**
 * @brief Evaluate the provided camera parameters
 * @param parameters The number of parameters that we are evaluating
 * @param errors The error score for each element
 * @return double Returns a double
 */
double ZhangProblem::Evaluate(Mat& parameters, Mat& errors)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Helper
//--------------------------------------------------

/**
 * @brief Converts the parameters into a B matrix
 * @param parameters The parameters that we are evaluating
 * @return Mat Returns a Mat
 */
Mat ZhangProblem::GetB(Mat& parameters)
{
	throw runtime_error("Not implemented");
}
