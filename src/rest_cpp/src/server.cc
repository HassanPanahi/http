#include "server.h"

#include <iostream>
#include <thread>

namespace hp {
namespace http {

CppRestServer::CppRestServer(const std::string& ip, unsigned short port, const std::shared_ptr<PathParser> &path_parser) :
    ip_(ip), port_(port)
{
    if (path_parser == nullptr)
        path_parser_ = std::make_shared<PathParser>();
    is_running_ = false;
    main_url_ = "http://"+ ip_+ ":"+ std::to_string(port_);
    http_listener_ = web::http::experimental::listener::http_listener(web::uri(U(main_url_)));

    methods_list_[web::http::methods::PUT]     = Methods::PUT;
    methods_list_[web::http::methods::DEL]     = Methods::DEL;
    methods_list_[web::http::methods::HEAD]    = Methods::HEAD;
    methods_list_[web::http::methods::POST]    = Methods::POST;
    methods_list_[web::http::methods::TRCE]    = Methods::TRCE;
    methods_list_[web::http::methods::PATCH]   = Methods::PATCH;
    methods_list_[web::http::methods::MERGE]   = Methods::MERGE;
    methods_list_[web::http::methods::OPTIONS] = Methods::OPTIONS;
    methods_list_[web::http::methods::CONNECT] = Methods::CONNECT;
}

void CppRestServer::add_path(const Methods method, const std::string &uri, const PutFunctionPtr &func)
{
    auto path_node = path_parser_->parse(uri);
    handler_default_[method][uri] = std::pair<std::shared_ptr<PathAddress>, PutFunctionPtr>(path_node, func);
}

void CppRestServer::add_path(const Methods method, const std::string &uri, const uint32_t protobuf_msg, const ProtobufFunctionPtr &func)
{
    auto path_node = path_parser_->parse(uri);
    handler_protobuf_default_[method][uri] = std::pair<std::shared_ptr<PathAddress>, ProtobufFunctionPtr>(path_node, func);
    handler_msg_default_[method][uri] = protobuf_msg;
}

void CppRestServer::set_msg_validator(const std::shared_ptr<MessageValidatorInterface> &msg_validator)
{

}

void CppRestServer::handle_request(web::http::http_request message)
{
    auto request_method = message.method();
    auto method = methods_list_[request_method];
    auto path = web::http::uri::decode(message.relative_uri().path());
    auto receive_data = message.extract_string(true).get();
    auto parser = handler_default_.find(method);
    std::string result = "This uri doesn't support";
    auto ret = web::http::status_codes::BadRequest;
    if (parser != handler_default_.end()) {
        auto rest_node = path_parser_->parse(path);
        for (const auto &map_node : parser->second) {
            std::vector<URIDynamicSection> inputs;
            bool is_same = path_parser_->is_same_path(map_node.second.first, rest_node, inputs);
            if (is_same) {
                auto handler = map_node.second.second;
                auto ret_value = handler(inputs, receive_data, result);
                ret = static_cast<web::http:: status_code>(ret_value);
                break;
            }
        }
    }
    message.reply(ret, result, "application/octet-stream");
}


void CppRestServer::start()
{
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1));
    try {
        http_listener_.open().wait();
        is_running_ = true;
        std::cout << "Start rest server on: " << main_url_ << std::endl;
    } catch (std::exception &e) {
        std::cout << "Cant start rest server:  " << e.what() << std::endl;
    }
}

void CppRestServer::stop()
{
    http_listener_.close().wait();
}

bool CppRestServer::is_running() const
{
    return is_running_;
}

std::string CppRestServer::get_ip() const
{
    return ip_;
}

unsigned short CppRestServer::get_port() const
{
    return port_;
}


}
}
