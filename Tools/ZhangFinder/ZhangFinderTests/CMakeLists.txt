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
add_executable(ZhangFinderTests
    Tests/Example_Tests.cpp
)

# Add link libraries
target_link_libraries(ZhangFinderTests ZhangFinderLib NVLib ${OpenCV_LIBS} OpenSSL::SSL uuid GTest::Main)

# Find the associated unit tests
gtest_discover_tests(ZhangFinderTests)
