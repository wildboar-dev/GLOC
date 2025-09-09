//--------------------------------------------------
// Unit Tests for class CostFunction
//
// @author: Wild Boar
//
// @date: 2025-09-09
//--------------------------------------------------

#include <gtest/gtest.h>

#include <MemeLib/CostFunction.h>
using namespace NVL_App;

#include "../Helpers/TestHelpers.h"

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Find the error that we are dealing with
 */
TEST(CostFunction_Test, calculate_error)
{
	// Setup
	auto gridSize = Size(4, 7);
	auto scenePoints = vector<Point3d>(); TestHelpers::BuildGrid(gridSize.width, gridSize.height, scenePoints);
	auto cameraMatrix = TestHelpers::BuildCameraMatrix(800.0, 800.0, Point2d(640.0, 480.0));

	auto points_1 = TestHelpers::BuildTestPoints(scenePoints, cameraMatrix);
	auto points_2 = TestHelpers::ApplyDistortion(cameraMatrix, Vec4d(-0.1, 0.0, 0.0, 0.0), points_1.get());
	auto points_3 = TestHelpers::ApplyDistortion(cameraMatrix, Vec4d(0.1, 0.0, 0.0, 0.0), points_1.get());
	auto points_4 = TestHelpers::ApplyDistortion(cameraMatrix, Vec4d(0.2, 0.0, 0.0, 0.0), points_1.get());
	auto points_5 = TestHelpers::ApplyDistortion(cameraMatrix, Vec4d(0.3, 0.0, 0.0, 0.0), points_1.get());	

	auto errors_1 = vector<double>();
	auto errors_2 = vector<double>();
	auto errors_3 = vector<double>();
	auto errors_4 = vector<double>();
	auto errors_5 = vector<double>();

	// Execute
	auto score_1 = CostFunction::CalculateError(gridSize, points_1.get(), errors_1);
	auto score_2 = CostFunction::CalculateError(gridSize, points_2.get(), errors_2);
	auto score_3 = CostFunction::CalculateError(gridSize, points_3.get(), errors_3);
	auto score_4 = CostFunction::CalculateError(gridSize, points_4.get(), errors_4);
	auto score_5 = CostFunction::CalculateError(gridSize, points_5.get(), errors_5);

	// Confirm

	// Teardown
}
