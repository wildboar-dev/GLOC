//--------------------------------------------------
// Represents a human pose in 3D space.
//
// @author: Wild Boar
//
// @date: 2025-09-19
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include <NVLib/PoseUtils.h>

namespace NVL_App
{
	class Pose
	{
	private:
		Vec3d _rvec;
		Vec3d _tvec;
	public:
		Pose(Vec3d& rvec, Vec3d& tvec) : _rvec(rvec), _tvec(tvec) {}

		inline Mat GetPoseMatrix() { return NVLib::PoseUtils::Vectors2Pose(_rvec, _tvec); }

		inline Vec3d& GetRvec() { return _rvec; }
		inline Vec3d& GetTvec() { return _tvec; }
	};
}
