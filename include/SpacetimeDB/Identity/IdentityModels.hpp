#pragma once

#include <string>
#include <SpacetimeDB/Utils/Json.hpp>

namespace SpacetimeDB {

    /// Request payload for POST /identity
    struct CreateIdentityRequest {
        [[nodiscard]] static Utils::Json ToJson() {
            return Utils::Json::object();
        }
    };

    /// Response payload for GET /identity/{id}
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
            } catch (const nlohmann::detail::out_of_range e)
            {
                ReturnError(e.what());
            }

            return Info;
        }
    };

} // namespace SpacetimeDb
