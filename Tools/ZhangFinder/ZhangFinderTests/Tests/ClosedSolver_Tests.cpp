//--------------------------------------------------
// Unit Tests for class ClosedSolver
//
// @author: Wild Boar
//
// @date: 2025-04-17
//--------------------------------------------------

#include <gtest/gtest.h>

#include <ZhangFinderLib/ZhangUtils.h>
#include <ZhangFinderLib/PointLoader.h>
#include <ZhangFinderLib/ClosedSolver.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Confirms the operation of the algorithm
 */
TEST(ClosedSolver_Test, match_test)
{
	// Setup: Load Points
	auto points = PointLoader::Load("points_0000.txt");

	// Setup: Homographies
	Mat H_1 = ZhangUtils::FindHomography(points.get(), 0);
	Mat H_2 = ZhangUtils::FindHomography(points.get(), 1);

	// Setup: Setup the solver
	auto solver = ClosedSolver();
	solver.AddHomography(H_1);
	solver.AddHomography(H_2);

	// Execute
	Mat K = solver.FindK();
	auto klink = (double *)K.data;

	// Evaluate
	ASSERT_NEAR(klink[0], 1000.0, 1e-2);
	ASSERT_NEAR(klink[4], 1000.0, 1e-2);
	ASSERT_NEAR(klink[2], 500.0, 1e-2);
	ASSERT_NEAR(klink[5], 500.0, 1e-2);
}
