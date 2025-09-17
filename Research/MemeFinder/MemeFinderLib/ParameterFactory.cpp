//--------------------------------------------------
// Implementation of class ParameterFactory
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#include "ParameterFactory.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param center The center point of the image
 */
ParameterFactory::ParameterFactory(const Point2d& center)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Generate
//--------------------------------------------------

/**
 * @brief Generates parameters for evaluation
 * @return unique_ptr<Parameters> Returns a unique_ptr<Parameters>
 */
unique_ptr<Parameters> ParameterFactory::Generate()
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Helpers
//--------------------------------------------------

/**
 * @brief Select an index at random
 * @param ignore The indices to ignore
 * @return int Returns a int
 */
int ParameterFactory::SelectIndex(vector<int> ignore)
{
	throw runtime_error("Not implemented");
}

/**
 * @brief Get the range of values for a parameter
 * @param index The parameter index
 * @return pair<double, double> Returns a pair<double, double>
 */
pair<double, double> ParameterFactory::GetValueRange(int index)
{
	throw runtime_error("Not implemented");
}

/**
 * @brief Generate a random value for a parameter
 * @param range The parameter range
 * @return double Returns a double
 */
double ParameterFactory::GetRandomValue(pair<double, double>& range)
{
	throw runtime_error("Not implemented");
}
