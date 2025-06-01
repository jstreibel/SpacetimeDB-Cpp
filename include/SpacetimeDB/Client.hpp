#pragma once

#include "Config.hpp"
#include "Utils/HttpClient.hpp"
#include <memory>

namespace SpacetimeDB {
    /**
    * The facade Client we use in our application. It aggregates the various
    * service clients (identity, database, subscriptions, etc.), manages our
    * current auth token/connection ID, and exposes high-level methods like:
    * ` client.Login(...);
    *   client.ExecuteSql("SELECT ...");
    *   client.Subscribe<MyTable>(...);
    * `
    * You only ever instantiate Client; internally it dispatches work to
    * the right sub-client (e.g. it calls IdentityClient to log in, then
    * passes the returned token into DatabaseClient to run queries).
    */
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