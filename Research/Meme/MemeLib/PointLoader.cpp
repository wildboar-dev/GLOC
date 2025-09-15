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
	auto points = Load(reader);
	reader.close();
	return points;
}

/**
 * @brief Add the functionality to load points from a stream
 * @param reader The stream that we are loading from
 * @return unique_ptr<Points> Returns a unique_ptr<Points>
 */
unique_ptr<Points> PointLoader::Load(istream& reader) 
{
	auto header = string(); getline(reader, header); // Read the header line

	auto scenePoints = vector<Point3d>();
	auto imagePoints_1 = vector<Point2d>();
	auto imagePoints_2 = vector<Point2d>();

	auto line = string();
	while (getline(reader, line))
	{
		auto parts = vector<string>();
		auto stream = stringstream(line);
		auto part = string();
		while (getline(stream, part, ','))
		{
			parts.push_back(part);
		}

		if (parts.size() != 7) { throw runtime_error("Invalid point format, expected 7 values per line"); }

		scenePoints.push_back(Point3d(stod(parts[0]), stod(parts[1]), stod(parts[2])));
		imagePoints_1.push_back(Point2d(stod(parts[3]), stod(parts[4])));
		imagePoints_2.push_back(Point2d(stod(parts[5]), stod(parts[6])));
	}

	return make_unique<Points>(scenePoints, imagePoints_1, imagePoints_2);
}