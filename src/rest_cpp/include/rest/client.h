#ifndef CLIENT_H
#define CLIENT_H

#include <cpprest/http_client.h>

using namespace web;
using namespace http;
using namespace utility;
using namespace web::http::client;


namespace hp {
namespace http {

const uint32_t time_out_ms_default_ = (30 * 1000);
class Client
{
public:
    Client(const std::string& ip_port, const long int time_out_ms);
    Client(const std::string& ip, const unsigned short port, const long int time_out_ms);
    std::string put_request(const std::string& url, const std::string& params);
    std::string get_request(const std::string& url);
    std::string post_request(const std::string& url,  const std::string& params);
    const std::string& get_ip() const;
    unsigned short get_port() const;
    const std::string& get_main_url() const;
private:
    std::string ip_;
    unsigned short port_;
    const long int time_out_ms_;
    std::string main_url_;
    web::http::client::http_client_config cfg_;
    std::shared_ptr<http_client> client_;
};

} // rest
}
#endif // CLIENT_H
