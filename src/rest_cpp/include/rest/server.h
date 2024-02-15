#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <functional>

#include <cpprest/http_listener.h>

#include "path/path_parser.h"
#include "../../rest_common/messages_interface.h"

namespace hp {
namespace http {

class CppRestServer : public RestServerInterface
{
public:
    CppRestServer(const std::string& ip_address, unsigned short port);
    CppRestServer(const std::string& ip_address, unsigned short port, const std::shared_ptr<PathParser>& path_parser = nullptr);

    void stop();
    void start();
    void add_path(const Methods method, const std::string &uri, const PutFunctionPtr &func);
    void set_msg_validator(const std::shared_ptr<MessageValidatorInterface> &msg_validator);
    void add_path(const Methods method, const std::string &uri, const uint32_t msg_id, const ProtobufFunctionPtr &func);

    bool is_running() const;
    std::string get_url() const;
    unsigned short get_port() const;

private:
    bool is_running_;
    std::string url_;
    unsigned short port_;
    std::string main_url_;

    std::shared_ptr<PathParser> path_parser_;
    std::shared_ptr<MessageValidatorInterface> msg_validator_;

    std::vector<std::vector<std::string>> paths_;
    std::map<Methods, std::map<uri, uint32_t>> handler_msg_default_;
    std::map<Methods, std::map<uri, std::pair<std::shared_ptr<PathAddress>, PutFunctionPtr>> > handler_default_;
    std::map<Methods, std::map<uri, std::pair<std::shared_ptr<PathAddress>, ProtobufFunctionPtr>> > handler_protobuf_default_;

    web::http::experimental::listener::http_listener http_listener_;
    web::http::status_codes handle_request(const web::http::http_request& message, const Methods &method);
};

}
}

#endif // SERVER_H
