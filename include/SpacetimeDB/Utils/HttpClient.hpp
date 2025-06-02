#pragma once
#include <string>
#include <map>
#include <cpr/cpr.h>

#include "Error.hpp"

namespace SpacetimeDB::Utils {

    /// A minimal HTTP response wrapper
    struct HttpResponse {
        long          StatusCode;
        std::string   Body;
    };

    /**
     * This is a thin wrapper around cpr::Session that knows our SpacetimeSB server's
     * base URL and timeout settings.
     *
     * It exposes simple `Get(path, headers)` and `Post(path, body, headers)` methods
     * and returns a small `HttpResponse` struct (status code + body) so that all
     * higher-level code can just think in terms of REST calls without caring about
     * the curl- or socket-level details.
     */
    class HttpClient {
    public:
        explicit HttpClient(std::string baseUrl="http://localhost:3000", int timeoutMs = 30000);
        ~HttpClient();

        [[nodiscard]] Result<HttpResponse> Get(
            const std::string& Path,
            const std::map<std::string,std::string>& Headers = {}) const;
        [[nodiscard]] Result<HttpResponse> Post(
            const std::string& Path,
            const std::string& Body,
            const std::map<std::string,std::string>& Headers = {}) const;

        [[nodiscard]] std::string GetUrl(const std::string &Path) const;

        [[nodiscard]] std::string GetBaseUrl() const { return baseUrl_; }
    private:
        std::string       baseUrl_;
        int               timeoutMs_;
        // cpr::Session      session_;
    };
} // namespace SpacetimeDb
