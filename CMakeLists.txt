cmake_minimum_required(VERSION 3.20)
project(StudentRegistry)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
enable_testing()

# Add subdirectories for libraries and the main application
add_subdirectory(libs/model)
add_subdirectory(libs/view)
add_subdirectory(libs/controller)
add_subdirectory(src)
add_subdirectory(tests) 