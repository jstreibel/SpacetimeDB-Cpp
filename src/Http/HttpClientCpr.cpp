//
// Created by joao on 6/6/25.
//

#ifdef STDB_HTTP_CPR_IMPL

#include "SpacetimeDB/Http/HttpClientCpr.hpp"

// Unreal Engine workaround:
// Before pulling in CPR, push & undefine Unreal Engine’s verify macro:
#pragma push_macro("verify")
#undef verify
#include <cpr/cpr.h> // using libcpr for HTTP
// Unreal Engine workaround:
// After including CPR headers, restore the original verify macro:
#pragma pop_macro("verify")

#warning "Http client libcpr implementation is currently incomplete. Expect lack of functionalities."

namespace SpacetimeDB::Cpr
{
    Result<HttpResponse> HttpClientCpr::Get(const String& Path, const HttpRequest& Request) const
    {
        const auto Url = GetUrl(Path, Request.GetParameters());

        const auto Head = Request.GetHeader();

        cpr::Session Session;
        Session.SetUrl(Url);
        Session.SetTimeout(timeoutMs_);
        Session.SetBody(Request.GetBody());
        Session.SetHeader({Head.begin(), Head.end()});

        const auto Response = Session.Get();
        if (Response.error)
        {
            ReturnError("HTTP GET error: " + Response.error.message);
        }

        return HttpResponse{Response.status_code, Response.text};
    }

    Result<HttpResponse> HttpClientCpr::Post(const String& Path, const HttpRequest& Request) const
    {
        const auto Url = GetUrl(Path, Request.GetParameters());

        const auto Head = Request.GetHeader();

        cpr::Session Session;
        Session.SetUrl(Url);
        Session.SetTimeout(timeoutMs_);
        Session.SetBody(Request.GetBody());
        Session.SetHeader({Head.begin(), Head.end()});

        const auto Response = Session.Post();
        if (Response.error)
        {
            ReturnError("HTTP POST error: " + Response.error.message);
        }

        return HttpResponse{Response.status_code, Response.text};
    }

    Result<HttpResponse> HttpClientCpr::Delete(const String& Path, const HttpRequest& Request) const
    {
        const auto Url = GetUrl(Path);

        const auto Head = Request.GetHeader();

        cpr::Session Session;
        Session.SetUrl(cpr::Url{Url});
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader({Head.begin(), Head.end()});
        Session.SetBody(Request.GetBody());

        const auto Response = Session.Delete();
        if (Response.error)
        {
            ReturnError("HTTP DELETE error: " + Response.error.message);
        }

        return HttpResponse{ Response.status_code, Response.text };

    }

    Result<HttpResponse> HttpClientCpr::Put(const String& Path, const HttpRequest& Request) const
    {
        ReturnError("NOT IMPLEMENTED");

    }

    Result<HttpResponse> HttpClientCpr::Patch(const String& Path, const HttpRequest& Request) const
    {
        ReturnError("NOT IMPLEMENTED");
    }

    Result<HttpResponse> HttpClientCpr::Get(const String& Path, const Header& Head) const
    {
        const auto Url = GetUrl(Path);

        cpr::Session Session;
        Session.SetUrl(cpr::Url{Url});
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader({Head.begin(), Head.end()});

        const auto Response = Session.Get();
        if (Response.error) {
            ReturnError("HTTP GET error: " + Response.error.message);
        }

        return HttpResponse{ Response.status_code, Response.text };
    }

    Result<HttpResponse> HttpClientCpr::Post(const String& Path, const String& Body, const Header& Head) const
    {
        const auto Url = GetUrl(Path);

        cpr::Session Session;
        Session.SetUrl(cpr::Url{Url});
        Session.SetTimeout(timeoutMs_);
        Session.SetHeader({Head.begin(), Head.end()});
        Session.SetBody(cpr::Body{Body});

        const auto Response = Session.Post();
        if (Response.error) {
            ReturnError("HTTP POST error: " + Response.error.message);
        }

        return HttpResponse{ Response.status_code, Response.text };

    }
}

#endif
