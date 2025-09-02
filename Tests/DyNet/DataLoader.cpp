//--------------------------------------------------
// Loads a data file from disk
//
// @author: Wild Boar
//
// @date: 2025-09-01
//--------------------------------------------------

#include "DataLoader.h"
using namespace NVL_App;

//--------------------------------------------------
// Functionality to load the data file
//--------------------------------------------------

/**
 * Loads a CSV file and returns a vector of Sample objects.
 * @param filename The path to the CSV file
 */
vector<Sample> DataLoader::LoadCSV(const string& filename) 
{
    vector<Sample> samples; unordered_map<string, int> classMap;
  
    ifstream file(filename);
    if (!file.is_open()) 
    {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        return samples;
    }

    string line;

    // Skip the first line as it contains the column headings
    getline(file, line);

    // Extract the data until the file is empty
    while (getline(file, line)) 
    {
        stringstream ss(line);
        array<float, 4> features;
        int label;

        for (int i = 0; i < 5; ++i) 
        {
            string value;
            if (!getline(ss, value, ',')) 
            {
                cerr << "Error reading feature value from line: " << line << endl;
                continue;
            }

            if (i == 0) continue; // Skip the row number

            features[i - 1] = stof(value);
        }

        string labelStr;
        if (!getline(ss, labelStr, ',')) 
        {
            cerr << "Error reading label from line: " << line << endl;
            continue;
        }
        label = MapClass(classMap, labelStr);

        samples.emplace_back(features, label);
    }

    file.close();
    return samples;
}

//--------------------------------------------------
// Helpers
//--------------------------------------------------

/**
 * Maps a class name to an integer ID.
 * @param classMap The map of class names to IDs
 * @param className The class name to map
 * @return The integer ID of the class
 */
int DataLoader::MapClass(unordered_map<string, int>& classMap, const string& className) 
{
    auto it = classMap.find(className);
    if (it != classMap.end()) {
        return it->second;
    }
    int newId = classMap.size();
    classMap[className] = newId;
    return newId;
}
 