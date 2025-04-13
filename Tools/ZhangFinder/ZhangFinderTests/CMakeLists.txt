#--------------------------------------------------------
# CMake for generating ZhangFinderLib Testing Suite
#
# @author: Wild Boar
#
# Date Created: 2025-04-12
#--------------------------------------------------------

# Add Google Test to the test group
find_package(GTest REQUIRED)

# Setup the includes
include_directories("../")

# Create the executable
add_executable (ZhangFinderTests
	Tests/Example_Tests.cpp
	Tests/PointLoader_Tests.cpp
	Tests/ZhangProblem_Tests.cpp
)



# Add link libraries
target_link_libraries(ZhangFinderTests ZhangFinderLib NVLib ${OpenCV_LIBS} OpenSSL::SSL uuid GTest::Main)

# Find the associated unit tests
gtest_discover_tests(ZhangFinderTests)

# Copy Test Resources across
add_custom_target(test_copy ALL
	COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/ZhangFinderTests/Resources ${CMAKE_BINARY_DIR}/ZhangFinderTests 
    DEPENDS ${MY_TARGET}
)