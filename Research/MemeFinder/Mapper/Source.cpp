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
#include <MemeLib/CostFunction.h>

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
Point2d FindExpectedMinima(const Size& imageSize, const Vec2d& minima, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2);
double PixelToCost(Mat& cameraMatrix, NVL_App::Points * inputPoints, const Point2d& pixel, const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2);
Vec2d PixelToK(Mat& cameraMatrix, const Point2d& pixel, const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2);
Point2d KToPixel(Mat& cameraMatrix, const Vec2d& k, const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2);
Mat RenderVImage(Mat& cameraMatrix, NVL_App::Points * inputPoints, vector<int>& indices, const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2);
Vec2d FindGradient(Mat& cameraMatrix, NVL_App::Points * inputPoints, vector<int>& indicies, const Vec2d& point);
double GetScore(Mat& cameraMatrix, NVL_App::Points * inputPoints, vector<int>& indices, const Vec2d& point);
Mat RenderPathImage(const string& pathFile,const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2);

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
    //---------------------------------------------------
    // HARDCODED PARAMETERS
    //---------------------------------------------------
    const double K1 = 0.6; const double K2 = -0.6;
    const double range = 0.65; auto imageSize = Size(640, 640);
    //---------------------------------------------------

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
    auto scene = NVL_App::Parameters(Point2d(cx, cy)); scene.SetValue(0, K1); scene.SetValue(2, K2);

    logger << NVL_App::Logger::Color(34) << "Applying the parameters to the basePoints" << NVL_App::Logger::Save();
    Mat camera = meta->GetCameraMatrix().clone(); Mat distortion = scene.GetDistortion();
    auto points = NVL_App::Utilities::ApplyDistortion(camera, pose_1.get(), pose_2.get(), &scene, basePoints.get());

    ////////////////////////////////////////////////////
    //logger << NVL_App::Logger::Color(34) << "Test Undistorted points" << NVL_App::Logger::Save();
    //auto errors = vector<double>();auto tscore_1 = NVL_App::CostFunction::CalculateError(basePoints.get(), errors);
    //logger << NVL_App::Logger::Color(34) << "Score on Undistorted base points: " << tscore_1 << NVL_App::Logger::Save();

    //logger << NVL_App::Logger::Color(34) << "Test Distorted points" << NVL_App::Logger::Save();
    //errors = vector<double>();auto tscore_2 = NVL_App::CostFunction::CalculateError(points.get(), errors);
    //logger << NVL_App::Logger::Color(34) << "Score on Distorted base points: " << tscore_2 << NVL_App::Logger::Save();
    ///////////////////////////////////////////////////

    // //logger << NVL_App::Logger::Color(34) << "Creating an image of the cost function" << NVL_App::Logger::Save();
    auto indices = scene.GetIndices();
    // Mat image = NVL_App::HelperUtils::RenderKSpace(camera, points.get(), Size(640, 640), indices, NVLib::Range<double>(-range, range), NVLib::Range<double>(-range, range));
    // auto fileName = "cost.tiff"; imwrite(fileName, image);
    // logger << NVL_App::Logger::Color(32) << "Wrote '" << fileName << "' to disk" << NVL_App::Logger::Save();

    // logger << NVL_App::Logger::Color(34) << "Finding the minimum" << NVL_App::Logger::Save();
    // double minVal, maxVal; Point minLoc, maxLoc; minMaxLoc(image, &minVal, &maxVal, &minLoc, &maxLoc);
    // logger << NVL_App::Logger::Color(32) << "Min Val: " << minVal << " at (" << minLoc.x << ", " << minLoc.y << ")" << NVL_App::Logger::Save();
    // auto K_min = PixelToK(camera, minLoc, imageSize, NVLib::Range<double>(-range, range), NVLib::Range<double>(-range, range));
    // logger << NVL_App::Logger::Color(32) << "K at Min: (" << K_min[0] << ", " << K_min[1] << ")" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Creating an image of the vector field" << NVL_App::Logger::Save();
    Mat vImage = RenderVImage(camera, points.get(), indices, imageSize, NVLib::Range<double>(-range, range), NVLib::Range<double>(-range, range));

    logger << NVL_App::Logger::Color(34) << "Determining the location of the expected minima" << NVL_App::Logger::Save();
    auto expected = FindExpectedMinima(imageSize, Vec2d(K1, K2), NVLib::Range<double>(-range, range), NVLib::Range<double>(-range, range));
    circle(vImage, Point((int)expected.x, (int)expected.y), 5, Scalar(0, 0, 255), -1);
    circle(vImage, Point(imageSize.width / 2, imageSize.height / 2), 5, Scalar(0, 255, 255), -1);
    logger << NVL_App::Logger::Color(32) << "Expected Minima: (" << expected.x << ", " << expected.y << ")" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Saving Vector Field" << NVL_App::Logger::Save();
    auto vFileName = "vector_field.png"; imwrite(vFileName, vImage);
    logger << NVL_App::Logger::Color(32) << "Wrote '" << vFileName << "' to disk" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Rendering the optimization path" << NVL_App::Logger::Save();
    auto pathImage = RenderPathImage("../MemeFinder/path.txt", imageSize, NVLib::Range<double>(-range, range), NVLib::Range<double>(-range, range));
    imwrite("optimization_path.png", pathImage);
    logger << NVL_App::Logger::Color(32) << "Wrote '" << "optimization_path.png" << "' to disk" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Creating a combo image" << NVL_App::Logger::Save();
    Mat combo = vImage + pathImage;
    imwrite("combo.png", combo);
    logger << NVL_App::Logger::Color(32) << "Wrote '" << "combo.png" << "' to disk" << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Calculating the score at point [615,24]" << NVL_App::Logger::Save();
    auto score_1 = PixelToCost(camera, points.get(), Point2d(615, 24), imageSize, NVLib::Range<double>(-range, range), NVLib::Range<double>(-range, range)); 
    logger << NVL_App::Logger::Color(32) << "Score at (615,24): " << score_1 << NVL_App::Logger::Save();

    logger << NVL_App::Logger::Color(34) << "Calculating the expected optimal point" << NVL_App::Logger::Save();
    auto optimal = KToPixel(camera, Vec2d(K1, K2), imageSize, NVLib::Range<double>(-range, range), NVLib::Range<double>(-range, range));
    logger << NVL_App::Logger::Color(32) << "Optimal Point: (" << optimal.x << ", " << optimal.y << ")" << NVL_App::Logger::Save();
    auto score_2 = PixelToCost(camera, points.get(), optimal, imageSize, NVLib::Range<double>(-range, range), NVLib::Range<double>(-range, range));
    logger << NVL_App::Logger::Color(32) << "Score at Optimal Point: " << score_2 << NVL_App::Logger::Save();
}

