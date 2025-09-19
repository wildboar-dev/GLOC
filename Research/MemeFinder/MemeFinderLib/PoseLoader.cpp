//--------------------------------------------------
// Implementation of class PoseLoader
//
// @author: Wild Boar
//
// @date: 2025-09-19
//--------------------------------------------------

#include "PoseLoader.h"
using namespace NVL_App;

//--------------------------------------------------
// Loader
//--------------------------------------------------

/**
 * @brief Add the logic to load pose data from disk
 * @param path Path to the pose data file
 * @return Mat Returns a Mat
 */
unique_ptr<Pose> PoseLoader::LoadPose(const string& path)
{
	auto reader = FileStorage(path, FileStorage::READ | FileStorage::FORMAT_XML);
	if (!reader.isOpened()) throw runtime_error("Could not open pose file: " + path);

	Vec3d rvec, tvec;
	reader["rvec"] >> rvec;
	reader["tvec"] >> tvec;
	reader.release();

	return make_unique<Pose>(rvec, tvec);
}
