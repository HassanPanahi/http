#include "rest_boost/client.h"
#include <boost/lexical_cast.hpp>

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

unsigned int BoostHttpClient::send_request(const Methods method, const std::string &url, const std::string &params, std::string &result)
{
    auto ret = boost::beast::http::status::bad_request;
    try {

    auto const host = ip_;
    auto const port = std::to_string(port_);
    version_ = 11;

    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::beast::tcp_stream stream(ioc);

    auto const results = resolver.resolve(host, port);
    stream.connect(results);

    boost::beast::http::request<boost::beast::http::string_body> req(boost::beast::http::verb::get, url, version_);
    req.set(boost::beast::http::field::host, host);
    req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.body() = params;
    req.prepare_payload();
    boost::beast::http::write(stream, req);

    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::dynamic_body> res;
    boost::beast::http::read(stream, buffer, res);

    result = boost::beast::buffers_to_string(res.body().data());
    boost::beast::error_code ec;
    stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if(ec && ec != boost::beast::errc::not_connected)
        throw boost::beast::system_error{ec};
    ret = res.result();
    } catch(std::exception &e) {
        std::cout << "std::error: " << e.what() << std::endl;

    }
    return static_cast<unsigned int>(ret);
}

} //namespace
} //namespace
