#include "rest_boost/client.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/beast/core/detail/bind_handler.hpp>
#include <binders.h>

namespace hp {
namespace http {

BoostHttpClient::BoostHttpClient(const std::string &ip, const unsigned short port) :
    ip_(ip), port_(port), resolver_{ioc}, stream_{ioc}
{
    version_ = 11;
    req_.version(version_);
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

unsigned int BoostHttpClient::send_request(const Methods method, const std::string &url, const std::string &params, std::string &result, const long time_out_ms)
{
    auto ret = boost::beast::http::status::bad_request;
    try {
        auto boost_method = methods_list_[method];
        req_.method(boost_method);
        req_.target(url);
        req_.set(boost::beast::http::field::host, ip_);
        req_.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        auto function_ptr = std::bind(&BoostHttpClient::on_resolve, this, result, time_out_ms, std::placeholders::_1, std::placeholders::_2);
        resolver_.async_resolve(ip_.c_str(), std::to_string(port_).c_str(), function_ptr);
    } catch(std::exception &e) {
        std::cout << "std::error: " << e.what() << std::endl;
    }
    return static_cast<unsigned int>(ret);
}

void BoostHttpClient::on_read(std::string& result, boost::beast::error_code &ec, const std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

//    if(ec)
//        return fail(ec, "read");

    // Write the message to standard out
    std::cout << res_ << std::endl;
//    result = boost::beast::buffers_to_string(res_.body().data());
    // Gracefully close the socket
    stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes so don't bother reporting it.
    if(ec && ec != boost::beast::errc::not_connected) {
//        return fail(ec, "shutdown");

    }
}

void BoostHttpClient::on_write(std::string& result, const boost::beast::error_code& ec, const std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

//    if(ec)
//        return fail(ec, "write");

    // Receive the HTTP response
    auto funct = std::bind(&BoostHttpClient::on_read, shared_from_this(), result, std::placeholders::_1, std::placeholders::_2);
    boost::beast::http::async_read(stream_, buffer_, res_, funct);
}

void BoostHttpClient::connect_feedback(std::string& result, const boost::beast::error_code& ec, const boost::asio::ip::tcp::endpoint& ep)
{
    if(ec) {
//        return fail(ec, "connect");

    } else {

//        stream_.expires_after(std::chrono::seconds(30));

       boost::beast::http::async_write(stream_, req_, std::bind(&BoostHttpClient::on_write, shared_from_this(), result, std::placeholders::_1, std::placeholders::_2));
    }

    // Set a timeout on the operation
}

void BoostHttpClient::on_resolve(std::string &result, const long time_out_ms, const boost::beast::error_code &ec, const boost::asio::ip::tcp::resolver::results_type &results)
{
    if(ec) {

    } else {
        stream_.expires_after(std::chrono::seconds(time_out_ms));
        stream_.async_connect(results, std::bind(&BoostHttpClient::connect_feedback, shared_from_this(), result, std::placeholders::_1, std::placeholders::_2));
    }

    // Make the connection on the IP address we get from a lookup
}

} //namespace
} //namespace
