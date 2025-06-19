//--------------------------------------------------
// Implementation of class StateGenerator
//
// @author: Wild Boar
//
// @date: 2025-06-19
//--------------------------------------------------

#include "StateGenerator.h"
using namespace NVL_App;

//--------------------------------------------------
// Generate
//--------------------------------------------------

/**
 * @brief Add the logic to create a solution
 * @param arguments The specifying the range of stuff to generate
 * @return unique_ptr<ProblemState> Returns a unique_ptr<ProblemState>
 */
unique_ptr<ProblemState> StateGenerator::Create(Arguments * arguments)
{
	NVLib::RandomUtils::TimeSeedRandomNumbers();

	auto cameraMatrix = GenerateCamera(arguments);
	auto distortionMatrix = GenerateDistortion(arguments);
	auto poseMatrix = GeneratePose(arguments);

	return make_unique<ProblemState>(cameraMatrix, distortionMatrix, poseMatrix);
}

//--------------------------------------------------
// Helper Methods
//--------------------------------------------------

/**
 * @brief Generate a camera matrix based on the arguments
 * @param arguments The arguments specifying the camera parameters
 * @return Mat Returns the generated camera matrix
 */
Mat StateGenerator::GenerateCamera(Arguments * arguments) 
{
	auto wh = arguments->GetImageSize().width + arguments->GetImageSize().height;
	auto factor = NVLib::RandomUtils::GetValue(NVLib::Range<double>(1.0/3.0, 3));

	auto fx = wh * factor;
	auto fy = wh * factor;

	auto rcx = arguments->GetCenter()[0]; auto rcy = arguments->GetCenter()[1];
	auto dcx = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-rcx, rcx));
	auto dcy = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-rcy, rcy));
	auto cx = dcx + arguments->GetImageSize().width / 2.0;
	auto cy = dcy + arguments->GetImageSize().height / 2.0;

	return (Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
}

/**
 * @brief Generate a distortion matrix based on the arguments
 * @param arguments The arguments specifying the distortion parameters
 * @return Mat Returns the generated distortion matrix
 */
Mat StateGenerator::GenerateDistortion(Arguments * arguments) 
{
	auto& delta = arguments->GetDistortion();

	auto k1 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-delta[0], delta[0]));
	auto k2 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-delta[1], delta[1]));
	auto p1 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-delta[2], delta[2]));
	auto p2 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-delta[3], delta[3]));

	return (Mat_<double>(1, 4) << k1, k2, p1, p2);
}

/**
 * @brief Generate a pose matrix based on the arguments
 * @param arguments The arguments specifying the pose parameters
 * @return Mat Returns the generated pose matrix
 */
Mat StateGenerator::GeneratePose(Arguments * arguments) 
{
	auto& rvec = arguments->GetRvec();
	auto& tvec = arguments->GetTvec();
	auto& baseline = arguments->GetBaseLine();
	auto distance = arguments->GetDistance();

	auto r1 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-rvec[0], rvec[0]));
	auto r2 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-rvec[1], rvec[1]));
	auto r3 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-rvec[2], rvec[2]));
	auto rvec_o = Vec3d(r1, r2, r3);

	auto t1 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-tvec[0], tvec[0]));
	auto t2 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-tvec[1], tvec[1]));
	auto t3 = NVLib::RandomUtils::GetValue(NVLib::Range<double>(-tvec[2], tvec[2]));
	auto tvec_o = Vec3d(t1, t2, t3 + distance);

	return NVLib::PoseUtils::Vectors2Pose(rvec_o, tvec_o);
}