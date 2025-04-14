//--------------------------------------------------
// Implementation of class ZhangProblem
//
// @author: Wild Boar
//
// @date: 2025-04-13
//--------------------------------------------------

#include "ZhangProblem.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param H1 The homography of the first point
 * @param H2 The homography of the second point
 */
ZhangProblem::ZhangProblem(Mat& H1, Mat& H2)
{
	_h11 = H1.col(0); _h12 = H1.col(1);
	_h21 = H2.col(0); _h22 = H2.col(1);
}

//--------------------------------------------------
// Data Size
//--------------------------------------------------

/**
 * @brief Returns the number of data points that we are evaluating
 * @return int Returns a int
 */
int ZhangProblem::GetDataSize()
{
	return 4;
}

//--------------------------------------------------
// Evaluate
//--------------------------------------------------

/**
 * @brief Evaluate the provided camera parameters
 * @param parameters The number of parameters that we are evaluating
 * @param errors The error score for each element
 * @return double Returns a double
 */
double ZhangProblem::Evaluate(Mat& parameters, Mat& errors)
{
	Mat B = GetB(parameters);

	Mat error_1 = _h11.t() * B * _h12;
	Mat error_2 = _h21.t() * B * _h22;
	Mat error_3 = _h11.t() * B * _h11 - _h12.t() * B * _h12;
	Mat error_4 = _h21.t() * B * _h21 - _h22.t() * B * _h22;

	auto score_1 = ((double *) error_1.data)[0];
	auto score_2 = ((double *) error_2.data)[0];
	auto score_3 = ((double *) error_3.data)[0];
	auto score_4 = ((double *) error_4.data)[0];

	auto elink = (double *)errors.data;
	elink[0] = score_1 * score_1;
	elink[1] = score_2 * score_2;
	elink[2] = score_3 * score_3;
	elink[3] = score_4 * score_4;
	
	auto score =  score_1 * score_1 + score_2 * score_2 + score_3 * score_3 + score_4 * score_4;
	cout << "Score: " << score << endl;

	return score;
}

//--------------------------------------------------
// Helper
//--------------------------------------------------

/**
 * @brief Converts the parameters into a B matrix
 * @param parameters The parameters that we are evaluating
 * @return Mat Returns a Mat
 */
Mat ZhangProblem::GetB(Mat& parameters)
{
	auto plink = (double *)parameters.data;
	auto fx = plink[0];
	auto fy = plink[1];
	auto cx = plink[2];
	auto cy = plink[3];

	Mat K = (Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
	Mat KT = K.t();
	Mat invKT = KT.inv();
	Mat invK = K.inv();

	Mat B = invKT * invK;

	return B;
}