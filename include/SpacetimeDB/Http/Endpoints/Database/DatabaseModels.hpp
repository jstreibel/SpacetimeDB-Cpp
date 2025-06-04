#pragma once

#include <Http/Json.hpp>
#include "Types.h"

namespace SpacetimeDB::Database {

    namespace Request
    {
        struct PublishNew     final : HttpRequest { };

        struct PublishTo      final : HttpRequest { };

        struct GetDescription final : HttpRequest { };

        struct Delete         final : HttpRequest
        {
            Delete() = delete;
            explicit Delete(const SpacetimeToken& Bearer) : HttpRequest(Bearer) { }
        };

        struct GetNames final : HttpRequest { };

        struct AddName  final : HttpRequest
        {
            String NewName;

            AddName() = delete;
            explicit AddName(const SpacetimeToken& Bearer, String NewName)
            : HttpRequest(Bearer), NewName(std::move(NewName)) { }

            [[nodiscard]] String GetBody() const override { return NewName; }
        };

        struct SetNames final : HttpRequest
        {
            Array<String> NewNames;
            SetNames() = delete;
            explicit SetNames(const SpacetimeToken& Bearer, const Array<String>& NewNames)
            : HttpRequest(Bearer), NewNames(NewNames) { }

            [[nodiscard]] String GetBody() const override { return Json(NewNames).dump(); }
        };

        struct GetIdentity final : HttpRequest { };

        struct Subscribe final : HttpRequest {};

        struct CallReducer final : HttpRequest {};

        struct GetSchema final : HttpRequest {};

        struct GetLogs final : HttpRequest {};

        struct ExecuteSql final : HttpRequest
        {

        };
    }

    namespace Response
    {
        struct PublishNew final : HttpResponse { };

        struct PublishTo  final : HttpResponse { };

        struct GetDescription final : HttpResponse
        {
            String Identity;
            String OwnerIdentity;
            String HostType;
            String InitialWasmHash;
        };

        struct Delete final : HttpResponse { };

        struct GetNames final : HttpRequest
        {
            Array<String> Names;
        };

        struct AddName final : HttpResponse
        {

            enum {Success, PermissionDenied, Invalid} Status;

            String Domain;
            Optional<String> Result; //Valid if Status==Success.

        };

        struct SetNames final : HttpResponse { };

        struct GetIdentity final : HttpResponse
        {
            HexString Identity;
        };

        struct Subscribe final : HttpResponse { };

        struct CallReducer final : HttpResponse { };

        struct GetSchema final : HttpResponse { };

        struct GetLogs final : HttpResponse { };

        struct ExecuteSql final : HttpResponse { };
    }


    /// Request for POST /db/{dbName}/sql
    struct ExecuteSqlRequest {
        String Query;

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
