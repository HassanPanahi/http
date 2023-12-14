#include "server.h"

unsigned get_info(const std::vector<std::string>& inputs, std::string& putdata, std::string& response)
{
    response = "{ \"name\": \"hassan\" } \n";
    return 200;
}

int main(int argc, char* argv[])
{
    hp::http::BoostHttpServer server("0.0.0.0", 8585);
    //TODO(HP): /info always use / for begin of uri address... fix this!!!
    server.add_path(hp::http::RestMethods::GET, "/info", std::bind(&get_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    server.start();
}
