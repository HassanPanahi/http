#ifndef BOOSTHTTPCLIENT_H
#define BOOSTHTTPCLIENT_H

#include <string>
#include "server.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include "../../rest_common/messages_interface.h"
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace hp {
namespace http {

class BoostHttpClient : public std::enable_shared_from_this<BoostHttpClient>
{
public:
    BoostHttpClient(const std::string& ip, const unsigned short port);
    unsigned int send_request(const Methods method, const std::string& url, const std::string& params, std::string& result, const long int time_out_ms = 0);

private:
    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver_;
    boost::beast::tcp_stream stream_;
    boost::beast::flat_buffer buffer_; // (Must persist between reads)
    boost::beast::http::request<boost::beast::http::empty_body> req_;
        boost::beast::http::response<boost::beast::http::string_body> res_;

    void on_read(std::string& result, boost::beast::error_code &ec, const std::size_t bytes_transferred);
    void on_write(std::string& result, const boost::beast::error_code& ec, const std::size_t bytes_transferred);
    void on_resolve(std::string &result, const long time_out_ms, const boost::beast::error_code &ec, const boost::asio::ip::tcp::resolver::results_type &results);
    void connect_feedback(std::string& result, const boost::beast::error_code& ec, const boost::asio::ip::tcp::endpoint& ep);
    int version_;

    const std::string ip_;
    const unsigned short port_;

    std::map<Methods, boost::beast::http::verb> methods_list_;

//    boost::beast::flat_buffer buffer_; // (Must persist between reads)
//    boost::beast::http::response<boost::beast::http::dynamic_body> res_;
//    boost::beast::http::response<boost::beast::http::string_body> req_;

};

} //namespace
} //namespace
#endif // BOOSTHTTPCLIENT_H
