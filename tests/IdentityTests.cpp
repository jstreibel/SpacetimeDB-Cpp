#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "SpacetimeDB/Identity/IdentityClient.hpp"
#include "SpacetimeDB/Utils/Json.hpp"

// A fake HTTP client to capture requests
struct FakeHttp {
    std::string lastPath;
    std::string lastPayload;
    std::map<std::string,std::string> lastHeaders;

    SpacetimeDB::Utils::Json Post(const std::string& path,
                                  const std::string& payload,
                                  const std::map<std::string,std::string>& headers) {
        lastPath = path;
        lastPayload = payload;
        lastHeaders = headers;
        return SpacetimeDB::Utils::Json{{"status","ok"}};
    }
};

TEST_CASE("IdentityClient::Login formats request correctly", "[Identity]") {
    // SpacetimeDB::Utils::HttpClient httpWrapper{};
    // SpacetimeDB::IdentityClient client(httpWrapper);

    SpacetimeDB::Utils::HttpClient Http("http://localhost:3000", /*timeoutMs=*/30000);

    const SpacetimeDB::IdentityClient IdClient(Http);

    // POST /v1/identity
    const auto IdentityResult = IdClient.CreateIdentity();
    REQUIRE(SpacetimeDB::Utils::IsValid(IdentityResult));
    const auto Identity = SpacetimeDB::Utils::GetResult(IdentityResult);

    // POST /v1/identity/websocket-token
    const auto WSTokenResult = IdClient.GetWebSocketToken(SpacetimeDB::GetIdentityWebSocketTokenRequest(Identity.Token));
    REQUIRE(SpacetimeDB::Utils::IsValid(WSTokenResult));

    // GET /v1/identity/public-key
    auto PublicKeyResult = IdClient.GetPublicKey();
    REQUIRE(SpacetimeDB::Utils::IsValid(PublicKeyResult));

    // GET /v1/identity/:identity/databases
    const auto [Data] = SpacetimeDB::Utils::GetResult(PublicKeyResult);
    const auto ClientDatabasesResult = IdClient.GetDatabases(Identity.Id);
    REQUIRE(SpacetimeDB::Utils::IsValid(ClientDatabasesResult));

    // GET /v1/identity/:identity/verify
    auto VerifyIdResult = IdClient.VerifyIdentity(SpacetimeDB::VerifyIdentityRequest{Identity});
    REQUIRE(SpacetimeDB::Utils::IsValid(VerifyIdResult));
    REQUIRE(SpacetimeDB::Utils::GetResult(VerifyIdResult).Status == SpacetimeDB::VerifyIdentityResponse::ValidMatch);
}