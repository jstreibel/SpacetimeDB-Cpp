#pragma once

#include "../Json.hpp"
#include "../../Types.h"

#include <functional>

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
        void Connect(const String& wsUrl);

        /// Send a JSON object (serialized) over the open WebSocket.
        void Send(const Json& message);

        /// Register a callback for every incoming JSON frame.
        /// Called on the thread that receives messages.
        void OnReceive(std::function<void(const Json&)> callback);

        /// Gracefully close the WebSocket.
        void Close();

    private:
     // Internal handle (e.g. std::unique_ptr<websocketpp::client<...>> or Boost::beast::websocket::stream<>)
     // bool isConnected_;
     // std::function<void(const Json&)> recvCallback_;
 };

} // namespace SpacetimeDB