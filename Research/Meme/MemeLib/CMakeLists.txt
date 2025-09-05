#--------------------------------------------------------
# CMake for generating the MemeLib
#
# @author: Wild Boar
#
# Date Created: 2025-09-05
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib")

# Create Library
add_library(MemeLib STATIC
    ArgUtils.cpp
)
