//--------------------------------------------------
// Unit Tests for class PointLoader
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#include <gtest/gtest.h>

#include <ZhangFinderLib/PointLoader.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Unit test to determine whether the load did as expected
 */
TEST(PointLoader_Test, confirm_load)
{
	// Setup
	auto points = PointLoader::Load("2025-04-11 23:18:40.txt");

	// Confirm the point counts
	ASSERT_EQ(points->GetScenePoints().size(), 100);
	ASSERT_EQ(points->GetImagePoints_1().size(), 100);
	ASSERT_EQ(points->GetImagePoints_2().size(), 100);

	// Confirm the last point in each collection
	ASSERT_EQ(points->GetScenePoints().back().x, 45.0);
	ASSERT_EQ(points->GetScenePoints().back().y, 45.0);
	ASSERT_EQ(points->GetScenePoints().back().z, 0.0);

	ASSERT_EQ(points->GetImagePoints_1().back().x, 90.1922);
	ASSERT_EQ(points->GetImagePoints_1().back().y, 283.786);

	ASSERT_EQ(points->GetImagePoints_2().back().x, 755.928);
	ASSERT_EQ(points->GetImagePoints_2().back().y, 367.816);
}