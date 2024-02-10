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
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace hp {
namespace http {

class BoostHttpClient : public std::enable_shared_from_this<BoostHttpClient>
{
public:
    BoostHttpClient(const std::string& ip, const unsigned short port);
    unsigned int send_request(const Methods method, const std::string& url, const std::string& params, std::string& result, const long int time_out_ms = 0);

private:
    boost::asio::io_context ioc;

     tcp::resolver resolver_;
     beast::tcp_stream stream_;
     beast::flat_buffer buffer_; // (Must persist between reads)
     boost::beast::http::request<boost::beast::http::empty_body> req_;
//     http::response<http::string_body> res_;


    void on_resolve(boost::beast::error_code ec, boost::asio::ip::basic_resolver::results_type results);
    void connect_feedback(const boost::beast::error_code& ec, const boost::asio::ip::tcp::endpoint& ep);
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
