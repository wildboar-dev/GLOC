//--------------------------------------------------
// Implementation of class HelperUtils
//
// @author: Wild Boar
//
// @date: 2025-09-17
//--------------------------------------------------

#include "Utilities.h"
using namespace NVL_App;

//--------------------------------------------------
// Distortion Removal
//--------------------------------------------------

/**
 * @brief Applies distortion from the image
 * @param camera The camera matrix
 * @param pose1 The first pose
 * @param pose2 The second pose
 * @param parameters The input parameters
 * @param points The input points to distort
 * @return unique_ptr<Points> Returns a unique_ptr<Points>
 */
unique_ptr<Points> Utilities::ApplyDistortion(Mat& camera, Pose * pose1, Pose * pose2, Parameters * parameters, Points * points)
{
	Mat newCamera = camera.clone();
	newCamera.at<double>(0, 2) = parameters->GetCx();
	newCamera.at<double>(1, 2) = parameters->GetCy();

	Mat distortion = parameters->GetDistortion();

	cout << "Using Distortion: " << distortion << endl;

	auto rvec_1 = pose1->GetRvec();
	auto rvec_2 = pose2->GetRvec();
    auto tvec_1 = pose1->GetTvec();
    auto tvec_2 = pose2->GetTvec();

    auto points_1 = vector<Point2d>();
    auto points_2 = vector<Point2d>();

    cv::projectPoints(points->GetScenePoints(), rvec_1, tvec_1, newCamera, distortion, points_1);
    cv::projectPoints(points->GetScenePoints(), rvec_2, tvec_2, newCamera, distortion, points_2);

    return make_unique<NVL_App::Points>(points->GetScenePoints(), points_1, points_2);
}
