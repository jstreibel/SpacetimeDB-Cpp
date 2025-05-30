#include <Identity/IdentityClient.hpp>

namespace SpacetimeDb {

    IdentityClient::IdentityClient(Utils::HttpClient& http)
      : http_(http)
    {}

    std::string IdentityClient::CreateIdentity(const CreateIdentityRequest& req) {
        // 1) serialize request
        Utils::Json jReq = req.toJson();

        // 2) call POST /identity
        auto resp = http_
            .Post("/identity", jReq.dump(), {{"Content-Type", "application/json"}});

        // 3) parse JSON response
        Utils::Json jResp = Utils::Json::parse(resp.text());
        return jResp.at("id").get<std::string>();
    }

    IdentityInfo IdentityClient::GetIdentity(const std::string& identityId) {
        // GET /identity/{id}
        std::string path = "/identity/" + identityId;
        auto resp = http_.Get(path, {});

        Utils::Json jResp = Utils::Json::parse(resp.text());
        return IdentityInfo::fromJson(jResp);
    }

} // namespace SpacetimeDb
