#ifndef MESSAGEVALIDATOR_H
#define MESSAGEVALIDATOR_H

#include "messages_interface.h"

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
};

#endif // MESSAGEVALIDATOR_H
