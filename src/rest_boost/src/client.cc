#include "rest_boost/client.h"

namespace hp {
namespace http {

BoostHttpClient::BoostHttpClient(const std::string &ip, const unsigned short port, const long time_out_ms) :
    ip_(ip), port_(port), time_out_ms_(time_out_ms)
{
    methods_list_[Methods::GET]      = boost::beast::http::verb::get;
    methods_list_[Methods::PUT]      = boost::beast::http::verb::put;
    methods_list_[Methods::DEL]      = boost::beast::http::verb::delete_;
    methods_list_[Methods::HEAD]     = boost::beast::http::verb::head;
    methods_list_[Methods::POST]     = boost::beast::http::verb::post;
    methods_list_[Methods::TRCE]     = boost::beast::http::verb::trace;
    methods_list_[Methods::PATCH]    = boost::beast::http::verb::patch;
    methods_list_[Methods::MERGE]    = boost::beast::http::verb::merge;
    methods_list_[Methods::OPTIONS]  = boost::beast::http::verb::options;
    methods_list_[Methods::CONNECT]  = boost::beast::http::verb::connect;
}
namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
unsigned int BoostHttpClient::send_request(const Methods method, const std::string &url, const std::string &params, std::string &result)
{
    net::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // Look up the domain name
    auto const results = resolver.resolve("0.0.0.0", "8585");

    // Make the connection on the IP address we get from a lookup
    stream.connect(results);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, "/info", 1};
    req.set(http::field::host, "0.0.0.0");
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

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
    if(ec && ec != beast::errc::not_connected)
        throw beast::system_error{ec};
    return 200;
    // If we get here then the connection is closed gracefully
}

} //namespace
} //namespace
