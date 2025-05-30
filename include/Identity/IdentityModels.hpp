#pragma once

#include <string>
#include <Utils/Json.hpp>

namespace SpacetimeDb {

    /// Request payload for POST /identity
    struct CreateIdentityRequest {
        /// (no fields today – returns an empty JSON object)
        [[nodiscard]] Utils::Json toJson() const {
            return Utils::Json::object();
        }
    };

    /// Response payload for GET /identity/{id}
    struct IdentityInfo {
        std::string Id;
        std::string Token;  // may be empty if not returned by server

        /// Deserialize from the raw JSON response
        static IdentityInfo fromJson(const Utils::Json& j) {
            IdentityInfo info;
            info.Id    = j.at("id").get<std::string>();
            info.Token = j.value("token", std::string{});
            return info;
        }
    };

} // namespace SpacetimeDb
