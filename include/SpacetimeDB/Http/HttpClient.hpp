#pragma once

#include <cpr/cpr.h>

#include "Error.hpp"
#include "Json.hpp"

namespace SpacetimeDB {
    using Header = cpr::Header;

    struct HttpRequest
    {
        Optional<SpacetimeToken> Bearer;
        Optional<String> ContentType;

        HttpRequest() = default;
        explicit
        HttpRequest(SpacetimeToken Bearer)                      : Bearer(Bearer) { }
        HttpRequest(SpacetimeToken Bearer, String ContentType)  : Bearer(Bearer), ContentType(ContentType) {}

        virtual ~HttpRequest() = default;
        virtual String GetBody()   const { return {}; }
        virtual Header GetHeader() const
        {
            Header Head;
            if (Bearer.has_value())      Head["Authorization"] = "Bearer " + Bearer.value();
            if (ContentType.has_value()) Head["Content-Type"] = ContentType.value();
            return Head;
        }
        virtual Json AsJson() const { return GetBody(); }
    };

    /// A minimal HTTP response wrapper
    struct HttpResponse {
        long     StatusCode;
        String   Body;
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
        explicit HttpClient(String baseUrl="http://localhost:3000", int timeoutMs = 30000);
        ~HttpClient();

        [[nodiscard]] Result<HttpResponse> Get(
            const String& Path,
            const Header& Head = {}) const;
        [[nodiscard]] Result<HttpResponse> Post(
            const String& Path,
            const String& Body,
            const Header& Head = {}) const;

        [[nodiscard]] String GetUrl(const String &Path) const;

        [[nodiscard]] String GetBaseUrl() const { return baseUrl_; }
    private:
        String       baseUrl_;
        int          timeoutMs_;
        // cpr::Session      session_;
    };
} // namespace SpacetimeDb
