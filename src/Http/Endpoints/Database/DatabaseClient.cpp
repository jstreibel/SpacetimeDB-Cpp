#include "../../include/SpacetimeDB/Http/Endpoints/Database/DatabaseClient.hpp"
#include <stdexcept>
#include <future>
#include <utility>

namespace SpacetimeDB::Database {

    DatabaseClient::DatabaseClient(const HttpClient& http, WebSocketClient& WebSocket, SpacetimeToken Token)
    : Http_(http), WebSocket_(WebSocket), Token_(std::move(Token)), WebSocketConnected_(false)
    {

    }

    DatabaseClient::~DatabaseClient() {
        // Optionally close the WebSocket if still open
        WebSocket_.Close();
    }

    Result<Json> DatabaseClient::ExecuteSql(const String& ModuleName, const String& Sql, const Json& Params) const
    {
        const std::string Path = "/v1/database/" + ModuleName + "/sql";
        Json payload = Json::object();
        payload["sql"] = Sql;
        if (!Params.is_null()) {
          payload["params"] = Params;
        }

        // Build headers
        Header Head;
        Head["Authorization"] = "Bearer " + Token_;
        Head["Content-Type"]  = "application/json";

        const auto PostResult = Http_.Post(Path, payload.dump(), Head);
        if (!IsValid(PostResult))
        {
            const auto Message = GetErrorMessage(PostResult);
            ReturnError("Failed to POST to " + Path + ": " + Message);
        }
        auto [StatusCode, Body] = GetResult(PostResult);
        if (StatusCode != 200) {
            ReturnError("SQL failed (status " + std::to_string(StatusCode) + ")");
        }
        return Json::parse(Body);
    }

    void DatabaseClient::Subscribe(const std::string& ModuleName,
                                   const std::string& SqlQuery,
                                   const std::function<void(const Json& Event)>& OnEvent)
    {
        if (!WebSocketConnected_) {
            // Build ws:// URL
            const std::string WebSocketUrl = "ws://localhost:3000/v1/database/" + ModuleName + "/ws?token=" + Token_;
            WebSocket_.Connect(WebSocketUrl);
            WebSocketConnected_ = true;
        }

        // Register the 'receive' callback
        WebSocket_.OnReceive([OnEvent](const Json& JsonMsg) {
            // Dispatch only Insert/Update/Delete types; user can inspect "type"
            OnEvent(JsonMsg);
        });

        // Send the Subscribe frame
        Json subMsg = Json::object();
        subMsg["type"]  = "Subscribe";
        subMsg["query"] = SqlQuery;
        WebSocket_.Send(subMsg);
    }

    Result<Json> DatabaseClient::CallReducer(const String& ModuleName, const String& ReducerName, const Json& Args)
    {
        if (!WebSocketConnected_) {
            const std::string WebSocketUrl = "ws://localhost:3000/v1/database/" + ModuleName + "/ws?token=" + Token_;
            WebSocket_.Connect(WebSocketUrl);
            WebSocketConnected_ = true;
        }

        // Build the request frame
        Json Request = Json::object();
        Request["type"]    = "CallReducer";
        Request["reducer"] = ReducerName;
        Request["args"]    = Args;
        WebSocket_.Send(Request);

        // Wait for exactly one ReducerResult frame. Use a promise/future for simplicity.
        std::promise<Json> Promise;
        std::future<Json> Future = Promise.get_future();

        WebSocket_.OnReceive([&Promise](const Json& JsonMsg) mutable {
            if (JsonMsg.contains("type") && JsonMsg["type"] == "ReducerResult") {
                Promise.set_value(JsonMsg);
            } else if (JsonMsg.contains("type") && JsonMsg["type"] == "Error") {
                // TODO: Could set_exception here, but for brevity, set_value with the error object
                Promise.set_value(JsonMsg);
            }
        });

        return Future.get();  // Blocks until ReducerResult or Error arrives
    }

} // namespace SpacetimeDB
