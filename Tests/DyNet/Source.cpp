//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2025-09-01
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Path/PathHelper.h>
#include <NVLib/Parameters/Parameters.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include <dynet/training.h>
#include <dynet/expr.h>
#include <dynet/io.h>
#include <dynet/model.h>
using namespace dynet;

#include "ArgReader.h"
#include "Network.h"
#include "DataLoader.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run();
float Accuracy(NVL_App::Network& net, const vector<NVL_App::Sample>& data); 

//--------------------------------------------------
// Execution Logic
//--------------------------------------------------

/**
 * Main entry point into the application
 */
void Run() 
{
    auto logger = NVLib::Logger(1);

    logger.StartApplication();

    logger.Log(1, "Load the data from disk");
    auto data = NVL_App::DataLoader::LoadCSV("data.csv");

    logger.Log(1, "Shuffling the data");
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shuffle(data.begin(), data.end(), rng);

    logger.Log(1, "Creating the network");
    auto pc = ParameterCollection();
    auto network = NVL_App::Network(pc);

    logger.Log(1, "Creating a trainer");
    auto trainer = AdamTrainer(pc);

    for (auto i = 0; i < 50; i++) 
    {
        logger.Log(1, "Iteration: %i", i);
        auto epoch_loss = 0.0f;
        for (const auto& sample : data) 
        {
            ComputationGraph cg;
            auto yhat = network(cg, sample.get_features());
            auto loss = pickneglogsoftmax(yhat, sample.get_label());
            epoch_loss  += as_scalar(cg.forward(loss));
            cg.backward(loss);
            trainer.update();
        }

        auto score = Accuracy(network, data); 

        logger.Log(1, "Epoch Loss: %f", epoch_loss);
        logger.Log(1, "Accuracy: %f", score);
    }


    logger.StopApplication();
}

//--------------------------------------------------
// determine Accuracy
//--------------------------------------------------

/**
 * Compute the accuracy of the model on the given dataset.
 * @param net The neural network to evaluate
 * @param data The dataset to evaluate on
 */
float Accuracy(NVL_App::Network& net, const vector<NVL_App::Sample>& data) 
{
    unsigned correct = 0;
    for (const auto& s : data) {
        ComputationGraph cg;
        Expression logits = net(cg, s.get_features());
        Expression y = softmax(logits);
        vector<float> probs = as_vector(cg.forward(y));
        unsigned pred = (unsigned) (max_element(probs.begin(), probs.end()) - probs.begin());
        if (pred == s.get_label()) ++correct;
    }
    return static_cast<float>(correct) / data.size();
}

//--------------------------------------------------
// Entry Point
//--------------------------------------------------

/**
 * Main Method
 * @param argc The count of the incoming arguments
 * @param argv The number of incoming arguments
 * @return SUCCESS and FAILURE
 */
int main(int argc, char ** argv) 
{
    dynet::initialize(argc, argv);

    try
    {
        Run();
    }
    catch (runtime_error exception)
    {
        cerr << "Error: " << exception.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        cerr << "Error: " << exception << endl;
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
