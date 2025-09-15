//--------------------------------------------------
// Implementation of class MetaLoader
//
// @author: Wild Boar
//
// @date: 2025-09-16
//--------------------------------------------------

#include "MetaLoader.h"
using namespace NVL_App;

//--------------------------------------------------
// Load
//--------------------------------------------------

/**
 * @brief Load meta data from disk
 * @param filePath Path to the meta data file
 * @return unique_ptr<MetaData> Returns a unique_ptr<MetaData>
 */
unique_ptr<MetaData> MetaLoader::Load(const string& filePath)
{
	// Open the file
	auto fs = FileStorage(filePath, FileStorage::READ | FileStorage::FORMAT_XML);
	if (!fs.isOpened()) return nullptr;

	// Read the data
	Mat cameraMatrix;
	Size imageSize;
	Size gridSize;
	double blockSize;
	fs["cameraMatrix"] >> cameraMatrix;
	fs["imageSize"] >> imageSize;
	fs["gridSize"] >> gridSize;
	fs["blockSize"] >> blockSize;
	fs.release();

	// Create the meta data object
	return make_unique<MetaData>(cameraMatrix, imageSize, gridSize, blockSize);
}
