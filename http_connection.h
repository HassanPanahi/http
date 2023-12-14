#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>


namespace hp {
namespace http {

using HttpRequestHanlder = std::function<boost::beast::http::status (const boost::beast::http::verb& method, const std::string& path, std::string &put_data, std::string &result)>;

class HTTPConnection : public std::enable_shared_from_this<HTTPConnection>
{
public:
    HTTPConnection(boost::asio::ip::tcp::socket socket, const HttpRequestHanlder& http_handler);
    void start();
private:
    void read_request();
    void process_request();
    void write_response();
    void check_deadline();
    HttpRequestHanlder http_handler_;
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_{8192}; //TODO(HP): fix this
    boost::beast::http::request<boost::beast::http::dynamic_body> request_;
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;

    boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_;
};

} // namespace
} // namespace

#endif // HTTPCONNECTION_H
