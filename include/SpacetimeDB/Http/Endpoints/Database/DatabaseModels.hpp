#pragma once

#include "../../Json.hpp"
#include "../../../Types.h"

#include <iostream>
#include <variant>

namespace SpacetimeDB::Database {

    namespace Request
    {
        struct PublishNew     final : HttpRequest { };

        struct PublishTo      final : HttpRequest { };

        struct GetDescription final : HttpRequest { };

        struct DeleteDB       final : HttpRequest
        {
            DeleteDB() = delete;
            explicit DeleteDB(const SpacetimeToken& Bearer) : HttpRequest(Bearer) { }
        };

        struct GetNames       final : HttpRequest { };

        struct AddName        final : HttpRequest
        {
            String NewName;

            AddName() = delete;
            explicit AddName(const SpacetimeToken& Bearer, String NewName)
            : HttpRequest(Bearer), NewName(std::move(NewName)) { }

            [[nodiscard]] String GetBody() const override { return NewName; }
        };

        struct SetNames       final : HttpRequest
        {
            Array<String> NewNames;

            SetNames() = delete;
            explicit SetNames(const SpacetimeToken& Bearer, const Array<String>& NewNames)
            : HttpRequest(Bearer), NewNames(NewNames) { }

            [[nodiscard]] String GetBody() const override { return Json(NewNames).dump(); }
        };

        struct GetIdentity    final : HttpRequest { };

        struct Subscribe      final : HttpRequest {};

        struct CallReducer    final : HttpRequest
        {
            String Reducer;
            Json SatsArgs;

            CallReducer() = delete;
            CallReducer(const SpacetimeToken& Bearer, String Reducer, Json SatsArgs)
            : HttpRequest(Bearer), Reducer(std::move(Reducer)), SatsArgs(std::move(SatsArgs)) { }

            [[nodiscard]] String GetBody() const override { return SatsArgs.dump(); }
        };

        struct GetSchema      final : HttpRequest
        {
            Int Version = 9;

            [[nodiscard]] StringMap GetParameters() const override
            {
                return {{"version", ToString(Version)}};
            };
        };

        struct GetLogs        final : HttpRequest {};

        struct ExecuteSql     final : HttpRequest
        {

        };
    }

    namespace Response
    {
        struct PublishNew     final : HttpResponse { };

        struct PublishTo      final : HttpResponse { };

        struct GetDescription final : HttpResponse
        {
            String Identity;
            String OwnerIdentity;
            String HostType;
            String InitialWasmHash;

            GetDescription(const long StatusCode, const String& Body) : HttpResponse(StatusCode, Body) { }

            static Result<GetDescription> FromResponse(const HttpResponse& Response)
            {
                GetDescription DescResponse(Response.StatusCode, Response.Body);

                auto JsonObj = Json::parse(Response.Body);

                if(!JsonObj.contains("database_identity"))
                    ReturnError("Failed to parse 'database_identity' field from http response.");
                if(!JsonObj.contains("owner_identity"))
                    ReturnError("Failed to parse 'owner_identity' field from http response.");
                if(!JsonObj.contains("host_type"))
                    ReturnError("Failed to parse 'host_type' field from http response.");
                if(!JsonObj.contains("initial_program"))
                    ReturnError("Failed to parse 'initial_program' field from http response.");

                using Exception = const nlohmann::json::exception&;
                try { DescResponse.Identity        = JsonObj["database_identity"]["__identity__"]; }
                catch (Exception& E) { ReturnError(String("Failed to parse 'database_identity' from http response: ") + E.what()); }
                try { DescResponse.OwnerIdentity   = JsonObj["owner_identity"]["__identity__"]; }
                catch (Exception& E) { ReturnError(String("Failed to parse 'owner_identity' from http response: ") + E.what()); }
                DescResponse.HostType        = "wasm";
                try { DescResponse.InitialWasmHash = JsonObj["initial_program"]; }
                catch (Exception& E) { ReturnError(String("Failed to parse 'initial_program' from http response: ") + E.what()); }

                return DescResponse;
            }
        };

        struct DeleteDB       final : HttpResponse
        {
            DeleteDB(const long StatusCode, const String& Body) : HttpResponse(StatusCode, Body) {}
        };

        struct GetNames       final : HttpRequest
        {
            Array<String> Names;
        };

        struct AddName        final : HttpResponse
        {

            enum {Success, PermissionDenied, Invalid} Status;

            String Domain;
            Optional<String> Result; //Valid if Status==Success.

        };

        struct SetNames       final : HttpResponse { };

        struct GetIdentity    final : HttpResponse
        {
            HexString Identity;
        };

        struct Subscribe      final : HttpResponse { };

        struct CallReducer    final : HttpResponse
        {
            CallReducer(const long StatusCode, const String& Body) : HttpResponse(StatusCode, Body) {}
        };

        struct GetSchema      final : HttpResponse
        {
            Json RawModuleDef;

            GetSchema(const long StatusCode, const String& Body) : HttpResponse(StatusCode, Body) {}
        };

        struct GetLogs        final : HttpResponse { };

        struct ExecuteSql     final : HttpResponse { };
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
