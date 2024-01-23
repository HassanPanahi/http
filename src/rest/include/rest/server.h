#ifndef SERVER_H
#define SERVER_H

#include <cpprest/http_listener.h>
#include <functional>
#include <map>
#include "path_parser.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

namespace rest {

enum class Methods{
    PUT,
    GET
};

enum class ErrorTypes
{
    OK,
    WRONG_PATH,
};

class Server
{
public:
    Server(const std::string &url, unsigned short port);
    Server(std::shared_ptr<PathParser> pathparser, const std::string &url, unsigned short port);
    void add_get_path(const std::string &path, std::function<std::string (std::vector<std::string>)> func);
    void add_put_path(const std::string &path, std::function<std::string (std::vector<std::string>, std::string)> func);
    void start(const std::vector<Methods> &methods);
    void stop();
    bool is_running() const;
    std::string get_url() const;
    unsigned short get_port() const;

private:
    std::string find_put_request(const std::string& path,const std::string& put_data);
    std::string find_get_request(const std::string& path);
    void handle_put(web::http::http_request message);
    void handle_get(web::http::http_request message);

    std::map<std::shared_ptr<PathAddress>, std::function<std::string (std::vector<std::string>, std::string)>> put_methods_;
    std::map<std::shared_ptr<PathAddress>, std::function<std::string (std::vector<std::string>)>> get_methods_;

    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_{8192};


    std::shared_ptr<PathParser> path_parser_;
    std::vector<std::vector<std::string>> paths_;
    bool is_running_;
    std::string url_;
    unsigned short port_;
    std::string main_url_;

};

} //peripheral

#endif // SERVER_H
