#include "../include/SpacetimeDB/Client.hpp"
#include "Config.hpp"
#include "../include/SpacetimeDB/Http/HttpClient.hpp"

namespace SpacetimeDB {

    struct Client::Impl {
        ClientConfig config;
        HttpClient http;
        bool isOpen = true;
    };

    Client::Builder& Client::Builder::WithConfig(const ClientConfig& cfg) {
        // STORE CONFIG
        // return *this;

        throw std::runtime_error("Not implemented");
    }

    std::unique_ptr<Client> Client::Builder::Build() {
        auto client = std::make_unique<Client>();
        client->pImpl_ = std::make_unique<Impl>();
        // INITIALIZE HttpClient FROM cfg
        return client;
    }

    void Client::Close() const
    {
        if (pImpl_ && pImpl_->isOpen) {
            // TEARDOWN
            pImpl_->isOpen = false;
        }
    }

} // namespace SpacetimeDb