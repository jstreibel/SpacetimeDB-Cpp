#include "Client.hpp"
#include "Config.hpp"
#include "Utils/HttpClient.hpp"

#include "Client.hpp"
#include "Config.hpp"
#include "Utils/HttpClient.hpp"

namespace SpacetimeDb {

    struct Client::Impl {
        ClientConfig config;
        Utils::HttpClient http;
        bool isOpen = true;
    };

    Client::Builder& Client::Builder::WithConfig(const ClientConfig& cfg) {
        // STORE CONFIG
        // return *this;
    }

    std::unique_ptr<Client> Client::Builder::Build() {
        auto client = std::make_unique<Client>();
        client->pImpl_ = std::make_unique<Impl>();
        // INITIALIZE HttpClient FROM cfg
        return client;
    }

    void Client::Close() {
        if (pImpl_ && pImpl_->isOpen) {
            // TEARDOWN
            pImpl_->isOpen = false;
        }
    }

} // namespace SpacetimeDb