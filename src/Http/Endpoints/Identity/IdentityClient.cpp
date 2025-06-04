#include <iostream>
#include <../../include/SpacetimeDB/Http/Endpoints/Identity/IdentityClient.hpp>

namespace SpacetimeDB {

    IdentityClient::IdentityClient(HttpClient& http)
      : http_(http)
    {}

    IdentityClient::~IdentityClient() = default;

    Result<IdentityInfo> IdentityClient::Login(const std::string& ExternalJwt) const
    {
        // Build headers: Authorization + Content-Type
        Header Head;
        Head["Authorization"] = "Bearer " + ExternalJwt;
        Head["Content-Type"]  = "application/json";

        // Create (empty) JSON request body via CreateIdentityRequest
        const Json bodyJson = CreateIdentityRequest { }.ToJson();
        const std::string bodyString = bodyJson.dump();  // "{}"

        // POST to "/v1/identity"
        // (here we expect our HttpClient not to inject "/v1")
        const auto PostResult = http_.Post("/v1/identity", bodyString, Head);
        if (!IsValid(PostResult))
        {
            ReturnError("IdentityClient::Login failed: " + GetErrorMessage(PostResult));
        }

        auto [StatusCode, Body] = GetResult(PostResult);

        // Check for HTTP-level success (200 OK)
        if (StatusCode != 200)
        {
            ReturnError(
                "IdentityClient::Login failed (status="
                + std::to_string(StatusCode)
                + ", body=" + Body + ")");
        }

        // Parse the JSON response into Json
        const Json JsonResponse = Json::parse(Body);

        // Convert to our IdentityInfo struct and return it
        return IdentityInfo::FromJson(JsonResponse);
    }


    Result<IdentityInfo> IdentityClient::CreateIdentity(const CreateIdentityRequest& Request) const
    {
        // call POST /identity
        const auto HttpPostResult = http_.Post(
            "/v1/identity",
            Request.ToJson().dump(),
            Request.GetHeaders() );
        if (!IsValid(HttpPostResult))
        {
            ReturnError("Failed to POST to /v1/identity: " + GetErrorMessage(HttpPostResult));
        }

        const auto& [StatusCode, Body] = GetResult(HttpPostResult);

        if (StatusCode != 200)
        {
            ReturnError("Unhandled status code: " + std::to_string(StatusCode) + " @ base URL '" + http_.GetBaseUrl() + "'");
        }

        Json JsonResponse = Json::parse(Body);
        if (!JsonResponse.contains("identity"))
        {
            ReturnError("Response did not contain 'identity' field. JsonResponse: " + JsonResponse.dump() + "");
        }

        IdentityInfo Info;
        Info.Id = JsonResponse.at("identity").get<std::string>();
        Info.Token = JsonResponse.at("token").get<std::string>();

        return Info;
    }

    Result<DatabasesInfo> IdentityClient::GetDatabases(const std::string& IdentityId) const
    {
        // GET /identity/{id}/databases
        const auto Path = "/v1/identity/" + IdentityId + "/databases";
        const auto HttpGetResult = http_.Get(Path, {});

        OnError(HttpGetResult, "Failed to GET " + Path + ": " + GetErrorMessage(HttpGetResult));

        const auto [StatusCode, Body] = GetResult(HttpGetResult);
        if (StatusCode != 200)
        {
            ReturnError("Unhandled status code: " + std::to_string(StatusCode) + " for GET method on Url " + http_.GetUrl(Path));
        }

        const Json JsonResponse = Json::parse(Body);

        const auto Info = DatabasesInfo::FromJson(JsonResponse);

        if (!IsValid(Info))
        {
            ReturnError("Failed to parse JSON response to 'GET " + http_.GetUrl(Path) + "': " + JsonResponse.dump());
        }

        return GetResult(Info);
    }

    Result<IdentityWebSocketTokenResponse> IdentityClient::GetWebSocketToken(
        const GetIdentityWebSocketTokenRequest& Request) const
    {
        const auto Path = std::string("/v1/identity/websocket-token");
        const auto Head = Request.GetHeaders();

        const auto HttpGetResult =
            http_.Post(
                Path,
                Json().dump(),
                Head);
        OnError(HttpGetResult, "Failed to POST " + Path + ": " + GetErrorMessage(HttpGetResult));

        const auto [StatusCode, Body] = GetResult(HttpGetResult);

        if (StatusCode != 200)
        {
            ReturnError("Unhandled status code: " + std::to_string(StatusCode) + " for POST method on Url " + http_.GetUrl(Path));
        }

        const Json JsonResponse = Json::parse(Body);
        return IdentityWebSocketTokenResponse::FromJson(JsonResponse);
    }

    Result<GetPublicKeyResponse> IdentityClient::GetPublicKey( ) const
    {
        const auto Path = std::string("/v1/identity/public-key");
        const auto HttpGetResult = http_.Get(Path,{});

        OnError(HttpGetResult, "Failed to GET " + Path + ": " + GetErrorMessage(HttpGetResult));

        const auto [StatusCode, Body] = GetResult(HttpGetResult);

        if (StatusCode != 200)
        {
            ReturnError("Unhandled status code: " + std::to_string(StatusCode) + " for GET method on Url " + http_.GetUrl(Path));
        }

        return GetPublicKeyResponse::FromPemCertificateChain(Body);
    }

    Result<SetEmailResponse> IdentityClient::SetEmail(const std::string& IdentityId, const SetEmailRequest& Request) const
    {
        const auto Path = std::string("/v1/identity/") + IdentityId + "/set-email";
        const auto HttpGetResult = http_.Post(Path, Request.ToJson().dump(), Request.GetHeaders());

        OnError(HttpGetResult, "Failed to POST " + Path + ": " + GetErrorMessage(HttpGetResult));

        if (const auto [StatusCode, Body] = GetResult(HttpGetResult); StatusCode != 200)
        {
            ReturnError("Unhandled status code: " + std::to_string(StatusCode) + " for POST method on Url " + http_.GetUrl(Path));
        }

        return {};
    }

    Result<VerifyIdentityResponse> IdentityClient::VerifyIdentity(const VerifyIdentityRequest& Request) const
    {
        const auto Path = "/v1/identity/" + Request.IdentityToVerify.Id + "/verify";
        const auto HttpGetResult = http_.Get(Path, Request.GetHeaders());
        OnError(HttpGetResult, "Failed to GET " + Path + ": " + GetErrorMessage(HttpGetResult));

        const auto [StatusCode, Body] = GetResult(HttpGetResult);
        if (StatusCode == 204)  return VerifyIdentityResponse{VerifyIdentityResponse::ValidMatch};
        if (StatusCode == 400)  return VerifyIdentityResponse{VerifyIdentityResponse::ValidMismatch};
        if (StatusCode == 401)  return VerifyIdentityResponse{VerifyIdentityResponse::InvalidOrNoAuthorizationToken};

        ReturnError(
            "Unhandled status code: " + std::to_string(StatusCode) + " for GET method on Url " + http_.GetUrl(Path));
    }
} // namespace SpacetimeDb
