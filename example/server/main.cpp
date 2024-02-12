#include "rest_boost/server.h"

#include <google/protobuf/util/json_util.h>
#include <google/protobuf/dynamic_message.h>
#include <thread>
#include <chrono>


#include <rest_boost/client.h>


//class BoostHttpClient {
//public:
//    BoostHttpClient(asio::io_context& io_context)
//        : io_context_(io_context), stream_(io_context) {}

//    void async_connect_with_retry(const tcp::resolver::results_type& results, int retry_count) {
//        async_connect_with_retry(results.begin(), retry_count);
//    }

//private:
//    asio::io_context& io_context_;
//    tcp::socket stream_;

//    void async_connect_with_retry(tcp::resolver::results_type::iterator endpoint_iter, int retry_count) {
//        if (endpoint_iter == tcp::resolver::results_type::iterator()) {
//            std::cerr << "All endpoints failed, giving up." << std::endl;
//            return;
//        }

//        // Set a timeout for the connection attempt
//        stream_.expires_after(std::chrono::seconds(5)); // Set timeout to 5 seconds

//        // Initiate the asynchronous connection
//        stream_.async_connect(*endpoint_iter, [this, endpoint_iter, retry_count](const boost::system::error_code& ec) {
//            if (!ec) {
//                // Connection succeeded
//                std::cout << "Connected successfully!" << std::endl;
//            } else {
//                // Connection failed or timed out
//                std::cerr << "Connection error: " << ec.message() << std::endl;
//                if (retry_count > 0) {
//                    // Retry connecting to the next endpoint
//                    async_connect_with_retry(++endpoint_iter, retry_count - 1);
//                }
//            }
//        });
//    }
//};

//int main() {
//    asio::io_context io_context;
//    BoostHttpClient client(io_context);

//    // Resolve the endpoint
//    tcp::resolver resolver(io_context);
//    auto results = resolver.resolve("example.com", "http");

//    // Set the number of retries
//    int retry_count = 3;

//    // Attempt to connect with retries
//    client.async_connect_with_retry(results, retry_count);

//    // Run the IO context to start the asynchronous operations
//    io_context.run();

//    return 0;
//}

unsigned get_info(const std::vector<std::string>& inputs, std::string& request, std::string& response)
{
    static uint32_t counter = 0;
    std::cout << "receive # " << counter++ << " : " << request << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    response = "{hassan panahi}";
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

//    hp::http::BoostRestServer server("0.0.0.0", 8585);
//    server.add_path(hp::http::Methods::GET, "/info", std::bind(&get_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//    server.add_path(hp::http::Methods::PUT, "/info", std::bind(&put_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//    server.start();

    hp::http::BoostHttpClient client("0.0.0.0", 8585);
    std::string input_data =  "{ \"panahi\" : 1}";
    std::string result;
    auto ret = client.send_request(hp::http::Methods::GET, "/info", input_data, result, 5000);
    std::cout << "result: " << result << std::endl;



    while(1)
        std::this_thread::sleep_for(std::chrono::seconds(1));
}
