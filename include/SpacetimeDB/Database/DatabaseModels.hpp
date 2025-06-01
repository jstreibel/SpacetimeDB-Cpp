#pragma once

#include <string>
#include <SpacetimeDB/Utils/Json.hpp>

namespace SpacetimeDB {

    /// Request for POST /db/{dbName}/sql
    struct ExecuteSqlRequest {
        std::string Query;

        [[nodiscard]] Utils::Json toJson() const {
            return Utils::Json{{"query", Query}};
        }
    };

    /// Typed wrapper around the raw SQL response JSON
    struct ExecuteSqlResponse {
        Utils::Json Raw;  // you can later add helpers to parse rows/columns

        static ExecuteSqlResponse fromJson(const Utils::Json& j) {
            return ExecuteSqlResponse{ j };
        }
    };

    /// Request for POST /db/{dbName}/collections/{coll}/documents
    struct PublishDocumentRequest {
        Utils::Json Document;

        [[nodiscard]] Utils::Json toJson() const {
            return Document;
        }
    };

} // namespace SpacetimeDb
