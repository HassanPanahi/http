#ifndef BOOSTHTTPCLIENT_H
#define BOOSTHTTPCLIENT_H

#include <string>
#include "server.h"

namespace hp {
namespace http {

class BoostHttpClient
{
public:
    BoostHttpClient(const std::string& ip_port, const long int time_out_ms);
    BoostHttpClient(const std::string& ip, const unsigned short port, const long int time_out_ms);
    unsigned int send_request(const RestMethods method, const std::string& url, const std::string& params, std::string& result);
    const std::string& get_ip() const;
    unsigned short get_port() const;
    const std::string& get_main_url() const;
private:
    std::string ip_;
    unsigned short port_;
    const long int time_out_ms_;
    std::string main_url_;
//    web::http::client::http_client_config cfg_;
//    std::shared_ptr<http_client> client_;
};

} //namespace
} //namespace
#endif // BOOSTHTTPCLIENT_H
