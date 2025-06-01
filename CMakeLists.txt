cmake_minimum_required(VERSION 3.15)
project(SpacetimeDBSdk VERSION 0.1.0 LANGUAGES CXX)

# Options
option(STDB_BUILD_EXAMPLES "Build example executables" ON)
option(STDB_BUILD_TESTS    "Build unit tests"          ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include paths
include_directories(${PROJECT_SOURCE_DIR}/include/SpacetimeDB)

# Source files
file(GLOB_RECURSE SDK_SOURCES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# SDK library
add_library(SpacetimeDBSdk STATIC ${SDK_SOURCES})

target_include_directories(SpacetimeDBSdk PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# HTTP/WS/JSON dependencies
# Use FetchContent to pull in cpr:
include(FetchContent)

# ── Fetch Curl for People ─────────────────────────────────────────────────────
FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06
)
FetchContent_MakeAvailable(cpr)

# ── Fetch nlohmann_json ───────────────────────────────────────────────────────
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# ── Fetch WebSocket C++ ───────────────────────────────────────────────────────
FetchContent_Declare(
        websocketpp
        GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
        GIT_TAG        master  # or whichever tag/commit you need
)
set (ENABLE_CPP11   ON  CACHE BOOL "Use C++11 STL instead of Boost" FORCE )
set (BUILD_EXAMPLES OFF CACHE BOOL "Build websocketpp examples."    FORCE )
set (BUILD_TESTS    OFF CACHE BOOL "Build websocketpp tests."       FORCE )
FetchContent_MakeAvailable(websocketpp)
# <— so <websocketpp/...> can be found:
target_include_directories(SpacetimeDBSdk PUBLIC ${websocketpp_SOURCE_DIR})


target_link_libraries(SpacetimeDBSdk PUBLIC
        cpr::cpr
        nlohmann_json::nlohmann_json
        # websocketpp::websocketpp
)

# Unit tests
if ( STDB_BUILD_EXAMPLES )
    message(STATUS "Adding SpacetimeDB C++ SDK examples")
    add_subdirectory(examples)
endif ()

# Examples
if ( STDB_BUILD_TESTS )
    message(STATUS "Adding SpacetimeDB C++ SDK tests")
    add_subdirectory(tests)
endif ()