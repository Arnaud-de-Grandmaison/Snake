add_subdirectory(googletest-release-1.8.1)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(test-Coords test-Coords.cpp)
target_link_libraries(test-Coords gtest_main)
