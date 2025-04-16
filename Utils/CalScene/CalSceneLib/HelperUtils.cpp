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
 */
void HelperUtils::WriteMeta(const string& path, Arguments * arguments)
{
	throw runtime_error("Not implemented");
}