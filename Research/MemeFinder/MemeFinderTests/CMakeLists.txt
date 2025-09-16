#--------------------------------------------------------
# CMake for generating MemeFinderLib Testing Suite
#
# @author: Wild Boar
#
# Date Created: 2025-09-15
#--------------------------------------------------------

# Add Google Test to the test group
find_package(GTest REQUIRED)

# Setup the includes
include_directories("../")

# Create the executable
add_executable(MemeFinderTests
    Tests/Example_Tests.cpp
)

# Add link libraries
target_link_libraries(MemeFinderTests MemeFinderLib MemeLib NVLib ${OpenCV_LIBS} OpenSSL::SSL uuid GTest::Main)

# Find the associated unit tests
gtest_discover_tests(MemeFinderTests)
