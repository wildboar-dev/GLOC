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
	throw runtime_error("Not implemented");
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
	throw runtime_error("Not implemented");
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
	throw runtime_error("Not implemented");
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
	throw runtime_error("Not implemented");
}

/**
 * @brief Add a type two row to the V matrix
 * @param V The matrix that we are updating
 * @param H The homography whose data we are inserting
 */
void ClosedSolver::AddRowT2(Mat& V, Mat& H)
{
	throw runtime_error("Not implemented");
}

/**
 * @brief Use SVD to extract the b vector and make the B matrix
 * @param V The V matrix that we are extracting from
 */
void ClosedSolver::FindB(Mat& V)
{
	throw runtime_error("Not implemented");
}

/**
 * @brief Extract the K matrix from the B matrix
 * @param B The B matrix that we are extracting K from
 * @return Mat Returns a Mat
 */
Mat ClosedSolver::ExtractK(Mat& B)
{
	throw runtime_error("Not implemented");
}