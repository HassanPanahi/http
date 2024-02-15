#ifndef PATHPARSER_H
#define PATHPARSER_H
#include "path_address.h"

namespace hp {

class PathParser
{
public:
    PathParser();
    std::shared_ptr<PathAddress> parse(const std::string& full_path);
    //Becareful first argument from map variable , and second from rest request.
    bool is_same_path(const std::shared_ptr<PathAddress> &map_node, const std::shared_ptr<PathAddress> &rest_node, std::vector<URIDynamicSection> &inputs);
private:
    void set_new_node(const URISectionType mode, const std::string& name);
    std::shared_ptr<PathAddress> root_node_;
    std::shared_ptr<PathAddress> current_node_;
};

} // namespace

#endif // PATHPARSER_H
