#include "path_address.h"

namespace hp {

PathAddress::PathAddress(const std::string& name, const URISectionType mode, const std::vector<URIDynamicSection>& inputs) :
    name_(name), mode_(mode), inputs_(inputs)
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

void PathAddress::set_next(const std::shared_ptr<PathAddress> &next)
{
    next_ = next;
}

URISectionType PathAddress::get_path_mode() const
{
    return mode_;
}

std::vector<URIDynamicSection> PathAddress::get_inputs() const
{
    return inputs_;
}

} // namespace
