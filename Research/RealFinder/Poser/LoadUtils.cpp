//--------------------------------------------------
// Implementation of class LoadUtils
//
// @author: Wild Boar
//
// @date: 2025-10-13
//--------------------------------------------------

#include "LoadUtils.h"
using namespace NVL_App;

//--------------------------------------------------
// Distortion
//--------------------------------------------------

/**
 * @brief Loads distortion parameters from disk
 * @param pathHelper A link to the path helper for finding the input
 * @return unique_ptr<DParams> Returns a unique_ptr<DParams>
 */
unique_ptr<DParams> LoadUtils::LoadDParams(NVLib::PathHelper * pathHelper)
{
	throw runtime_error("Not implemented");
}