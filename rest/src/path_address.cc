#include "path_address.h"

namespace rest {

PathAddress::PathAddress(std::string name, PathMode mode, std::vector<std::string> inputs) : inputs_(inputs), mode_(mode), name_(name)
{
}

std::string PathAddress::get_name() const
{
    return name_;
}

std::shared_ptr<PathAddress> PathAddress::get_next() const
{
    return next_;
}

void PathAddress::set_next(const std::shared_ptr<PathAddress> next)
{
    next_ = next;
}

PathMode PathAddress::get_path_mode() const
{
    return mode_;
}

std::vector<std::string> PathAddress::get_inputs() const
{
    return inputs_;
}

} // namespace rest
