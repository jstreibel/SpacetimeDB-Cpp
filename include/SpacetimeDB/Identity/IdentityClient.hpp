#pragma once

#include <SpacetimeDB/Utils/HttpClient.hpp>
#include <SpacetimeDB/Utils/Json.hpp>
#include "IdentityModels.hpp"

namespace SpacetimeDB {


    ///
    /// Wraps all /v1/identity HTTP operations.
    ///
    class IdentityClient {
    public:
        /// Construct with a reference to an existing HttpClient.
        explicit IdentityClient(Utils::HttpClient& http);
        ~IdentityClient();

        IdentityInfo Login(const std::string& ExternalJwt) const;

        ///
        /// Create a new identity by POSTing {} to /v1/identity.
        /// Returns the newly generated identity string.
        ///
        [[nodiscard]] std::string CreateIdentity(const CreateIdentityRequest& req = CreateIdentityRequest()) const;

        ///
        /// Fetch existing identity info via GET /v1/identity/{identityId}.
        /// Returns IdentityInfo, possibly containing a refreshed token.
        ///
        [[nodiscard]] IdentityInfo GetIdentity(const std::string& identityId) const;

    private:
        Utils::HttpClient& http_;
    };

} // namespace SpacetimeDb