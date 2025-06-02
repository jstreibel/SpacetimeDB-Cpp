#pragma once

#include <SpacetimeDB/Utils/HttpClient.hpp>
#include <SpacetimeDB/Utils/Json.hpp>
#include "IdentityModels.hpp"
#include "Utils/Error.hpp"

namespace SpacetimeDB {
    ///
    /// Wraps all /v1/identity HTTP operations.
    ///
    class IdentityClient {
    public:
        /// Construct with a reference to an existing HttpClient.
        explicit IdentityClient(Utils::HttpClient& http);
        ~IdentityClient();

        [[nodiscard]] Utils::Result<IdentityInfo> Login(const std::string& ExternalJwt) const;

        ///
        /// Create a new identity by POSTing {} to /v1/identity.
        /// Returns the newly generated identity string.
        ///
        [[nodiscard]] Utils::Result<IdentityInfo> CreateIdentity(const CreateIdentityRequest& Request = CreateIdentityRequest()) const;

        ///
        /// Generate a short-lived access token which can be used in untrusted contexts, e.g. embedded in URLs.
        /// POST /v1/identity/websocket-token
        /// Returns
        ///
        [[nodiscard]] Utils::Result<IdentityWebSocketTokenResponse> GetWebSocketToken(
            const GetIdentityWebSocketTokenRequest& Request) const;

        [[nodiscard]] Utils::Result<GetPublicKeyResponse> GetPublicKey(
            const GetPublicKeyRequest& Request = GetPublicKeyRequest()) const;

        ///
        /// List databases owned by an identity via GET /v1/identity/{identityId}/databases.
        /// Returns DatabasesInfo.
        ///
        [[nodiscard]] Utils::Result<DatabasesInfo> GetDatabases(const std::string& IdentityId) const;


        [[nodiscard]] Utils::Result<SetEmailResponse> SetEmail(
            const std::string& IdentityId,
            const SetEmailRequest& Request) const;

        [[nodiscard]] Utils::Result<VerifyIdentityResponse> VerifyIdentity(const VerifyIdentityRequest& Request) const;

    private:
        Utils::HttpClient& http_;
    };
} // namespace SpacetimeDb
