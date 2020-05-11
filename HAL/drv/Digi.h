#ifndef _DIGI_H_
#define _DIGI_H_

#include <string>
#include "CommonType.h"
#include "DigiApi.h"
#include "AlarmId.h"

#define RUN_PMC_SDK(fun,...)  \
    { \
        PMC_ERROR ret = 0; \
        ret = fun(dev_id, __VA_ARGS__); \
        if(ret != PMC_SUCCESS) \
        {\
            std::cout << "SDK  Fun: " << #fun << ", ret: " << ret << std::endl; \
            std::cout << "CALL Fun: " << __FUNCTION__ << ",   LINE:" << __LINE__ << std::endl; \
        }\
    }

#define RUN_PMC_SDK_NO_ERROR(fun,...)  \
    { \
        fun(dev_id, __VA_ARGS__); \
    }

int DigiSdkMenu(int argc, char *argv[]);

typedef void (*DigiCallback)(int id, AlarmId::Enum alm, bool active);

namespace DigiApi
{
    const int SFP_PORT_FIRST  = 1;
    const int SFP_PORT_LAST   = 10;
    const int LINE_CFP_PORT   = 11;
    const int CLIENT_CFP_PORT = 12;
    const int PORT_NUM_MIN    = 1;
    const int PORT_NUM_MAX    = 12;
    const int PORT_NUM = 12 + 1; // 1-10 sfp, 11-lineCfp, 12-clientCfp
}

typedef struct
{
    char name[16];
    PortMode::Enum sigType;
    LoopbackType::Enum lpbkType;
    FecType::Enum fecType;
    bool prbsEnable;
    uint32_t chanId;
    uint32_t oduId;
    uint32_t serdesMask;
    digi_oduk_t odukType;
    digi_traffic_mode_t trafficMode;
    digi_mapotn_mode_t mappingType;
} DigiPortConfig;

struct EthAlarm
{
    EthAlarm()
    {
        reset();
    }

    bool valid;
    int los;
    int lossync;
    int lf;
    int rf;
    int highber;

    void reset()
    {
        valid = false;
        los = 0;
        lossync = 0;
        lf = 0;
        rf = 0;
        highber = 0;
    }
};

struct OtuAlarm
{
    OtuAlarm()
    {
        reset();
    }

    bool valid;
    int los;
    int lol;
    int lof;
    int lom;
    int tim;
    int bdi;
    int bersd;
    int ais;

    void reset()
    {
        valid = false;
        los = 0;
        lol = 0;
        lof = 0;
        lom = 0;
        tim = 0;
        bdi = 0;
        bersd = 0;
        ais = 0;
    }
};

struct OduAlarm
{
    OduAlarm()
    {
        reset();
    }

    bool valid;
    int bersd;
    int lck;
    int oci;
    int ais;
    int bdi;
    int tim;
    int plm;
    int loomfi;

    void reset()
    {
        valid = false;
        bersd = 0;
        lck = 0;
        oci = 0;
        ais = 0;
        bdi = 0;
        tim = 0;
        plm = 0;
        loomfi = 0;
    }
};

struct StmAlarm
{
    StmAlarm()
    {
        reset();
    }

    bool valid;
    int los;
    int lof;
    int tim_r;
    int ais_l;
    int rfi_l;

    void reset()
    {
        valid = false;
        los = 0;
        lof = 0;
        tim_r = 0;
        ais_l = 0;
        rfi_l = 0;
    }
};

class Digi
{
public:
    static Digi& GetInst();
    static void DelInst();

    Digi();
    ~Digi() {};

    void InitCold();
    void InitWarm() {}
    void ShutdownCold() {}
    void ShutdownWarm() {}

    void UpdateXc(int port, BOOL enable);
    void SetSignalType(int port, PortMode::Enum sigType);
    PortMode::Enum GetSignalType(int port) { return mPortCfg[port].sigType; }
    void SetLoopbackType(int port, LoopbackType::Enum lpbk);
    LoopbackType::Enum GetLoopbackType(int port) { return mPortCfg[port].lpbkType; };
    void SetFecType(int port, FecType::Enum fecType);
    FecType::Enum GetFecType(int port) { return mPortCfg[port].fecType; };
    void SetOtuPrbsEnable(int port, bool enable);
    void GetOtuPrbsStatus(int port, PrbsStatus::Enum &status);

    void UpdateCounter(int port);
    void ClearCounter(int port);
    void DumpCounter(int port);
    EthCounter *GetEthCnt(int port);
    OtuCounter *GetOtuCnt(int port);

    void UpdateAlarm(int port);
    void ClearAlarm(int port);
    void DumpAlarm(int port);
    EthAlarm *GetEthAlarm(int port);
    OtuAlarm *GetOtuAlarm(int port);
    OduAlarm *GetOduAlarm(int port);
    StmAlarm *GetStmAlarm(int port);
    void RegCallback(DigiCallback cb) { mCb = cb; }
    void SendEvent(int id, AlarmId::Enum alm, bool active)
    {
        if (mCb) mCb(id, alm, active);
    }

    // Debug
    std::string GetPortName(int port);
    void Dump(int port);
    void Menu(int argc, char *argv[]);
    void CheckPortParam(int port);

private:
    void write(uint32_t reg, uint32_t val);
    uint32_t read(uint32_t reg);

private:
    static Digi *mInst;

