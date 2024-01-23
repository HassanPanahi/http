#include "server.h"

#include <iostream>
#include <thread>

namespace hp {
namespace http {

Server::Server(const std::string& ip_address, unsigned short port) : Server(std::make_shared<PathParser>(), ip_address, port)
{

}

Server::Server(std::shared_ptr<PathParser> path_parser, const std::string& ip_address, unsigned short port) : path_parser_(path_parser), url_(ip_address), port_(port)
{
    is_running_ = false;
    main_url_ = "http://"+ url_+ ":"+ std::to_string(port);
    http_listener_ = web::http::experimental::listener::http_listener(web::uri(U(main_url_)));
}

void Server::add_get_path(const std::string& path, std::function<std::string(std::vector<std::string>)> func)
{
    auto path_node = path_parser_->parse(path);
    get_methods_[path_node] = func;
}

void Server::add_put_path(const std::string& path, std::function<std::string(std::vector<std::string>, std::string)> func)
{
    auto path_node = path_parser_->parse(path);
    put_methods_[path_node] = func;
}

std::string Server::find_put_request(const std::string &path, const std::string &put_data)
{
    auto path_parser = std::make_shared<PathParser>();
    auto rest_node = path_parser->parse(path);
    std::string result = "Path is not correct!!!";
    bool find = false;
    for (auto map_node : put_methods_) {
        std::vector<std::string> inputs;
        if (path_parser->is_same_path(map_node.first, rest_node, inputs)) {
            result = map_node.second(inputs, put_data);
            find = true;
            break;
        }
    }
    return result;
}

std::string Server::find_get_request(const std::string &path)
{
    auto path_parser = std::make_shared<PathParser>();
    auto rest_node = path_parser->parse(path);
    std::string result = "Path is not correct!!!";
    bool find = false;
    for (auto map_node : get_methods_) {
        std::vector<std::string> inputs;
        bool is_same = path_parser->is_same_path(map_node.first, rest_node, inputs);
        if (is_same) {
            result = map_node.second(inputs);
            find = true;
            break;
        }
    }
    return result;
}

void Server::start(const std::vector<RestMethods> &methods)
{
    for (auto method : methods) {
        if (method == RestMethods::GET) {
            http_listener_.support(web::http::methods::GET, std::bind(&Server::handle_get, this, std::placeholders::_1));
        }
        else if (method == RestMethods::PUT) {
            http_listener_.support(web::http::methods::PUT, std::bind(&Server::handle_put, this, std::placeholders::_1));
        } else {
            std::cout << "Method not implement!!!" << std::endl;
            return;
        }
    }
    try {
        http_listener_.open().wait();
        is_running_ = true;
        std::cout << "Start rest server on: " << main_url_ << std::endl;
    } catch (std::exception &e) {
        std::cout << "Cant start rest server:  " << e.what() << std::endl;
    }
}

void Server::stop()
{
    http_listener_.close().wait();
}

bool Server::is_running() const
{
    return is_running_;
}

std::string Server::get_url() const
{
    return url_;
}

unsigned short Server::get_port() const
{
    return port_;
}

void Server::handle_get(web::http::http_request message)
{
    auto paths = web::http::uri::split_path(web::http::uri::decode(message.relative_uri().path()));
    std::string path;
    if (paths.size() == 0) {
        message.reply(web::http::status_codes::BadRequest, "Wrong Path", "application/octet-stream");
    } else {
        path += paths[0];
        for (unsigned int i = 1; i < paths.size(); ++i)
            path += "/" + paths[i];
    }
    try {
        std::string name = find_get_request(path);
        message.reply(web::http::status_codes::OK, name, "application/octet-stream");
    } catch (std::string &e) {
        std::cout << "Server Handle Get has error: " <<  e << std::endl;
        message.reply(web::http::status_codes::BadRequest, e, "application/octet-stream");
    }
}

void Server::handle_put(web::http::http_request message)
{
    auto paths = web::http::uri::split_path(web::http::uri::decode(message.relative_uri().path()));
    std::string path;
    if (paths.size() == 0) {
        message.reply(web::http::status_codes::BadRequest, "Wrong Path", "application/octet-stream");
    } else {
        path += paths[0];
        for (unsigned int i = 1; i < paths.size(); ++i)
            path += "/" + paths[i];
    }
    try {
        auto put_data = message.extract_string(true).get();
        std::string name = find_put_request(path, put_data);
        message.reply(web::http::status_codes::OK, name, "application/octet-stream");
    } catch (std::string &e)  {
        message.reply(web::http::status_codes::BadRequest, e, "application/octet-stream");
    }
}

}
}
