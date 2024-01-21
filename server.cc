#include "server.h"

#include <iostream>
#include <thread>

#include "http_connection.h"

//int handle_request(const Methods method, const std::string& path, std::string &put_data, std::string &result);
namespace hp {
namespace http {

BoostHttpServer::BoostHttpServer(const std::string& ip_address, unsigned short port) : BoostHttpServer(ip_address, port, std::make_shared<PathParser>())
{
}

BoostHttpServer::BoostHttpServer(const std::string& ip_address, unsigned short port, const std::shared_ptr<PathParser>& path_parser) :
    ip_acceptor_({ioc_, {boost::asio::ip::make_address(ip_address), port}}), tcp_socekt_{ioc_}, path_parser_(path_parser)
{
    is_running_ = false;
    methods_list_[boost::beast::http::verb::get]         = Methods::GET;
    methods_list_[boost::beast::http::verb::put]         = Methods::PUT;
    methods_list_[boost::beast::http::verb::delete_]     = Methods::DEL;
    methods_list_[boost::beast::http::verb::head]        = Methods::HEAD;
    methods_list_[boost::beast::http::verb::post]        = Methods::POST;
    methods_list_[boost::beast::http::verb::trace]       = Methods::TRCE;
    methods_list_[boost::beast::http::verb::patch]       = Methods::PATCH;
    methods_list_[boost::beast::http::verb::merge]       = Methods::MERGE;
    methods_list_[boost::beast::http::verb::options]     = Methods::OPTIONS;
    methods_list_[boost::beast::http::verb::connect]     = Methods::CONNECT;
    accept_connection(ip_acceptor_, tcp_socekt_);
}

void BoostHttpServer::add_path(const Methods method, const std::string &uri, const PutFunctionPtr &func)
{
    auto path_node = path_parser_->parse(uri);
    handler_default_[method][uri] = std::pair<std::shared_ptr<PathAddress>, PutFunctionPtr>(path_node, func);
}

void BoostHttpServer::add_path(const Methods method, const std::string &uri, const ProtobufValidatorFuncPtr &validator, const ProtobufFunctionPtr &func)
{
    auto path_node = path_parser_->parse(uri);
    handler_protobuf_default_[method][uri] = std::pair<std::shared_ptr<PathAddress>, ProtobufFunctionPtr>(path_node, func);
    handler_protobuf_validator_default_[method][uri] = validator;
}

void BoostHttpServer::accept_connection(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::ip::tcp::socket &socket)
{
    ip_acceptor_.async_accept(socket, [&](boost::beast::error_code ec) {
        if(!ec) {
            auto http_connection = std::make_shared<BoostHTTPConnection>(std::move(socket), std::bind(&BoostHttpServer::handle_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
            http_connection->start();
        }
        accept_connection(acceptor, socket);
    });
}

boost::beast::http::status BoostHttpServer::handle_request(const boost::beast::http::verb& method, const std::string &path, std::string &put_data, std::string& result)
{
    //    auto parser = handler_default_.find(methods_list_[method]);
    //    result = "This uri doesn't support";
    //    boost::beast::http::status ret = boost::beast::http::status::bad_request;
    //    if (parser != handler_default_.end()) {
    //        PathParser path_parser;
    //        auto rest_node = path_parser.parse(path);
    //        for (const auto &map_node : parser->second) {
    //            std::vector<std::string> inputs;
    //            bool is_same = path_parser.is_same_path(map_node.second.first, rest_node, inputs);
    //            if (is_same) {
    //                auto handler = map_node.second.second;
    //                ret = static_cast<boost::beast::http::status>(handler(inputs, put_data, result));
    //                break;
    //            }
    //        }

    //    }
    auto new_method = methods_list_[method];
    auto proto_parser = handler_protobuf_default_.find(new_method);
    result = "This uri doesn't support";
    boost::beast::http::status ret = boost::beast::http::status::bad_request;
    if (proto_parser != handler_protobuf_default_.end()) {
        PathParser path_parser;
        auto rest_node = path_parser.parse(path);
        for (const auto &map_node : proto_parser->second) {
            std::vector<std::string> inputs;
            bool is_same = path_parser.is_same_path(map_node.second.first, rest_node, inputs);
            if (is_same) {
                auto validator = handler_protobuf_validator_default_[new_method][path];
                std::shared_ptr<google::protobuf::Message> msg;
                if (validator(msg, put_data)) {
                    auto handler = map_node.second.second;
                    ret = static_cast<boost::beast::http::status>(handler(inputs, msg, result));
                    break;
                }
            }
        }

    }

    return ret;
}

void BoostHttpServer::start()
{
    std::vector<std::thread> v;
    v.reserve(3);
    for(auto i = 0; i < 3; i++)
        v.emplace_back([this]{ ioc_.run(); } );
    std::cout << "Http server listen to " << get_port() << " port" << std::endl;
    ioc_.run();
}

void BoostHttpServer::stop()
{
    ioc_.stop();
}

bool BoostHttpServer::is_running() const
{
    return is_running_;
}

std::string BoostHttpServer::get_url() const
{
    return ip_acceptor_.local_endpoint().address().to_string();
}

unsigned short BoostHttpServer::get_port() const
{
    return ip_acceptor_.local_endpoint().port();
}

} // namespace
} // namespace
