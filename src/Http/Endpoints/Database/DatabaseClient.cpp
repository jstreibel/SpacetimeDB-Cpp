#include "../../include/SpacetimeDB/Http/Endpoints/Database/DatabaseClient.hpp"
#include <stdexcept>
#include <future>
#include <boost/asio/ip/basic_resolver_iterator.hpp>

namespace SpacetimeDB::Database {

    const String DatabaseEndpoint = "/v1/database";

    Client::Client(const String& NameOrIdentity, const String& BaseUrl, const Milliseconds Timeout)
    : IHttpClient(BaseUrl, Timeout)
    , NameOrIdentity(NameOrIdentity)
    {

    }

    Client::~Client() = default;

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
        const String Endpoint = DatabaseEndpoint + "/" + NameOrIdentity;
        const auto HttpResponse = Get(Endpoint, Request);
        OnError(HttpResponse, "Spacetime GET database description")

        return Response::GetDescription::FromResponse(GetResult(HttpResponse));
    }

    Result<Response::DeleteDB> Client::DeleteDB(const Request::DeleteDB& Request) const
    {
        const String Endpoint = DatabaseEndpoint + "/" + NameOrIdentity;
        const auto HttpResponse = Delete(Endpoint, Request);
        OnError(HttpResponse, "Spacetime DELETE database");

        const auto& Response = GetResult(HttpResponse);
        return Response::DeleteDB{Response.StatusCode, Response.Body};
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
        const auto Reducer = Request.Reducer;

        const String Endpoint = DatabaseEndpoint + "/" + NameOrIdentity + "/call/";
        const auto HttpResponse = Post(Endpoint, Request);
        OnError(HttpResponse, "Spacetime GET calling reducer")

        const auto& Response = GetResult(HttpResponse);
        return Response::CallReducer{Response.StatusCode, Response.Body};
    }

    Result<Response::GetSchema> Client::GetSchema(const Request::GetSchema& Request) const
    {
        const String Endpoint = DatabaseEndpoint + "/" + NameOrIdentity + "/schema";
        const auto HttpResponse = Get(Endpoint, Request);
        OnError(HttpResponse, "Spacetime GET schema")

        const auto& Response = GetResult(HttpResponse);

        Response::GetSchema SchemaResponse{Response.StatusCode, Response.Body};
        SchemaResponse.RawModuleDef = Json::parse(Response.Body);

        return SchemaResponse;
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
