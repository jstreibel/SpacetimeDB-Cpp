#pragma once

#include "Error.hpp"
#include "Json.hpp"

#define HTTP_METHOD_SIGNATURE(METHOD_NAME) \
    [[nodiscard]] Result<Response::METHOD_NAME> METHOD_NAME(const Request::METHOD_NAME&) const;

#define HTTP_METHOD_IMPLEMENTATION(METHOD_NAME) \
    Result<Response::METHOD_NAME> Client::METHOD_NAME(const Request::METHOD_NAME&) const

namespace SpacetimeDB {
    using Header = Map<String, String>;

    struct HttpRequest
    {
        Optional<SpacetimeToken> Bearer;
        Optional<String> ContentType;

        HttpRequest() = default;
        explicit
        HttpRequest(SpacetimeToken Bearer)                      : Bearer(Bearer) { }
        HttpRequest(SpacetimeToken Bearer, String ContentType)  : Bearer(Bearer), ContentType(ContentType) {}

        virtual ~HttpRequest() = default;
        [[nodiscard]] virtual String GetBody()   const { return "{}"; }
        [[nodiscard]] virtual Header GetHeader() const
        {
            Header Head;
            if (Bearer.has_value())      Head["Authorization"] = "Bearer " + Bearer.value();
            if (ContentType.has_value()) Head["Content-Type"] = ContentType.value();
            return Head;
        }
        [[nodiscard]] virtual StringMap GetParameters() const { return {}; }
    };

    /// A minimal HTTP response wrapper
    struct HttpResponse {
        // TODO make sure whats below is always filled
        long     StatusCode;
        String   Body;

        HttpResponse() = delete;
        HttpResponse(const long StatusCode, String Body)
        : StatusCode(StatusCode), Body(std::move(Body)) { }

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
    class IHttpClient {
    public:
        explicit IHttpClient(String BaseUrl="http://localhost:3000", int timeoutMs = 30000);
        virtual ~IHttpClient();

        [[nodiscard]] virtual Result<HttpResponse> Get    (const String& Path, const HttpRequest& Request) const = 0;
        [[nodiscard]] virtual Result<HttpResponse> Post   (const String& Path, const HttpRequest& Request) const = 0;
        [[nodiscard]] virtual Result<HttpResponse> Delete (const String& Path, const HttpRequest& Request) const = 0;
        [[nodiscard]] virtual Result<HttpResponse> Put    (const String& Path, const HttpRequest& Request) const = 0;
        [[nodiscard]] virtual Result<HttpResponse> Patch  (const String& Path, const HttpRequest& Request) const = 0;

        [[nodiscard]] virtual Result<HttpResponse> Get(
            const String& Path,
            const Header& Head = {}) const = 0;
        [[nodiscard]] virtual Result<HttpResponse> Post(
            const String& Path,
            const String& Body,
            const Header& Head = {}) const = 0;

        [[nodiscard]] String GetUrl(const String &Path, const StringMap& Parameters={}) const;

        [[nodiscard]] String GetBaseUrl() const { return BaseUrl; }
    protected:
        const String BaseUrl;
        int          timeoutMs_;
        // cpr::Session      session_;
    };
} // namespace SpacetimeDb
