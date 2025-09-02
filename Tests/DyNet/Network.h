//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2025-09-01
//--------------------------------------------------

#pragma once

#include <array>
#include <iostream>
using namespace std;

#include <dynet/dynet.h>
#include <dynet/expr.h>
using namespace dynet;

namespace NVL_App 
{

    class Network 
    {
    private:
        Parameter p_W1, p_b1;
        Parameter p_W2, p_b2;

    public:    

        /**
         * Constructor for the Network class
         */
        Network(ParameterCollection& pc, unsigned H=16) 
        {
            p_W1 = pc.add_parameters({H, 4});
            p_b1 = pc.add_parameters({H});
            p_W2 = pc.add_parameters({3, H});
            p_b2 = pc.add_parameters({3});
        }

        /**
         * Operator to execute the network
         * @param cg The computation graph to use
         * @param x The input values
         * @return The output expression
         */
        Expression operator()(ComputationGraph& cg, const array<float,4>& x) 
        {
            Expression W1 = parameter(cg, p_W1);
            Expression b1 = parameter(cg, p_b1);
            Expression W2 = parameter(cg, p_W2);
            Expression b2 = parameter(cg, p_b2);

            // input column vector (4 x 1)
            auto inputValues = vector<dynet::real>(); inputValues.reserve(x.size());
            transform(x.begin(), x.end(), back_inserter(inputValues), [](float v) { return dynet::real(v); });
            Expression xvec = input(cg, {4}, inputValues);

            Expression h = tanh(affine_transform({b1, W1, xvec}));
            Expression yhat = affine_transform({b2, W2, h}); // logits (3 x 1)
            return yhat;
        }
};

}