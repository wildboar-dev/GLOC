//--------------------------------------------------
// Defines a sample element
//
// @author: Wild Boar
//
// @date: 2025-09-01
//--------------------------------------------------

#pragma once

#include <unordered_map>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

#include "Sample.h"

namespace NVL_App
{
    class DataLoader
    {
    public:
        static vector<Sample> LoadCSV(const string& filename);
    private:
        static int MapClass(unordered_map<string, int>& classMap, const string& className);
    };
}