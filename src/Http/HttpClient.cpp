#include "SpacetimeDB/Http/HttpClient.hpp"

#include <iostream>
#include <cpr/cpr.h>          // Example: using libcpr for HTTP
#include <stdexcept>
#include <boost/config/detail/suffix.hpp>

namespace SpacetimeDB {
    HttpClient::HttpClient(String BaseUrl, Milliseconds Timeout)
    : BaseUrl(std::move(BaseUrl))
    , timeoutMs_(Timeout) {
        // Optionally configure cpr::Session here
    }

    HttpClient::~HttpClient() = default;

    cpr::Response HttpClient::Get(const String &Endpoint, const HttpRequest& Request) const
    {
        const auto Url = GetUrl(Endpoint);

        cpr::Session Session;
        Session.SetUrl(Url);
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader(Request.GetHeader());

        return Session.Get();
    }

    Result<HttpResponse> HttpClient::Get(const String& Path, const Header& Head) const
    {
        const auto Url = GetUrl(Path);

        cpr::Session Session;
        Session.SetUrl(cpr::Url{Url});
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader(Head);

        const auto Response = Session.Get();
        if (Response.error) {
            ReturnError("HTTP GET error: " + Response.error.message);
        }

        return HttpResponse{ Response.status_code, Response.text };
    }

    Result<HttpResponse> HttpClient::Post(
        const String& Path,
        const String& Body,
        const Header& Head
    ) const
    {
        const auto Url = GetUrl(Path);

        cpr::Session Session;
        Session.SetUrl(cpr::Url{Url});
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader(Head);
        Session.SetBody(cpr::Body{Body});

        const auto Response = Session.Post();
        if (Response.error) {
            ReturnError("HTTP POST error: " + Response.error.message);
        }

        return HttpResponse{ Response.status_code, Response.text };
    }

    cpr::Response HttpClient::Delete(const String& Path, const HttpRequest& Request) const
    {
        const auto Url = GetUrl(Path);
        cpr::Session Session;
        Session.SetUrl(cpr::Url{Url});
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader(Request.GetHeader());
        Session.SetBody(Request.GetBody());

        return Session.Delete();
    }


    std::string HttpClient::GetUrl(const std::string& Path) const
    {
        return BaseUrl + Path;
    }
} // namespace SpacetimeDB
