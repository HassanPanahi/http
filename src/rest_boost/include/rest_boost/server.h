#ifndef REST_BOOST_SERVER_H
#define REST_BOOST_SERVER_H

#include "path/path_parser.h"

#include <map>
#include <string>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <google/protobuf/message.h>

#include "../../rest_common/messages_interface.h"

namespace hp {
namespace http {


class BoostRestServer : public RestServerInterface
{
public:
    BoostRestServer(const std::string& ip_address, unsigned short port, const uint32_t threads = 16);
    BoostRestServer(const std::string& ip_address, unsigned short port, const std::shared_ptr<PathParser>& path_parser, const uint32_t threads = 16);

    bool is_running() const;
    std::string get_url() const;
    unsigned short get_port() const;

    void stop() override;
    void start() override;
    void set_msg_validator(const std::shared_ptr<MessageValidatorInterface>& msg_validator) override;
    void add_path(const Methods method, const std::string &uri, const PutFunctionPtr& func) override;
    void add_path(const Methods method, const std::string &uri, const uint32_t protobuf_msg,  const ProtobufFunctionPtr& func) override;
    ~BoostRestServer();
private:
    bool is_running_;
    const uint32_t threads_count_;
    std::string main_url_;
    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::socket tcp_socekt_;
    boost::asio::ip::tcp::acceptor ip_acceptor_;

    std::vector<std::thread> ioc_threads_;

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
