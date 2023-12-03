#include "server.h"

#include <iostream>
#include <thread>


//int handle_request(const RestMethods method, const std::string& path, std::string &put_data, std::string &result);
namespace rest {

using HttpRequestHanlder = std::function<boost::beast::http::status (const boost::beast::http::verb& method, const std::string& path, std::string &put_data, std::string &result)>;

class HTTPConnection : public std::enable_shared_from_this<HTTPConnection>
{
public:
    HTTPConnection(boost::asio::ip::tcp::socket socket, const HttpRequestHanlder& http_handler);
    void start();
    ~HTTPConnection();
private:
    void read_request();
    void process_request();
    void write_response();
    void check_deadline();
    HttpRequestHanlder http_handler_;
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_{8192}; //TODO(HP): fix this
    boost::beast::http::request<boost::beast::http::dynamic_body> request_;
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;

    boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_;
};


HTTPConnection::HTTPConnection(boost::asio::ip::tcp::socket socket, const HttpRequestHanlder &http_handler) :
    http_handler_(http_handler), socket_(std::move(socket)), deadline_{socket_.get_executor(), std::chrono::seconds(5)}
{

}

void HTTPConnection::start()
{
    read_request();
    check_deadline();
}

HTTPConnection::~HTTPConnection()
{
    std::cout << "distructor " << std::endl;
}

void HTTPConnection::read_request()
{
    auto self = shared_from_this();

    boost::beast::http::async_read(socket_, buffer_, request_, [self](boost::beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        if(!ec)
            self->process_request();
    });
}

void HTTPConnection::process_request()
{
    response_.version(request_.version());
    response_.keep_alive(false);

    auto method = request_.method();
    std::string path = request_.target().to_string();

    const auto& dynamicBuffer = request_.body().data();
    std::string content(boost::asio::buffers_begin(dynamicBuffer), boost::asio::buffers_end(dynamicBuffer));

    std::string result = "";
    std::cout << "start" << std::endl;

    auto ret = http_handler_(method, path, content, result);
    std::cout << "finished" << std::endl;
    response_.result(ret);
    response_.set(boost::beast::http::field::content_type, "text/plain");
    boost::beast::ostream(response_.body()) << result;
    write_response();
}

void HTTPConnection::write_response()
{
    auto self = shared_from_this();

//    response_.set(boost::beast::http::field::content_length, response_.body().size());

    boost::beast::http::async_write(socket_, response_, [self](boost::beast::error_code ec, std::size_t)
    {
        self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
        self->deadline_.cancel();
    });
}

void HTTPConnection::check_deadline()
{
    auto self = shared_from_this();

    deadline_.async_wait(
                [self](boost::beast::error_code ec)
    {
        if(!ec)
        {
            // Close socket to cancel any outstanding operation.
            self->socket_.close(ec);
        }
    });
}

BoostHttpServer::BoostHttpServer(const std::string& ip_address, unsigned short port) : BoostHttpServer(ip_address, port, std::make_shared<PathParser>())
{
}

BoostHttpServer::BoostHttpServer(const std::string& ip_address, unsigned short port, const std::shared_ptr<PathParser>& path_parser) :
    ip_acceptor_({ioc, {boost::asio::ip::make_address(ip_address), port}}), tcp_socekt_{ioc}, path_parser_(path_parser)
{
    is_running_ = false;
    methods_list_[boost::beast::http::verb::get]         = RestMethods::GET;
    methods_list_[boost::beast::http::verb::put]         = RestMethods::PUT;
    methods_list_[boost::beast::http::verb::delete_]     = RestMethods::DEL;
    methods_list_[boost::beast::http::verb::head]        = RestMethods::HEAD;
    methods_list_[boost::beast::http::verb::post]        = RestMethods::POST;
    methods_list_[boost::beast::http::verb::trace]       = RestMethods::TRCE;
    methods_list_[boost::beast::http::verb::patch]       = RestMethods::PATCH;
    methods_list_[boost::beast::http::verb::merge]       = RestMethods::MERGE;
    methods_list_[boost::beast::http::verb::options]     = RestMethods::OPTIONS;
    methods_list_[boost::beast::http::verb::connect]     = RestMethods::CONNECT;
    http_server(ip_acceptor_, tcp_socekt_);
}

void BoostHttpServer::add_path(const RestMethods method, const std::string &uri, const PutFunctionPtr &func)
{
    auto path_node = path_parser_->parse(uri);
    handler_default_[method][uri] = std::pair<std::shared_ptr<PathAddress>, PutFunctionPtr>(path_node, func);
}

void BoostHttpServer::http_server(boost::asio::ip::tcp::acceptor &acceptor, boost::asio::ip::tcp::socket &socket)
{
    ip_acceptor_.async_accept(socket, [&](boost::beast::error_code ec) {
        if(!ec) {
            auto http_connection = std::make_shared<HTTPConnection>(std::move(socket), std::bind(&BoostHttpServer::handle_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
            http_connection->start();
        }
        http_server(acceptor, socket);
    });
}

boost::beast::http::status BoostHttpServer::handle_request(const boost::beast::http::verb& method, const std::string &path, std::string &put_data, std::string& result)
{
    auto parser = handler_default_.find(methods_list_[method]);
    result = "This uri doesn't support";
    boost::beast::http::status ret = boost::beast::http::status::bad_request;
    if (parser != handler_default_.end()) {
        PathParser path_parser;
        auto rest_node = path_parser.parse(path);
        for (const auto &map_node : parser->second) {
            std::vector<std::string> inputs;
            bool is_same = path_parser.is_same_path(map_node.second.first, rest_node, inputs);
            if (is_same) {
                auto handler = map_node.second.second;
                ret = static_cast<boost::beast::http::status>(handler(inputs, put_data, result));
                break;
            }
        }
    }
    return ret;
}

void BoostHttpServer::start()
{
    ioc.run();
}

void BoostHttpServer::stop()
{
    ioc.stop();
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

}
