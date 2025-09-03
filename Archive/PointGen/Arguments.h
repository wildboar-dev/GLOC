//--------------------------------------------------
// The list of arguments that serve as input for the application
//
// @author: Wild Boar
//
// @date: 2024-03-06
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
		double _focal;
		Size _imageSize;
		Vec3d _rvec_1;
		Vec3d _tvec_1;
		Vec3d _rvec_2;
		Vec3d _tvec_2;
		double _blockSize;
		int _gridSize;
	public:
		Arguments(double focal, Size& imageSize, Vec3d& rvec_1, Vec3d& tvec_1, Vec3d& rvec_2, Vec3d& tvec_2, double blockSize, int gridSize) :
			_focal(focal), _imageSize(imageSize), _rvec_1(rvec_1), _tvec_1(tvec_1), _rvec_2(rvec_2), _tvec_2(tvec_2), _blockSize(blockSize), _gridSize(gridSize) {}

		inline double& GetFocal() { return _focal; }
		inline Size& GetImageSize() { return _imageSize; }
		inline Vec3d& GetRvec_1() { return _rvec_1; }
		inline Vec3d& GetTvec_1() { return _tvec_1; }
		inline Vec3d& GetRvec_2() { return _rvec_2; }
		inline Vec3d& GetTvec_2() { return _tvec_2; }
		inline double& GetBlockSize() { return _blockSize; }
		inline int& GetGridSize() { return _gridSize; }
	};
}