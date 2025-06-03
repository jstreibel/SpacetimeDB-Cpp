#pragma once

#include <Http/HttpClient.hpp>
#include <Http/Json.hpp>
#include "DatabaseModels.hpp"
#include "Http/WebSocket/WebSocketClient.hpp"

namespace SpacetimeDB {

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
            const std::string & token);
        ~DatabaseClient();

        ///
        /// Execute raw SQL via POST /v1/database/{module}/sql
        /// Returns the parsed JSON response (e.g., {"rows": [...]} or {"rows_affected": N}).
        ///
        [[nodiscard]] Result<Json> ExecuteSql(const std::string& ModuleName,
                               const std::string& Sql,
                               const Json& Params = {}) const;

        ///
        /// Open a WebSocket to ws://…/v1/database/{moduleName}/ws?token={token}.
        /// Then send a Subscribe frame: {"type":"Subscribe","query":sqlQuery}.
        /// All Insert/Update/Delete JSON events are forwarded to onEvent.
        ///
        void Subscribe(const std::string& ModuleName,
                       const std::string& SqlQuery,
                       std::function<void(const Json& event)> OnEvent);

        ///
        /// Call a reducer over WebSocket:
        ///   Send {"type":"CallReducer","reducer":reducerName,"args":args}.
        ///   Block (or asynchronously wait) for a {"type":"ReducerResult",…} frame.
        /// Returns the raw JSON result.
        ///
        Result<Json> CallReducer(const std::string& ModuleName,
                                const std::string& ReducerName,
                                const Json& Args);

        ///
        /// Unsubscribe from an existing subscription by its ID.
        /// Sends {"type":"Unsubscribe","subscription_id":id}.
        ///
        void Unsubscribe(const std::string& SubscriptionId) const;

    private:
        const HttpClient& Http_;
        WebSocketClient&         WebSocket_;
        std::string              Token_;
        bool                     WebSocketConnected_;
    };

} // namespace SpacetimeDb
