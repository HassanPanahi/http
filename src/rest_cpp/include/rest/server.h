#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <functional>

#include <cpprest/http_listener.h>

#include "path/path_parser.h"
#include "../../rest_common/messages_interface.h"

namespace hp {
namespace http {

class CppRestServer : public RestServerInterface
{
public:
    CppRestServer(const std::string& ip, unsigned short port, const std::shared_ptr<PathParser>& path_parser = nullptr);

    void stop();
    void start();
    bool is_running() const;
    std::string get_ip() const;
    unsigned short get_port() const;
    ~CppRestServer();
private:
    bool is_running_;
    std::string ip_;
    unsigned short port_;
    std::string main_url_;

    std::map<web::http::method, Methods> methods_list_;

    std::vector<std::vector<std::string>> paths_;

    web::http::experimental::listener::http_listener http_listener_;
    void handle_request(web::http::http_request message);
};

}
}

#endif // SERVER_H
