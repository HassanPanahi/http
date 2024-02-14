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
#include <thread>


namespace hp {
namespace http {

BoostHttpClient::BoostHttpClient(const std::string &ip, const unsigned short port) :
    ip_(ip), port_(port), stream_sync_(ioc_sync_), resolver_sync_(ioc_sync_)
{
    http_version_ = 11;
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

void on_timeout(boost::beast::tcp_stream& stream, bool& expired,  const boost::system::error_code& error)
{
    try {
        expired = true;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    } catch (std::exception &ect) {
        std::cout << "shutdown: " << ect.what() << std::endl;
    }
}

unsigned int BoostHttpClient::send_request(const Methods method, const std::string &url, const std::string &request, std::string &response, const long time_out_ms)
{
    boost::asio::deadline_timer timer_(ioc_deadtime_sync_);
    auto const results = resolver_sync_.resolve(ip_, std::to_string(port_));
    bool expired_timeout = false;
    std::thread ioc_thread;
    if (time_out_ms != 0) {
        timer_.expires_from_now(boost::posix_time::milliseconds(time_out_ms));
        ioc_thread = std::thread( [&]() {ioc_deadtime_sync_.run();} );
    }
    boost::beast::error_code ec;
    auto ret = boost::beast::http::status::request_timeout;
    try {
        timer_.async_wait(std::bind(on_timeout, std::ref(stream_sync_), std::ref(expired_timeout), std::placeholders::_1));

        while (1) {
            try {
                stream_sync_.connect(results);
                break;
            }  catch (boost::system::system_error &exc) {
                if (expired_timeout) {
                    ioc_thread.join();
                    return static_cast<unsigned int>(boost::beast::http::status::request_timeout);
                }
            }
        }
        auto boost_method = methods_list_[method];
        boost::beast::http::request<boost::beast::http::string_body> req{boost_method, url, http_version_};
        req.body() = request;
        req.prepare_payload();
        boost::beast::http::write(stream_sync_, req, ec);
        if (ec) {
            ret = boost::beast::http::status::request_timeout;
        } else {
            boost::beast::flat_buffer buffer;
            boost::beast::http::response<boost::beast::http::dynamic_body> res;

            boost::beast::http::read(stream_sync_, buffer, res, ec);
            if (ec) {
                ret = boost::beast::http::status::request_timeout;
            } else {

                ioc_deadtime_sync_.stop();
                response = boost::beast::buffers_to_string(res.body().data());
                stream_sync_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
                ret = boost::beast::http::status::ok;
            }
        }
    } catch (std::exception &ece) {
        std::cout << "exception: " << ece.what() << " boost: " << ec.message() << std::endl;
    }
    if (ioc_thread.joinable())
        ioc_thread.join();

    return static_cast<unsigned int>(ret);
}

} //namespace
} //namespace
