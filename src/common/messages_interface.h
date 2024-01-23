#ifndef MESSAGESINTERFACE_H
#define MESSAGESINTERFACE_H

#include <google/protobuf/message.h>

namespace hp {
namespace http {

class MessageValidatorInterface
{
public:
    virtual std::shared_ptr<google::protobuf::Message> get_message(const uint32_t msg_id, const std::string& put_data) = 0;
//    virtual ~MessageValidatorInterface();
};

}
} //



#endif // MESSAGESINTERFACE_H
