#ifndef BoostHTTPConnection_H
#define BoostHTTPConnection_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

#include "../../rest_common/messages_interface.h"

namespace hp {
namespace http {

struct ContentType {
    std::string type;
    std::string content;
};


using HttpRequestHanlder = std::function<unsigned int (const Methods& method, const std::string& path, std::vector<URIDynamicSection>& inputs, std::string &result)>;
using PartsHandler = std::function<std::vector<URIDynamicSection> (const std::string& body, const ContentType& header)>;

//BASED ON https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Content-Disposition
class BoostHTTPConnection : public std::enable_shared_from_this<BoostHTTPConnection>
{
public:
    BoostHTTPConnection(boost::asio::ip::tcp::socket socket, const HttpRequestHanlder& http_handler);
    ~BoostHTTPConnection();
    void start();
private:
    HttpRequestHanlder http_handler_;
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_{8192}; //TODO(HP): fix this
    std::map<std::string, PartsHandler> parts_handler_;
    boost::beast::http::request_parser<boost::beast::http::string_body> request_;
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_;
    static std::map<boost::beast::http::verb, Methods> methods_list_;

    void read_request();
    void process_request();
    void write_response();
    std::vector<URIDynamicSection> form_data_handler(const std::string& body, const ContentType& header);
    std::vector<URIDynamicSection> application_json_handler(const std::string& body, const ContentType& header);
    std::vector<URIDynamicSection> x_www_form_urlencoded_handler(const std::string& body, const ContentType& header);


};

} // namespace
} // namespace

#endif // BoostHTTPConnection_H
