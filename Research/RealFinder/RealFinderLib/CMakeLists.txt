#--------------------------------------------------------
# CMake for generating the RealFinderLib
#
# @author: Wild Boar
#
# Date Created: 2025-10-08
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib")

# Create Library
add_library(RealFinderLib STATIC
    ArgUtils.cpp
)
