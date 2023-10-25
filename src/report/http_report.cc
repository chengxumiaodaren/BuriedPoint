#include "report/http_report.h"

#include "boost/asio/connect.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "spdlog/spdlog.h"

namespace beast = boost::beast;  // from <boost/beast.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;     // from <boost/asio.hpp>
using tcp = net::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

namespace buried {

static boost::asio::io_context ioc;

HttpReporter::HttpReporter(std::shared_ptr<spdlog::logger> logger)
    : logger_(logger) {}

bool HttpReporter::Report() {
  try {
    int version = 11;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    boost::asio::ip::tcp::resolver::query query(host_, port_);
    // Look up the domain name
    auto const results = resolver.resolve(query);

    // Make the connection on the IP address we get from a lookup
    stream.connect(results);

    // Set up an HTTP POST request message
    http::request<http::string_body> req{http::verb::post, topic_, version};
    req.set(http::field::host, host_);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.body() = body_;
    req.prepare_payload();

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(stream, buffer, res);

    // Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes
    // so don't bother reporting it.
    //
    if (ec && ec != beast::errc::not_connected) throw beast::system_error{ec};

    auto res_status = res.result();
    if (res_status != http::status::ok) {
      SPDLOG_LOGGER_ERROR(logger_,
                          "report error " + std::to_string(res.result_int()));
      return false;
    }

    std::string res_body = boost::beast::buffers_to_string(res.body().data());
    // Write the message to log
    SPDLOG_LOGGER_TRACE(logger_, "report success" + res_body);
    // If we get here then the connection is closed gracefully
  } catch (std::exception const& e) {
    SPDLOG_LOGGER_ERROR(logger_, "report error " + std::string(e.what()));
    return false;
  }
  return true;
}

}  // namespace buried
