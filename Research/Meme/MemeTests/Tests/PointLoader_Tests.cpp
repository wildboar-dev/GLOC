//--------------------------------------------------
// Unit Tests for Point Loader
//
// @author: Wild Boar
//
// @date: 2025-09-05
//--------------------------------------------------

#include <gtest/gtest.h>

#include <MemeLib/PointLoader.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Confirms that basic point loading functionality works as expected
 */
TEST(PointLoader_Test, basic_load_functionality)
{
	// Setup the reader
	auto testData = stringstream();
	testData << "X,Y,Z,u1,v1,u2,v2" << endl;
	testData << "0.00000000,0.00000000,0.00000000,518.40000000,115.20000000,640.00000000,115.20000000" << endl;
	testData << "50.00000000,0.00000000,0.00000000,424.80761570,101.82965939,729.13560410,101.82965939" << endl;
	testData << "100.00000000,0.00000000,0.00000000,324.09368076,87.44195439,825.05363737,87.44195439" << endl;

	// Load the points
	auto points = PointLoader::Load(testData);

	ASSERT_NE(points, nullptr);
	ASSERT_EQ(points->GetScenePoints().size(), 3);
	ASSERT_EQ(points->GetImagePoints_1().size(), 3);
	ASSERT_EQ(points->GetImagePoints_2().size(), 3);

	// Validate the points
	auto scenePoints = points->GetScenePoints();
	EXPECT_DOUBLE_EQ(scenePoints[0].x, 0.0);
	EXPECT_DOUBLE_EQ(scenePoints[0].y, 0.0);
	EXPECT_DOUBLE_EQ(scenePoints[0].z, 0.0);
	EXPECT_DOUBLE_EQ(scenePoints[1].x, 50.0);
	EXPECT_DOUBLE_EQ(scenePoints[1].y, 0.0);
	EXPECT_DOUBLE_EQ(scenePoints[1].z, 0.0);
	EXPECT_DOUBLE_EQ(scenePoints[2].x, 100.0);
	EXPECT_DOUBLE_EQ(scenePoints[2].y, 0.0);
	EXPECT_DOUBLE_EQ(scenePoints[2].z, 0.0);

	auto imagePoints1 = points->GetImagePoints_1();
	EXPECT_DOUBLE_EQ(imagePoints1[0].x, 518.4);
	EXPECT_DOUBLE_EQ(imagePoints1[0].y, 115.2);
	EXPECT_DOUBLE_EQ(imagePoints1[1].x, 424.80761570);
	EXPECT_DOUBLE_EQ(imagePoints1[1].y, 101.82965939);
	EXPECT_DOUBLE_EQ(imagePoints1[2].x, 324.09368076);
	EXPECT_DOUBLE_EQ(imagePoints1[2].y, 87.44195439);

	auto imagePoints2 = points->GetImagePoints_2();
	EXPECT_DOUBLE_EQ(imagePoints2[0].x, 640.0);
	EXPECT_DOUBLE_EQ(imagePoints2[0].y, 115.2);
	EXPECT_DOUBLE_EQ(imagePoints2[1].x, 729.13560410);
	EXPECT_DOUBLE_EQ(imagePoints2[1].y, 101.82965939);
	EXPECT_DOUBLE_EQ(imagePoints2[2].x, 825.05363737);
}