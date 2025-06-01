#pragma once
#include <functional>
#include <string>
#include "SpacetimeDB/Utils/Json.hpp"

namespace SpacetimeDB {
    ///
    /// A thin WebSocket wrapper. Internally can use WebSocket++ or Boost.Beast.
    /// Allows sending JSON messages and registering a receive callback.
    ///
    class WebSocketClient {
    public:
        WebSocketClient();
        ~WebSocketClient();

        /// Connect to the given ws:// or wss:// URL.
        /// Throws or signals an error if the handshake fails.
        static void Connect(const std::string& wsUrl);

        /// Send a JSON object (serialized) over the open WebSocket.
        void Send(const Utils::Json& message);

        /// Register a callback for every incoming JSON frame.
        /// Called on the thread that receives messages.
        void OnReceive(std::function<void(const Utils::Json&)> callback);

        /// Gracefully close the WebSocket.
        void Close();

    private:
     // Internal handle (e.g. std::unique_ptr<websocketpp::client<...>> or Boost::beast::websocket::stream<>)
     // bool isConnected_;
     // std::function<void(const Utils::Json&)> recvCallback_;
 };

} // namespace SpacetimeDB