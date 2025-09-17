//--------------------------------------------------
// Implementation of class Parameters
//
// @author: Wild Boar
//
// @date: 2025-09-16
//--------------------------------------------------

#include "Parameters.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param center The center point of the image
 */
Parameters::Parameters(const Point2d& center) : _center(center)
{
	_k1 = 0.0;
	_k2 = 0.0;
	_p1 = 0.0;
	_p2 = 0.0;
	
	_cx = center.x;
	_cy = center.y;
}

//--------------------------------------------------
// Update and Set Values
//--------------------------------------------------

/**
 * @brief Update a parameter value by index
 * @param index The index of the parameter to set
 * @param value The new value to set
 */
void Parameters::UpdateValue(int index, double delta)
{
	switch (index)
	{
	case 0:
		_k1 += delta; 
		break;
	case 1:
		_k2 += delta;
		break;
	case 2:
		_p1 += delta;
		break;
	case 3:
		_p2 += delta;
		break;
	case 4:
		_cx += delta;
		break;
	case 5:
		_cy += delta;
		break;
	default:
		throw runtime_error("Index out of range");
	}

	if (_activeSet.find(index) == _activeSet.end())
	{
		_activeSet.insert(index);
	}
}

/**
 * @brief Update a parameter value by index
 * @param index The index of the parameter to set
 * @param value The new value to set
 */
void Parameters::SetValue(int index, double value)
{
	switch (index)
	{
	case 0:
		_k1 = value; 
		break;
	case 1:
		_k2 = value;
		break;
	case 2:
		_p1 = value;
		break;
	case 3:
		_p2 = value;
		break;
	case 4:
		_cx = _center.x + value;
		break;
	case 5:
		_cy = _center.y + value;
		break;
	default:
		throw runtime_error("Index out of range");
	}

	if (_activeSet.find(index) == _activeSet.end())
	{
		_activeSet.insert(index);
	}

}


//--------------------------------------------------
// Get
//--------------------------------------------------

/**
 * @brief Get the current distortion parameters
 * @return Mat Returns a Mat
 */
Mat Parameters::GetDistortion()
{
	return (Mat_<double>(1, 4) << _k1, _k2, _p1, _p2);
}

/**
 * @brief Get the current center point
 * @return Point2d Returns a Point2d
 */
Point2d Parameters::GetCenter()
{
	return Point2d(_cx, _cy);
}

/**
 * @brief Get the delta value for a given parameter index
 * @param index The index of the parameter to get
 * @return double Returns the delta value
 */
double Parameters::GetValueDelta(int index) 
{
	switch (index)
	{
	case 0:
		return _k1;
	case 1:
		return _k2;
	case 2:
		return _p1;
	case 3:
		return _p2;
	case 4:
		return _cx - _center.x;
	case 5:
		return _cy - _center.y;
	default:
		throw runtime_error("Index out of range");
	}
}