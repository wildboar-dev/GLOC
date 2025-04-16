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
		int _index;
		double _blockSize;
		Vec2i _gridSize;
		Vec2d _shiftXY;
		Vec2d _RotYZ;
		double _angle;
		double _distance;
		Vec2d _focals;
		Point2d _center;
		Size _imageSize;
		int _decimals;
		string _folder;

	public:
		Arguments(int index, double blockSize, Vec2i& gridSize, Vec2d& shiftXY, Vec2d& RotYZ, double angle, double distance, Vec2d& focals, Point2d& center, Size& imageSize, int decimals, const string& folder) :
			_index(index), _blockSize(blockSize), _gridSize(gridSize), _shiftXY(shiftXY), _RotYZ(RotYZ), _angle(angle), _distance(distance), _focals(focals), _center(center), _imageSize(imageSize), _decimals(decimals), _folder(folder) {}

		inline int& GetIndex() { return _index; }
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
		inline string& GetFolder() { return _folder; }
	};
}