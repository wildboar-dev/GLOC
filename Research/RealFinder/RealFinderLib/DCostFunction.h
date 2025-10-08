//--------------------------------------------------
// A set of utilities for helping the application
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#pragma once

#include <random>
#include <chrono>
#include <iostream>
using namespace std;

#include <OptLib/Common/CostBase.h>

#include "Points.h"
#include "HelperUtils.h"
#include "CostFunction.h"

#include "Parameters.h"

namespace NVL_App
{
    class DCostFunction : public CostBase
    {
    private:
        Mat _camera;
        Parameters * _parameters;
        Points * _dPoints;
    public:
        DCostFunction(Mat& camera, const vector<int>& indices, Points * dPoints);
        ~DCostFunction();
        
        virtual double Evaluate(const Eigen::VectorXd& inputs) override;
    };
}