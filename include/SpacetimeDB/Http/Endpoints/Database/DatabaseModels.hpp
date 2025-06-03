#pragma once

#include <string>
#include <Http/Json.hpp>

namespace SpacetimeDB {

    /// Request for POST /db/{dbName}/sql
    struct ExecuteSqlRequest {
        std::string Query;

        [[nodiscard]] Json toJson() const {
            return Json{{"query", Query}};
        }
    };

    /// Typed wrapper around the raw SQL response JSON
    struct ExecuteSqlResponse {
        Json Raw;  // you can later add helpers to parse rows/columns

        static ExecuteSqlResponse fromJson(const Json& j) {
            return ExecuteSqlResponse{ j };
        }
    };

    /// Request for POST /db/{dbName}/collections/{coll}/documents
    struct PublishDocumentRequest {
        Json Document;

        [[nodiscard]] Json toJson() const {
            return Document;
        }
    };

} // namespace SpacetimeDb
