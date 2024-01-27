#include "rest_boost/server.h"

#include <google/protobuf/util/json_util.h>
#include <google/protobuf/dynamic_message.h>
#include <thread>
#include <chrono>


#include <rest_boost/client.h>

unsigned get_info(const std::vector<std::string>& inputs, std::string& request, std::string& response)
{
    static uint32_t counter = 0;
    std::cout << "receive # " << counter++ << " : " << request << std::endl;
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

    hp::http::BoostHttpClient client("0.0.0.0", 8585, 2);
    std::string input_data =  "{ \"panahi\" : 1}";
    std::string result;
    auto ret = client.send_request(hp::http::Methods::GET, "/info", input_data, result);
    std::cout << "result: " << result << std::endl;

//    while(1)
//        std::this_thread::sleep_for(std::chrono::seconds(1));
}
