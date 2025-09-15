//--------------------------------------------------
// A container for Meme Data
//
// @author: Wild Boar
//
// @date: 2025-09-16
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace NVL_App
{
	class MetaData
	{
	private:
		Mat _cameraMatrix;
		Size _imageSize;
		Size _gridSize;
		double _blockSize;
	public:
		MetaData(Mat& cameraMatrix, Size& imageSize, Size& gridSize, double blockSize) :
			_cameraMatrix(cameraMatrix), _imageSize(imageSize), _gridSize(gridSize), _blockSize(blockSize) {}

		inline Mat& GetCameraMatrix() { return _cameraMatrix; }
		inline Size& GetImageSize() { return _imageSize; }
		inline Size& GetGridSize() { return _gridSize; }
		inline double& GetBlockSize() { return _blockSize; }
	};
}
