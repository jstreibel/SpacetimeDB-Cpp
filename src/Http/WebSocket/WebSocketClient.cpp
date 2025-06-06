#include "Http/WebSocket/WebSocketClient.hpp"
// #include <websocketpp/config/asio_no_tls_client.hpp>
// #include <websocketpp/client.hpp>
#include <thread>

namespace SpacetimeDB {
    // using wspp_client = websocketpp::client<websocketpp::config::asio_client>;

    class WebSocketContext {
    public:
        // wspp_client    client;
        // websocketpp::connection_hdl connection;
        std::thread    runThread;
        std::function<void(const Json&)> recvCallback;
        bool  connected{false};
    };

    WebSocketClient::WebSocketClient() {
        // nothing yet
    }

    WebSocketClient::~WebSocketClient() {
        Close();
    }

    void WebSocketClient::Connect(const std::string& wsUrl) {
        //// Initialize a new context for each connection
        //auto ctx = std::make_shared<WebSocketContext>();
        //ctx->client.init_asio();
//
        //// Set logging off (optional)
        //ctx->client.clear_access_channels(websocketpp::log::alevel::all);
//
        //// Set open handler
        //ctx->client.set_open_handler([ctx](websocketpp::connection_hdl hdl) {
        //    ctx->connected = true;
        //});
//
        //// Set message handler
        //ctx->client.set_message_handler(
        //    [ctx](websocketpp::connection_hdl hdl, wspp_client::message_ptr msg)
        //    {
        //        try {
        //            Json j = Json::parse(msg->get_payload());
        //            if (ctx->recvCallback) {
        //                ctx->recvCallback(j);
        //            }
        //        } catch (...) {
        //          // TODO: swallow parse errors or optionally log
        //        }
        //    }
        //);
//
        //// Create and connect
        //websocketpp::lib::error_code ec;
        //const auto con = ctx->client.get_connection(wsUrl, ec);
        //if (ec) {
        //    throw std::runtime_error("WebSocket connection error: " + ec.message());
        //}
        //ctx->connection = con->get_handle();
        //ctx->client.connect(con);
//
        //// Run in a separate thread
        //ctx->runThread = std::thread([ctx]() {
        //    ctx->client.run();
        //});
//
        //// Store context in a member variable
        //// (for simplicity, assume we have a single connection per WsClient)
        //// We might store this in a unique_ptr<WsContext> member.
        //// e.g.: this->context_ = ctx;
    }

    void WebSocketClient::Send(const Json& message) {
        throw std::runtime_error("Not implemented");
        // if not connected, throw
        // auto ctx = context_.lock();
        // if (!ctx || !ctx->connected) throw ...
        // ctx->client.send(ctx->connection, message.dump(), websocketpp::frame::opcode::text);
    }

    void WebSocketClient::OnReceive(std::function<void(const Json&)> callback) {
        throw std::runtime_error("Not implemented");
        // auto ctx = context_.lock();
        // ctx->recvCallback = std::move(callback);
    }

    void WebSocketClient::Close() {
        throw std::runtime_error("Not implemented");
        // auto ctx = context_.lock();
        // if (ctx && ctx->connected) {
        //   ctx->client.close(ctx->connection, websocketpp::close::status::going_away, "");
        //   if (ctx->runThread.joinable()) ctx->runThread.join();
        //   ctx->connected = false;
        // }
    }

} // namespace SpacetimeDB
