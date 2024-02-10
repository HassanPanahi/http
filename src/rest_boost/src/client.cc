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
//        resolver_.async_resolve(ip_, port_, beast::bind_front_handler(&BoostHttpClient::on_resolve, shared_from_this()));
//        boost::asio::ip::tcp::resolver resolver(ioc);
//        //        ioc.run_one_until(std::chrono::milliseconds(time_out_ms));
//        boost::beast::tcp_stream stream(ioc);
//        stream.expires_after(std::chrono::milliseconds(time_out_ms));

//        auto const results = resolver.resolve(host, port);
        //        stream.connect(results);
        //        stream.async_connect(result, boost::beast::bind_front_handler(&BoostHttpClient::connect_feedback, this, std::placeholders::_1, std::placeholders::_2));
        //        boost::beast::http::request<boost::beast::http::string_body> req(boost_method, url, version_);
        //        req.set(boost::beast::http::field::host, host);
        //        req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        //        req.body() = params;
        //        req.prepare_payload();
        //        boost::beast::http::write(stream, req);
        //        std::cout << 1 << std::endl;
        //        boost::beast::http::read(stream, buffer_, res_);
        //std::cout << 2 << std::endl;
        //        result = boost::beast::buffers_to_string(res_.body().data());
        //        boost::beast::error_code ec;
        //        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        //        if(ec && ec != boost::beast::errc::not_connected)
        //            throw boost::beast::system_error{ec};
        //        ret = res_.result();
    } catch(std::exception &e) {
        std::cout << "std::error: " << e.what() << std::endl;
    }
    return static_cast<unsigned int>(ret);
}

void BoostHttpClient::connect_feedback(const boost::beast::error_code& ec, const boost::asio::ip::tcp::endpoint& ep)
{

}

void BoostHttpClient::on_resolve(boost::beast::error_code ec, boost::asio::ip::basic_resolver::results_type results)
{

}

} //namespace
} //namespace
