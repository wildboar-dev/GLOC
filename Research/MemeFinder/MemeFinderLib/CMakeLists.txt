#--------------------------------------------------------
# CMake for generating the MemeFinderLib
#
# @author: Wild Boar
#
# Date Created: 2025-09-15
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib")

# Create Library
add_library(MemeFinderLib STATIC
    ArgUtils.cpp
)
