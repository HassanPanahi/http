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
    ip_(ip), port_(port), resolver_(boost::asio::make_strand(ioc_)), stream_(boost::asio::make_strand(ioc_))
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

unsigned int BoostHttpClient::send_async_request(const Methods method, const std::string &url, const std::string &params, std::string &result, const long time_out_ms)
{
    auto ret = boost::beast::http::status::bad_request;
    try {
        auto boost_method = methods_list_[method];
        req_.method(boost_method);
        req_.target(url);
        req_.set(boost::beast::http::field::host, "0.0.0.0");
        req_.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        stream_.expires_after(std::chrono::seconds(15));

        resolver_.async_resolve("0.0.0.0", "8585", boost::beast::bind_front_handler(&BoostHttpClient::on_resolve, this));
        ioc_.run();

    } catch(std::exception &e) {
        std::cout << "std::error: " << e.what() << std::endl;
    }
    return static_cast<unsigned int>(ret);
}

void on_timeout(boost::beast::tcp_stream& stream, const boost::system::error_code& error)
{
    stream.close();
}

unsigned int BoostHttpClient::send_request(const Methods method, const std::string &url, const std::string &params, std::string &result, const long time_out_ms)
{
    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::beast::tcp_stream stream(ioc);
    boost::asio::io_context ioc_deadtime;
    boost::asio::deadline_timer timer_(ioc_deadtime);
    auto const results = resolver.resolve("0.0.0.0", "8585");
    uint32_t connect_try_count = time_out_ms / 10;
    timer_.expires_from_now(boost::posix_time::milliseconds(time_out_ms));
    auto func = std::bind(on_timeout, std::ref(stream), std::placeholders::_1);
    timer_.async_wait(func);
    ioc_deadtime.run();
    if (connect_try_count == 0) {
        while(1) {
            try {
                stream.connect(results);
                break;
            }  catch (std::exception &exc) {
                std::cout << "contine: " << exc.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } else {
        while(connect_try_count) {
            try {
                stream.connect(results);
                break;
            }  catch (std::exception &exc) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                connect_try_count--;
                if (connect_try_count == 0)
                    return static_cast<unsigned int>(boost::beast::http::status::bad_request);
            }
        }
    }

    boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, "/info", 11};
    req.set(boost::beast::http::field::host, "0.0.0.0");
    req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);


     boost::beast::http::write(stream, req);



    boost::beast::flat_buffer buffer;

    boost::beast::http::response<boost::beast::http::dynamic_body> res;

    boost::beast::http::read(stream, buffer, res);

    std::cout << "result: " <<  res << std::endl;

    boost::beast::error_code ec;
    stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

    if(ec && ec != boost::beast::errc::not_connected) {
        std::cout << "error: " << ec.message() << std::endl;
        throw boost::beast::system_error{ec};
    }
    return ec.value();
}

void BoostHttpClient::on_read(boost::beast::error_code &ec, const std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);
    if(ec) {
        std::cout << "read problem: " << ec.message() << std::endl;
        return;
    }
    std::cout << res_ << std::endl;
    stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if(ec && ec != boost::beast::errc::not_connected) {

    }
}

void BoostHttpClient::on_write(const boost::beast::error_code& ec, const std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);
    boost::beast::http::async_read(stream_, buffer_, res_, boost::beast::bind_front_handler(&BoostHttpClient::on_read, this));
}

void BoostHttpClient::connect_feedback(const boost::beast::error_code& ec, const boost::asio::ip::tcp::endpoint& ep)
{
    if(ec) {
        std::cout << "connect prblem:  " << ec.message() <<  std::endl;
    } else {
        std::cout << "connect" << std::endl;
        stream_.expires_after(std::chrono::seconds(2));
        boost::beast::http::async_write(stream_, req_, boost::beast::bind_front_handler(&BoostHttpClient::on_write, this));
    }

}

void BoostHttpClient::on_resolve(const boost::beast::error_code &ec, const boost::asio::ip::tcp::resolver::results_type &results)
{
    if(ec) {
        std::cout << "resolve error" << std::endl;
    } else {
        stream_.expires_after(std::chrono::seconds(5));
        stream_.async_connect(results, boost::beast::bind_front_handler(&BoostHttpClient::connect_feedback, this));
    }

}

} //namespace
} //namespace
