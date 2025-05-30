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

    HttpResponse HttpClient::Get(const std::string& path, const std::map<std::string, std::string>& headers)
    {
        throw std::runtime_error("Not implemented");
    }

    HttpResponse HttpClient::Post(const std::string& path, const std::string& body,
        const std::map<std::string, std::string>& headers)
    {
        throw std::runtime_error("Not implemented");
    }
} // namespace SpacetimeDb::Utils