/**
 * @brief Finds the expected minima in the cost function map.
 * @param imageSize Size of the image
 * @param minima The minima point
 * @param RangeK1 Range for K1 parameter
 * @param RangeK2 Range for K2 parameter
 * @return Point3d The expected minima point
 */
Point2d FindExpectedMinima(const Size& imageSize, const Vec2d& minima, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2) 
{
	auto stepK1 = (RangeK1.GetMax() - RangeK1.GetMin()) / (float)imageSize.width;
	auto stepK2 = (RangeK2.GetMax() - RangeK2.GetMin()) / (float)imageSize.height;

    auto x = (minima[0] - RangeK1.GetMin()) / stepK1;
    auto y = (minima[1] - RangeK2.GetMin()) / stepK2;

    return Point2d(x, y);
}

/**
 * @brief Converts a pixel location to cost values.
 * @param cameraMatrix The camera matrix
 * @param inputPoints The input points
 * @param pixel The pixel location
 * @param imageSize The size of the image
 * @param RangeK1 Range for K1 parameter
 * @param RangeK2 Range for K2 parameter
 * @return Vec2d The cost values corresponding to the pixel location
 */
double PixelToCost(Mat& cameraMatrix, NVL_App::Points * inputPoints, const Point2d& pixel, const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2) 
{
	auto stepK1 = (RangeK1.GetMax() - RangeK1.GetMin()) / (float)imageSize.width;
	auto stepK2 = (RangeK2.GetMax() - RangeK2.GetMin()) / (float)imageSize.height;

    auto k1 = RangeK1.GetMin() + pixel.x * stepK1;
    auto k2 = RangeK2.GetMin() + pixel.y * stepK2;

    auto parameters = NVL_App::Parameters(Point2d(cameraMatrix.at<double>(0, 2), cameraMatrix.at<double>(1, 2))); 
    parameters.SetValue(0, k1); parameters.SetValue(2, k2);
    auto distortion = parameters.GetDistortion();
    cout << "Distortion: " << distortion << endl;

    auto undistortedPoints = NVL_App::HelperUtils::Undistort(cameraMatrix, distortion, inputPoints);
    auto errors = vector<double>();
    auto score = NVL_App::CostFunction::CalculateError(undistortedPoints.get(), errors);

    return score;
}

