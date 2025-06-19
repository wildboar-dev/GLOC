//--------------------------------------------------
// Implementation of class Grid
//
// @author: Wild Boar
//
// @date: 2025-06-19
//--------------------------------------------------

#include "Grid.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param arguments The input arguments that we are adding
 * @param problemState The problem state to which this grid belongs
 */
Grid::Grid(Arguments * arguments, ProblemState * problemState)
{
	auto basePoints = vector<Point3d>(); MakeVanillaScenePoints(arguments, basePoints);
	NVLib::Math3D::TransformPointSet(problemState->GetPose(), basePoints, _scenePoints);
	projectPoints(_scenePoints, Vec3d(), Vec3d(), problemState->GetCamera(), problemState->GetDistortion(), _imagePoints);
}

//--------------------------------------------------
// Is Valid
//--------------------------------------------------

/**
 * @brief Determine whether the grid is valid
 * @return bool Returns a bool
 */
bool Grid::IsValid()
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Render
//--------------------------------------------------

/**
 * @brief Render the grid on an image
 * @param image The image that we are rendering upon
 */
void Grid::Render(Mat& image)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Error
//--------------------------------------------------

/**
 * @brief Generate the errors of the grids wrt a homography
 * @return Mat Returns a Mat
 */
Mat Grid::GetErrors()
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Helper Methods
//--------------------------------------------------

/**
 * @brief Generate the scene points for a vanilla grid
 * @param arguments The input arguments that we are adding
 * @param scenePoints The vector of scene points to be filled
 */
void Grid::MakeVanillaScenePoints(Arguments * arguments, vector<Point3d>& scenePoints)
{
	auto width = arguments->GetGridSize().width;
	auto height = arguments->GetGridSize().height;
	auto gridSpacing = arguments->GetBlockSize();

	// Generate a simple grid of points in 3D space
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			scenePoints.push_back(Point3d(i * gridSpacing, j * gridSpacing, 0));
		}
	}
}

/**
 * @brief Validate the points in the grid
 * @return bool Returns a bool indicating whether the points are valid
 */
bool Grid::ValidatePoints(Arguments * arguments) 
{

}
