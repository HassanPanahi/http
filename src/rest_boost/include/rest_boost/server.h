#ifndef REST_BOOST_SERVER_H
#define REST_BOOST_SERVER_H


#include <map>
#include <string>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>


#include "../../rest_common/messages_interface.h"

namespace hp {
namespace http {


class BoostRestServer : public RestServerInterface
{
public:
    BoostRestServer(const std::string& ip_address, unsigned short port, const uint32_t threads = 16, const std::shared_ptr<PathParser>& path_parser = nullptr);

    bool is_running() const;
    std::string get_url() const;
    unsigned short get_port() const;

    void stop() override;
    void start() override;
    ~BoostRestServer();
private:
    bool is_running_;
    const uint32_t threads_count_;
    std::string main_url_;
    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::socket tcp_socekt_;
    boost::asio::ip::tcp::acceptor ip_acceptor_;

    std::vector<std::thread> ioc_threads_;


    std::vector<std::vector<std::string>> paths_;

    void accept_connection(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::ip::tcp::socket& socket);
};

} // namespace
} // namespace

#endif
