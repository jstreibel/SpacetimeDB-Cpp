#pragma once
#include <string>
#include <map>
#include <cpr/cpr.h>

namespace SpacetimeDb::Utils {

    /// A minimal HTTP response wrapper
    struct HttpResponse {
        int           StatusCode;
        std::string   Body;

        /// for compatibility with previous code
        [[nodiscard]] std::string text() const { return Body; }
    };

    class HttpClient {
    public:
        explicit HttpClient(std::string baseUrl="localhost", int timeoutMs = 30000);
        ~HttpClient();

        HttpResponse Get(const std::string& path,
                         const std::map<std::string,std::string>& headers = {});
        HttpResponse Post(const std::string& path,
                          const std::string& body,
                          const std::map<std::string,std::string>& headers = {});

    private:
        std::string       baseUrl_;
        int               timeoutMs_;
        cpr::Session      session_;
    };
} // namespace SpacetimeDb