    uint32_t dev_id;
    DigiPortConfig mPortCfg[DigiApi::PORT_NUM];
    EthCounter mEthCnt[DigiApi::PORT_NUM];
    OtuCounter mOtuCnt[DigiApi::PORT_NUM];
    EthAlarm mEthAlarm[DigiApi::PORT_NUM];
    OtuAlarm mOtuAlarm[DigiApi::PORT_NUM];
    OduAlarm mOduAlarm[DigiApi::PORT_NUM];
    StmAlarm mStmAlarm[DigiApi::PORT_NUM];
    DigiCallback mCb;
};

#endif

#if 0
OTU2(Line) <> ODU2 <> ODUk SW <> ODU2 <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_traffic_mode_cfg 1008 1 8 0 8 1
adpt_cvt_odu_mux_cfg 1008 0 2 4 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 0 2 1 1
adpt_packet_switch 1008 0 1 2 1
adpt_serdes_start_adaptation 1008 0 1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

STM64(Line) <> ODU2(AMP) <> ODUk SW <> ODU2 <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x2000003 0 8 1
adpt_client_map_cfg 1008 1 36 8 1
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_cvt_odu_mux_cfg 1008 0 2 4 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 0 1 2 1
adpt_packet_switch 1008 0 2 1 1
adpt_serdes_start_adaptation 1008 0 0x1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

OC192(Line) <> ODU2(BMP) <> ODUk SW <> ODU2 <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x2000002 0 8 1
adpt_client_map_cfg 1008 1 34 8 1
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_cvt_odu_mux_cfg 1008 0 2 4 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 0 1 2 1
adpt_packet_switch 1008 0 2 1 1
adpt_serdes_start_adaptation 1008 0 0x1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

10GE(Line) <> GFP-F(G.Sup43 6.2) <> ODU2 <> ODUk SW <> ODU2 <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x1000006 0 8 1
adpt_client_map_cfg 1008 1 41 8 1
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_cvt_odu_mux_cfg 1008 0 2 4 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 0 1 2 1
adpt_packet_switch 1008 0 2 1 1
adpt_serdes_start_adaptation 1008 0 0x1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

10GE(Line) <> GFP-F (G.Sup43 7.3) <> ODU2 <> ODUk SW <> ODU2 <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x1000007 0 8 1
adpt_client_map_cfg 1008 1 5 8 1
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_cvt_odu_mux_cfg 1008 0 2 4 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 0 1 2 1
adpt_packet_switch 1008 0 2 1 1
adpt_serdes_start_adaptation 1008 0 0x1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

OC192(Line) <> ODU2(AMP) <> ODUk SW <> ODU2 <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x2000002 0 8 1
adpt_client_map_cfg 1008 1 36 8 1
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_cvt_odu_mux_cfg 1008 0 2 4 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 0 1 2 1
adpt_packet_switch 1008 0 2 1 1
adpt_serdes_start_adaptation 1008 0 0x1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

FC1200 <> CPB <> ODU2e(GFP) <> ODUk SW <> ODU2e <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x10000 0 0 1
adpt_traffic_mode_cfg 1008 0 0x2000006 0 0 1
adpt_client_map_cfg 1008 0 10 8 1
adpt_cvt_odu_mux_cfg 1008 1 2 5 1 1 0xFF 0 0 1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0
adpt_serdes_start_adaptation 1008 0 1 0
adpt_packet_switch 1008 0 0 2 1
adpt_packet_switch 1008 0 2 0 1

10GE <> CPB <> ODU2e(BMP) <> ODUk SW <> ODU2e <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x100000a 0 8 1
adpt_client_map_cfg 1008 1 30  8 1
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_cvt_odu_mux_cfg 1008 0 2 5 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 0 1 2 1
adpt_packet_switch 1008 0 2 1 1
adpt_serdes_start_adaptation 1008 0 0x1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

FC800 <> CPB <> ODU2(GMP) <> ODUk SW <> ODU2 <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x2000007 0 0 1
adpt_client_map_cfg 1008 1 11 7 1
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_cvt_odu_mux_cfg 1008 0 2 4 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 0 1 2 1
adpt_packet_switch 1008 0 2 1 1
adpt_serdes_start_adaptation 1008 0 0x1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

OTU2E(Line) <> ODU2e <> ODUk SW <> ODU2e <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_traffic_mode_cfg 1008 1 6 0 8 1
adpt_cvt_odu_mux_cfg 1008 0 2 5 1 1 0x11111111 0 0 1 0
adpt_packet_switch 1008 2 2 1 1
adpt_packet_switch 1008 2 1 2 1
adpt_serdes_start_adaptation 1008 0 1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

FC800 <> CPB <> ODUflex(BMP) <> ODUk SW <> ODUflex <> ODU4 <> OTU4(SYS)
adpt_traffic_mode_cfg 1008 1 0x2000007 0 0 1
adpt_client_map_cfg 1008 1 26 7 1
adpt_traffic_mode_cfg 1008 0 0x10000 0 80 1
adpt_cvt_odu_mux_cfg 1008 0 2 11 1 1 0x1111111 0 0 1 8535714285
adpt_packet_switch 1008 0 1 2 1
adpt_packet_switch 1008 0 2 1 1
adpt_serdes_start_adaptation 1008 0 0x1 0
adpt_serdes_start_adaptation 1008 1 0x3FF 0

#endif

