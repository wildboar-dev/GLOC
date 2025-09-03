//--------------------------------------------------
// The incomming arguments outlining how the calibration model needs to be rendered
//
// @author: Wild Boar
//
// @date: 2025-04-16
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
		double _blockSize;
		Vec2i _gridSize;
		Vec2d _shiftXY;
		Vec2d _RotYZ;
		double _angle;
		double _distance;
		Vec2d _focals;
		Point2d _center;
		Vec4d _distortion;
		Size _imageSize;
		int _decimals;

	public:
		Arguments(double blockSize, const Vec2i& gridSize, const Vec2d& shiftXY, const Vec2d& RotYZ, double angle, double distance, const Vec2d& focals, const Point2d& center, const Vec4d& distortion, Size& imageSize, int decimals) :
			_blockSize(blockSize), _gridSize(gridSize), _shiftXY(shiftXY), _RotYZ(RotYZ), _angle(angle), _distance(distance), _focals(focals), _center(center), _distortion(distortion), _imageSize(imageSize), _decimals(decimals) {}

		inline double& GetBlockSize() { return _blockSize; }
		inline Vec2i& GetGridSize() { return _gridSize; }
		inline Vec2d& GetShiftXY() { return _shiftXY; }
		inline Vec2d& GetRotYZ() { return _RotYZ; }
		inline double& GetAngle() { return _angle; }
		inline double& GetDistance() { return _distance; }
		inline Vec2d& GetFocals() { return _focals; }
		inline Point2d& GetCenter() { return _center; }
		inline Size& GetImageSize() { return _imageSize; }
		inline int& GetDecimals() { return _decimals; }
		inline Vec4d& GetDistortion() { return _distortion; }
	};
}