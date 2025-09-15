//--------------------------------------------------
// Unit Tests for class MetaLoader
//
// @author: Wild Boar
//
// @date: 2025-09-16
//--------------------------------------------------

#include <gtest/gtest.h>

#include <MemeLib/MetaLoader.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Confirm that we are able to load meta data properly
 */
TEST(MetaLoader_Test, load_meta)
{
	// Setup the meta data that we want to test with
	Mat cameraMatrix = (Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
	auto imageSize = Size(640, 480);
	auto gridSize = Size(8, 6);
	double blockSize = 25.0;

	// Save the meta data to disk
	auto writer = FileStorage("meta.xml", FileStorage::WRITE | FileStorage::FORMAT_XML);
	writer << "cameraMatrix" << cameraMatrix;
	writer << "imageSize" << imageSize;
	writer << "gridSize" << gridSize;
	writer << "blockSize" << blockSize;
	writer.release();

	// Execute
	auto metaData = MetaLoader::Load("meta.xml");

	// Get the camera matrix stuff
	Mat actualCameraMatrix = metaData->GetCameraMatrix();

	// Confirm
	ASSERT_NE(metaData, nullptr);
	ASSERT_EQ(actualCameraMatrix.rows, cameraMatrix.rows);
	ASSERT_EQ(actualCameraMatrix.cols, cameraMatrix.cols);
	ASSERT_EQ(metaData->GetImageSize(), imageSize);
	ASSERT_EQ(metaData->GetGridSize(), gridSize);
	ASSERT_EQ(metaData->GetBlockSize(), blockSize);
}
