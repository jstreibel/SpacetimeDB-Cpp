#pragma once
#include <string>
#include <chrono>

namespace SpacetimeDb {

    struct ClientConfig {
        std::string BaseUrl;
        std::chrono::milliseconds Timeout = std::chrono::seconds(30);
        std::string AuthToken;
    };

} // namespace SpacetimeDb