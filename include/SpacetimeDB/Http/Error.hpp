#pragma once

#include "../Types.h"

namespace SpacetimeDB {

    using ErrorType = std::runtime_error;

    template<typename RETURN_TYPE>
    using Result = std::variant<RETURN_TYPE, ErrorType>;

    #define ReturnError(Message) \
        return SpacetimeDB::ErrorType("Location " + String(__FILE__) + ":" + std::to_string(__LINE__) + "\n\tMessage: \"" + (Message) + "\"")

    #define OnError(CallResult, Message) \
        if (!SpacetimeDB::IsValid(CallResult)) ReturnError(Message);


    template<typename RETURN_TYPE>
    String GetErrorMessage(Result<RETURN_TYPE> SomeReturnedValue)
    {
        return std::get<std::runtime_error>(SomeReturnedValue).what();
    }

    template<typename RETURN_TYPE>
    RETURN_TYPE GetResult(Result<RETURN_TYPE> SomeReturnedValue)
    {
        return std::get<RETURN_TYPE>(SomeReturnedValue);
    }

    template<typename RETURN_TYPE>
    bool IsValid(Result<RETURN_TYPE> SomeReturnedValue)
    {
        return std::holds_alternative<RETURN_TYPE>(SomeReturnedValue);
    }

    class HttpError final : public std::runtime_error {
    public:
        HttpError(const int Code, const String& msg)
            : std::runtime_error(msg), Code(Code) {}
        int Code;
    };

} // namespace SpacetimeDb