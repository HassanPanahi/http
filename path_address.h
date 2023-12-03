#ifndef PATHADDRESS_H
#define PATHADDRESS_H

#include <string>
#include <vector>
#include <memory>

namespace rest {

// NUMERIC = just index of key, for example /streamer/<idx>/output, "<idx>" is a value. idx must be a int  //TODO(HP): <idx> can be a double
// STRING = /setting/<string>/1 : <string> can be enything like streamer or encoder
// ATTRIBUTE = /setting/ecoder/1/output/1   setting, encoder, output all of them are Attribute

enum class PathMode{
    NUMERIC = 0,
    STRING = 1,
    ATTRIBUTE = 2
};

class PathAddress
{
public:
    PathAddress(std::string name, PathMode mode, std::vector<std::string> inputs);
    void set_next(const std::shared_ptr<PathAddress> next);
    std::shared_ptr<PathAddress> get_next() const;
    std::vector<std::string> get_inputs() const;
    PathMode get_path_mode() const;
    std::string get_name() const;

private:
    std::vector<std::string> inputs_;
    PathMode mode_;
    std::string name_;
    std::shared_ptr<PathAddress> next_;

};

} //rest
#endif // PATHADDRESS_H
