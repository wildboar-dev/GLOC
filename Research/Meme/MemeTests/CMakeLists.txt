#--------------------------------------------------------
# CMake for generating MemeLib Testing Suite
#
# @author: Wild Boar
#
# Date Created: 2025-09-05
#--------------------------------------------------------

# Add Google Test to the test group
find_package(GTest REQUIRED)

# Setup the includes
include_directories("../")

# Create the executable
add_executable (MemeTests
	Helpers/TestHelpers.cpp
	Tests/Example_Tests.cpp
	Tests/CostFunction_Tests.cpp
)

# Add link libraries
target_link_libraries(MemeTests MemeLib NVLib ${OpenCV_LIBS} OpenSSL::SSL uuid GTest::Main)

# Find the associated unit tests
gtest_discover_tests(MemeTests)
