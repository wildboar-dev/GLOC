#--------------------------------------------------------
# CMake for generating the RealFinderLib
#
# @author: Wild Boar
#
# Date Created: 2025-10-08
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib" "${CMAKE_CURRENT_SOURCE_DIR}/../Meme" "${LIBRARY_BASE}/OptLib")

# Create Library
add_library(RealFinderLib STATIC
    ArgUtils.cpp
    MetaLoader.cpp
    PointLoader.cpp
    CostFunction.cpp
    HelperUtils.cpp
    Parameters.cpp
    DCostFunction.cpp
)