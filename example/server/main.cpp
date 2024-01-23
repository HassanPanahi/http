#include "rest_boost/server.h"
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/dynamic_message.h>

unsigned get_info(const std::vector<std::string>& inputs, std::string& request, std::string& response)
{
    response = "{ \"name\": \"hassan\" } \n";
    return 200;
}


unsigned put_info(const std::vector<std::string>& inputs, std::string& input_data, std::string& response)
{
    std::cout << "put_data: " << input_data << std::endl;
    response = "{ \"name\": \"hassan\" } \n";
    return 200;
}

template <class Proto_Msg>
static std::string message_to_json(Proto_Msg &proto_message)
{
    std::string res = "";
    google::protobuf::util::MessageToJsonString(*proto_message.get(), &res);
    return res;
}

int main(int argc, char* argv[])
{
    hp::http::BoostRestServer server("0.0.0.0", 8585);

    //TODO(HP): /info always use / for begin of uri address... fix this!!!
    server.add_path(hp::http::Methods::GET, "/info", std::bind(&get_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    server.add_path(hp::http::Methods::PUT, "/info", std::bind(&put_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    server.start();
}
