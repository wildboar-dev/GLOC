#--------------------------------------------------------
# CMake for generating the CalSceneLib
#
# @author: Wild Boar
#
# Date Created: 2025-04-16
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib")

# Create Library
add_library(CalSceneLib STATIC
    ArgUtils.cpp
)
