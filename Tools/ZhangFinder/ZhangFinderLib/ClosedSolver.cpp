//--------------------------------------------------
// Implementation of class ClosedSolver
//
// @author: Wild Boar
//
// @date: 2025-04-17
//--------------------------------------------------

#include "ClosedSolver.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Default Constructor
 */
ClosedSolver::ClosedSolver()
{
	// Extra implementation
}

//--------------------------------------------------
// Update
//--------------------------------------------------

/**
 * @brief Inserts a new homography for consideration within the system
 * @param H The homography that we are inserting
 */
void ClosedSolver::AddHomography(Mat & H)
{
	_homographies.push_back(H);
}

//--------------------------------------------------
// Main
//--------------------------------------------------

/**
 * @brief Determine the camera matrix from the homography list
 * @return Mat Returns a Mat
 */
Mat ClosedSolver::FindK()
{
	// Create a container for the V matrix
	Mat V;

	// Build up the V Matrix
	for (auto& H : _homographies)
	{
		AddRowT1(V, H);
		AddRowT2(V, H);
	}

	// Solve for B
	auto B = FindB(V);

	//cout << "b: " << B << endl;

	// Extract K
	Mat K = ExtractK(B);

	// Return the K matrix
	return K;
}

//--------------------------------------------------
// Helpers
//--------------------------------------------------

/**
 * @brief Add a type one row to the V matrix
 * @param V The matrix that we are updating
 * @param H The homography whose data we are inserting
 */
void ClosedSolver::AddRowT1(Mat& V, Mat& H)
{
	Mat row = GetVector(H, 0, 1);

	if (V.empty()) V = row;
	else V.push_back(row);
}

/**
 * @brief Add a type two row to the V matrix
 * @param V The matrix that we are updating
 * @param H The homography whose data we are inserting
 */
void ClosedSolver::AddRowT2(Mat& V, Mat& H)
{
	Mat v1 = GetVector(H, 0, 0);
	Mat v2 = GetVector(H, 1, 1);
	Mat row = v1 - v2;

	if (V.empty()) V = row;
	else V.push_back(row);
}

/**
 * @brief Get the vector from the homography
 * @param H The homography that we are extracting from
 * @param i The row index
 * @param j The column index
 * @return Mat Returns a Mat
 */
Mat ClosedSolver::GetVector(Mat& H, int i, int j) 
{
	return (Mat_<double>(1, 4) <<
		H.at<double>(0, i) * H.at<double>(0, j) + H.at<double>(1, i) * H.at<double>(1, j),
		H.at<double>(2, i) * H.at<double>(0, j) + H.at<double>(0, i) * H.at<double>(2, j),
		H.at<double>(2, i) * H.at<double>(1, j) + H.at<double>(1, i) * H.at<double>(2, j),
		H.at<double>(2, i) * H.at<double>(2, j));
}

/**
 * @brief Use SVD to extract the b vector and make the B matrix
 * @param V The V matrix that we are extracting from
 * @return Mat Returns a Mat
 */
Vec4d ClosedSolver::FindB(Mat& V)
{
	// Perform SVD
	Mat U, S, Vt;
	SVD::compute(V, S, U, Vt);

	// Get the last column of Vt
	auto b = Vt.row(Vt.rows - 1);

	// Convert to Vec4d
	Vec4d B(b.at<double>(0), b.at<double>(1), b.at<double>(2), b.at<double>(3));

	return B;
}

/**
 * @brief Extract the K matrix from the B matrix
 * @param B The B matrix that we are extracting K from
 * @return Mat Returns a Mat
 */
Mat ClosedSolver::ExtractK(const Vec4d & B)
{	
	auto f = sqrt(1 / B[0]);
	auto cx = -B[1] * (1 / B[0]);
	auto cy = -B[2] * (1 / B[0]);	

	return Mat_<double>(3, 3) << f, 0, cx, 0, f, cy, 0, 0, 1;
}