#include "path_parser.h"

#include <iostream>
#include <vector>

#include "path_address.h"

bool isInt(std::string value) try
{
    std::stoi(value);
    return true;
}
catch (...)
{
    return false;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> result;
    std::string data = std::move(str);
    size_t pos = 0;
    std::string token;
    while ((pos = data.find(delimiter)) != std::string::npos) {
        token = data.substr(0, pos);
        result.push_back(token);
        data.erase(0, pos + delimiter.length());
    }
    if (data.size() > 0)
        result.push_back(data);
    return result;
}

namespace hp {

PathParser::PathParser()
{
}

std::shared_ptr<PathAddress> PathParser::parse(const std::string &full_path)
{
    root_node_ = nullptr;
    current_node_ = nullptr;
    std::vector<std::string> paths = split(full_path, "/");
    for (const auto &path : paths) {
        if (path == "") {
            continue;
        } else if (path == "<int>") {
            auto mode = URISectionType::numeric;
            set_new_node(mode, path);
        } else if (path == "<string>") {
            auto mode = URISectionType::string;
            set_new_node(mode, path);
        } else {
            auto mode = URISectionType::attribute;
            set_new_node(mode, path);
        }
    }
    return root_node_;
}

bool PathParser::is_same_path(const std::shared_ptr<PathAddress>& map_node, const std::shared_ptr<PathAddress>& rest_node, std::vector<URIDynamicSection>& inputs)
{
    if (map_node == nullptr && rest_node == nullptr)
        return true;
    else if ((map_node == nullptr) != (rest_node == nullptr))
        return false;

    auto map = map_node;
    auto rest = rest_node;
    auto map_mode = map->get_path_mode();
    switch (map_mode) {
    case URISectionType::numeric: {
        std::string key = rest->get_name();
        if (!isInt(key))
            return false;
        URIDynamicSection uri_section;
        uri_section.type = URISectionType::numeric;
        uri_section.key = key;
        uri_section.value = "";
        inputs.push_back(uri_section);
        break;
    }
    case URISectionType::string: {
        std::string key = rest->get_name();
        if (isInt(key))
            return false;
        URIDynamicSection uri_section;
        uri_section.type = URISectionType::string;
        uri_section.key = key;
        uri_section.value = "";
        inputs.push_back(uri_section);
        break;
    }
    case URISectionType::attribute: {
        std::string section = rest_node->get_name();
        std::vector<std::string> parameters = split(section, "?");
        for (auto const &param : parameters) {
            std::vector<std::string> variables = split(param, "&");
            for (auto const& var : variables) {
                std::vector<std::string> key_value = split(var, "=");
                if (key_value.size() == 2){
                    URIDynamicSection uri_section;
                    uri_section.type = URISectionType::parameter;
                    uri_section.key = key_value[0];
                    uri_section.value = key_value[1];
                    inputs.push_back(uri_section);
                }
            }
        }
        std::string key1 = parameters[0];
        std::string key2 = map->get_name();
        if (key1 != key2)
            return false;
        break;
    }
    }
    return is_same_path(map->get_next(), rest->get_next(), inputs);
}

void PathParser::set_new_node(const URISectionType mode, const std::string &name)
{
    std::vector<URIDynamicSection> inputs;
    auto new_node = std::make_shared<PathAddress>(name, mode, inputs);

    if (root_node_ == nullptr) {
        root_node_ = new_node;
        current_node_ = root_node_;
    }
    else {
        current_node_->set_next(new_node);
        current_node_ = current_node_->get_next();
    }
}

} // namespace
