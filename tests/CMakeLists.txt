enable_testing()

# Pull in Catch2 for unit testing
include(FetchContent)
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.9
)
FetchContent_MakeAvailable(catch2)

# IdentityClient tests
add_executable(IdentityClientTest IdentityTests.cpp)
target_link_libraries(IdentityClientTest PRIVATE SpacetimeDBSdk Catch2::Catch2)
add_test(NAME IdentityClientTest COMMAND IdentityClientTest)

# DatabaseClient tests
add_executable(DatabaseClientTest DatabaseTests.cpp)
target_link_libraries(DatabaseClientTest PRIVATE SpacetimeDBSdk Catch2::Catch2)
add_test(NAME DatabaseClientTest COMMAND DatabaseClientTest)