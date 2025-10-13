//--------------------------------------------------
// Implementation of class LoadUtils
//
// @author: Wild Boar
//
// @date: 2025-10-13
//--------------------------------------------------

#include "LoadUtils.h"
using namespace NVL_App;

//--------------------------------------------------
// Distortion
//--------------------------------------------------

/**
 * @brief Loads distortion parameters from disk
 * @param path The path to the distortion parameters file	
 * @return unique_ptr<DParams> Returns a unique_ptr<DParams>
 */
unique_ptr<DParams> LoadUtils::LoadDParams(const string& path)
{
	auto reader = FileStorage(path, FileStorage::READ | FileStorage::FORMAT_XML);
	if (!reader.isOpened()) throw runtime_error("Could not open distortion parameters file: " + path);

	Mat camera, distortion;
	reader["CameraMatrix"] >> camera;
	reader["DistCoeffs"] >> distortion;
	reader.release();

	return make_unique<DParams>(camera, distortion);
}