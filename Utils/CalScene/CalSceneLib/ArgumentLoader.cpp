//--------------------------------------------------
// Implementation of class ArgumentLoader
//
// @author: Wild Boar
//
// @date: 2025-04-16
//--------------------------------------------------

#include "ArgumentLoader.h"
using namespace NVL_App;

//--------------------------------------------------
// Loader
//--------------------------------------------------

/**
 * @brief Load arguments from disk
 * @param path The path that we are loading from
 * @return unique_ptr<Arguments> Returns a unique_ptr<Arguments>
 */
unique_ptr<Arguments> ArgumentLoader::Load(const string& path)
{
	auto reader = FileStorage(path, FileStorage::READ & FileStorage::FORMAT_XML);
	if (!reader.isOpened()) throw runtime_error("Failed to open file: " + path);
	
	int blockSize; reader["block_size"] >> blockSize;
	Vec2i gridSize; reader["grid_size"] >> gridSize;
	Vec2d shiftXY; reader["shift_xy"] >> shiftXY;
	Vec2d RotYZ; reader["rot_yz"] >> RotYZ;
	double angle; reader["angle"] >> angle;
	double distance; reader["distance"] >> distance;
	Vec2d focals; reader["focals"] >> focals;
	Point2d center; reader["center"] >> center;
	Size imageSize; reader["image_size"] >> imageSize;
	int decimals; reader["decimals"] >> decimals;
	string folder; reader["folder"] >> folder;
	int index; reader["index"] >> index;

	reader.release();
	
	return make_unique<Arguments>(index, blockSize, gridSize, shiftXY, RotYZ, angle, distance, focals, center, imageSize, decimals, folder);
}