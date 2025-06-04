//
// Created by joao on 6/3/25.
//

#ifndef TYPES_H
#define TYPES_H

#include <stdexcept>
#include <string>
#include <vector>
#include <optional>
#include <map>
#include <variant>

namespace SpacetimeDB
{
    using String = std::string;
    using HexString = std::string;
    using SpacetimeToken = std::string;

    template <typename T>
    String ToString(T Val) { return std::to_string(Val); }

    using Int = int32_t;
    using Milliseconds = Int;
    using StringMap = std::map<String, String>;

    template <typename T>
    using Array = std::vector<T>;

    template <typename T>
    using Optional = std::optional<T>;

}

#endif //TYPES_H
