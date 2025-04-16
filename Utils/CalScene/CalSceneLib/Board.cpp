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
Board::Board(Mat & camera, Mat & pose, vector<Point3d> & scenePoints) : _camera(camera), _pose(pose), _scenePoints(scenePoints)
{
	auto rvec = Vec3d(), tvec = Vec3d(); NVLib::PoseUtils::Pose2Vectors(_pose, rvec, tvec);
	projectPoints(scenePoints, rvec, tvec, camera, noArray(), _imagePoints);
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
	// Draw the points
	for (size_t i = 0; i < _imagePoints.size(); i++)
	{
		circle(image, _imagePoints[i], 5, Scalar(0, 0, 255), FILLED, LINE_AA);
		circle(image, _imagePoints[i], 1, Scalar::all(255), FILLED);
	}
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
	for (size_t i = 0; i < _imagePoints.size(); i++)
	{
		writer << _imagePoints[i].x << " " << _imagePoints[i].y << endl;
	}
}
