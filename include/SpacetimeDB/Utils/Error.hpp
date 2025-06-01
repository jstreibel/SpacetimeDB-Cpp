#pragma once
#include <stdexcept>
#include <string>

namespace SpacetimeDB::Utils {

    class HttpError final : public std::runtime_error {
    public:
        HttpError(const int Code, const std::string& msg)
            : std::runtime_error(msg), Code(Code) {}
        int Code;
    };

} // namespace SpacetimeDb::Utils