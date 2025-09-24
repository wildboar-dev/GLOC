#--------------------------------------------------------
# CMake for generating the main executable
#
# @author: Wild Boar
#
# Date Created: 2025-09-15
#--------------------------------------------------------

# Setup the includes
include_directories("../")

# Create the executable
add_executable(Mapper
    Source.cpp
)

# Add link libraries                               
target_link_libraries(Mapper MemeFinderLib MemeLib NVLib OptLib ${OpenCV_LIBS} OpenSSL::SSL uuid)

# Copy Resources across
add_custom_target(map_resource_copy ALL
	COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/Mapper 
    DEPENDS ${MY_TARGET}
)