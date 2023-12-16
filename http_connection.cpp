#include "http_connection.h"

namespace hp {
namespace http {

BoostHTTPConnection::BoostHTTPConnection(boost::asio::ip::tcp::socket socket, const HttpRequestHanlder &http_handler) :
    http_handler_(http_handler), socket_(std::move(socket)), deadline_{socket_.get_executor(), std::chrono::seconds(0)}
{

}

void BoostHTTPConnection::start()
{
    read_request();
    check_deadline();
}

void BoostHTTPConnection::read_request()
{
    auto self = shared_from_this();

    boost::beast::http::async_read(socket_, buffer_, request_, [self](boost::beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        if(!ec)
            self->process_request();
    });
}

void BoostHTTPConnection::process_request()
{
    response_.version(request_.version());
    response_.keep_alive(false);

    auto method = request_.method();
    std::string path = request_.target().to_string();

    const auto& dynamicBuffer = request_.body().data();
    std::string content(boost::asio::buffers_begin(dynamicBuffer), boost::asio::buffers_end(dynamicBuffer));

    std::string result = "";
    auto ret = http_handler_(method, path, content, result);
    response_.result(ret);
    response_.set(boost::beast::http::field::content_type, "text/plain");
    boost::beast::ostream(response_.body()) << result;
    write_response();
}

void BoostHTTPConnection::write_response()
{
    auto self = shared_from_this();

    boost::beast::http::async_write(socket_, response_, [self](boost::beast::error_code ec, std::size_t)
    {
        self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
        self->deadline_.cancel();
    });
}

void BoostHTTPConnection::check_deadline()
{
    return;
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

} // namespace
} // namespace
