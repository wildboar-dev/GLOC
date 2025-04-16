//--------------------------------------------------
// Implementation of class Board
//
// @author: Wild Boar
//
// @date: 2025-04-16
//--------------------------------------------------

#include "Board.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param camera The camera matrix
 * @param pose The pose matrix
 * @param scenePoints The 3D points in the scene
 */
Board::Board(Mat & camera, Mat & pose, vector<Point3d> & scenePoints)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Render
//--------------------------------------------------

/**
 * @brief Render the board on an image
 * @param image The image that we are rendering the board upon
 */
void Board::Render(Mat& image)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Save
//--------------------------------------------------

/**
 * @brief Save the board points to a stream
 * @param writer The writer that we are saving to
 */
void Board::Save(ostream& writer)
{
	throw runtime_error("Not implemented");
}
