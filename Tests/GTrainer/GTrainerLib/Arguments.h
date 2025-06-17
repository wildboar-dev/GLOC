//--------------------------------------------------
// The incomming arguments for the application
//
// @author: Wild Boar
//
// @date: 2025-06-17
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class Arguments
	{
	private:
		Size _gridSize;
		int _blockSize;
		Size _imageSize;
		Vec4d _distortion;
		Vec2d _center;
		double _baseLine;
		Vec3d _rvec;
		Vec3d _tvec;
		double _distance;
	public:
		Arguments(Size& gridSize, int blockSize, Size& imageSize, Vec4d& distortion, Vec2d& center, double baseLine, Vec3d& rvec, Vec3d& tvec, double distance) :
			_gridSize(gridSize), _blockSize(blockSize), _imageSize(imageSize), _distortion(distortion), _center(center), _baseLine(baseLine), _rvec(rvec), _tvec(tvec), _distance(distance) {}

		inline Size& GetGridSize() { return _gridSize; }
		inline int& GetBlockSize() { return _blockSize; }
		inline Size& GetImageSize() { return _imageSize; }
		inline Vec4d& GetDistortion() { return _distortion; }
		inline Vec2d& GetCenter() { return _center; }
		inline double& GetBaseLine() { return _baseLine; }
		inline Vec3d& GetRvec() { return _rvec; }
		inline Vec3d& GetTvec() { return _tvec; }
		inline double GetDistance() { return _distance; }
	};
}