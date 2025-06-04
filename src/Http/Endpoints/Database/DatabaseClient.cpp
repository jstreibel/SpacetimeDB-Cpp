#include "../../include/SpacetimeDB/Http/Endpoints/Database/DatabaseClient.hpp"
#include <stdexcept>
#include <future>
#include <utility>

namespace SpacetimeDB::Database {

    Client::Client(const HttpClient& http, WebSocketClient& WebSocket, SpacetimeToken Token)
    : Http_(http), WebSocket_(WebSocket), Token_(std::move(Token)), WebSocketConnected_(false)
    {

    }

    Client::~Client() {
        // Optionally, close the WebSocket if still open
        WebSocket_.Close();
    }

    Result<Response::PublishNew> Client::PublishNew(const Request::PublishNew& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::PublishTo> Client::PublishTo(const Request::PublishTo& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::GetDescription> Client::GetDescription(const Request::GetDescription& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::Delete> Client::Delete(const Request::Delete& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::GetNames> Client::GetNames(const Request::GetNames& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::AddName> Client::AddName(const Request::AddName& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::SetNames> Client::SetNames(const Request::SetNames& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::GetIdentity> Client::GetIdentity(const Request::GetIdentity& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::Subscribe> Client::Subscribe(const Request::Subscribe& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::CallReducer> Client::CallReducer(const Request::CallReducer& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::GetSchema> Client::GetSchema(const Request::GetSchema& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::GetLogs> Client::GetLogs(const Request::GetLogs& Request) const
    {
        ReturnError("Not implemented");
    }

    Result<Response::ExecuteSql> Client::ExecuteSql(const Request::ExecuteSql& Request) const
    {
        ReturnError("Not implemented");
    }

} // namespace SpacetimeDB
