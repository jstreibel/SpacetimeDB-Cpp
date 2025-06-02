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

        Utils::Result<IdentityInfo> Login(const std::string& ExternalJwt) const;

        ///
        /// Create a new identity by POSTing {} to /v1/identity.
        /// Returns the newly generated identity string.
        ///
        [[nodiscard]] Utils::Result<IdentityInfo> CreateIdentity(const CreateIdentityRequest& req = CreateIdentityRequest()) const;

        ///
        /// Fetch database addresses via GET /v1/identity/{identityId}/databases.
        /// Returns DatabasesInfo.
        ///
        [[nodiscard]] Utils::Result<DatabasesInfo> GetDatabases(const std::string& IdentityId) const;

    private:
        Utils::HttpClient& http_;
    };

} // namespace SpacetimeDb