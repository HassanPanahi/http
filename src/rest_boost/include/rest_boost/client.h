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
    explicit BoostHttpClient(const std::string& ip, const unsigned short port);
    unsigned int send_request(const Methods method, const std::string& url, const std::string& request, std::string& response, const long int time_out_ms = 0);

private:
    int http_version_;
    const std::string ip_;
    const unsigned short port_;
    boost::asio::io_context ioc_sync_;
    boost::beast::tcp_stream stream_sync_;
    boost::asio::io_context ioc_deadtime_sync_;
    boost::asio::ip::tcp::resolver resolver_sync_;
    std::map<Methods, boost::beast::http::verb> methods_list_;
};

} //namespace
} //namespace
#endif // BOOSTHTTPCLIENT_H
