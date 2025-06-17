//--------------------------------------------------
// Implementation of class ArgumentLoader
//
// @author: Wild Boar
//
// @date: 2025-06-17
//--------------------------------------------------

#include "ArgumentLoader.h"
using namespace NVL_App;

//--------------------------------------------------
// Loader
//--------------------------------------------------

/**
 * @brief Load the associated arguments from disk
 * @param pathHelper Defines where the path needs to be loaded from
 * @return unique_ptr<Arguments> Returns a unique_ptr<Arguments>
 */
unique_ptr<Arguments> ArgumentLoader::Load(NVLib::PathHelper * pathHelper)
{
	auto path = pathHelper->GetPath("Meta", "arguments.xml");

	auto reader = FileStorage(path, FileStorage::READ | FileStorage::FORMAT_XML);
	if (!reader.isOpened()) throw runtime_error("Failed to open the arguments file: " + path);

	Size gridSize; reader["grid_size"] >> gridSize;
	int blockSize; reader["block_size"] >> blockSize;
	Size imageSize; reader["image_size"] >> imageSize;
	Vec4d distortion; reader["distortion"] >> distortion;
	Vec2d center; reader["center"] >> center;
	double baseLine; reader["base_line"] >> baseLine;
	Vec3d rvec; reader["rvec"] >> rvec;
	Vec3d tvec; reader["tvec"] >> tvec;
	double distance; reader["distance"] >> distance;

	reader.release();

	return make_unique<Arguments>(gridSize, blockSize, imageSize, distortion, center, baseLine, rvec, tvec, distance);
}