#ifndef SERVER_H
#define SERVER_H

#include "path_parser.h"

#include <map>
#include <string>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <google/protobuf/message.h>

#include "messages_interface.h"

namespace hp {
namespace http {

using uri = std::string;
using PutFunctionPtr = std::function<unsigned (const std::vector<std::string>&, std::string& put_data, std::string& response)>;
using ProtobufFunctionPtr = std::function<unsigned (const std::vector<std::string>&, std::shared_ptr<google::protobuf::Message>& input_msg, std::string& response)>;

enum class Methods {
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
    void set_msg_validator(const std::shared_ptr<MessageValidatorInterface>& msg_validator);
    void add_path(const Methods method, const std::string &uri, const PutFunctionPtr& func);
    void add_path(const Methods method, const std::string &uri, const uint32_t protobuf_msg,  const ProtobufFunctionPtr& func);

private:
    bool is_running_;
    std::string main_url_;

    boost::asio::io_context ioc_ {3};
    boost::asio::ip::tcp::socket tcp_socekt_;
    boost::asio::ip::tcp::acceptor ip_acceptor_;

    std::shared_ptr<PathParser> path_parser_;
    std::shared_ptr<MessageValidatorInterface> msg_validator_;

    std::vector<std::vector<std::string>> paths_;
    std::map<boost::beast::http::verb, Methods> methods_list_;
    std::map<Methods, std::map<uri, std::pair<std::shared_ptr<PathAddress>, PutFunctionPtr>> > handler_default_;
    std::map<Methods, std::map<uri, std::pair<std::shared_ptr<PathAddress>, ProtobufFunctionPtr>> > handler_protobuf_default_;
    std::map<Methods, std::map<uri, uint32_t>> handler_msg_default_;

    void accept_connection(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::ip::tcp::socket& socket);
    boost::beast::http::status handle_request(const boost::beast::http::verb &method, const std::string& path, std::string &put_data, std::string &result);
};

} // namespace
} // namespace

#endif
