//
// Created by joao on 6/5/25.
//

#ifdef STDB_HTTP_ASIO_BEAST_IMPL

#include "SpacetimeDB/Http/HttpClient.hpp"
#include "AsioBeastWrapper.inl"

namespace SpacetimeDB
{


    Result<HttpResponse> IHttpClient::Get(const String &Path, const HttpRequest& Request) const
    {
        const auto Url = GetUrl(Path, Request.GetParameters());

        const auto Head = Request.GetHeader();
        const auto Body = Request.GetBody();

        ReturnError("Asio + Beast HTTP GET not implemented.");
    }

    Result<HttpResponse> IHttpClient::Post(const String& Path, const HttpRequest& Request) const
    {
        const auto Url = GetUrl(Path, Request.GetParameters());

        const auto Head = Request.GetHeader();
        const auto Body = Request.GetBody();

        ReturnError("Asio + Beast HTTP POST not implemented.");
    }

    Result<HttpResponse> IHttpClient::Get(const String& Path, const Header& Head) const
    {
        ReturnError("Not implemented.");
    }

    Result<HttpResponse> IHttpClient::Post(
        const String& Path,
        const String& Body,
        const Header& Head
    ) const
    {
        ReturnError("Not implemented.");
    }

    Result<HttpResponse> IHttpClient::Delete(const String& Path, const HttpRequest& Request) const
    {
        ReturnError("Not implemented.");
    }

    Result<HttpResponse> IHttpClient::Put(const String& Path, const HttpRequest& Request) const
    {
        ReturnError("NOT IMPLEMENTED");
    }

    Result<HttpResponse> IHttpClient::Patch(const String& Path, const HttpRequest& Request) const
    {
        ReturnError("NOT IMPLEMENTED");
    }
}

#endif