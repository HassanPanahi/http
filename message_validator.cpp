#include "message_validator.h"
#include "protos/crosspoint.pb.h"

MessageValidator::MessageValidator()
{

}

std::shared_ptr<google::protobuf::Message> MessageValidator::get_message(const uint32_t msg_id, const std::string &put_data)
{
    std::shared_ptr<google::protobuf::Message> msg;
    auto msg_name = static_cast<MessagesID>(msg_id);
    switch (msg_id) {
    case MessagesID::Crosspoint:
    {
        msg = std::make_shared<hp::protos::Versions>();
        break;
    }
    default:
        break;
    }
    return msg;
}

//MessageValidator::~MessageValidator()
//{

//}
