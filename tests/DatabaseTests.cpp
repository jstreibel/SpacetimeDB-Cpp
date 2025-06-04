#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "../include/SpacetimeDB/Http/Endpoints/Database/DatabaseClient.hpp"

TEST_CASE("Database Client Endpoint Tests", "[Database]") {

    const SpacetimeDB::Database::Client DatabaseClient(
        "quickstart-chatty-chat",
        "http://localhost:3000",
        /*timeoutMs=*/30000);

    const auto DescriptionResult = DatabaseClient.GetDescription();
    REQUIRE(SpacetimeDB::IsValid(DescriptionResult));

    // auto Description = SpacetimeDB::GetResult(DescriptionResult);
}