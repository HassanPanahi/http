#ifndef BOOSTHTTPCLIENT_H
#define BOOSTHTTPCLIENT_H

#include <string>
#include "server.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>

namespace hp {
namespace http {

class BoostHttpClient : public std::enable_shared_from_this<BoostHttpClient>
{
public:
    BoostHttpClient(const std::string& ip_port, const long int time_out_ms);
    BoostHttpClient(const std::string& ip, const unsigned short port, const long int time_out_ms);
    unsigned int send_request(const Methods method, const std::string& url, const std::string& params, std::string& result);
    const std::string& get_ip() const;
    unsigned short get_port() const;
    const std::string& get_main_url() const;
private:
    std::string ip_;
    unsigned short port_;
    const long int time_out_ms_;
    std::string main_url_;
    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver_;
    boost::beast::tcp_stream stream_;
    boost::beast::flat_buffer buffer_; // (Must persist between reads)
    boost::beast::http::request<boost::beast::http::empty_body> req_;
    boost::beast::http::response<boost::beast::http::string_body> res_;

//    web::http::client::http_client_config cfg_;
//    std::shared_ptr<http_client> client_;
};

} //namespace
} //namespace
#endif // BOOSTHTTPCLIENT_H
