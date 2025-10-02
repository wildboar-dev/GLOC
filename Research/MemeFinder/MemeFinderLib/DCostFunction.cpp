//--------------------------------------------------
// Implementation of class DCostFunction
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#include "DCostFunction.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Main Constructor
 * @param camera The camera matrix
 * @param indices The indices we are updating
 * @param dPoints The distorted points to use for optimization
 */
DCostFunction::DCostFunction(Mat& camera, const vector<int>& indices, Points * dPoints) : _camera(camera), _dPoints(dPoints)
{
    _parameters = new Parameters(Point2d(camera.at<double>(0, 2), camera.at<double>(1, 2)));
    for (auto index : indices) _parameters->SetValue(index, 0);
}

/**
 * @brief Destructor
 */
DCostFunction::~DCostFunction() 
{
    delete _parameters;
}

//--------------------------------------------------
// Evaluation Method
//--------------------------------------------------

/**
 * @brief Evaluates the cost function
 * @param inputs The input parameters as an Eigen vector
 * @return double Returns the computed cost
 */
double DCostFunction::Evaluate(const Eigen::VectorXd& inputs)
{
    auto indices = _parameters->GetIndices();
    for (size_t i = 0; i < indices.size(); i++)
    {
        _parameters->SetValue(indices[i], inputs[i]);
    }

    // Setup the camera matrix
    Mat camera = _camera.clone();
    camera.at<double>(0, 2) = _parameters->GetCx();
    camera.at<double>(1, 2) = _parameters->GetCy();

    // Get the distortion parameters
    Mat distortion = _parameters->GetDistortion();
    //cout << "Using Distortion: " << distortion << endl;

    // Get the points
    auto upoints = HelperUtils::Undistort(camera, distortion, _dPoints);

    // Compute the cost
    auto scores = vector<double>(); auto score = CostFunction::CalculateError(upoints.get(), scores);
        
    // Return the score result
    return score;
}