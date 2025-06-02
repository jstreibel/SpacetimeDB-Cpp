#include <iostream>
#include <SpacetimeDB/Identity/IdentityClient.hpp>

namespace SpacetimeDB {

    IdentityClient::IdentityClient(Utils::HttpClient& http)
      : http_(http)
    {}

    IdentityClient::~IdentityClient() = default;

    Utils::Result<IdentityInfo> IdentityClient::Login(const std::string& ExternalJwt) const
    {
        // Build headers: Authorization + Content-Type
        std::map<std::string,std::string> headers;
        headers["Authorization"] = "Bearer " + ExternalJwt;
        headers["Content-Type"]  = "application/json";

        // Create (empty) JSON request body via CreateIdentityRequest
        const Utils::Json bodyJson = CreateIdentityRequest::ToJson();
        const std::string bodyString = bodyJson.dump();  // "{}"

        // POST to "/v1/identity"
        // (here we expect our HttpClient not to inject "/v1")
        auto PostResult = http_.Post("/v1/identity", bodyString, headers);
        if (!Utils::IsValid(PostResult))
        {
            ReturnError("IdentityClient::Login failed: " + Utils::GetErrorMessage(PostResult));
        }

        auto [StatusCode, Body] = Utils::GetResult(PostResult);

        // Check for HTTP-level success (200 OK)
        if (StatusCode != 200)
        {
            ReturnError(
                "IdentityClient::Login failed (status="
                + std::to_string(StatusCode)
                + ", body=" + Body + ")");
        }

        // Parse the JSON response into Utils::Json
        const Utils::Json JsonResponse = Utils::Json::parse(Body);

        // Convert to our IdentityInfo struct and return it
        return IdentityInfo::FromJson(JsonResponse);
    }


    Utils::Result<std::string> IdentityClient::CreateIdentity(const CreateIdentityRequest& req) const
    {
        // serialize request
        const Utils::Json JsonRequest = SpacetimeDB::CreateIdentityRequest::ToJson();

        // call POST /identity
        const auto RequestDump = JsonRequest.dump();
        const auto HttpPostResult = http_.Post("/v1/identity", RequestDump, {{"Content-Type", "application/json"}});
        if (!Utils::IsValid(HttpPostResult))
        {
            ReturnError("Failed to POST to /v1/identity: " + Utils::GetErrorMessage(HttpPostResult));
        }

        const auto& [StatusCode, Body] = Utils::GetResult(HttpPostResult);

        if (StatusCode != 200)
        {
            ReturnError("Unhandled status code: " + std::to_string(StatusCode) + " @ base URL '" + http_.GetBaseUrl() + "'");
        }

        Utils::Json JsonResponse = Utils::Json::parse(Body);
        if (!JsonResponse.contains("identity"))
        {
            ReturnError("Response did not contain 'identity' field. JsonResponse: " + JsonResponse.dump() + "");
        }
        return JsonResponse.at("identity").get<std::string>();
    }

    Utils::Result<IdentityInfo> IdentityClient::GetIdentity(const std::string& IdentityId) const
    {
        // GET /identity/{id}
        const std::string Path = "/v1/identity/" + IdentityId;
        const auto HttpGetResult = http_.Get(Path, {});
        if (!Utils::IsValid(HttpGetResult))
        {
            ReturnError("Failed to GET " + Path + ": " + Utils::GetErrorMessage(HttpGetResult));
        }

        const auto [StatusCode, Body] = Utils::GetResult(HttpGetResult);

        if (StatusCode != 200)
        {
            ReturnError("Unhandled status code: " + std::to_string(StatusCode) + ". Path: " + Path);
        }

        const Utils::Json JsonResponse = Utils::Json::parse(Body);
        return IdentityInfo::FromJson(JsonResponse);
    }

} // namespace SpacetimeDb
