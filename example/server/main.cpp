#include "rest_boost/server.h"

#include <google/protobuf/util/json_util.h>
#include <google/protobuf/dynamic_message.h>
#include <thread>
#include <chrono>


#include <rest_boost/client.h>

unsigned get_info(const std::vector<std::string>& inputs, std::string& request, std::string& response)
{
    response = "hassan panahi";
    return 200;
}


unsigned put_info(const std::vector<std::string>& inputs, std::string& input_data, std::string& response)
{
    std::cout << "put_data: " << input_data << std::endl;
    response = "<header>Test </header> \n";
    return 200;
}

template <class Proto_Msg>
static std::string message_to_json(Proto_Msg &proto_message)
{
    std::string res = "";
    google::protobuf::util::MessageToJsonString(*proto_message.get(), &res);
    return res;
}
namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
int main(int argc, char* argv[])
{

    hp::http::BoostHttpClient client("0.0.0.0", 8585, 2);
    std::string result;
    client.send_request(hp::http::Methods::GET, "/info", "", result);
    std::cout << "result: " << result << std::endl;
//    try
//      {
//          auto const host = "0.0.0.0";
//          auto const port = "8585";
//          auto const target = "/info";
//          int version =  11;

//          // The io_context is required for all I/O
//          net::io_context ioc;

//          // These objects perform our I/O
//          tcp::resolver resolver(ioc);
//          beast::tcp_stream stream(ioc);

//          // Look up the domain name
//          auto const results = resolver.resolve(host, port);

//          // Make the connection on the IP address we get from a lookup
//          stream.connect(results);

//          // Set up an HTTP GET request message
//          http::request<http::string_body> req{http::verb::get, target, version};
//          req.set(http::field::host, host);
//          req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

//          // Send the HTTP request to the remote host
//          http::write(stream, req);

//          // This buffer is used for reading and must be persisted
//          beast::flat_buffer buffer;

//          // Declare a container to hold the response
//          http::response<http::dynamic_body> res;

//          // Receive the HTTP response
//          http::read(stream, buffer, res);

//          // Write the message to standard out
//          std::cout << res << std::endl;

//          // Gracefully close the socket
//          beast::error_code ec;
//          stream.socket().shutdown(tcp::socket::shutdown_both, ec);

//          // not_connected happens sometimes
//          // so don't bother reporting it.
//          //
//          if(ec && ec != beast::errc::not_connected)
//              throw beast::system_error{ec};

//          // If we get here then the connection is closed gracefully
//      }
//      catch(std::exception const& e)
//      {
//          std::cerr << "Error: " << e.what() << std::endl;
//          return EXIT_FAILURE;
//      }
//    while(1)
//        std::this_thread::sleep_for(std::chrono::seconds(1));
}
