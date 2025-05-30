#include "Utils/HttpClient.hpp"

namespace SpacetimeDb::Utils {

    HttpClient::HttpClient() {
        // INIT LIBCURL OR OTHER CLIENT
    }

    HttpClient::~HttpClient() {
        // CLEANUP
    }

    std::string HttpClient::Get(const std::string& url) {
        // WRAP LIBCURL GET
        return {};
    }

    std::string HttpClient::Post(const std::string& url, const std::string& body, const std::string& contentType) {
        // WRAP LIBCURL POST
        return {};
    }

} // namespace SpacetimeDb::Utils