#pragma once
#include <stdexcept>
#include <string>

namespace SpacetimeDb::Utils {

    class HttpError : public std::runtime_error {
    public:
        HttpError(int code, const std::string& msg)
            : std::runtime_error(msg), Code(code) {}
        int Code;
    };

} // namespace SpacetimeDb::Utils