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
    request_.body_limit(boost::optional<std::uint64_t>(1024* 1024 * 1024));
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

std::vector<std::string> split_data(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> result;
    std::string data = std::move(str);
    size_t pos = 0;
    std::string token;
    while ((pos = data.find(delimiter)) != std::string::npos) {
        token = data.substr(0, pos);
        result.push_back(token);
        data.erase(0, pos + delimiter.length());
    }
    if (data.size() > 0)
        result.push_back(data);
    return result;
}


//enum MultipartLineType{
//    MetaData,
//    Content,
//    Unknown
//};

// Structure to hold each part of the form data
struct FormDataPart {
    std::string name;
    std::string file_name;
    std::string content_type;
    std::string content;
};

std::string extract_boundary(const std::string& contentType) {
    std::string boundary_title ="boundary=";
    std::string boundary_data = "";
    auto pos = contentType.find(boundary_title);
    if (pos == std::string::npos) {
        std::cout << "throw an exception hp" << std::endl;
    } else {
        pos += boundary_title.size();
        boundary_data = contentType.substr(pos, contentType.size());
        if (boundary_data.front() == '"' && boundary_data.back() == '"') //TODO(HP): Somewhere i read it that boundary may be have ""
            boundary_data = boundary_data.substr(1, boundary_data.length() - 2);
        boundary_data.push_back('\r');
    }
    return boundary_data;
}

const std::string HeaderNameString = "name=\"";
const std::string ContentTypeString = "Content-Type";
const std::string HeaderFileNameString = "filename=\"";
const std::string ContentDispositionString = "Content-Disposition";

//BASED ON https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Content-Disposition
std::vector<FormDataPart> parse_multipart_form_data(const std::string& body, const std::string& boundary) {
    std::vector<FormDataPart> parts;
    std::istringstream stream(body);
    std::string temp_boundary = "--" + boundary;
//    boundary.insert(boundary.end() - 2, "--");
    std::string end_boundary = temp_boundary;
    end_boundary.insert(end_boundary.length() - 1, "--");

    std::string line;
    FormDataPart current_part;

    // Flag to check if we're currently reading content
    bool readingContent = false;
    while (std::getline(stream, line)) {
        if (line == temp_boundary || line == end_boundary) {
            if (readingContent) {
                parts.push_back(current_part);
                current_part = FormDataPart();
                readingContent = false;
            }
        } else if (!readingContent) {
            // Parse headers
            auto colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                auto header_name = line.substr(0, colon_pos);
                // colos_pos + 2 for : the first one is  ':' and the second one ' '
                auto header_value = line.substr(colon_pos + 2, line.length() - 1);

                if (header_name == ContentDispositionString) {
                    auto name_pos = header_value.find(HeaderNameString);
                    if (name_pos != std::string::npos) {
                        auto name_end = header_value.find("\"", name_pos + HeaderNameString.size());
                        current_part.name = header_value.substr(name_pos + HeaderNameString.size(), name_end - (name_pos + HeaderNameString.size()));
                    }
                    auto file_name_pos = header_value.find(HeaderFileNameString);
                    if (file_name_pos != std::string::npos) {
                        auto file_name_end = header_value.find("\"", file_name_pos + HeaderFileNameString.size());
                        current_part.file_name = header_value.substr(file_name_pos + HeaderFileNameString.size(), file_name_end - (file_name_pos + HeaderFileNameString.size()));
                    }
                } else if (header_name == ContentTypeString) {
                    current_part.content_type = header_value;
                }
            } else if (line.empty() || line == "\r") {
                readingContent = true;
            }
        } else {
            if (line == "")
                current_part.content += "\n";
            else
                current_part.content += line;
        }
    }

    return parts;
}

void BoostHTTPConnection::process_request()
{
    boost::beast::http::request<boost::beast::http::string_body>& req = request_.get();

    size_t body_size = req.body().size();
    std::vector<URIDynamicSection> inputs;
    std::stringstream ss;
    ss << req.body();
    std::string request_str = ss.str();



    auto content_type = req.base()[boost::beast::http::field::content_type];

    auto boundary_data = extract_boundary(content_type.to_string());
    auto parts = parse_multipart_form_data(request_str, boundary_data);
//    for (const auto& part : parts) {
//        std::cout << "Name: " << part.name << std::endl;
//        std::cout << "Filename: " << part.file_name << std::endl;
//        std::cout << "Content-Type: " << part.content_type << std::endl;
//        std::cout << "Content:\n" << part.content << std::endl << std::endl;
//    }

//    std::cout  << request_str << std::endl;
    // Output the sizes
//    std::cout << "Headers size: " << headers_size << " bytes" << std::endl;
//    std::cout << "Body size: " << body_size << " bytes" << std::endl;
//    auto results = split_data(content_type.to_string(), ";");
//    if (results[0] == "multipart/form-data") {
//        auto boundary_data = split_data(results[1], "=");
//        if (boundary_data.size() != 2) {
//            std::cout << "throw an exception for boundary check!!!" << std::endl;
//        } else {
////            auto multiparts_data = split_data(request_str, boundary_data[1]);
////            for (auto file : multiparts_data) {
////                auto names = split_data(file, ";");
////                auto var_name = split_data(names[1], "=");
////                auto file_name = split_data(names[2], "=");

////                std::cout << "var_name: " << names[1] << " filename: " << names[2];
////            }
//        }
//    }




    // Convert the request to a string

    // Get the string representation of the request
    int a = 0;
    // Output the string representation of the request
//    for (const auto &d : data)
//        std::cout << "Request: \n" << data.size() << std::endl;

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
