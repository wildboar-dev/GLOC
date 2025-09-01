#--------------------------------------------------------
# Top-Level: DynetTest
#
# @author: Wild Boar
#
# @Date Created: 2025-09-01
#--------------------------------------------------------

cmake_minimum_required(VERSION 3.12.0)
project(DynetTest VERSION 1.0.0)

# Set the base path of the libraries
set(LIBRARY_BASE "$ENV{HOME}/Libraries")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add OpenCV to the solution
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Include OpenSSL
find_package(OpenSSL REQUIRED)

# Include Package for DyNet
add_library(DyNet SHARED IMPORTED)
set_target_properties(DyNet PROPERTIES
    IMPORTED_LOCATION "/home/trevor/Installs/dynet/build/dynet/libdynet.so"
    INTERFACE_INCLUDE_DIRECTORIES "/home/trevor/Installs/dynet/"
)


# Add the NVLib library to the folder
add_library(NVLib STATIC IMPORTED)
set_target_properties(NVLib PROPERTIES
    IMPORTED_LOCATION "${LIBRARY_BASE}/NVLib/build/NVLib/libNVLib.a"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBRARY_BASE}/NVLib"
)

# Create the executable
add_executable(DynetTest
    Source.cpp
)

# Add link libraries                               
target_link_libraries(DynetTest NVLib ${OpenCV_LIBS} OpenSSL::SSL uuid DyNet)