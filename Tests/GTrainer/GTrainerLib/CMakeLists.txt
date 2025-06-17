#--------------------------------------------------------
# CMake for generating the GTrainerLib
#
# @author: Wild Boar
#
# Date Created: 2025-06-17
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib")

# Create Library
add_library(GTrainerLib STATIC
    ArgUtils.cpp
)
