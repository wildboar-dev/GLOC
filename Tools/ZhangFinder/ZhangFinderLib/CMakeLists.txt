#--------------------------------------------------------
# CMake for generating the ZhangFinderLib
#
# @author: Wild Boar
#
# Date Created: 2025-04-12
#--------------------------------------------------------

# Setup include directories
include_directories("../" "${LIBRARY_BASE}/NVLib")

# Create Library
add_library (ZhangFinderLib STATIC
	ArgUtils.cpp
	PointLoader.cpp
)

