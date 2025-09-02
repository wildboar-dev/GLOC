//--------------------------------------------------
// Defines a sample element
//
// @author: Wild Boar
//
// @date: 2025-09-01
//--------------------------------------------------

#pragma once

#include <array>
#include <iostream>
using namespace std;

namespace NVL_App 
{
    class Sample 
    {
    private:
        array<float, 4> features;
        int label;

    public:
        Sample(const array<float, 4>& features, int label) 
            : features(features), label(label) {}

        const array<float, 4>& get_features() const { return features; }
        int get_label() const { return label; }
    };
}