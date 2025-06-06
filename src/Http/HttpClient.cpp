#include "SpacetimeDB/Http/HttpClient.hpp"



namespace SpacetimeDB {
    IHttpClient::IHttpClient(String BaseUrl, Milliseconds Timeout)
    : BaseUrl(std::move(BaseUrl))
    , timeoutMs_(Timeout) {
        // Optionally configure cpr::Session here
    }

    IHttpClient::~IHttpClient() = default;

    String IHttpClient::GetUrl(const String& Path, const StringMap& Parameters) const
    {
        auto Url = BaseUrl + Path;

        for (const auto& [Key, Value] : Parameters)
        {
            Url += "?" + Key + "=" + Value;
        }

        return Url;
    }
} // namespace SpacetimeDB
