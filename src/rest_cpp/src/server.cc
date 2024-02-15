#include "server.h"

#include <iostream>
#include <thread>

namespace hp {
namespace http {

CppRestServer::CppRestServer(const std::string& ip_address, unsigned short port) :
    CppRestServer(ip_address, port, std::make_shared<PathParser>())
{

}

CppRestServer::CppRestServer(const std::string& ip_address, unsigned short port, const std::shared_ptr<PathParser> &path_parser) : path_parser_(path_parser), url_(ip_address), port_(port)
{
    is_running_ = false;
    main_url_ = "http://"+ url_+ ":"+ std::to_string(port);
    http_listener_ = web::http::experimental::listener::http_listener(web::uri(U(main_url_)));    

//    methods_list_[web::http::methods::PUT]     = Methods::PUT;
//    methods_list_[web::http::methods::DEL]     = Methods::DEL;
//    methods_list_[web::http::methods::HEAD]    = Methods::HEAD;
//    methods_list_[web::http::methods::POST]    = Methods::POST;
//    methods_list_[web::http::methods::TRCE]    = Methods::TRCE;
//    methods_list_[web::http::methods::PATCH]   = Methods::PATCH;
//    methods_list_[web::http::methods::MERGE]   = Methods::MERGE;
//    methods_list_[web::http::methods::OPTIONS] = Methods::OPTIONS;
//    methods_list_[web::http::methods::CONNECT] = Methods::CONNECT;
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

web::http::status_codes CppRestServer::handle_request(const web::http::http_request& message, const Methods &method)
{
//    auto rest_method = methods_list_[method];
//    auto parser = handler_default_.find(method);
//    result = "This uri doesn't support";
//    auto ret = web::http::status_codes::BadRequest;
//    if (parser != handler_default_.end()) {
//        PathParser path_parser;
//        auto rest_node = path_parser.parse(path);
//        for (const auto &map_node : parser->second) {
//            std::vector<URIDynamicSection> inputs;
//            bool is_same = path_parser.is_same_path(map_node.second.first, rest_node, inputs);
//            if (is_same) {
//                auto handler = map_node.second.second;
////                ret = static_cast<web::http::status_codes>(handler(inputs, put_data, result));
//                break;
//            }
//        }
//    }

//    auto proto_parser = handler_protobuf_default_.find(method);
//    try {
//        if (proto_parser != handler_protobuf_default_.end()) {
//            PathParser path_parser;
//            auto rest_node = path_parser.parse(path);
//            for (const auto &map_node : proto_parser->second) {
//                std::vector<URIDynamicSection> inputs;
//                bool is_same = path_parser.is_same_path(map_node.second.first, rest_node, inputs);
//                if (is_same) {
//                    auto msg_id = handler_msg_default_[method][path];
//                    auto msg = msg_validator_->get_message(msg_id, put_data);
//                    auto proto_ptr_func = map_node.second.second;
////                    ret = static_cast<web::http::status_codes>(proto_ptr_func(inputs, msg, result));
//                    break;
//                }
//            }
//        }

//    }  catch (...) {
//        std::cout << "exception is boost rest" << std::endl;
//    }

//    return ret;
}


void CppRestServer::start()
{
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::GET));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::PUT));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::DEL));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::HEAD));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::TRCE));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::MERGE));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::PATCH));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::POST));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::CONNECT));
    http_listener_.support(std::bind(&CppRestServer::handle_request, this, std::placeholders::_1, Methods::OPTIONS));

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

std::string CppRestServer::get_url() const
{
    return url_;
}

unsigned short CppRestServer::get_port() const
{
    return port_;
}


}
}
