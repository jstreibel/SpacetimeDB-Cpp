#pragma once

#include "Config.hpp"
#include "Utils/HttpClient.hpp"
#include <memory>

#pragma once

#include "Config.hpp"
#include "Utils/HttpClient.hpp"
#include <memory>

namespace SpacetimeDb {

    class Client {
    public:
        // BUILDER NESTED CLASS
        class Builder {
        public:
            static Builder& WithConfig(const ClientConfig& cfg);
            static std::unique_ptr<Client> Build();
        };

        // CORE API METHODS
        void Close() const;
        // EXAMPLE: IDENTITY SERVICE ACCESSOR
        // IdentityClient& Identity();

    private:
        struct Impl;
        std::unique_ptr<Impl> pImpl_;
        friend class Builder;
    };

} // namespace SpacetimeDb