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
    ///
    class DatabaseClient {
    public:
        ///
        /// Construct with:
        ///  - an HttpClient (configured with base URL),
        ///  - a WsClient instance (initially not connected),
        ///  - a valid JWT token to use for auth.
        ///
        explicit DatabaseClient(
            const HttpClient& http,
            WebSocketClient& WebSocket,
            SpacetimeToken);
        ~DatabaseClient();

        /// POST /v1/database
        /// Publish a new database given its module code.
        Result<String> PublishNewDatabase();

        /// POST /v1/database/:name_or_identity
        /// Publish to a database given its module code.
        void PublishToDatabase();

        /// GET /v1/database/:name_or_identity
        /// Get a JSON description of a database.
        void GetDatabaseDescription();

        /// DELETE /v1/database/:name_or_identity
        /// Delete a database.
        void DeleteDatabase();

        /// GET /v1/database/:name_or_identity/names
        /// Get the names this database can be identified by.
        void GetDatabaseNames();

        /// POST /v1/database/:name_or_identity/names
        /// Add a new name for this database.
        void AddDatabaseName(const String & databaseName);

        /// PUT /v1/database/:name_or_identity/names
        /// Set the list of names for this database.
        void SetDatabaseNames();

        /// GET /v1/database/:name_or_identity/identity
        /// Get the identity of a database.
        void GetDatabaseIdentity();

        /// GET /v1/database/:name_or_identity/subscribe
        /// Begin a WebSocket connection.
        void Subscribe(const std::string& ModuleName,
                                   const std::string& SqlQuery,
                                   const std::function<void(const Json& Event)>& OnEvent);

        /// POST /v1/database/:name_or_identity/call/:reducer
        /// Invoke a reducer in a database.
        Result<Json> CallReducer(const String& ModuleName, const String& ReducerName, const Json& Args);

        /// GET /v1/database/:name_or_identity/schema
        /// Get the schema for a database.
        void GetSchema();

        /// GET /v1/database/:name_or_identity/logs
        /// Retrieve logs from a database.
        void GetLogs();

        /// POST /v1/database/:name_or_identity/sql
        /// Run a SQL query against a database.
        [[nodiscard]] Result<Json> ExecuteSql(const String& ModuleName, const String& Sql, const Json& Params) const;

    private:
        const HttpClient& Http_;
        WebSocketClient&         WebSocket_;
        SpacetimeToken           Token_;
        bool                     WebSocketConnected_;
    };

} // namespace SpacetimeDb
