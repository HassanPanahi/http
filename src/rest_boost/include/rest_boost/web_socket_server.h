#ifndef WEB_SOCKET_BOOST_SERVER_H
#define WEB_SOCKET_BOOST_SERVER_H

#include <map>
#include <string>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

#include "../../rest_common/messages_interface.h"

namespace hp {
namespace http {

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class WebSocketServer : public RestServerInterface
{
public:
    WebSocketServer(const std::string& ip_address, unsigned short port, const uint32_t threads = 16, const std::shared_ptr<PathParser>& path_parser = nullptr);

    bool is_running() const;
    std::string get_url() const;
    unsigned short get_port() const;

    void stop() override;
    void start() override;
    ~WebSocketServer();
private:
    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::acceptor ip_acceptor_;
    boost::asio::ip::tcp::endpoint endpoint_;

    bool is_running_;
    const uint32_t threads_count_;
    std::string main_url_;
    boost::asio::ip::tcp::socket tcp_socekt_;
    std::vector<std::thread> ioc_threads_;
    std::vector<std::vector<std::string>> paths_;


    void accept_connection(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::ip::tcp::socket& socket);
};

} // namespace
} // namespace

#endif
