#pragma once

#include <iostream>
#include <string>
#include <SpacetimeDB/Utils/Json.hpp>
#include <utility>

namespace SpacetimeDB {

    /// Request payload for POST /v1/identity
    /// -> Generate a new identity and token.
    struct CreateIdentityRequest {
        [[nodiscard]]
        Utils::Json ToJson() const { return Utils::Json::object(); }
        [[nodiscard]]
        std::map<std::string, std::string> GetHeaders() const
        {
            return {{"Content-Type", "application/json"}};
        }
    };

    /// Response payload for POST /v1/identity/
    struct IdentityInfo {
        std::string Id;
        std::string Token;  // may be empty if not returned by server

        /// Deserialize from the raw JSON response
        static Utils::Result<IdentityInfo> FromJson(const Utils::Json& JsonData) {
            IdentityInfo Info;

            try
            {
                Info.Id    = JsonData.at("id").get<std::string>();
                Info.Token = JsonData.value("token", std::string{});
            } catch (const nlohmann::detail::out_of_range& e)
            {
                ReturnError(e.what() + std::string(" in ") + JsonData.dump() + ". ");
            }

            return Info;
        }
    };


    /// Request payload for POST /v1/identity/websocket-token
    /// -> Generate a short-lived access token for use in untrusted contexts.
    struct GetIdentityWebSocketTokenRequest
    {
        std::string Token;

        explicit GetIdentityWebSocketTokenRequest(std::string Token) : Token(std::move(Token)) {}

        std::map<std::string, std::string> GetHeaders() const
        {
            return {
                {"Authorization", "Bearer " + Token},
                {"Content-Type", "application/json"}
            };
        }
        
    };

    /// Response for POST /v1/identity/websocket-token
    struct IdentityWebSocketTokenResponse
    {
        std::string Token;
        static Utils::Result<IdentityWebSocketTokenResponse> FromJson(const Utils::Json& JsonData)
        {
            if (!JsonData.contains("token"))
            {
                ReturnError("Response did not contain 'token' field. JsonResponse: " + JsonData.dump() + "");
            }
            return IdentityWebSocketTokenResponse{JsonData["token"]};
        }
    };


    /// Response payload for GET /v1/identity/{id}/databases
    struct DatabasesInfo
    {
        std::vector<std::string> Addresses;

        static Utils::Result<DatabasesInfo> FromJson(const Utils::Json& JsonData) {
            DatabasesInfo Info;

            /* According to SpacetimeDB DOCS:
            * GET /v1/identity/:identity/databases`
            * List all databases owned by an identity.
            * Returns JSON in the form:
            * {"addresses": array<string>}

            * The addresses value is an array of zero or more strings, each of which is the address of a database owned by the identity passed as a parameter.
            *
            * BUT APPARENTLY, it is "identities", instead of "addresses"
            */

            // const auto Key = "addresses"; // should be this, according to docs, but in reality its the following:
            const auto Key = "identities";

            if (!JsonData.contains(Key))
            {
                ReturnError("Response did not contain 'addresses' field. JsonResponse: " + JsonData.dump() + "");
            }

            Info.Addresses = JsonData.at(Key).get<std::vector<std::string>>();

            return Info;
        }
    };


    /// GET /v1/identity/public-key	Get the public key used for verifying tokens.
    /// Returns a response of content-type application/pem-certificate-chain.
    struct GetPublicKeyResponse
    {
        std::string Data;

        static Utils::Result<GetPublicKeyResponse> FromPemCertificateChain(const std::string& Data) {
            return GetPublicKeyResponse{Data};
        }
    };


    /// POST /v1/identity/:identity/set-email
    /// Associate an email with a Spacetime identity
    struct SetEmailRequest
    {
        std::string Email;
        std::string AuthorizationToken;

        [[nodiscard]] Utils::Json ToJson() const {
            return Utils::Json{{"email", Email}};
        }

        [[nodiscard]] std::map<std::string, std::string> GetHeaders() const {
            return {{"Authorization", "Bearer " + AuthorizationToken}};
        }
    };

    struct SetEmailResponse { };


    /// GET /v1/identity/{id}/verify
    /// Verify the validity of an identity/token pair.
    struct VerifyIdentityRequest
    {
        IdentityInfo IdentityToVerify;

        [[nodiscard]] std::map<std::string, std::string> GetHeaders() const
        {
            return {{"Authorization", "Bearer " + IdentityToVerify.Token}};
        }
    };

    struct VerifyIdentityResponse
    {
        enum Status {
            ValidMatch,
            ValidMismatch,
            InvalidOrNoAuthorizationToken
        } Status;
    };
} // namespace SpacetimeDb
