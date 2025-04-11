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

namespace NVL_App
{
    class MetaData
    {
    private:
        double _focal;
        Size _imageSize;
        Vec3d _rvec_1;
        Vec3d _tvec_1;
        Vec3d _rvec_2;
        Vec3d _tvec_2;
        int _blockSize;
        int _gridSize;
    public:
        MetaData(double focal, Size imageSize, Vec3d rvec_1, Vec3d tvec_1, Vec3d rvec_2, Vec3d tvec_2, int blockSize, int gridSize)
            : _focal(focal), _imageSize(imageSize), _rvec_1(rvec_1), _tvec_1(tvec_1), _rvec_2(rvec_2), _tvec_2(tvec_2), _blockSize(blockSize), _gridSize(gridSize) {}

        double GetFocal() { return _focal; }
        Size& GetImageSize() { return _imageSize; }
        Vec3d& GetRVec_1() { return _rvec_1; }
        Vec3d& GetTVec_1() { return _tvec_1; }
        Vec3d& GetRVec_2() { return _rvec_2; }
        Vec3d& GetTVec_2() { return _tvec_2; }
        int GetBlockSize() { return _blockSize; }
        int GetGridSize() { return _gridSize; }
    };
} 