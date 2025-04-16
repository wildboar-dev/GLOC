//--------------------------------------------------
// Implementation of class PoseHelper
//
// @author: Wild Boar
//
// @date: 2025-04-16
//--------------------------------------------------

#include "PoseHelper.h"
using namespace NVL_App;


//--------------------------------------------------
// Main
//--------------------------------------------------

/**
 * @brief Derives the pose of a board using the input arguments
 * @param arguments The arguments that we are generating the psoe from
 * @param boardIndex The identifier of the board that we are generating the index for
 * @return Mat Returns a Mat
 */
Mat PoseHelper::FindPose(Arguments * arguments, int boardIndex)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Rotation
//--------------------------------------------------

/**
 * @brief Retrieve the rotation around the X-axis
 * @param angle The angle to rotation (in degrees)
 * @return Mat Returns a Mat
 */
Mat PoseHelper::GetRotX(double angle)
{
	throw runtime_error("Not implemented");
}

/**
 * @brief Retrieve the rotation around the Y-axis
 * @param angle The angle to rotation (in degrees)
 * @return Mat Returns a Mat
 */
Mat PoseHelper::GetRotY(double angle)
{
	throw runtime_error("Not implemented");
}

/**
 * @brief Retrieve the rotation around the Z-axis
 * @param angle The angle to rotation (in degrees)
 * @return Mat Returns a Mat
 */
Mat PoseHelper::GetRotZ(double angle)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Translation
//--------------------------------------------------

/**
 * @brief Retrieve the translation of the board
 * @param rotation The rotation component of the board pose
 * @param arguments The arguments that are generating the rendering from
 * @return Mat Returns a Mat
 */
Mat PoseHelper::GetTranslation(Mat& rotation, Arguments * arguments)
{
	throw runtime_error("Not implemented");
}
