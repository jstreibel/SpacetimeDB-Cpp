#pragma once

#include <Http/HttpClient.hpp>
#include <Http/Json.hpp>
#include "DatabaseModels.hpp"
#include "Http/WebSocket/WebSocketClient.hpp"

namespace SpacetimeDB::Database {

    ///
    /// Provides both HTTP and WebSocket database operations:
    ///  - Raw SQL over HTTP
    ///  - Subscribe / Reducer calls over WebSocket
    ///  - Etc.
    ///
    class Client {
    public:
        ///
        /// Construct with:
        ///  - an HttpClient (configured with base URL),
        ///  - a WsClient instance (initially not connected),
        ///  - a valid JWT token to use for auth.
        ///
        explicit Client(
            const HttpClient& http,
            WebSocketClient& WebSocket,
            SpacetimeToken);
        ~Client();

        /// POST /v1/database
        /// Publish a new database given its module code.
        HTTP_METHOD_SIGNATURE(PublishNew)

        /// POST /v1/database/:name_or_identity
        /// Publish to a database given its module code.
        HTTP_METHOD_SIGNATURE(PublishTo)

        /// GET /v1/database/:name_or_identity
        /// Get a JSON description of a database.
        HTTP_METHOD_SIGNATURE(GetDescription)

        /// DELETE /v1/database/:name_or_identity
        /// Delete a database.
        HTTP_METHOD_SIGNATURE(Delete)

        /// GET /v1/database/:name_or_identity/names
        /// Get the names this database can be identified by.
        HTTP_METHOD_SIGNATURE(GetNames)

        /// POST /v1/database/:name_or_identity/names
        /// Add a new name for this database.
        HTTP_METHOD_SIGNATURE(AddName)

        /// PUT /v1/database/:name_or_identity/names
        /// Set the list of names for this database.
        HTTP_METHOD_SIGNATURE(SetNames)

        /// GET /v1/database/:name_or_identity/identity
        /// Get the identity of a database.
        HTTP_METHOD_SIGNATURE(GetIdentity)

        /// GET /v1/database/:name_or_identity/subscribe
        /// Begin a WebSocket connection.
        void Subscribe(const std::string& ModuleName,
                                   const std::string& SqlQuery,
                                   const std::function<void(const Json& Event)>& OnEvent);
        HTTP_METHOD_SIGNATURE(Subscribe)

        /// POST /v1/database/:name_or_identity/call/:reducer
        /// Invoke a reducer in a database.
        Result<Json> CallReducer(const String& ModuleName, const String& ReducerName, const Json& Args);
        HTTP_METHOD_SIGNATURE(CallReducer)

        /// GET /v1/database/:name_or_identity/schema
        /// Get the schema for a database.
        HTTP_METHOD_SIGNATURE(GetSchema)

        /// GET /v1/database/:name_or_identity/logs
        /// Retrieve logs from a database.
        HTTP_METHOD_SIGNATURE(GetLogs)

        /// POST /v1/database/:name_or_identity/sql
        /// Run an SQL query against a database.
        [[nodiscard]] Result<Json> ExecuteSql(const String& ModuleName, const String& Sql, const Json& Params) const;
        HTTP_METHOD_SIGNATURE(ExecuteSql)

    private:
        const HttpClient& Http_;
        WebSocketClient&         WebSocket_;
        SpacetimeToken           Token_;
        bool                     WebSocketConnected_;
    };

} // namespace SpacetimeDb
