#include "rest_boost/server.h"

#include <thread>
#include <chrono>

#include <boost/beast/core/bind_handler.hpp>

#include "rest_boost/client.h"

#define SERVER_IP   "0.0.0.0"
#define SERVER_PORT (8585)

unsigned get_info(const std::vector<hp::URIDynamicSection>& inputs, std::string& response)
{
    for (auto const& param : inputs)
        std::cout << param.key << "=" << param.value << std::endl;

    static uint32_t counter = 0;
//    std::cout << "Server receive: "  << request << std::endl;
    response = " Answer  = Yes ";
    return 200;
}

unsigned put_info(const std::vector<hp::URIDynamicSection>& inputs, std::string& response)
{
//    std::cout << "Server receive: "  << request << std::endl;
    response = " Answer  = Yes ";
    return 200;
}

int main(int argc, char* argv[])
{
    hp::http::BoostRestServer server(SERVER_IP, SERVER_PORT);
    server.add_path(hp::http::Methods::POST, "/info", std::bind(&put_info, std::placeholders::_1, std::placeholders::_2));
    server.start();
    while(1) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
