#ifndef BoostHTTPConnection_H
#define BoostHTTPConnection_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

#include "../../rest_common/messages_interface.h"

namespace hp {
namespace http {

using HttpRequestHanlder = std::function<unsigned int (const Methods& method, const std::string& path, std::string &put_data, std::string &result)>;

class BoostHTTPConnection : public std::enable_shared_from_this<BoostHTTPConnection>
{
public:
    BoostHTTPConnection(boost::asio::ip::tcp::socket socket, const HttpRequestHanlder& http_handler);
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
    static std::map<boost::beast::http::verb, Methods> methods_list_;

};

} // namespace
} // namespace

#endif // BoostHTTPConnection_H
