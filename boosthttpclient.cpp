#include "boosthttpclient.h"

namespace hp {
namespace http {

BoostHttpClient::BoostHttpClient(const std::string &ip_port, const long time_out_ms)
{

}

BoostHttpClient::BoostHttpClient(const std::string &ip, const unsigned short port, const long time_out_ms) :
      BoostHttpClient(ip+ ":"+ std::to_string(port), time_out_ms)
{
    ip_ = ip;
    port_ = port;
}

unsigned int BoostHttpClient::send_request(const RestMethods method, const std::string &url, const std::string &params, std::string &result)
{

}

} //namespace
} //namespace
