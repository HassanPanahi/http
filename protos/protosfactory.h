#ifndef PROTOSFACTORY_H
#define PROTOSFACTORY_H
//#include "protobuf-msg/public.pb.h"

namespace mip {
namespace types {

enum class Protos_Classes{
    CurrentDateTime,
    TimeSettings,
    MIPStatus,
    RestRequestResult,
    Layout,
    MIPCards,
    TimeManual,
    NetworkConfigs,
    VideoError,
    EmbeddedAudioStatus,
    VideoNotifications,
    AudioNotifications,
    AllNotifications,
    SDICard,
    Tally,
    UMD,
    VideoFrame,
    CurrentMIPConfig,
    GPOState,
    GPOClear,
};



class ProtosFactory
{
public:
    ProtosFactory();
    template<class Shared_Proto_MSG>
    Shared_Proto_MSG get_mesg(Protos_Classes name);
};

}
}

#endif // PROTOSFACTORY_H
