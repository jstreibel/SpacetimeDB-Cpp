cmake_minimum_required(VERSION 3.15)
project(SpacetimeDbSdk VERSION 0.1.0 LANGUAGES CXX)

# Options
option(BUILD_EXAMPLES "Build example executables" ON)
option(BUILD_TESTS    "Build unit tests"       ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SDK_SOURCES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# SDK library add_library(SpacetimeDbSdk STATIC ${SDK_SOURCES})

target_include_directories(SpacetimeDbSdk PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# HTTP/WS/JSON dependencies (link as needed)
# find_package(cpr QUIET)
# target_link_libraries(SpacetimeDbSdk PUBLIC cpr::cpr nlohmann_json::nlohmann_json)