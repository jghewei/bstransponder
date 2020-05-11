#ifndef _COMMON_TYPE_
#define _COMMON_TYPE_

namespace PortMode
{
    enum Enum
    {
        NONE,
        ETH10G,
        ETH100G,
        STM64,
        OC192,
        OTU2,
        OTU2E,
        OTU4,
        FC8G,
        FC10G,
        ETH10G_GFP,
    };
}
#define PortMode_to_String(x) \
        ((x) == PortMode::NONE        ? "NONE"    : \
         (x) == PortMode::ETH10G      ? "ETH10G"  : \
         (x) == PortMode::ETH100G     ? "ETH100G" : \
         (x) == PortMode::STM64       ? "STM64"   : \
         (x) == PortMode::OC192       ? "OC192"   : \
         (x) == PortMode::OTU2        ? "OTU2"    : \
         (x) == PortMode::OTU2E       ? "OTU2E"   : \
         (x) == PortMode::OTU4        ? "OTU4"    : \
         (x) == PortMode::FC8G        ? "FC8G"    : \
         (x) == PortMode::FC10G       ? "FC10G"   : \
         (x) == PortMode::ETH10G_GFP  ? "ETH10G_GFP"   : "?" )

namespace LoopbackType
{
    enum Enum
    {
        Disabled,
        Facility,
        Terminal,
    };
}
#define LoopbackType_to_String(x) \
        ((x) == LoopbackType::Disabled     ? "Disabled": \
         (x) == LoopbackType::Facility     ? "Facility": \
         (x) == LoopbackType::Terminal     ? "Terminal": "?" )

namespace FecType
{
    enum Enum
    {
        NONE,
		GFEC,
        I4,
        I7,
        SDFEC_ENHANCE,
        SDFEC_COMP,
        TRANSPARENT,
    };
}
#define FecType_to_String(x) \
        ((x) == FecType::NONE   ? "NONE"            : \
		 (x) == FecType::GFEC   ? "GFEC"            : \
         (x) == FecType::I4     ? "I4"              : \
         (x) == FecType::I4     ? "I7"              : \
         (x) == FecType::I4     ? "SDFEC_ENHANCE"   : \
         (x) == FecType::I4     ? "SDFEC_COMP"      : \
         (x) == FecType::I7     ? "TRANSPARENT"     : "?" )

namespace TimDefectMode
{
    enum Enum
    {
        NONE,
		SAPI,
        DAPI,
        OPER,
        SAPI_DAPI,
        SAPI_OPER,
        DAPI_OPER,
        SAPI_DAPI_OPER,        
    };
}
#define TimDefectMode_TO_STRING(x) \
        ((x) == TimDefectMode::NONE             ? "NONE"            : \
		 (x) == TimDefectMode::SAPI             ? "SAPI"            : \
		 (x) == TimDefectMode::DAPI             ? "DAPI"            : \
		 (x) == TimDefectMode::OPER             ? "OPER"            : \
		 (x) == TimDefectMode::SAPI_DAPI        ? "SAPI_DAPI"       : \
		 (x) == TimDefectMode::SAPI_OPER        ? "SAPI_OPER"       : \
		 (x) == TimDefectMode::DAPI_OPER        ? "DAPI_OPER"       : \
         (x) == TimDefectMode::SAPI_DAPI_OPER   ? "SAPI_DAPI_OPER"  : "?" )

namespace PrbsStatus
{
    enum Enum
    {
        Disabled,
        InSync,
        OutSync,
        ErrorSync,
    };
}
#define PrbsStatus_To_String(x) \
        ((x) == PrbsStatus::Disabled            ? "Disabled"   : \
		 (x) == PrbsStatus::InSync              ? "InSync"     : \
		 (x) == PrbsStatus::OutSync             ? "OutSync"    : \
         (x) == PrbsStatus::ErrorSync           ? "ErrorSync"  : "?" )

struct EthCounter
{
    bool valid;
    //uint64_t etherSequenceErrCnt;
    //uint64_t etherStatsDropEvents;
    uint64_t etherStatsOctets;
    uint64_t etherStatsPkts;
    uint64_t etherStatsBroadcastPkts;
    uint64_t etherStatsMulticastPkts;
    uint64_t etherStatsCRCAlignErrors;
    uint64_t etherStatsUndersizePkts;
    uint64_t etherStatsOversizePkt;
    uint64_t etherStatsFragments;
    uint64_t etherStatsJabbers;
    uint64_t etherStatsPkts64Octets;
    uint64_t etherStatsPkts65to127Octets;
    uint64_t etherStatsPkts128to255Octets;
    uint64_t etherStatsPkts256to511Octets;
    uint64_t etherStatsPkts512to1023Octets;
    uint64_t etherStatsPkts1024to1518Octets;
    uint64_t etherStatsPkts1519toMaxOctets;
    //uint64_t etherStatsDroppedFrames;
    uint64_t ifInUcastPkts;
    //uint64_t ifInDiscards;
    //uint64_t ifInMulticastPkts;
    //uint64_t ifInBroadcastPkts;
    uint64_t ifInOctets;
    //uint64_t ifInErrors;
    uint64_t ifOutMulticastPkts;
    uint64_t ifOutBroadcastPkts;
    uint64_t ifOutOctets;
    uint64_t ifOutUcastPkts;
    //uint64_t ifOutDiscards;
    //uint64_t ifOutErrors;
    //uint64_t ifInSymbolErrors;
    uint64_t aTxPauseMacCtrlFrames;
    uint64_t aRxPauseMacCtrlFrames;
};

struct OtuCounter
{
    bool valid;
    uint64_t correctableBytes;
    uint64_t correctableOnes;
    uint64_t correctableZeros;
    uint64_t uncorrectables;
    uint64_t correctBits;
    uint64_t frameCount;
    uint64_t bip8;
    uint64_t bei;
};

struct OchCounter
{
    bool valid;
    uint64_t uncorrectables;
    uint64_t correctBits;
    uint64_t bip8;
    uint64_t bei;
};

#endif  // _COMMON_TYPE_

