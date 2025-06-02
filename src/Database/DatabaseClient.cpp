#include "SpacetimeDB/Database/DatabaseClient.hpp"
#include <stdexcept>
#include <future>

namespace SpacetimeDB {

    DatabaseClient::DatabaseClient(
        const Utils::HttpClient& http,
        WebSocketClient& WebSocket,
        const std::string& token)
    : Http_(http), WebSocket_(WebSocket), Token_(token), WebSocketConnected_(false)
    {

    }

    DatabaseClient::~DatabaseClient() {
        // Optionally close the WebSocket if still open
        WebSocket_.Close();
    }

    Utils::Result<Utils::Json> DatabaseClient::ExecuteSql(const std::string& ModuleName,
                                           const std::string& Sql,
                                           const Utils::Json& Params) const
    {
        const std::string Path = "/v1/database/" + ModuleName + "/sql";
        Utils::Json payload = Utils::Json::object();
        payload["sql"] = Sql;
        if (!Params.is_null()) {
          payload["params"] = Params;
        }

        // Build headers
        std::map<std::string, std::string> headers;
        headers["Authorization"] = "Bearer " + Token_;
        headers["Content-Type"]  = "application/json";

        const auto PostResult = Http_.Post(Path, payload.dump(), headers);
        if (!Utils::IsValid(PostResult))
        {
            const auto Message = Utils::GetErrorMessage(PostResult);
            ReturnError("Failed to POST to " + Path + ": " + Message);
        }
        auto [StatusCode, Body] = Utils::GetResult(PostResult);
        if (StatusCode != 200) {
            ReturnError("SQL failed (status " + std::to_string(StatusCode) + ")");
        }
        return Utils::Json::parse(Body);
    }

    void DatabaseClient::Subscribe(const std::string& ModuleName,
                                   const std::string& SqlQuery,
                                   std::function<void(const Utils::Json& Event)> OnEvent)
    {
        if (!WebSocketConnected_) {
            // Build ws:// URL
            const std::string WebSocketUrl = "ws://localhost:3000/v1/database/" + ModuleName + "/ws?token=" + Token_;
            WebSocket_.Connect(WebSocketUrl);
            WebSocketConnected_ = true;
        }

        // Register the 'receive' callback
        WebSocket_.OnReceive([OnEvent](const Utils::Json& JsonMsg) {
            // Dispatch only Insert/Update/Delete types; user can inspect "type"
            OnEvent(JsonMsg);
        });

        // Send the Subscribe frame
        Utils::Json subMsg = Utils::Json::object();
        subMsg["type"]  = "Subscribe";
        subMsg["query"] = SqlQuery;
        WebSocket_.Send(subMsg);
    }

    Utils::Result<Utils::Json> DatabaseClient::CallReducer(const std::string& ModuleName,
                                            const std::string& ReducerName,
                                            const Utils::Json& Args)
    {
        if (!WebSocketConnected_) {
            const std::string WebSocketUrl = "ws://localhost:3000/v1/database/" + ModuleName + "/ws?token=" + Token_;
            WebSocket_.Connect(WebSocketUrl);
            WebSocketConnected_ = true;
        }

        // Build the request frame
        Utils::Json Request = Utils::Json::object();
        Request["type"]    = "CallReducer";
        Request["reducer"] = ReducerName;
        Request["args"]    = Args;
        WebSocket_.Send(Request);

        // Wait for exactly one ReducerResult frame. Use a promise/future for simplicity.
        std::promise<Utils::Json> Promise;
        std::future<Utils::Json> Future = Promise.get_future();

        WebSocket_.OnReceive([&Promise](const Utils::Json& JsonMsg) mutable {
            if (JsonMsg.contains("type") && JsonMsg["type"] == "ReducerResult") {
                Promise.set_value(JsonMsg);
            } else if (JsonMsg.contains("type") && JsonMsg["type"] == "Error") {
                // TODO: Could set_exception here, but for brevity, set_value with the error object
                Promise.set_value(JsonMsg);
            }
        });

        return Future.get();  // Blocks until ReducerResult or Error arrives
    }

    void DatabaseClient::Unsubscribe(const std::string& SubscriptionId) const
    {
        if (!WebSocketConnected_) return;
        Utils::Json Message = Utils::Json::object();
        Message["type"] = "Unsubscribe";
        Message["subscription_id"] = SubscriptionId;
        WebSocket_.Send(Message);
    }

} // namespace SpacetimeDB
