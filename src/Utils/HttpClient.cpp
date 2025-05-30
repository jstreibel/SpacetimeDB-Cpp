#include "Utils/HttpClient.hpp"
#include <cpr/cpr.h>

namespace SpacetimeDb::Utils {

    HttpClient::HttpClient(std::string baseUrl, int timeoutMs)
      : baseUrl_(std::move(baseUrl))
      , timeoutMs_(timeoutMs)
      , session_()
    {
        // configure the CPR session
        session_.SetUrl(cpr::Url{baseUrl_});
        session_.SetTimeout(cpr::Timeout{timeoutMs_});
    }

    HttpClient::~HttpClient() = default;

    HttpResponse HttpClient::Get(const std::string& path, const std::map<std::string, std::string>& headers) const
    {
        // Build full URL
        const std::string url = baseUrl_ + path;

        // Perform the GET request
        const cpr::Response r = cpr::Get(
            cpr::Url{url},
            cpr::Header{headers.begin(), headers.end()}, // for std::map compatibility
            cpr::Timeout{timeoutMs_}
        );

        // Wrap and return
        return HttpResponse{ r.status_code, r.text };
    }

    HttpResponse HttpClient::Post(const std::string& path, const std::string& body,
        const std::map<std::string, std::string>& headers) const
    {
        // Build full URL
        const std::string url = baseUrl_ + path;

        // Perform the POST request
        const cpr::Response r = cpr::Post(
            cpr::Url{url},
            cpr::Body{body},
            cpr::Header{headers.begin(), headers.end()},
            cpr::Timeout{timeoutMs_}
        );

        // Wrap and return
        return HttpResponse{ r.status_code, r.text };
    }
} // namespace SpacetimeDb::Utils