#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
#include "path_parser.h"
#include <google/protobuf/message.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

namespace rest {

enum class RestMethods{
    GET,
    PUT,
    DEL,
    POST,
    HEAD,
    TRCE,
    PATCH,
    MERGE,
    OPTIONS,
    CONNECT
};

using PutFunctionPtr = std::function<int (const std::vector<std::string>&, std::string& put_data, std::string& response)>;
using PutFunctionPtrMSG = std::function<int (const std::vector<std::string>&, google::protobuf::Message* const receive_msg, google::protobuf::Message* const response_msg)>;

class BoostHttpServer
{
public:
    BoostHttpServer(const std::string& ip_address, unsigned short port);
    BoostHttpServer(const std::string& ip_address, unsigned short port, const std::shared_ptr<PathParser>& path_parser);

    void stop();
    void start();

    bool is_running() const;

    std::string get_url() const;
    unsigned short get_port() const;

    void add_path(const RestMethods method, const std::string &uri, const PutFunctionPtr& func);

private:
    void http_server(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::ip::tcp::socket& socket);

    std::string main_url_;
    bool is_running_;

    boost::asio::io_context ioc;
    boost::asio::ip::tcp::acceptor ip_acceptor_;
    boost::asio::ip::tcp::socket tcp_socekt_;

    std::shared_ptr<PathParser> path_parser_;
    std::vector<std::vector<std::string>> paths_;
    std::map<boost::beast::http::verb, RestMethods> methods_list_;

    std::map<RestMethods, std::map<std::string, std::pair<std::shared_ptr<PathAddress>, PutFunctionPtr>> > handler_default_;
    std::map<RestMethods, std::map<std::string, std::pair<std::shared_ptr<PathAddress>, PutFunctionPtrMSG>> > handler_protobuf_msgs_;

    void handle_method(const RestMethods method, boost::beast::http::verb message);
    int find_request(const boost::beast::http::verb &method, const std::string& path, std::string &put_data, std::string &result);
};

}

#endif
