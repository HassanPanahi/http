#ifndef PATHADDRESS_H
#define PATHADDRESS_H

#include <string>
#include <vector>
#include <memory>

namespace hp {

// numeric = just index of key, for example /streamer/<idx>/output, "<idx>" is a value. idx must be a int  //TODO(HP): <idx> can be a double
// STRING = /setting/<string>/1 : <string> can be enything like streamer or encoder
// ATTRIBUTE = /setting/ecoder/1/output/1   setting, encoder, output all of them are Attribute

enum class URISectionType{
    numeric = 0,
    string = 1,
    attribute = 2,
    parameter = 3,
    anchor
};

struct URIDynamicSection{
    URISectionType type;
    std::string key;
    std::string value;
};


class PathAddress
{
public:
    PathAddress(const std::string& name, const URISectionType mode, const std::vector<URIDynamicSection>& inputs);

    std::string get_name() const;
    URISectionType get_path_mode() const;
    std::shared_ptr<PathAddress> get_next() const;
    std::vector<URIDynamicSection> get_inputs() const;
    void set_next(const std::shared_ptr<PathAddress>& next);

private:
    std::string name_;
    URISectionType mode_;
    std::vector<URIDynamicSection> inputs_;

    std::shared_ptr<PathAddress> next_;
};

} // namespace

#endif // PATHADDRESS_H
