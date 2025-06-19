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
 * @param gridNumber The number associated with the grid
 */
Grid::Grid(Arguments * arguments, ProblemState * problemState, int gridNumber)
{
	throw runtime_error("Not implemented");
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