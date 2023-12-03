#include "client.h"


namespace rest {

Client::Client(const std::string &ip, const unsigned short port, const long time_out_ms)
    : Client(ip+ ":"+ std::to_string(port), time_out_ms)
{
    ip_ = ip;
    port_ = port;
}

Client::Client(const std::string &ip_port, const long int time_out_ms)
    : ip_(""), port_(0), time_out_ms_(time_out_ms)
{
    main_url_ = "http://"+ ip_port;
    cfg_.set_timeout(std::chrono::milliseconds(time_out_ms_));
    client_ = std::make_shared<http_client>(main_url_, cfg_);
}

//TODO(HP): Should I return by reference for less memory and performance(RAII)
std::string Client::put_request(const std::string &url, const std::string &params)
{
    std::string result = "";
    try {
        auto response = client_->request(methods::PUT, url, params).get();
        if (response.status_code() == status_codes::OK) {
            result = response.extract_string(true).get();
        } else {
            result = response.extract_string(true).get();
        }
    } catch (std::exception &e) {
        result = e.what();
    }
    return result;
}

//TODO(HP): Should I return by reference for less memory and performance(RAII)
std::string Client::get_request(const std::string &ip)
{
    std::string result = "";
    try {
        auto response = client_->request(methods::GET, ip).get();
        if (response.status_code() == status_codes::OK) {
            result = response.extract_string(true).get();
        } else {
            result = response.extract_string(true).get();
        }
    } catch (std::exception &e) {
        result = e.what();
    }
    return result;
}

std::string Client::post_request(const std::string &url, const std::string &params)
{
    std::string result = "";
    try {
        auto response = client_->request(methods::POST, url, params).get();
        if (response.status_code() == status_codes::OK) {
            result = response.extract_string(true).get();

        } else {
            result = response.extract_string(true).get();
        }
    } catch (std::exception &e) {
        result = e.what();
    }
    return result;
}

const std::string &Client::get_ip() const
{
    return ip_;
}

unsigned short Client::get_port() const
{
    return port_;
}

const std::string &Client::get_main_url() const
{
    return main_url_;
}

} // namespace rest
