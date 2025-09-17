//--------------------------------------------------
// Defines a set of generated parameters
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
	class Parameters
	{
	private:
		double _k1;
		double _k2;
		double _p1;
		double _p2;
		double _cx;
		double _cy;
		Point2d _center;
		vector<double> _activeParams;
	public:
		Parameters(const Point2d& center);

		void SetValue(int index, double value);

		Mat GetDistortion();
		Point2d GetCenter();
		double GetValueDelta(int index);

		inline double& GetK1() { return _k1; }
		inline double& GetK2() { return _k2; }
		inline double& GetP1() { return _p1; }
		inline double& GetP2() { return _p2; }
		inline double& GetCx() { return _cx; }
		inline double& GetCy() { return _cy; }

		inline vector<double>& GetActiveParams() { return _activeParams; }
	};
}
