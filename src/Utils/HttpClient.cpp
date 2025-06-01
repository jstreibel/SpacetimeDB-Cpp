#include "SpacetimeDB/Utils/HttpClient.hpp"
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

    Utils::HttpResponse Utils::HttpClient::Get(const std::string& path,
                                               const std::map<std::string, std::string>& headers) const
    {
        std::string url = baseUrl_ + path;
        cpr::Session session;
        session.SetUrl(cpr::Url{url});
        session.SetTimeout(timeoutMs_);
        for (auto& [k,v] : headers) {
            session.SetHeader({{k, v}});
        }
        cpr::Response r = session.Get();
        if (r.error) {
            throw std::runtime_error("HTTP GET error: " + r.error.message);
        }
        return HttpResponse{ r.status_code, r.text };
    }

    Utils::HttpResponse Utils::HttpClient::Post(const std::string& path,
                                                const std::string& body,
                                                const std::map<std::string, std::string>& headers) const
    {
        std::string url = baseUrl_ + path;
        cpr::Session session;
        session.SetUrl(cpr::Url{url});
        session.SetTimeout(timeoutMs_);
        for (auto& [k,v] : headers) {
            session.SetHeader({{k, v}});
        }
        session.SetBody(cpr::Body{body});
        session.SetHeader(cpr::Header{{"Content-Type", "application/json"}});
        cpr::Response r = session.Post();
        if (r.error) {
            throw std::runtime_error("HTTP POST error: " + r.error.message);
        }
        return HttpResponse{ r.status_code, r.text };
    }

} // namespace SpacetimeDB
