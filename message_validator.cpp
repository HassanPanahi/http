#include "message_validator.h"
#include "protos/crosspoint.pb.h"

MessageValidator::MessageValidator()
{

}

bool validator(const std::string_view& msg_name, const std::string& input)
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
        break;
    }
    return msg;
}

//MessageValidator::~MessageValidator()
//{

//}
