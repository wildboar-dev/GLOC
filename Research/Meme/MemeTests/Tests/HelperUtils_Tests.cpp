//--------------------------------------------------
// Unit Tests for class HelperUtils
//
// @author: Wild Boar
//
// @date: 2025-09-09
//--------------------------------------------------

#include <gtest/gtest.h>

#include <MemeLib/HelperUtils.h>
using namespace NVL_App;

#include <MemeLib/DistortCost.h>
#include <MemeLib/CostFunction.h>
#include <MemeLib/HelperUtils.h>
using namespace NVL_App;

#include <OptLib/Helpers/Tracker.h>
#include <OptLib/GradientDescent/GradientDescent.h>

#include "../Helpers/TestHelpers.h"

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Undistort the associated pointset that has been provided
 */
TEST(HelperUtils_Test, undistort)
{
	// Setup
	auto gridSize = Size(4, 7);
	auto scenePoints = vector<Point3d>(); TestHelpers::BuildGrid(gridSize.width, gridSize.height, scenePoints);
	auto cameraMatrix = TestHelpers::BuildCameraMatrix(800.0, 800.0, Point2d(640.0, 480.0));

	auto points_1 = TestHelpers::BuildTestPoints(scenePoints, cameraMatrix);
	auto points_2 = TestHelpers::ApplyDistortion(cameraMatrix, Vec4d(0.3, 0.0, 0.0, 0.0), points_1.get());	

	// Execute
	auto upoints_1 = HelperUtils::Undistort(cameraMatrix, Vec4d(0.0, 0.0, 0.0, 0.0), points_2.get());
	auto upoints_2 = HelperUtils::Undistort(cameraMatrix, Vec4d(0.2, 0.0, 0.0, 0.0), points_2.get());
	auto upoints_3 = HelperUtils::Undistort(cameraMatrix, Vec4d(0.3, 0.0, 0.0, 0.0), points_2.get());

	auto errors_1 = vector<double>();
	auto errors_2 = vector<double>();
	auto errors_3 = vector<double>();

	auto score_1 = CostFunction::CalculateError(upoints_1.get(), errors_1);
	auto score_2 = CostFunction::CalculateError(upoints_2.get(), errors_2);
	auto score_3 = CostFunction::CalculateError(upoints_3.get(), errors_3);

	// Confirm
	ASSERT_NEAR(0.0, score_3, 0.001);
	ASSERT_LE(score_3, score_2);
	ASSERT_LE(score_2, score_1);
}

/**
 * @brief Render the cost topography for k1 and k2
 */
TEST(HelperUtils_Test, render)
{
	// Setup
	auto gridSize = Size(4, 7);
	auto scenePoints = vector<Point3d>(); TestHelpers::BuildGrid(gridSize.width, gridSize.height, scenePoints);
	auto cameraMatrix = TestHelpers::BuildCameraMatrix(500.0, 500.0, Point2d(640.0, 480.0));
	auto cameraMatrix2 = TestHelpers::BuildCameraMatrix(450.0, 450.0, Point2d(640.0, 480.0));

	auto basePoints = TestHelpers::BuildTestPoints(scenePoints, cameraMatrix);
	auto points = TestHelpers::ApplyDistortion(cameraMatrix2, Vec4d(0.3, -0.2, 0.0, 0.0), basePoints.get());	

	// Execute
	//Mat image = HelperUtils::RenderKSpace(cameraMatrix, points.get(), Size(1280, 960), Range(-2, 2), Range(-2, 2));
	//imwrite("KSpace.tiff", image);
}

/**
 * @brief Perform a refinement of the distortion coefficients
 */
TEST(HelperUtils_Test, refinement)
{
	// Setup
	auto gridSize = Size(4, 7);
	auto scenePoints = vector<Point3d>(); TestHelpers::BuildGrid(gridSize.width, gridSize.height, scenePoints);
	auto cameraMatrix = TestHelpers::BuildCameraMatrix(500.0, 500.0, Point2d(640.0, 480.0));

	auto basePoints = TestHelpers::BuildTestPoints(scenePoints, cameraMatrix);
	auto points = TestHelpers::ApplyDistortion(cameraMatrix, Vec4d(0.3, -0.2, 0.0, 0.0), basePoints.get());	

	auto distCost = NVL_App::DistortCost(points.get(), cameraMatrix);
	auto initial = Eigen::Vector2d(-1.8, 2.0);

	auto tracker = NVL_App::Tracker();

	auto result = NVL_App::GradientDescent::Solve(&distCost, initial, 2e3, 1e-5, &tracker);

	cout << "Result: " << result.transpose() << endl;

	auto path = vector<Point2d>(); for (auto step : tracker.GetPath()) path.push_back(Point2d(step[0], step[1]));

	auto image = HelperUtils::RenderPath(Size(1280, 960), path, Range(-2, 2), Range(-2, 2));
	imwrite("Path.png", image);
}