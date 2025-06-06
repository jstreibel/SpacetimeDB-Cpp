//
// Created by joao on 6/5/25.
//

#if !NO_CPR

#include "SpacetimeDB/Http/HttpClient.hpp"

// Unreal Engine workaround:
// Before pulling in CPR, push & undefine Unreal Engine’s verify macro:
#pragma push_macro("verify")
#undef verify
#include <cpr/cpr.h> // using libcpr for HTTP
// Unreal Engine workaround:
// After including CPR headers, restore the original verify macro:
#pragma pop_macro("verify")

namespace SpacetimeDB
{
    Result<HttpResponse> HttpClient::Get(const String &Path, const HttpRequest& Request) const
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

    Result<HttpResponse> HttpClient::Post(const String& Path, const HttpRequest& Request) const
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

    Result<HttpResponse> HttpClient::Get(const String& Path, const Header& Head) const
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

    Result<HttpResponse> HttpClient::Post(
        const String& Path,
        const String& Body,
        const Header& Head
    ) const
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

    Result<HttpResponse> HttpClient::Delete(const String& Path, const HttpRequest& Request) const
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

    Result<HttpResponse> HttpClient::Put(const String& Path, const HttpRequest& Request) const
    {
        ReturnError("NOT IMPLEMENTED");
    }

    Result<HttpResponse> HttpClient::Patch(const String& Path, const HttpRequest& Request) const
    {
        ReturnError("NOT IMPLEMENTED");
    }
}

#endif