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
    else
        path_parser_ = path_parser;

    is_running_ = false;
    main_url_ = "http://"+ ip_+ ":"+ std::to_string(port_);
    http_listener_ = web::http::experimental::listener::http_listener(web::uri(U(main_url_)));

    methods_list_[web::http::methods::GET]     = Methods::GET;
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


void CppRestServer::handle_request(web::http::http_request message)
{
    auto request_method = message.method();
    auto methods_iterator = methods_list_.find(request_method);
    std::string result = "This uri doesn't support";
    auto ret = web::http::status_codes::BadRequest;
    if (methods_iterator == methods_list_.end()) {
        result = "Method "+ std::string(request_method.data())+ " doesn't support" ;
        ret = web::http::status_codes::MethodNotAllowed;
    } else {
        auto handler = handler_default_.find(methods_iterator->second);
        if (handler != handler_default_.end()) {
            auto path = message.absolute_uri().to_string();
            auto rest_node = path_parser_->parse(path);
            for (const auto &map_node : handler->second) {
                std::vector<URIDynamicSection> inputs;
                bool is_same = path_parser_->is_same_path(map_node.second.first, rest_node, inputs);
                if (is_same) {
                    auto func_ptr = map_node.second.second;
                    auto receive_data = message.extract_string(true).get();
                    auto ret_value = func_ptr(inputs, receive_data, result);
                    ret = static_cast<web::http:: status_code>(ret_value);
                    break;
                }
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

CppRestServer::~CppRestServer()
{
    CppRestServer::stop();
}


}
}
