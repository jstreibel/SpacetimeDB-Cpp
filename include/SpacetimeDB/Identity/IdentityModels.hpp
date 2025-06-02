#pragma once

#include <iostream>
#include <string>
#include <SpacetimeDB/Utils/Json.hpp>

namespace SpacetimeDB {

    /// Request payload for POST /identity
    struct CreateIdentityRequest {
        [[nodiscard]] static Utils::Json ToJson() {
            return Utils::Json::object();
        }
    };

    /// Request payload for GET /identity/{id}/databases
    struct ListOwnedDatabasesRequest {
        [[nodiscard]] static Utils::Json ToJson() {
            return Utils::Json::object();
        }
    };

    /// Response payload for GET /identity/{id}/databases
    struct DatabasesInfo
    {
        std::vector<std::string> Addresses;

        static Utils::Result<DatabasesInfo> FromJson(const Utils::Json& JsonData) {
            DatabasesInfo Info;

            /* According to SpacetiomeDB DOCS:
            * GET /v1/identity/:identity/databases`
            * List all databases owned by an identity.
            * Returns JSON in the form:
            * {"addresses": array<string>}

            * The addresses value is an array of zero or more strings, each of which is the address of a database owned by the identity passed as a parameter.
            *
            * BUT APPARENTLY, it is "identities", instead of "addresses"
            */

            // const auto Key = "addresses";
            const auto Key = "identities";

            if (!JsonData.contains(Key))
            {
                ReturnError("Response did not contain 'addresses' field. JsonResponse: " + JsonData.dump() + "");
            }

            Info.Addresses = JsonData.at(Key).get<std::vector<std::string>>();

            return Info;
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
                ReturnError(e.what() + std::string(" in ") + JsonData.dump() + ". ");
            }

            return Info;
        }
    };

} // namespace SpacetimeDb
