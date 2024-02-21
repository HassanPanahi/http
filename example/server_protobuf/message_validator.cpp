#include "message_validator.h"
#include "protos/crosspoint.pb.h"
#include <google/protobuf/util/json_util.h>

MessageValidator::MessageValidator()
{

}

bool MessageValidator::validator(std::shared_ptr<google::protobuf::Message>& msg , const std::string& json)
{
    bool ret = true;
    auto status = google::protobuf::util::JsonStringToMessage(json, msg.get());
    if (!status.ok()) {
        ret = false;
        auto error = "Unable to convert json string to " +  msg->GetTypeName();
        const auto &st_error = status.ToString();
        if (!st_error.empty())
            error.append(st_error.data());
        std::cout << "error: " << error  << std::endl;
    }
    return ret;
}

std::shared_ptr<google::protobuf::Message> MessageValidator::get_message(const uint32_t msg_id, const std::string &put_data)
{
    std::shared_ptr<google::protobuf::Message> msg;
    auto msg_name = static_cast<MessagesID>(msg_id);
    switch (msg_id) {
    case MessagesID::Crosspoint:
        msg = std::make_shared<hp::protos::Versions>();
        break;
    default:
        std::cout << "Wrong msg id for validator" << std::endl;
        throw "This id for protobuf doesn't handle;";
        break;
    }
    validator(msg, put_data);
    return msg;
}

//MessageValidator::~MessageValidator()
//{

//}
