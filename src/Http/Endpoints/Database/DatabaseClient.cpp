#include "../../include/SpacetimeDB/Http/Endpoints/Database/DatabaseClient.hpp"
#include <stdexcept>
#include <future>
#include <utility>
#include <boost/asio/ip/basic_resolver_iterator.hpp>

namespace SpacetimeDB::Database {

    const String DatabaseEndpoint = "/v1/database";

    Client::Client(const String& NameOrIdentity, const String& BaseUrl, const Milliseconds Timeout)
    : HttpClient(BaseUrl, Timeout)
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
        if (HttpResponse.error)
        {
            ReturnError("HTTP GET error: " + HttpResponse.error.message);
        }

        return Response::GetDescription::FromResponse(HttpResponse);
    }

    Result<Response::DeleteDB> Client::DeleteDB(const Request::DeleteDB& Request) const
    {
        const String Endpoint = DatabaseEndpoint + "/" + NameOrIdentity;
        if (const auto HttpResponse = Delete(Endpoint, Request); HttpResponse.error)
        {
            ReturnError("HTTP GET error: " + HttpResponse.error.message);
        }

        return Response::DeleteDB{};
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
        if (const auto HttpResponse = Delete(Endpoint, Request); HttpResponse.error)
        {
            ReturnError("HTTP POST error: " + HttpResponse.error.message);
        }

        return Response::CallReducer{};
    }

    Result<Response::GetSchema> Client::GetSchema(const Request::GetSchema& Request) const
    {
        const String Endpoint = DatabaseEndpoint + "/" + NameOrIdentity + "/schema";
        const auto HttpResponse = Get(Endpoint, Request);
        if (HttpResponse.error)
        {
            ReturnError("HTTP GET error: " + HttpResponse.error.message);
        }

        Response::GetSchema Response{};
        Response.StatusCode = HttpResponse.status_code;
        Response.Body = HttpResponse.text;
        Response.RawModuleDef = Json::parse(HttpResponse.text);

        return Response;
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
