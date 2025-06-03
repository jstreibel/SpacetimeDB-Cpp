#include "SpacetimeDB/Utils/HttpClient.hpp"

#include <iostream>
#include <cpr/cpr.h>          // Example: using libcpr for HTTP
#include <stdexcept>

namespace SpacetimeDB {
    Utils::HttpClient::HttpClient(std::string baseUrl, int timeoutMs)
      : baseUrl_(std::move(baseUrl)), timeoutMs_(timeoutMs) {
        // Optionally configure cpr::Session here
    }

    Utils::HttpClient::~HttpClient() {
        // Cleanup if needed
    }

    Utils::Result<Utils::HttpResponse> Utils::HttpClient::Get(const std::string& Path,
                                                              const std::map<std::string, std::string>& Headers) const
    {
        const auto Url = baseUrl_ + Path;

        cpr::Session Session;
        Session.SetUrl(cpr::Url{Url});
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader({Headers.begin(), Headers.end()});

        const auto Response = Session.Get();
        if (Response.error) {
            ReturnError("HTTP GET error: " + Response.error.message);
        }

        return HttpResponse{ Response.status_code, Response.text };
    }

    Utils::Result<Utils::HttpResponse> Utils::HttpClient::Post(
        const std::string& Path,
        const std::string& Body,
        const std::map<std::string, std::string>& Headers
    ) const
    {
        const auto Url = GetUrl(Path);

        cpr::Session Session;
        Session.SetUrl(cpr::Url{Url});
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader(cpr::Header{Headers.begin(), Headers.end()});
        Session.SetBody(cpr::Body{Body});

        const auto Response = Session.Post();
        if (Response.error) {
            ReturnError("HTTP POST error: " + Response.error.message);
        }

        return HttpResponse{ Response.status_code, Response.text };
    }

    std::string Utils::HttpClient::GetUrl(const std::string& Path) const
    {
        return baseUrl_ + Path;
    }
} // namespace SpacetimeDB
