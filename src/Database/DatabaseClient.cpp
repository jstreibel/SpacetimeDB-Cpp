#include <Database/DatabaseClient.hpp>

namespace SpacetimeDb {

    DatabaseClient::DatabaseClient(Utils::HttpClient& http, const std::string& dbName)
      : http_(http), dbName_(dbName)
    {}

    Utils::Json DatabaseClient::ExecuteSql(const std::string& sql) {
        Utils::Json payload = {
            {"query", sql}
        };
        std::string path = "/db/" + dbName_ + "/sql";
        auto resp = http_
            .Post(path, payload.dump(), {{"Content-Type", "application/json"}});
        return Utils::Json::parse(resp.text());
    }

    void DatabaseClient::Publish(const std::string& collection, const Utils::Json& doc) {
        std::string path = "/db/" + dbName_ + "/collections/" + collection + "/documents";
        http_.Post(path, doc.dump(), {{"Content-Type", "application/json"}});
    }

} // namespace SpacetimeDb
