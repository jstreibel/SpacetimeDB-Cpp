//
// Created by joao on 6/6/25.
//

#ifndef HTTPCLIENTCPR_H
#define HTTPCLIENTCPR_H

#ifdef STDB_HTTP_CPR_IMPL

#include "HttpClient.hpp"

namespace SpacetimeDB::Cpr {

    class HttpClientCpr final : public IHttpClient {
    public:
        explicit HttpClientCpr(
            const String& BaseUrl="http://localhost:3000",
            const int TimeoutMs = 30000)
        : IHttpClient(BaseUrl, TimeoutMs) {};

        [[nodiscard]] Result<HttpResponse>
        Get(const String& Path, const HttpRequest& Request) const override;
        [[nodiscard]] Result<HttpResponse>
        Post(const String& Path, const HttpRequest& Request) const override;
        [[nodiscard]] Result<HttpResponse>
        Delete(const String& Path, const HttpRequest& Request) const override;
        [[nodiscard]] Result<HttpResponse>
        Put(const String& Path, const HttpRequest& Request) const override;
        [[nodiscard]] Result<HttpResponse>
        Patch(const String& Path, const HttpRequest& Request) const override;

        [[nodiscard]] Result<HttpResponse>
        Get(const String& Path, const Header& Head) const override;
        [[nodiscard]] Result<HttpResponse>
        Post(const String& Path, const String& Body, const Header& Head) const override;
    };

} // Cpr::SpacetimeDB

#endif

#endif //HTTPCLIENTCPR_H
