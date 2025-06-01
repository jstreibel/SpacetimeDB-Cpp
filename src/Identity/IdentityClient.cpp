#include <SpacetimeDB/Identity/IdentityClient.hpp>

namespace SpacetimeDB {

    IdentityClient::IdentityClient(Utils::HttpClient& http)
      : http_(http)
    {}

    IdentityClient::~IdentityClient() = default;

    IdentityInfo IdentityClient::Login(const std::string& ExternalJwt) const
    {
        // Build headers: Authorization + Content-Type
        std::map<std::string,std::string> headers;
        headers["Authorization"] = "Bearer " + ExternalJwt;
        headers["Content-Type"]  = "application/json";

        // Create (empty) JSON request body via CreateIdentityRequest
        const Utils::Json bodyJson = CreateIdentityRequest::toJson();
        const std::string bodyString = bodyJson.dump();  // "{}"

        // POST to "/v1/identity"
        // (here we expect our HttpClient not to inject "/v1")
        auto [StatusCode, Body] = http_.Post("/v1/identity", bodyString, headers);

        // Check for HTTP-level success (200 OK)
        if (StatusCode != 200)
        {
            // TODO: functional-style error handling, everywhere!
            throw std::runtime_error(
                "IdentityClient::Login failed (status="
                + std::to_string(StatusCode)
                + ", body=" + Body + ")"
            );
        }

        // Parse the JSON response into Utils::Json
        const Utils::Json respJson = Utils::Json::parse(Body);

        // Convert to our IdentityInfo struct and return it
        return IdentityInfo::fromJson(respJson);
    }


    std::string IdentityClient::CreateIdentity(const CreateIdentityRequest& req) const
    {
        // serialize request
        const Utils::Json jReq = SpacetimeDB::CreateIdentityRequest::toJson();

        // call POST /identity
        auto [StatusCode, Body] = http_
            .Post("/identity", jReq.dump(), {{"Content-Type", "application/json"}});

        if (StatusCode != 200)
        {
            throw std::runtime_error("Unhandled status code: " + std::to_string(StatusCode) + "");
        }

        // parse JSON response
        Utils::Json jResp = Utils::Json::parse(Body);
        return jResp.at("id").get<std::string>();
    }

    IdentityInfo IdentityClient::GetIdentity(const std::string& identityId) const
    {
        // GET /identity/{id}
        const std::string path = "/identity/" + identityId;
        const auto resp = http_.Get(path, {});

        const Utils::Json jResp = Utils::Json::parse(resp.Body);
        return IdentityInfo::fromJson(jResp);
    }

} // namespace SpacetimeDb
