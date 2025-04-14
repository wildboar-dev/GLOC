//--------------------------------------------------
// Unit Tests for class ZhangProblem
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#include <gtest/gtest.h>

#include <ZhangFinderLib/ZhangUtils.h>
#include <ZhangFinderLib/PointLoader.h>
#include <ZhangFinderLib/ZhangProblem.h>
using namespace NVL_App;

#include "../Helpers/TestHelpers.h"

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Makes sure that a correct match returns essentially zero match score
 */
TEST(ZhangProblem_Test, verify_correct_match)
{
	// Setup: Load Points
	auto points = PointLoader::Load("2025-04-11 23:18:40.txt");

	// Setup: Homographies
	Mat H_1 = ZhangUtils::FindHomography(points.get(), 0);
	Mat H_2 = ZhangUtils::FindHomography(points.get(), 1);

	// Setup: Setup the error finder
	auto problem = ZhangProblem(H_1, H_2);

	// Setup: Parameters
	Mat parameters = (Mat_<double>(4, 1) << 1177.59, 1177.59, 320.0, 240.0);
	Mat errors = Mat_<double>::zeros(4, 1);
	auto elink = (double *)errors.data;

	// Execute
	auto score = problem.Evaluate(parameters, errors);

	// Evaluate
	ASSERT_LT(score, 1e-14);
}

/**
 * @brief Makes sure that an incorrect match returns a non-zero match score
 */
TEST(ZhangProblem_Test, verify_incorrect_match)
{
	// Setup: Load Points
	auto points = PointLoader::Load("2025-04-11 23:18:40.txt");

	// Setup: Homographies
	Mat H_1 = ZhangUtils::FindHomography(points.get(), 0);
	Mat H_2 = ZhangUtils::FindHomography(points.get(), 1);

	// Setup: Setup the error finder
	auto problem = ZhangProblem(H_1, H_2);

	// Setup: Parameters
	Mat parameters = (Mat_<double>(4, 1) << 500.0, 500.0, 320.0, 240.0);
	Mat errors = Mat_<double>::zeros(4, 1);

	// Execute
	auto score = problem.Evaluate(parameters, errors);

	// Evaluate
	ASSERT_GT(score, 1e-14);
}