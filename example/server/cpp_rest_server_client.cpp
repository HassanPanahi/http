#include "rest/server.h"

#include <thread>
#include <chrono>

#define SERVER_IP   "0.0.0.0"
#define SERVER_PORT (8585)

unsigned get_info(const std::vector<hp::URIDynamicSection>& inputs, std::string& request, std::string& response)
{
    for (auto const& param : inputs)
        std::cout << param.key << "=" << param.value << std::endl;

    static uint32_t counter = 0;
    std::cout << "Server receive: "  << request << std::endl;
    response = " Answer  = Yes ";
    return 200;
}

unsigned put_info(const std::vector<hp::URIDynamicSection>& inputs, std::string& request, std::string& response)
{
    std::cout << "Server receive: "  << request << std::endl;
    response = " Answer  = Yes ";
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 200;
}

int main(int argc, char* argv[])
{
    hp::http::CppRestServer server(SERVER_IP, SERVER_PORT);
//    server.add_path(hp::http::Methods::GET, "/info/<string>/<int>/data", std::bind(&get_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//    server.add_path(hp::http::Methods::GET, "/info", std::bind(&put_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    server.start();

    while(1)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

//    hp::http::BoostHttpClient client(SERVER_IP, SERVER_PORT);
//    std::string result = "";
//    std::string input_data =  "Question : Are you still there?";

//    auto ret = client.send_request(hp::http::Methods::GET, "/info/everything/3/data?name=hassan&family=panahi", input_data, result, 2000); // blocking until 2000 ms
//    std::cout << "get error code: " << ret << " and response: " << result << std::endl;

//    ret = client.send_request(hp::http::Methods::GET, "/info/allstrings/5/data?name=hassan&family=panahi", input_data, result, 2000); // blocking until 2000 ms
//    std::cout << "get error code: " << ret << " and response: " << result << std::endl;

//    ret = client.send_request(hp::http::Methods::GET, "/info/anything/data", input_data, result); // blocking
//    std::cout << "get error code: " << ret << " and response: " << result << std::endl;

//    ret = client.send_request(hp::http::Methods::PUT, "/info", input_data, result); // blocking
//    std::cout << "put error code: " << ret << " and response: " << result << std::endl;
}
