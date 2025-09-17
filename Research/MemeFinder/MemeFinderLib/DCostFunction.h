//--------------------------------------------------
// A set of utilities for helping the application
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <MemeLib/Points.h>

#include <OptLib/Common/CostBase.h>

#include "Parameters.h"

namespace NVL_App
{
    class DCostFunction : public CostBase
    {
    private:
        Mat _camera;
        Parameters * _parameters;
        Points * _points;
    public:
        DCostFunction(Mat& camera, vector<int>& indices, Points * points);
        ~DCostFunction();
        
        virtual double Evaluate(const Eigen::VectorXd& inputs) override;
    };
}