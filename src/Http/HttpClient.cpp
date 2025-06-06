#include "SpacetimeDB/Http/HttpClient.hpp"

#include <iostream>
#include <stdexcept>
#include <boost/config/detail/suffix.hpp>

namespace SpacetimeDB {
    HttpClient::HttpClient(String BaseUrl, Milliseconds Timeout)
    : BaseUrl(std::move(BaseUrl))
    , timeoutMs_(Timeout) {
        // Optionally configure cpr::Session here
    }

    HttpClient::~HttpClient() = default;

    std::string HttpClient::GetUrl(const std::string& Path, const StringMap& Parameters) const
    {
        auto Url = BaseUrl + Path;

        for (const auto& [Key, Value] : Parameters)
        {
            Url += "?" + Key + "=" + Value;
        }

        return Url;
    }
} // namespace SpacetimeDB
