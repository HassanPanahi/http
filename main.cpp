#include "server.h"
#include "protos/crosspoint.pb.h"
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/dynamic_message.h>
#include "messages_interface.h"
#include "message_validator.h"

unsigned get_info(const std::vector<std::string>& inputs, std::string& putdata, std::string& response)
{
    response = "{ \"name\": \"hassan\" } \n";
    return 200;
}


unsigned put_info(const std::vector<std::string>& inputs, std::string& putdata, std::string& response)
{
    std::cout << "put_data: " << putdata << std::endl;
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

unsigned handle_message(const std::vector<std::string>& inputs, std::shared_ptr<google::protobuf::Message>& msg, std::string& response)
{
    auto new_msg = std::dynamic_pointer_cast<hp::protos::Versions>(msg);
    std::cout << message_to_json(new_msg) << std::endl;
    response = "{ \"name\": \"hassan\" } \n";
    return 200;
}

//TODO(HP): use enum for this
std::shared_ptr<google::protobuf::Message> validator(const std::string_view& msg_name, const std::string& input)
{
    auto new_msg = std::make_shared<hp::protos::Versions>();

    auto status = google::protobuf::util::JsonStringToMessage(input, new_msg.get());
    if (!status.ok()) {
        auto error = "Unable to convert json string to " +  new_msg->GetTypeName();
        const auto &st_error = status.ToString();
        if (!st_error.empty()) {
            error.append(st_error.data());
        }
        std::cout << "error: " << error  << std::endl;
    }
    return new_msg;
}




int main(int argc, char* argv[])
{
    //    hp::protos::Versions version;
    //    version.set_major(1);
    //    version.set_minor(2);
    //    version.set_patch(3);
    //    std::vector<std::string> inputs;
    //    std::string result;
    //    handle_message(inputs, &version, result);

    hp::http::BoostHttpServer server("0.0.0.0", 8585);

    //TODO(HP): /info always use / for begin of uri address... fix this!!!
    //    server.add_path(hp::http::Methods::GET, "/info", std::bind(&get_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    //    server.add_path(hp::http::Methods::PUT, "/info", std::bind(&put_info, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    auto msg_validator = std::make_shared<MessageValidator>();
    server.set_msg_validator(msg_validator);
    server.add_path(hp::http::Methods::PUT, "/infos", 1, std::bind(&handle_message, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    server.start();
}
