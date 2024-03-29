#ifndef MESSAGEVALIDATOR_H
#define MESSAGEVALIDATOR_H

#include "../../rest_common/messages_interface.h"

enum MessagesID
{
    Crosspoint = 1

};

class MessageValidator : public hp::http::MessageValidatorInterface
{
public:
    MessageValidator();
    virtual std::shared_ptr<google::protobuf::Message> get_message(const uint32_t msg_id, const std::string &put_data);
//    virtual ~MessageValidator();
private:
    bool validator(std::shared_ptr<google::protobuf::Message>& msg , const std::string& json);
};

#endif // MESSAGEVALIDATOR_H