/**
 * @brief Converts a pixel location to K values.
 * @param cameraMatrix The camera matrix
 * @param pixel The pixel location
 * @param imageSize The size of the image
 * @param RangeK1 Range for K1 parameter
 * @param RangeK2 Range for K2 parameter
 * @return Vec2d The K values corresponding to the pixel location
 */
Vec2d PixelToK(Mat& cameraMatrix, const Point2d& pixel, const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2) 
{
    auto stepK1 = (RangeK1.GetMax() - RangeK1.GetMin()) / (float)imageSize.width;
    auto stepK2 = (RangeK2.GetMax() - RangeK2.GetMin()) / (float)imageSize.height;

    auto k1 = RangeK1.GetMin() + pixel.x * stepK1;
    auto k2 = RangeK2.GetMin() + pixel.y * stepK2;

    return Vec2d(k1, k2);
}

/**
 * @brief Converts K values to a pixel location.
 * @param cameraMatrix The camera matrix
 * @param k The K values
 * @param imageSize The size of the image
 * @param RangeK1 Range for K1 parameter
 * @param RangeK2 Range for K2 parameter
 * @return Point2d The pixel location corresponding to the K values
 */
Point2d KToPixel(Mat& cameraMatrix, const Vec2d& k, const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2) 
{
    auto stepK1 = (RangeK1.GetMax() - RangeK1.GetMin()) / (float)imageSize.width;
    auto stepK2 = (RangeK2.GetMax() - RangeK2.GetMin()) / (float)imageSize.height;

    auto x = (k[0] - RangeK1.GetMin()) / stepK1;
    auto y = (k[1] - RangeK2.GetMin()) / stepK2;

    return Point2d(x, y);
}


//-----------------------------------------------------------------
// Logic to render a Vector image
//-----------------------------------------------------------------

/**
 * @brief Renders a vector image based on the provided parameters.
 * @param cameraMatrix The camera matrix
 * @param inputPoints The input points
 * @param pixel The pixel location
 * @param imageSize The size of the image
 * @param RangeK1 Range for K1 parameter
 * @param RangeK2 Range for K2 parameter
 * @return Mat The rendered vector image
 */
Mat RenderVImage(Mat& cameraMatrix, NVL_App::Points * inputPoints, vector<int>& indices, const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2) 
{
    Mat image = Mat_<Vec3b>::zeros(imageSize);

    auto stepK1 = (RangeK1.GetMax() - RangeK1.GetMin()) / (float)imageSize.width;
	auto stepK2 = (RangeK2.GetMax() - RangeK2.GetMin()) / (float)imageSize.height;

	for (auto row = 0; row < image.rows; row+= 10) 
	{
		for (auto column = 0; column < image.cols; column+= 10) 
		{
			double k1 = RangeK1.GetMin() + column * stepK1;
			double k2 = RangeK2.GetMin() + row * stepK2;

            auto gradient = FindGradient(cameraMatrix, inputPoints, indices, Vec2d(k1, k2));

            auto x = column + min(gradient[0] * 8, 8.0);
            auto y = row + min(gradient[1] * 8, 8.0);

            if (__isnan(x) || __isnan(y)) continue;

            arrowedLine(image, Point(x,y), Point(column, row), Scalar(0, 255, 0), 1, LINE_AA, 0, 0.3);
		}
	}

    return image;
}

