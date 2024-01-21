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
//    req_.version(version);
    req_.method(boost::beast::http::verb::get);
    req_.target(url);
    req_.set(boost::beast::http::field::host, ip_);
    req_.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Look up the domain name
    resolver_.async_resolve(ip_, port_, boost::beast::bind_front_handler(&BoostHttpClient::on_resolve, shared_from_this()));
}

} //namespace
} //namespace
