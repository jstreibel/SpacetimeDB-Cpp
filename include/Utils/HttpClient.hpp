#pragma once
#include <string>
#include "Error.hpp"

namespace SpacetimeDb::Utils {

    class HttpClient {
    public:
        HttpClient();
        ~HttpClient();

        // PERFORM GET, POST, etc.
        std::string Get(const std::string& url);
        std::string Post(const std::string& url, const std::string& body, const std::string& contentType);

    private:
        // PIMPL or CURL HANDLE
    };

} // namespace SpacetimeDb::Utils