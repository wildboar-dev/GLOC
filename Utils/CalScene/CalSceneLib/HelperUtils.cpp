//--------------------------------------------------
// Implementation of class HelperUtils
//
// @author: Wild Boar
//
// @date: 2025-04-16
//--------------------------------------------------

#include "HelperUtils.h"
using namespace NVL_App;

//--------------------------------------------------
// Scene Points
//--------------------------------------------------

/**
 * @brief Generate the set of scene points for the calibration
 * @param arguments The arguments that we need to generate the scene points correctly
 * @return vector<Point3d> Returns a vector<Point3d>
 */
vector<Point3d> HelperUtils::GetScenePoints(Arguments * arguments)
{
	auto result = vector<Point3d>();

	for (auto row = 0; row < arguments->GetGridSize()[0]; row++)
	{
		for (auto col = 0; col < arguments->GetGridSize()[1]; col++)
		{
			auto x = col * arguments->GetBlockSize();
			auto y = row * arguments->GetBlockSize();
			auto z = 0.0;
			result.push_back(Point3d(x, y, z));
		}
	}

	return result;
}

//--------------------------------------------------
// Camera Matrix
//--------------------------------------------------

/**
 * @brief Generate a camera matrix from the provided arguments
 * @param arguments The arguments that we are basing our camera matrix off
 * @return Mat Returns a Mat
 */
Mat HelperUtils::GetCameraMatrix(Arguments * arguments)
{
	auto fx = arguments->GetFocals()[0];
	auto fy = arguments->GetFocals()[1];
	auto cx = arguments->GetCenter().x;
	auto cy = arguments->GetCenter().y;
	return Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1;
}

//--------------------------------------------------
// Meta Writer
//--------------------------------------------------

/**
 * @brief Write meta data to disk
 * @param path The path of the file that we are rendering to
 * @param arguments The arguments that we are rendering (partially)
 * @param pose_1 The first pose that we are rendering
 * @param pose_2 The second pose that we are rendering
 */
void HelperUtils::WriteMeta(const string& path, Arguments * arguments, Mat& pose_1, Mat& pose_2)
{
	auto writer = FileStorage(path, FileStorage::WRITE | FileStorage::FORMAT_XML);
	if (!writer.isOpened()) throw runtime_error("Could not open the file for writing: " + path);

	writer << "focal" << arguments->GetFocals();
	writer << "imageSize" << arguments->GetImageSize();

	auto rvec_1 = Vec3d(), tvec_1 = Vec3d(); NVLib::PoseUtils::Pose2Vectors(pose_1, rvec_1, tvec_1);
	writer << "rvec_1" << rvec_1; writer << "tvec_1" << tvec_1;

	auto rvec_2 = Vec3d(), tvec_2 = Vec3d(); NVLib::PoseUtils::Pose2Vectors(pose_2, rvec_2, tvec_2);
	writer << "rvec_2" << rvec_2; writer << "tvec_2" << tvec_2;

	writer << "blockSize" << arguments->GetBlockSize();
	writer << "gridSize" << arguments->GetGridSize();

	writer.release();
}

//--------------------------------------------------
// Generate the folder structure
//--------------------------------------------------

/**
 * @brief Create the folder structure for the calibration
 * @param databasePath The path where the database is located
 * @param folder The folder that we are creating
 */
void HelperUtils::CreateFolders(const string& databasePath, const string& folder) 
{
	// Add the base folder
	auto basePath = NVLib::FileUtils::PathCombine(databasePath, folder);
	if (!NVLib::FileUtils::Exists(basePath)) NVLib::FileUtils::AddFolder(basePath);

	// Add meta folder
	auto metaPath = NVLib::FileUtils::PathCombine(basePath, "Meta");
	if (!NVLib::FileUtils::Exists(metaPath)) NVLib::FileUtils::AddFolder(metaPath);

	// Add image folder
	auto imagePath = NVLib::FileUtils::PathCombine(basePath, "Image");
	if (!NVLib::FileUtils::Exists(imagePath)) NVLib::FileUtils::AddFolder(imagePath);

	// Add point folder
	auto pointPath = NVLib::FileUtils::PathCombine(basePath, "Point");
	if (!NVLib::FileUtils::Exists(pointPath)) NVLib::FileUtils::AddFolder(pointPath);
}

//--------------------------------------------------
// Write Points
//--------------------------------------------------

/**
 * Add the logic to write points to disk
 * @param paths The path where the points are being written
 * @param precision The precision of the points
 * @param scenePoints The scene points that we are writing
 * @param imagePoints_1 The first set of image points
 * @param imagePoints_2 The second set of image points
 */
void HelperUtils::WritePoints(const string& path, int precision, const vector<Point3d>& scenePoints, vector<Point2d>& imagePoints_1, vector<Point2d>& imagePoints_2) 
{
	auto writer = ofstream(path);

	// Write the point count
	writer << scenePoints.size() << endl;

	writer << endl;

	// Write the scene points
	for (auto point : scenePoints)
	{
		writer << fixed << setprecision(precision) << point.x << " " << point.y << " " << point.z << endl;
	}

	writer << endl;

	// Write the image points 1
	for (auto point : imagePoints_1)
	{
		writer << fixed << setprecision(precision) << point.x << " " << point.y << endl;
	}

	writer << endl;

	// Write the image points 2
	for (auto point : imagePoints_2)
	{
		writer << fixed << setprecision(precision) << point.x << " " << point.y << endl;
	}

	writer.close();
}
