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
	// Determine the rotation around the Y
	auto direction = boardIndex == 0 ? -1 : 1;
	double angleY = direction * arguments->GetAngle();
	Mat rotationY = GetRotY(angleY);

	// Determine the rotation around Z and X
	Mat rotationX = GetRotX(arguments->GetRotXZ()[0]);
	Mat rotationZ = GetRotZ(arguments->GetRotXZ()[1]);

	// Compose the final rotation
	Mat rotation = rotationZ * rotationX * rotationY;
	auto rvec = Vec3d(); Rodrigues(rotation, rvec);

	// Determine the translation
	auto tvec = GetTranslation(rotation, arguments, boardIndex);

	// Return the final pose
	return NVLib::PoseUtils::Vectors2Pose(rvec, tvec);
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
	auto radians = angle * CV_PI / 180.0;
	auto rvec = Vec3d(radians, 0, 0);
	Mat rotation; Rodrigues(rvec, rotation);
	return rotation;
}

/**
 * @brief Retrieve the rotation around the Y-axis
 * @param angle The angle to rotation (in degrees)
 * @return Mat Returns a Mat
 */
Mat PoseHelper::GetRotY(double angle)
{
	auto radians = angle * CV_PI / 180.0;
	auto rvec = Vec3d(0, radians, 0);
	Mat rotation; Rodrigues(rvec, rotation);
	return rotation;
}

/**
 * @brief Retrieve the rotation around the Z-axis
 * @param angle The angle to rotation (in degrees)
 * @return Mat Returns a Mat
 */
Mat PoseHelper::GetRotZ(double angle)
{
	auto radians = angle * CV_PI / 180.0;
	auto rvec = Vec3d(0, 0, radians);
	Mat rotation; Rodrigues(rvec, rotation);
	return rotation;
}

//--------------------------------------------------
// Translation
//--------------------------------------------------

/**
 * @brief Retrieve the translation of the board
 * @param rotation The rotation component of the board pose
 * @param arguments The arguments that are generating the rendering from
 * @param boardIndex The index of the board that we are processing
 * @return Mat Returns a Mat
 */
Vec3d PoseHelper::GetTranslation(Mat& rotation, Arguments * arguments, int boardIndex)
{
	auto initialX = boardIndex == 0 ? -arguments->GetBlockSize() : arguments->GetBlockSize();
	Mat tempT = (rotation * Mat(Vec3d(initialX, 0, 0))); auto tvec = Vec3d(tempT.col(0));
	auto offset = Vec3d(arguments->GetShiftXY()[0], arguments->GetShiftXY()[1], arguments->GetDistance());
	return tvec + offset;
}