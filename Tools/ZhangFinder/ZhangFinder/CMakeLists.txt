#--------------------------------------------------------
# CMake for generating the main executable
#
# @author: Wild Boar
#
# Date Created: 2025-04-12
#--------------------------------------------------------

# Setup the includes
include_directories("../")

# Create the executable
add_executable(ZhangFinder
    Engine.cpp
    Source.cpp
)

# Add link libraries                               
target_link_libraries(ZhangFinder ZhangFinderLib  NVLib ${OpenCV_LIBS} OpenSSL::SSL uuid)

# Copy Resources across
add_custom_target(resource_copy ALL
	COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/ZhangFinder 
    DEPENDS ${MY_TARGET}
)