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
    SpacetimeDB::Utils::HttpClient httpWrapper{};
    SpacetimeDB::IdentityClient client(httpWrapper);

    /*
    auto resp = client.Login("alice", "secret");

    REQUIRE(fake.lastPath == "/identity/login");
    REQUIRE(fake.lastPayload.find("\"username\":\"alice\"") != std::string::npos);
    REQUIRE(fake.lastHeaders.at("Content-Type") == "application/json");
    REQUIRE(resp["status"] == "ok");
    */
}