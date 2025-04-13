//--------------------------------------------------
// Unit Tests for class ZhangUtils
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#include <gtest/gtest.h>

#include <ZhangFinderLib/PointLoader.h>
#include <ZhangFinderLib/ZhangUtils.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Test that the homography is valid
 */
TEST(ZhangUtils_Test, confirm_homography)
{
	// Setup: Load Points
	auto points = PointLoader::Load("2025-04-11 23:18:40.txt");

	// Setup: Calculate Homographies
	Mat H_1 = ZhangUtils::FindHomography(points.get(), 0);
	Mat H_2 = ZhangUtils::FindHomography(points.get(), 1);

	// Execute
	auto score_1 = ZhangUtils::TestHomography(H_1, points.get(), 0);
	auto score_2 = ZhangUtils::TestHomography(H_2, points.get(), 1);

	// Confirm: Check that the homographies are valid
	ASSERT_NEAR(score_1, 0.0, 0.01);
	ASSERT_NEAR(score_2, 0.0, 0.01);
}
