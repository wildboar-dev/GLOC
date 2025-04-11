//--------------------------------------------------
// MetaData
//
// @author: Wild Boar
//
// @date: 2025-04-11
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

namespace name
{
    class MetaData
    {
    private:
        double _focal;
        Size _imageSize;
        Vec3d _rvec;
        Vec3d _tvec;
        int _blockSize;
        int _gridSize;
    public:
        MetaData(double focal, Size imageSize, Vec3d rvec, Vec3d tvec, int blockSize, int gridSize)
            : _focal(focal), _imageSize(imageSize), _rvec(rvec), _tvec(tvec), _blockSize(blockSize), _gridSize(gridSize) {}

        double GetFocal() { return _focal; }
        Size& GetImageSize() { return _imageSize; }
        Vec3d& GetRVec() { return _rvec; }
        Vec3d& GetTVec() { return _tvec; }
        int GetBlockSize() { return _blockSize; }
        int GetGridSize() { return _gridSize; }
    };
} 