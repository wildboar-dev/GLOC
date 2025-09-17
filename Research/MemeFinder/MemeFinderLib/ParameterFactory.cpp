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
ParameterFactory::ParameterFactory(const Point2d& center) : _center(center)
{
	NVLib::RandomUtils::TimeSeedRandomNumbers();
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
	// Generate the indices to update
	vector<int> ignore;
	auto index1 = SelectIndex(ignore); ignore.push_back(index1);
	auto index2 = SelectIndex(ignore); ignore.push_back(index2);

	// Create the parameters object
	auto parameters = make_unique<Parameters>(_center);

	// Generate random values for the active parameters
	for (auto index : { index1, index2 })
	{
		auto range = GetValueRange(index);
		parameters->SetValue(index, GetRandomValue(range));
	}

	return parameters;
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
	auto duplicate = unordered_set<int>(ignore.begin(), ignore.end());

	int index;
	
	do
	{
		index = NVLib::RandomUtils::GetInteger(0, 6);
	} 
	while (duplicate.count(index) > 0);

	return index;
}

/**
 * @brief Get the range of values for a parameter
 * @param index The parameter index
 * @return pair<double, double> Returns a pair<double, double>
 */
pair<double, double> ParameterFactory::GetValueRange(int index)
{
	return { -2.0, 2.0 };
}

/**
 * @brief Generate a random value for a parameter
 * @param range The parameter range
 * @return double Returns a double
 */
double ParameterFactory::GetRandomValue(pair<double, double>& range)
{
	std::mt19937_64 eng(std::chrono::system_clock::now().time_since_epoch().count());
	auto distribution = uniform_real_distribution<double>(range.first, range.second);
	return distribution(eng);
}
