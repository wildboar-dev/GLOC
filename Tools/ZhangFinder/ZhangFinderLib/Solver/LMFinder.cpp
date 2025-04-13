//--------------------------------------------------
// Implementation of class LMFinder
//
// @author: Wild Boar
//
// @date: 2023-12-29
//--------------------------------------------------

#include "LMFinder.h"
using namespace NVL_AI;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Initializer Constructor
 * @param problem Initialize variable <problem>
 */
LMFinder::LMFinder(ProblemBase * problem) : _problem(problem)
{
	// Extra initialization can go here
}

/**
 * @brief Main Terminator
 */
LMFinder::~LMFinder()
{
	delete _problem;
}

//--------------------------------------------------
// Solve
//--------------------------------------------------

/**
 * @brief Solve the given problem
 * @param parameters The set of parameters
 * @return double Returns a double
 */
double LMFinder::Solve(Mat& parameters)
{
	// Setup the parameters;
	auto m = _problem->GetDataSize(); auto n = parameters.rows;
	auto x = (double *) parameters.data;
	Mat mask = Mat_<int>(n, 1); mask.setTo(1); auto msk = (int *) mask.data;
	Mat errors = Mat_<double>::zeros(m, 1); auto fvec = (double *) errors.data;
	auto tol = 1.0e-10; auto info = 0; auto nfev= 0;
	Evaluate(parameters, errors);

	// Refine the associated point
	lmdif0(LMFinder::Callback, m, n, x, msk, fvec, tol, &info, &nfev, this);

	// Return the result of the final evaluation
	return Evaluate(parameters, errors);
}

//--------------------------------------------------
// Evaluate
//--------------------------------------------------

/**
 * @brief Evaluate a set of parameters for their suitability
 * @param parameters The list of parameters
 * @param errors The resultant set of errors
 * @return double Returns a double
 */
double LMFinder::Evaluate(Mat& parameters, Mat& errors)
{
	auto score = _problem->Evaluate(parameters, errors);
	return score;
}

//--------------------------------------------------
// Callback
//--------------------------------------------------

/**
 * @brief A callback for the LMFinder
 * @param dataCount The number of data points
 * @param paramCount The number of parameters that we have
 * @param params The input parameters
 * @param errors The error parameters
 * @param errorNumber The number of errors
 * @param callback A callback variable
 */
void LMFinder::Callback(int dataCount, int paramCount, double * params, double * errors, int * errorNumber, void * callback)
{
	// Create the matrices
	Mat parameterMatrix = Mat_<double>(paramCount, 1, params);
	Mat errorMatrix = Mat_<double>(dataCount, 1, errors);

	// Get the parent variable
	auto parent = (LMFinder *) callback;

	// Find the errors
	parent->Evaluate(parameterMatrix, errorMatrix);
}