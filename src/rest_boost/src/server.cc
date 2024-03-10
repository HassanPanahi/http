#include "rest_boost/server.h"

#include <iostream>
#include <thread>

#include "rest_boost/http_connection.h"

namespace hp {
namespace http {

BoostRestServer::BoostRestServer(const std::string& ip_address, unsigned short port, const uint32_t threads, const std::shared_ptr<PathParser>& path_parser) :
    ioc_(threads), ip_acceptor_({ioc_, {boost::asio::ip::make_address(ip_address), port}}), tcp_socekt_{ioc_}, threads_count_(threads)
{
    if (path_parser == nullptr)
        path_parser_ = std::make_shared<PathParser>();
    else
        path_parser_ = path_parser;

    is_running_ = false;
}

BoostRestServer::~BoostRestServer()
{
    BoostRestServer::stop();
    for(auto i = 0; i < threads_count_; i++)
        if (ioc_threads_[i].joinable())
            ioc_threads_[i].join();
}

void BoostRestServer::accept_connection(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::ip::tcp::socket &socket)
{
    ip_acceptor_.async_accept(socket, [&](boost::beast::error_code ec) {
        if(!ec) {
            auto func = std::bind(&BoostRestServer::analyze_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
            auto http_connection = std::make_shared<BoostHTTPConnection>(std::move(socket), func);
            http_connection->start();
        }
        accept_connection(acceptor, socket);
    });
}

void BoostRestServer::start()
{
    accept_connection(ip_acceptor_, tcp_socekt_);
    ioc_threads_.reserve(threads_count_);
    for(auto i = 0; i < threads_count_; i++)
        ioc_threads_.emplace_back([this]{ ioc_.run(); } );
    std::cout << "Http server listen to " << get_port() << " port" << std::endl;
    is_running_ = true;
}

void BoostRestServer::stop()
{
    ioc_.stop();
}

bool BoostRestServer::is_running() const
{
    return is_running_;
}

std::string BoostRestServer::get_url() const
{
    return ip_acceptor_.local_endpoint().address().to_string();
}

unsigned short BoostRestServer::get_port() const
{
    return ip_acceptor_.local_endpoint().port();
}

} // namespace
} // namespace
