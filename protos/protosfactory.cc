#include "protosfactory.h"

//#include "protobuf-msg/umd.pb.h"
//#include "protobuf-msg/public.pb.h"
//#include "protobuf-msg/layout.pb.h"
//#include "protobuf-msg/mip_cards.pb.h"
//#include "protobuf-msg/notification.pb.h"
//#include "protobuf-msg/time.pb.h"
//#include "protobuf-msg/sdi_cards.pb.h"

namespace mip {
namespace types {

ProtosFactory::ProtosFactory()
{

}
template<class Shared_Proto_MSG>
Shared_Proto_MSG mip::types::ProtosFactory::get_mesg(const Protos_Classes name)
{
    switch (name) {
//    case mip::types::Protos_Classes::UMD : {
//        return std::make_shared<types::UMD>();
//        break;
//    }
//    case mip::types::Protos_Classes::NetworkConfigs : {
//        return std::make_shared<types::NetworkConfigs>();
//        break;
//    }
//    case mip::types::Protos_Classes::Layout : {
//        return std::make_shared<types::Layout>();
//        break;
//    }
//    case mip::types::Protos_Classes::CurrentMIPConfig : {
//        return std::make_shared<types::CurrentMIPConfig>();
//        break;
//    }
//    case mip::types::Protos_Classes::RestRequestResult : {
//        return std::make_shared<types::RestRequestResult>();
//        break;
//    }
//    case mip::types::Protos_Classes::MIPStatus : {
//        return std::make_shared<types::MIPStatus>();
//        break;
//    }
//    case mip::types::Protos_Classes::MIPCards : {
//        return std::make_shared<types::MIPCards>();
//        break;
//    }
//    case mip::types::Protos_Classes::GPOClear : {
//        return std::make_shared<types::GPOClear>();
//        break;
//    }
//    case mip::types::Protos_Classes::GPOState : {
//        return std::make_shared<types::GPOState>();
//        break;
//    }
//    case mip::types::Protos_Classes::AllNotifications : {
//        return std::make_shared<types::AllNotifications>();
//        break;
//    }
//    case mip::types::Protos_Classes::TimeSettings : {
//        return std::make_shared<types::TimeSettings>();
//        break;
//    }
//    case mip::types::Protos_Classes::CurrentDateTime : {
//        return std::make_shared<types::CurrentDateTime>();
//        break;
//    }
//    case mip::types::Protos_Classes::SDICard : {
//        return std::make_shared<types::SDIStatus>();
//        break;
//    }
    default: {
        return nullptr;
    }
    }

}

} // namespace types
} // namespace mip
