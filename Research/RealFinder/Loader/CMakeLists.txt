#--------------------------------------------------------
# CMake for Loader
#
# @author: Wild Boar
#
# Date Created: 2025-10-08
#--------------------------------------------------------

# Setup the includes
include_directories("../")

# Create the executable
add_executable(Loader
    Source.cpp
)

# Add link libraries                               
target_link_libraries(Loader RealFinderLib NVLib ${OpenCV_LIBS} OpenSSL::SSL uuid)

# Copy Resources across
configure_file(${CMAKE_SOURCE_DIR}/Loader/config.xml ${CMAKE_BINARY_DIR}/Loader/config.xml COPYONLY)
