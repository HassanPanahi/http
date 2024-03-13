#ifndef MESSAGESINTERFACE_H
#define MESSAGESINTERFACE_H

#include <google/protobuf/message.h>
#include <functional>
#include "../rest_path/include/path/path_address.h"
#include "path/path_parser.h"
#include <google/protobuf/message.h>

namespace hp {
namespace http {

using uri = std::string;
using PutFunctionPtr = std::function<unsigned (const std::vector<URIDynamicSection>& request, std::string& response)>;
using ProtobufFunctionPtr = std::function<unsigned (const std::vector<URIDynamicSection>& inputs, std::shared_ptr<google::protobuf::Message>& input_msg, std::string& response)>;

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
    CONNECT,
    Unknown
};


class MessageValidatorInterface
{
public:
    virtual std::shared_ptr<google::protobuf::Message> get_message(const uint32_t msg_id, const std::string& msg_json) = 0;
    //    virtual ~MessageValidatorInterface();
};

class RestServerInterface
{
public:
    virtual void stop() = 0;
    virtual void start() = 0;

    virtual void add_path(const Methods method, const std::string &uri, const PutFunctionPtr& func) final
    {
        auto path_node = path_parser_->parse(uri);
        handler_default_[method][uri] = std::pair<std::shared_ptr<PathAddress>, PutFunctionPtr>(path_node, func);
    }

    virtual void add_path(const Methods method, const std::string &uri, const uint32_t protobuf_msg,  const ProtobufFunctionPtr& func) final
    {
        auto path_node = path_parser_->parse(uri);
        handler_protobuf_default_[method][uri] = std::pair<std::shared_ptr<PathAddress>, ProtobufFunctionPtr>(path_node, func);
        handler_msg_default_[method][uri] = protobuf_msg;
    }

    virtual void set_msg_validator(const std::shared_ptr<MessageValidatorInterface>& msg_validator) final
    {
        msg_validator_ = msg_validator;
    }

    virtual unsigned int analyze_request(const Methods& method, const std::string &path, std::vector<URIDynamicSection>& inputs, std::string& result) final
    {
        auto parser = handler_default_.find(method);
        result = "This uri doesn't support";
        unsigned int ret = 400;
        if (parser != handler_default_.end()) {
            PathParser path_parser;
            auto rest_node = path_parser.parse(path);
            for (const auto &map_node : parser->second) {
                bool is_same = path_parser.is_same_path(map_node.second.first, rest_node, inputs);
                if (is_same) {
                    auto handler = map_node.second.second;
                    ret = handler(inputs, result);
                    break;
                }
            }
        }

        auto proto_parser = handler_protobuf_default_.find(method);
        try {
            if (proto_parser != handler_protobuf_default_.end()) {
                PathParser path_parser;
                auto rest_node = path_parser.parse(path);
                for (const auto &map_node : proto_parser->second) {
                    bool is_same = path_parser.is_same_path(map_node.second.first, rest_node, inputs);
                    if (is_same) {
                        auto msg_id = handler_msg_default_[method][path];
                        if (!msg_validator_)
                            throw std::runtime_error("Null pointer for msg_validator in rest server");
//                        auto msg = msg_validator_->get_message(msg_id, put_data);
//                        auto proto_ptr_func = map_node.second.second;
//                        ret = proto_ptr_func(inputs, msg, result);
                        break;
                    }
                }
            }

        }  catch (std::exception &ec) {
            std::cout << "exception is boost rest: " << ec.what() << std::endl;
        }

        return ret;
    }

protected:
    std::shared_ptr<PathParser> path_parser_;
    std::shared_ptr<MessageValidatorInterface> msg_validator_;
    std::map<Methods, std::map<uri, uint32_t>> handler_msg_default_;
    std::map<Methods, std::map<uri, std::pair<std::shared_ptr<PathAddress>, PutFunctionPtr>> > handler_default_;
    std::map<Methods, std::map<uri, std::pair<std::shared_ptr<PathAddress>, ProtobufFunctionPtr>> > handler_protobuf_default_;
};

}
} //



#endif // MESSAGESINTERFACE_H
