//--------------------------------------------------
// The distortion cost function
//
// @author: Wild Boar
//
// @date: 2025-09-11
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <MemeLib/Points.h>
#include <MemeLib/HelperUtils.h>
#include <MemeLib/CostFunction.h>

#include <OptLib/Common/CostBase.h>

namespace NVL_App
{
	class DistortCost : public CostBase
	{
    private:
        Points * _points;
        Mat _cameraMatrix;            
    public:

        /**
         * Constructor
         * @param points The points to use for the distortion cost
         * @param cameraMatrix The camera matrix to use for the distortion cost
         */
        DistortCost(Points * points, const Mat& cameraMatrix) : _points(points), _cameraMatrix(cameraMatrix) 
        {
            // Extra initialization if needed
        } 
    
        /**
         * Evaluate the distortion cost
         * @param inputs The input parameters (distortion coefficients)
         * @return The distortion cost
         */
        double Evaluate(const Eigen::VectorXd& inputs) override
		{
            auto distCoeffs = Vec4d(inputs[0], inputs[1], 0, 0);
            auto undistortedPoints = HelperUtils::Undistort(_cameraMatrix, distCoeffs, _points);
            auto scores = vector<double>();
            auto score = CostFunction::CalculateError(undistortedPoints.get(), scores);
            return score;
        }
	};
}
