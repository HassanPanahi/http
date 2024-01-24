#ifndef BOOSTHTTPCLIENT_H
#define BOOSTHTTPCLIENT_H

#include <string>
#include "server.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include "../../rest_common/messages_interface.h"

namespace hp {
namespace http {

class BoostHttpClient : public std::enable_shared_from_this<BoostHttpClient>
{
public:
    BoostHttpClient(const std::string& ip, const unsigned short port, const long int time_out_ms);
    unsigned int send_request(const Methods method, const std::string& url, const std::string& params, std::string& result);
private:
    const long int time_out_ms_;
    int version_;

    const std::string ip_;
    const unsigned short port_;

    std::map<Methods, boost::beast::http::verb> methods_list_;

    boost::beast::flat_buffer buffer_; // (Must persist between reads)
    boost::beast::http::request<boost::beast::http::empty_body> req_;
    boost::beast::http::response<boost::beast::http::string_body> res_;

};

} //namespace
} //namespace
#endif // BOOSTHTTPCLIENT_H
