#pragma once
#include <string>
#include <chrono>

namespace SpacetimeDB {

    struct ClientConfig {
        std::string BaseUrl;
        std::chrono::milliseconds Timeout = std::chrono::seconds(30);
        std::string AuthToken;
    };

} // namespace SpacetimeDb