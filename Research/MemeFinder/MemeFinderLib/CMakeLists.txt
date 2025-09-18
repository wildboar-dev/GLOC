#--------------------------------------------------------
# CMake for generating the MemeFinderLib
#
# @author: Wild Boar
#
# Date Created: 2025-09-15
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib" "${CMAKE_CURRENT_SOURCE_DIR}/../Meme" "${LIBRARY_BASE}/OptLib")

# Create Library
add_library (MemeFinderLib STATIC
	ArgUtils.cpp
	Parameters.cpp
	ParameterFactory.cpp
	Utilities.cpp
	DCostFunction.cpp
	PoseLoader.cpp
)
