//--------------------------------------------------
// Implementation of class PointLoader
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#include "PointLoader.h"
using namespace NVL_App;

//--------------------------------------------------
// Load
//--------------------------------------------------

/**
 * @brief Add the functionality to load points from disk
 * @param path The path that we are loading from
 * @return unique_ptr<Points> Returns a unique_ptr<Points>
 */
unique_ptr<Points> PointLoader::Load(const string& path)
{
	auto reader = ifstream(path);  if (!reader.is_open()) { throw runtime_error("Could not open file: " + path); }

	auto pointCount = 0; reader >> pointCount;

	auto scenePoints = vector<Point3d>(pointCount);
	for (auto i = 0; i < pointCount; ++i)
	{
		auto x = 0.0, y = 0.0, z = 0.0;
		reader >> x >> y >> z;
		scenePoints[i] = Point3d(x, y, z);
	}

	auto imagePoints_1 = vector<Point2d>(pointCount);
	for (auto i = 0; i < pointCount; ++i)
	{
		auto x = 0.0, y = 0.0;
		reader >> x >> y;
		imagePoints_1[i] = Point2d(x, y);
	}

	auto imagePoints_2 = vector<Point2d>(pointCount);
	for (auto i = 0; i < pointCount; ++i)
	{
		auto x = 0.0, y = 0.0;
		reader >> x >> y;
		imagePoints_2[i] = Point2d(x, y);
	}

	reader.close();

	return make_unique<Points>(scenePoints, imagePoints_1, imagePoints_2);
}