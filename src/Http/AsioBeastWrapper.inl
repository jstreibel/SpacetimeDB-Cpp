#pragma once

#ifndef ASIO_STANDALONE
// #error "Please define ASIO_STANDALONE on build level"
#define ASIO_STANDALONE
#endif

// #include <asio.hpp>                       // Standalone Asio
#include <boost/beast.hpp>

#include "Types.h"

namespace SpacetimeDB::AsioBeast
{
    using boost::asio::ip::tcp;
    namespace beast = boost::beast;
    namespace http  = beast::http;

    // A convenience alias for header maps:
    using HeaderMap = Map<String, String>;

    // The Response struct returned by both Get() and Post():
    struct Response {
        bool        ok;          // false if any error occurred
        int         status;      // HTTP status code (e.g. 200), valid only if ok==true
        String body;        // response body
        String error_msg;   // if ok==false, this contains the Beast/Asio error
    };

    // A small HTTP client that does synchronous GET/POST over plain TCP
    // (no SSL). You drop this header into your project and include it.
    class SimpleHttpClient {
    public:
        // timeout_seconds: how long to wait on connect/read/write, in whole seconds
        explicit SimpleHttpClient(int timeout_seconds = 30)
            : timeout_secs_(timeout_seconds) {}

        // Synchronous HTTP GET:
        Response Get(const String& url, HeaderMap const& headers = {}) {
            return perform_request(http::verb::get, url, headers, /*body=*/"");
        }

        // Synchronous HTTP POST:
        Response Post(const String& url,
                      HeaderMap const& headers,
                      const String& body) {
            return perform_request(http::verb::post, url, headers, body);
        }

    private:
        int timeout_secs_;

        // Splits "http://host:port/target?query" (or "https://...") into
        // (scheme, host, port, target). On error, returns false.
        static bool parse_url(String const& url,
                              String& out_scheme,
                              String& out_host,
                              String& out_port,
                              String& out_target,
                              String& out_error)
        {
            // Only support http or https:
            constexpr char HTTP_PREFIX[]  = "http://";
            constexpr char HTTPS_PREFIX[] = "https://";

            size_t scheme_len = String::npos;
            if (url.rfind(HTTP_PREFIX, 0) == 0) {
                scheme_len = sizeof(HTTP_PREFIX) - 1;  // skip "http://"
                out_scheme = "http";
                out_port   = "80";
            }
            else if (url.rfind(HTTPS_PREFIX, 0) == 0) {
                scheme_len = sizeof(HTTPS_PREFIX) - 1; // skip "https://"
                out_scheme = "https";
                out_port   = "443";
            }
            else {
                out_error = "Unsupported URL scheme (need http:// or https://)";
                return false;
            }

            // Find first '/' after scheme --> end of authority:
            size_t authority_end = url.find('/', scheme_len);
            String authority;
            if (authority_end == String::npos) {
                authority   = url.substr(scheme_len);
                out_target  = "/";
            } else {
                authority   = url.substr(scheme_len, authority_end - scheme_len);
                out_target  = url.substr(authority_end);
            }

            // Split host[:port] if needed:
            auto colon_pos = authority.find(':');
            if (colon_pos != String::npos) {
                out_host = authority.substr(0, colon_pos);
                out_port = authority.substr(colon_pos + 1);
                if (out_port.empty()) {
                    out_error = "URL has empty port: " + url;
                    return false;
                }
            }
            else {
                out_host = authority;
                // out_port remains defaulted (80 or 443)
            }
            return true;
        }

        // The core routine that performs a blocking HTTP request:
        Response perform_request(http::verb method,
                                 String const& url,
                                 HeaderMap const& headers,
                                 String const& body)
        {
            Response result;
            result.ok = false;

            // 1) Parse URL
            String scheme, host, port, target, parse_err;
            if (!parse_url(url, scheme, host, port, target, parse_err)) {
                result.error_msg = "URL parse error: " + parse_err;
                return result;
            }
            bool use_ssl = (scheme == "https");

            // We only implement plain‐HTTP here.
            if (use_ssl) {
                result.error_msg = "HTTPS not supported by this simple client";
                return result;
            }

            try {
                // 2) Set up Asio I/O context, resolver, and Beast socket
                boost::asio::io_context ioc;
                tcp::resolver    resolver{ioc};
                beast::tcp_stream stream{ioc};

                // 3) Resolve host:port
                beast::error_code ec;
                auto const endpoints = resolver.resolve(host, port, ec);
                if (ec) {
                    result.error_msg = "Resolve error: " + ec.message();
                    return result;
                }

                // 4) Connect with a timeout
                stream.expires_after(std::chrono::seconds(timeout_secs_));
                stream.connect(endpoints, ec);
                if (ec) {
                    result.error_msg = "Connect error: " + ec.message();
                    return result;
                }

                // 5) Build the HTTP request
                http::request<http::string_body> req{method, target, 11};
                req.set(http::field::host, host);
                req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

                // Copy user‐provided headers:
                for (auto const& kv : headers) {
                    req.set(kv.first, kv.second);
                }

                if (method == http::verb::post) {
                    req.body() = body;
                    req.prepare_payload();  // sets Content-Length
                }

                // 6) Send the request
                stream.expires_after(std::chrono::seconds(timeout_secs_));
                http::write(stream, req, ec);
                if (ec) {
                    result.error_msg = "Write error: " + ec.message();
                    return result;
                }

                // 7) Read the response
                beast::flat_buffer                buffer;
                http::response<http::string_body> res;
                stream.expires_after(std::chrono::seconds(timeout_secs_));
                http::read(stream, buffer, res, ec);
                if (ec) {
                    result.error_msg = "Read error: " + ec.message();
                    return result;
                }

                // 8) Gracefully shut down the socket
                stream.socket().shutdown(tcp::socket::shutdown_both, ec);
                // Ignore ec from shutdown if “not_connected,” etc.

                // 9) Fill out our Response and return
                result.ok     = true;
                result.status = static_cast<int>(res.result_int());
                result.body   = res.body();
                return result;
            }
            catch (std::exception const& ex) {
                result.error_msg = "Exception: " + String(ex.what());
                return result;
            }
        }
    };

}