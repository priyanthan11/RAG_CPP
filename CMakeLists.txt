cmake_minimum_required(VERSION 3.20)
project(rag_cpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable folders in IDEs (VS, Rider, CLion)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Find external packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
