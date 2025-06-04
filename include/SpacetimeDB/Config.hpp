#pragma once

#include <Types.h>
#include <chrono>

namespace SpacetimeDB {

    struct ClientConfig {
        String BaseUrl;
        std::chrono::milliseconds Timeout = std::chrono::seconds(30);
        String AuthToken;
    };

} // namespace SpacetimeDb