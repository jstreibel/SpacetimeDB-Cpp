#pragma once

#include <Http/HttpClient.hpp>
#include <Http/Json.hpp>
#include "IdentityModels.hpp"
#include "Http/Error.hpp"

namespace SpacetimeDB {
    ///
    /// Wraps all /v1/identity HTTP operations.
    ///
    class IdentityClient {
    public:
        /// Construct with a reference to an existing HttpClient.
        explicit IdentityClient(HttpClient& http);
        ~IdentityClient();

        [[nodiscard]] Result<IdentityInfo> Login(const String& ExternalJwt) const;

        ///
        /// Create a new identity by POSTing {} to /v1/identity.
        /// Returns the newly generated identity string.
        ///
        [[nodiscard]] Result<IdentityInfo> CreateIdentity(const CreateIdentityRequest& Request = CreateIdentityRequest()) const;

        ///
        /// Generate a short-lived access token which can be used in untrusted contexts, e.g. embedded in URLs.
        /// POST /v1/identity/websocket-token
        /// Returns
        ///
        [[nodiscard]] Result<IdentityWebSocketTokenResponse> GetWebSocketToken(
            const GetIdentityWebSocketTokenRequest& Request) const;

        [[nodiscard]] Result<GetPublicKeyResponse> GetPublicKey( ) const;

        ///
        /// List databases owned by an identity via GET /v1/identity/{identityId}/databases.
        /// Returns DatabasesInfo.
        ///
        [[nodiscard]] Result<DatabasesInfo> GetDatabases(const String& IdentityId) const;


        [[nodiscard]] Result<SetEmailResponse> SetEmail(
            const String& IdentityId,
            const SetEmailRequest& Request) const;

        [[nodiscard]] Result<VerifyIdentityResponse> VerifyIdentity(const VerifyIdentityRequest& Request) const;

    private:
        HttpClient& http_;
    };
} // namespace SpacetimeDb