/**
 * @brief Finds the gradient at a given point in the parameter space.
 * @param cameraMatrix The camera matrix
 * @param inputPoints The input points
 * @param indices The indices of the parameters to consider
 * @param point The point in parameter space
 * @return Vec2d The gradient vector at the given point
 */
Vec2d FindGradient(Mat& cameraMatrix, NVL_App::Points * inputPoints, vector<int>& indices, const Vec2d& point) 
{
    Vec2d gradient;

    // Compute the gradient using finite differences
    const double epsilon = 1e-6;

    // Find the base score  
    auto baseScore = GetScore(cameraMatrix, inputPoints, indices, point);

    for (size_t i = 0; i < indices.size(); i++) 
    {
        // Perturb the parameter
        Vec2d perturbedPoint = Vec2d(point);
        perturbedPoint[i] += epsilon;

        // Compute the cost at the perturbed point
        double cost = GetScore(cameraMatrix, inputPoints, indices, perturbedPoint);

        // Compute the gradient
        gradient[i] = (cost - baseScore) / epsilon;
    }

    // normalize the gradient
    double norm = sqrt(gradient[0] * gradient[0] + gradient[1] * gradient[1]);
    if (norm > 1e-6) gradient /= norm;   

    return gradient;
}

/**
 * @brief Gets the score for a given point in parameter space.
 * @param cameraMatrix The camera matrix
 * @param inputPoints The input points
 * @param indices The indices of the parameters to consider
 * @param point The point in parameter space
 * @return double The score at the given point
 */
double GetScore(Mat& cameraMatrix, NVL_App::Points * inputPoints, vector<int>& indices, const Vec2d& point)
{
    auto distortion = Vec4d();
    distortion[indices[0]] = point[0];	
    distortion[indices[1]] = point[1];

    auto undistortedPoints = NVL_App::HelperUtils::Undistort(cameraMatrix, distortion, inputPoints);
    auto errors = vector<double>(); auto score = NVL_App::CostFunction::CalculateError(undistortedPoints.get(), errors);

    return score;    
}

//-----------------------------------------------------------------
// Path Image
//-----------------------------------------------------------------

/**
 * @brief Renders an image of the optimization path.
 * @param pathFile The file containing the optimization path
 * @param imageSize The size of the image
 * @param RangeK1 Range for K1 parameter
 * @param RangeK2 Range for K2 parameter
 * @return Mat The rendered path image
 */
Mat RenderPathImage(const string& pathFile,const Size& imageSize, const NVLib::Range<double>& RangeK1, const NVLib::Range<double>& RangeK2) 
{
    Mat image = Mat_<Vec3b>::zeros(imageSize);

    auto stepK1 = (RangeK1.GetMax() - RangeK1.GetMin()) / (float)imageSize.width;
    auto stepK2 = (RangeK2.GetMax() - RangeK2.GetMin()) / (float)imageSize.height;

    auto reader = ifstream(pathFile); if (!reader.is_open()) throw runtime_error("Could not open path file: " + pathFile);

    // Read initial point
    auto k1 = 0.0; auto k2 = 0.0; reader >> k1 >> k2;

    while (!reader.eof())
    {
        auto x1 = (k1 - RangeK1.GetMin()) / stepK1;
        auto y1 = (k2 - RangeK2.GetMin()) / stepK2;

        reader >> k1 >> k2;

        auto x2 = (k1 - RangeK1.GetMin()) / stepK1;
        auto y2 = (k2 - RangeK2.GetMin()) / stepK2;

        line(image, Point((int)x1, (int)y1), Point((int)x2, (int)y2), Scalar(255, 0, 0), 2, LINE_AA);
    }

    reader.close();

    return image;
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