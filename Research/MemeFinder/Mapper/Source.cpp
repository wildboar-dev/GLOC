//-----------------------------------------------------------------
// Defines the C++ source file for generating cost function maps
//
// @author: 
//-----------------------------------------------------------------

#include <iostream>
using namespace std;

#include <MemeLib/Points.h>
#include <MemeLib/PointLoader.h>
#include <MemeLib/MetaLoader.h>
#include <MemeLib/HelperUtils.h>

#include <MemeFinderLib/ArgUtils.h>
#include <MemeFinderLib/ParameterFactory.h>
#include <MemeFinderLib/PoseLoader.h>
#include <MemeFinderLib/Parameters.h>
#include <MemeFinderLib/Utilities.h>
#include <MemeFinderLib/Logger.h>

#include <NVLib/Path/PathHelper.h>


//-----------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------
void Run(NVL_App::Logger & logger, NVLib::Parameters * parameters);

//-----------------------------------------------------------------
// Runner Method
//-----------------------------------------------------------------

/**
 * @brief Runs the main logic of the Mapper application.
 * @param logger Logger instance for logging messages
 * @param parameters Parameters instance for configuration
 */
void Run(NVL_App::Logger & logger, NVLib::Parameters * parameters) 
{
    logger << NVL_App::Logger::Color(34) << "Loading Parameters" << NVL_App::Logger::Save();
    auto database = NVL_App::ArgUtils::GetString(parameters, "database");
    auto dataset = NVL_App::ArgUtils::GetString(parameters, "dataset");
    auto pathHelper = NVLib::PathHelper(database, dataset);

    logger << NVL_App::Logger::Color(34) << "Loading Points" << NVL_App::Logger::Save();
    auto basePoints = NVL_App::PointLoader::Load(pathHelper.GetPath("Point","point.txt"));
    logger << "Loaded " << basePoints->PointCount() << " basePoints" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Loading Meta" << NVL_App::Logger::Save();
    auto meta = NVL_App::MetaLoader::Load(pathHelper.GetPath("Meta","meta.xml"));

    logger << NVL_App::Logger::Color(34) << "Loading Pose" << NVL_App::Logger::Save();
    auto pose_1 = NVL_App::PoseLoader::LoadPose(pathHelper.GetPath("Meta","pose1.xml"));
    auto pose_2 = NVL_App::PoseLoader::LoadPose(pathHelper.GetPath("Meta","pose2.xml"));

    logger << NVL_App::Logger::Color(34) << "Create a parameters" << NVL_App::Logger::Save();
    auto cx = meta->GetCameraMatrix().at<double>(0, 2); auto cy = meta->GetCameraMatrix().at<double>(1, 2);
    auto scene = NVL_App::Parameters(Point2d(cx, cy)); scene.SetValue(0, 0.1); scene.SetValue(2, 0.1);

    logger << NVL_App::Logger::Color(34) << "Applying the parameters to the basePoints" << NVL_App::Logger::Save();
    Mat camera = meta->GetCameraMatrix().clone(); Mat distortion = scene.GetDistortion();
    auto points = NVL_App::Utilities::ApplyDistortion(camera, pose_1.get(), pose_2.get(), &scene, basePoints.get());

    logger << NVL_App::Logger::Color(34) << "Creating an image of the cost function" << NVL_App::Logger::Save();
    auto indices = scene.GetIndices();
    Mat image = NVL_App::HelperUtils::RenderKSpace(camera, points.get(), Size(640, 640), indices, NVLib::Range<double>(-0.2, 0.2), NVLib::Range<double>(-0.2, 0.2));
    auto fileName = "cost.tiff"; imwrite(fileName, image);
    logger << NVL_App::Logger::Color(32) << "Wrote '" << fileName << "' to disk" << NVL_App::Logger::Save();
}

//-----------------------------------------------------------------
// Entry Point
//-----------------------------------------------------------------

/**
 * @brief Main entry point for the Mapper source file.
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit status
 */
int main(int argc, char* argv[])
{
    auto logger = NVL_App::Logger();

    try 
    {
        logger << NVL_App::Logger::Color(32) << "Starting Mapper Application" << NVL_App::Logger::Save();

        auto parameters = NVL_App::ArgUtils::Load("MemeFinder", argc, argv);
        Run(logger, parameters);
        delete parameters;
    }
    catch (const exception& ex) 
    {
        logger << NVL_App::Logger::Color(31) << "Error: " << ex.what() << NVL_App::Logger::Save();
        return EXIT_FAILURE;
    }

    logger << NVL_App::Logger::Color(32) << "Mapper Application Finished" << NVL_App::Logger::Save();

    return EXIT_SUCCESS;
}