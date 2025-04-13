//--------------------------------------------------
// Unit Tests for class ZhangProblem
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#include <gtest/gtest.h>

#include <ZhangFinderLib/PointLoader.h>
#include <ZhangFinderLib/ZhangProblem.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Makes sure that a correct match returns essentially zero match score
 */
TEST(ZhangProblem_Test, verify_correct_match)
{
	// Setup
	auto points = PointLoader::Load("2025-04-11 23:18:40.txt");

	// Execute

	// Confirm

	// Teardown
}

/**
 * @brief Makes sure that an incorrect match returns a non-zero match score
 */
TEST(ZhangProblem_Test, verify_incorrect_match)
{
	FAIL() << "Not implemented";

	// Setup

	// Execute

	// Confirm

	// Teardown
}
