#pragma once
#include <string>
#include <map>
#include <cpr/cpr.h>

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

        HttpResponse Get(const std::string& path,
                         const std::map<std::string,std::string>& headers = {}) const;
        HttpResponse Post(const std::string& path,
                          const std::string& body,
                          const std::map<std::string,std::string>& headers = {}) const;

    private:
        std::string       baseUrl_;
        int               timeoutMs_;
        // cpr::Session      session_;
    };
} // namespace SpacetimeDb
