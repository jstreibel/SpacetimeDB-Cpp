#pragma once

#include <Utils/HttpClient.hpp>
#include <Utils/Json.hpp>
#include "IdentityModels.hpp"

namespace SpacetimeDb {

    class IdentityClient {
    public:
        // inject your HTTP machinery
        explicit IdentityClient(Utils::HttpClient& http);

        // Creates a new identity. Returns the generated ID.
        std::string CreateIdentity(const CreateIdentityRequest& req);

        // Fetches info for an existing identity
        IdentityInfo GetIdentity(const std::string& identityId);

    private:
        Utils::HttpClient& http_;
    };

} // namespace SpacetimeDb