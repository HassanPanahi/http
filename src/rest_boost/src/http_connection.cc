#include "rest_boost/http_connection.h"

namespace hp {
namespace http {


std::map<boost::beast::http::verb, Methods>  BoostHTTPConnection::methods_list_ {
    {boost::beast::http::verb::get    , Methods::GET},
    {boost::beast::http::verb::put    , Methods::PUT},
    {boost::beast::http::verb::delete_, Methods::DEL},
    {boost::beast::http::verb::head   , Methods::HEAD},
    {boost::beast::http::verb::post   , Methods::POST},
    {boost::beast::http::verb::trace  , Methods::TRCE},
    {boost::beast::http::verb::patch  , Methods::PATCH},
    {boost::beast::http::verb::merge  , Methods::MERGE},
    {boost::beast::http::verb::options, Methods::OPTIONS},
    {boost::beast::http::verb::connect, Methods::CONNECT}
};

BoostHTTPConnection::BoostHTTPConnection(boost::asio::ip::tcp::socket socket, const HttpRequestHanlder &http_handler) :
    http_handler_(http_handler), socket_(std::move(socket)), deadline_{socket_.get_executor(), std::chrono::seconds(0)}
{

}

BoostHTTPConnection::~BoostHTTPConnection()
{
    std::cout << "end of client" << std::endl;
}

void BoostHTTPConnection::start()
{
    read_request();
    check_deadline();
}

void BoostHTTPConnection::read_request()
{
    auto self = shared_from_this();

    request_.get().clear();
    request_.body_limit(boost::optional<std::uint64_t>(1024* 1024 * 1024));
    boost::beast::http::async_read(socket_, buffer_, request_, [self](boost::beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        if(!ec) {

            std::cout << "trasnfer: " << bytes_transferred  << std::endl;
            self->process_request();
        }
        else
            std::cout << "error: " << ec.message() << std::endl;
    });
}

void BoostHTTPConnection::process_request()
{
// Access the parsed request object
   boost::beast::http::request<boost::beast::http::string_body>& req = request_.get();

   size_t headers_size = 0;
   for (const auto& header : req.base()) {
       headers_size += header.name_string().size() + header.value().size() + 4; // Add 4 for ": " and "\r\n"
   }

   // Calculate the size of the body data
   size_t body_size = req.body().size();
   std::vector<URIDynamicSection> inputs;
   // Output the sizes
   std::cout << "Headers size: " << headers_size << " bytes" << std::endl;
   std::cout << "Body size: " << body_size << " bytes" << std::endl;
   auto data = req.header_type[boost::beast::http::field::content_type];
    std::string header_content;
   for (const auto& header : req.base())
         header_content += header.name_string().to_string() + ": " + header.value().to_string() + "\r\n";
   std::cout << "header content: " << header_content << std::endl;

   // Convert the request to a string
   std::stringstream ss;
   ss << req;

   // Get the string representation of the request
   std::string request_str = ss.str();

   // Output the string representation of the request
   std::cout << "Request: \n" << request_str << std::endl;

//    std::cout << "size: " << request_.is_done() << std::endl;
//    response_.version(request_.get().version());
//    response_.keep_alive(false);
//    auto method = request_.get().method();
//    std::string path = request_.get().target().to_string();

//    boost::beast::http::request<boost::beast::http::string_body> req;
//    request_.get(req);

//    // Serialize the request to a string
//    std::string content = boost::beast::http::to_string(req);


//    const auto& dynamicBuffer = request_.get().body().data();
//    std::string content = boost::asio::buffers_begin(dynamicBuffer), boost::asio::buffers_end(dynamicBuffer));
//    std::string result = "";
//    Methods new_method = Methods::Unknown;
//    auto method_iterator = methods_list_.find(method);
//    if (method_iterator != methods_list_.end())
//        new_method = method_iterator->second;

//    auto ret = http_handler_(new_method, result, content, result);
//    response_.result(ret);
//    response_.set(boost::beast::http::field::content_type, "text/plain");
//    boost::beast::ostream(response_.body()) << result;
//    write_response();
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

    deadline_.async_wait([self](boost::beast::error_code ec)
    {
        if(!ec)
            self->socket_.close(ec);
    });
}

} // namespace
} // namespace
