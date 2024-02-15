#ifndef MESSAGESINTERFACE_H
#define MESSAGESINTERFACE_H

#include <google/protobuf/message.h>
#include <functional>

namespace hp {
namespace http {

using uri = std::string;
using PutFunctionPtr = std::function<unsigned (const std::vector<URIDynamicSection>&, std::string& put_data, std::string& response)>;
using ProtobufFunctionPtr = std::function<unsigned (const std::vector<URIDynamicSection>&, std::shared_ptr<google::protobuf::Message>& input_msg, std::string& response)>;


enum class Methods {
    GET,
    PUT,
    DEL,
    POST,
    HEAD,
    TRCE,
    PATCH,
    MERGE,
    OPTIONS,
    CONNECT
};


class MessageValidatorInterface
{
public:
    virtual std::shared_ptr<google::protobuf::Message> get_message(const uint32_t msg_id, const std::string& put_data) = 0;
    //    virtual ~MessageValidatorInterface();
};

class RestServerInterface
{
public:
    virtual void stop() = 0;
    virtual void start() = 0;
    virtual void add_path(const Methods method, const std::string &uri, const PutFunctionPtr& func) = 0;
    virtual void set_msg_validator(const std::shared_ptr<MessageValidatorInterface>& msg_validator) = 0;
    virtual void add_path(const Methods method, const std::string &uri, const uint32_t msg_id,  const ProtobufFunctionPtr& func) = 0;
};

}
} //



#endif // MESSAGESINTERFACE_H
