#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "../include/SpacetimeDB/Http/Endpoints/Identity/IdentityClient.hpp"

TEST_CASE("IdentityClient::Login formats request correctly", "[Identity]") {
    // SpacetimeDB::HttpClient httpWrapper{};
    // SpacetimeDB::IdentityClient client(httpWrapper);

    SpacetimeDB::HttpClient Http("http://localhost:3000", /*timeoutMs=*/30000);

    const SpacetimeDB::IdentityClient IdClient(Http);

    // POST /v1/identity
    const auto IdentityResult = IdClient.CreateIdentity();
    REQUIRE(SpacetimeDB::IsValid(IdentityResult));
    const auto Identity = SpacetimeDB::GetResult(IdentityResult);

    // POST /v1/identity/websocket-token
    const auto WSTokenResult = IdClient.GetWebSocketToken(SpacetimeDB::GetIdentityWebSocketTokenRequest(Identity.Token));
    REQUIRE(SpacetimeDB::IsValid(WSTokenResult));

    // GET /v1/identity/public-key
    auto PublicKeyResult = IdClient.GetPublicKey();
    REQUIRE(SpacetimeDB::IsValid(PublicKeyResult));

    // GET /v1/identity/:identity/databases
    const auto [Data] = SpacetimeDB::GetResult(PublicKeyResult);
    const auto ClientDatabasesResult = IdClient.GetDatabases(Identity.Id);
    REQUIRE(SpacetimeDB::IsValid(ClientDatabasesResult));

    // GET /v1/identity/:identity/verify
    auto VerifyIdResult = IdClient.VerifyIdentity(SpacetimeDB::VerifyIdentityRequest{Identity});
    REQUIRE(SpacetimeDB::IsValid(VerifyIdResult));
    REQUIRE(SpacetimeDB::GetResult(VerifyIdResult).Status == SpacetimeDB::VerifyIdentityResponse::ValidMatch);
}