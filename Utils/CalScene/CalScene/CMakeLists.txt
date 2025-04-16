#--------------------------------------------------------
# CMake for generating the main executable
#
# @author: Wild Boar
#
# Date Created: 2025-04-16
#--------------------------------------------------------

# Setup the includes
include_directories("../")

# Create the executable
add_executable(CalScene
    Engine.cpp
    Source.cpp
)

# Add link libraries                               
target_link_libraries(CalScene CalSceneLib  NVLib ${OpenCV_LIBS} OpenSSL::SSL uuid)

# Copy Resources across
add_custom_target(resource_copy ALL
	COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/CalScene 
    DEPENDS ${MY_TARGET}
)
