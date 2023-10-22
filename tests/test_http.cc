#include <cstdlib>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "src/third_party/boost/asio/connect.hpp"
#include "src/third_party/boost/asio/ip/tcp.hpp"
#include "src/third_party/boost/beast/core.hpp"
#include "src/third_party/boost/beast/http.hpp"
#include "src/third_party/boost/beast/version.hpp"

namespace beast = boost::beast;  // from <boost/beast.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;     // from <boost/asio.hpp>
using tcp = net::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

// Performs an HTTP POST and prints the response
TEST(BuriedHttpTest, HttpPost) {
  try {
    auto const host = "localhost";
    auto const target = "/buried";
    int version = 11;

    // The io_context is required for all I/O
    net::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    boost::asio::ip::tcp::resolver::query query(host, "5678");
    // Look up the domain name
    auto const results = resolver.resolve(query);

    // Make the connection on the IP address we get from a lookup
    stream.connect(results);

    // Set up an HTTP POST request message
    http::request<http::string_body> req{http::verb::post, target, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.body() = "{\"hello\":\"world\"}";
    req.prepare_payload();

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(stream, buffer, res);

    // Write the message to standard out
    std::cout << res << std::endl;

    // Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes
    // so don't bother reporting it.
    //
    if (ec && ec != beast::errc::not_connected) throw beast::system_error{ec};

    // If we get here then the connection is closed gracefully
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
