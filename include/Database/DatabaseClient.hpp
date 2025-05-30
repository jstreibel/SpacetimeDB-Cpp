#pragma once

#include <Utils/HttpClient.hpp>
#include <Utils/Json.hpp>
#include "DatabaseModels.hpp"

namespace SpacetimeDb {

    class DatabaseClient {
    public:
        explicit DatabaseClient(Utils::HttpClient& http, const std::string& dbName);

        // Executes a SQL query and returns raw JSON response
        Utils::Json ExecuteSql(const std::string& sql);

        // Publishes a document (arbitrary JSON)
        void Publish(const std::string& collection, const Utils::Json& doc);

    private:
        Utils::HttpClient& http_;
        std::string dbName_;
    };

} // namespace SpacetimeDb
