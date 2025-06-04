#include "SpacetimeDB/Http/HttpClient.hpp"

#include <iostream>
#include <cpr/cpr.h>          // Example: using libcpr for HTTP
#include <stdexcept>

namespace SpacetimeDB {
    HttpClient::HttpClient(String baseUrl, int timeoutMs)
      : baseUrl_(std::move(baseUrl)), timeoutMs_(timeoutMs) {
        // Optionally configure cpr::Session here
    }

    HttpClient::~HttpClient() = default;

    Result<HttpResponse> HttpClient::Get(const String& Path, const Header& Head) const
    {
        const auto Url = baseUrl_ + Path;

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

    std::string HttpClient::GetUrl(const std::string& Path) const
    {
        return baseUrl_ + Path;
    }
} // namespace SpacetimeDB
