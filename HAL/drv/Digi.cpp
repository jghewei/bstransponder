#include "DanOS.h"
#include "MemMap.h"
#include "Digi.h"
#include "M21036.h"
#include "CThread.h"
#include "MsaCfp.h"

using namespace DigiApi;

#define DIGI_BASE       0x80000000
#define DIGI_SIZE       0x02000000

MemAddr digiBase = NULL;
Digi* Digi::mInst = NULL;
static CMutex mDigiLock;

extern int isTranspondMode;

#define DIGI_ALARM_UPDATE(alarm, event) \
    if (defect != ((alarm & 0x2) ? TRUE : FALSE)) \
    { \
        SendEvent(port, event, defect == TRUE); \
    } \
    (defect == TRUE) ? alarm |= 0x3 : alarm &= ~0x2;


uint32_t reg_swap(uint32_t data)
{
    uint32_t val = 0;
    val |= ((data >> 24) & 0xff);
    val |= ((data >> 8) & 0xff00);
    val |= ((data << 8) & 0xff0000);
    val |= ((data << 24) & 0xff000000);
    return val;
}

void GetTribSlot(int port, int signal_type, uint32_t &ts1, uint32_t &ts2, uint32_t &ts3)
{
    ts1 = 0; ts2 = 0; ts3 = 0;

    if (port >= 1 && port <= 4)
    {
        ts1 = 0xFF << ((port-1) * 8);
    }
    else if (port >= 5 && port <= 8)
    {
        ts2 = 0xFF << ((port-5) * 8);
    }
    else if (port >= 9 && port <= 10)
    {
        ts3 = 0xFF << ((port-9) * 8);
    }
}

Digi::Digi()
{
    digiBase = MemMap(DIGI_BASE, DIGI_SIZE);
    
    if (isTranspondMode)
    {
        APP_LOG("Digi Enter Transponder Mode");
        dev_id = HW_CARD_100G_TRANSPONDER_TO_OTU4;
    }
    else
    {
        APP_LOG("Digi Enter Muxponder Mode");
        dev_id = HW_CARD_10x10G_ANY_MUXPONDER_TO_OTU4;
    }

    for (int i = PORT_NUM_MIN; i <= PORT_NUM_MAX; i++)
    {
        snprintf(mPortCfg[i].name, 15, "Sfp %d", i);
        mPortCfg[i].sigType = PortMode::NONE;
        mPortCfg[i].lpbkType = LoopbackType::Disabled;
        mPortCfg[i].fecType = FecType::NONE;
        mPortCfg[i].prbsEnable = false;

        if (i >= SFP_PORT_FIRST && i <= SFP_PORT_LAST)
        {
            mPortCfg[i].chanId = (i-1) << 24 | i;
            mPortCfg[i].oduId = 10 + i;
            mPortCfg[i].serdesMask = 1 << (i - 1);
        }
        else if (i == LINE_CFP_PORT)
        {
            mPortCfg[i].chanId = 0;
            mPortCfg[i].oduId = 0;
            mPortCfg[i].serdesMask = 0x3ff;
        }
        else if (i == CLIENT_CFP_PORT)
        {
            mPortCfg[i].chanId = 1;
            mPortCfg[i].oduId = 1;
            mPortCfg[i].serdesMask = 0x3ff;
        }
    }

    mCb = NULL;
}

Digi& Digi::GetInst()
{
    if (mInst == NULL)
    {
        mInst = new Digi();
    }
    return *mInst;
}

void Digi::DelInst()
{
    if (mInst != NULL)
    {
        delete mInst;
        mInst = NULL;
    }
}

void* DigiPeriodAction(void *param)
{
    int first = isTranspondMode ? 11 : 1;
    int end = isTranspondMode ? 12 : 11;
    
    sleep(1);
    
    CMutexLock lock(mDigiLock);
    for (int port = first; port <= end; port++)
    {
        Digi::GetInst().UpdateAlarm(port);
    }
    
    return NULL;
}

void Digi::InitCold()
{
    APP_LOG("DIGI DeviceID: 0x%08x", read(0x1800180));
    adpt_log_init(PMC_LOG_SEV_MEDIUM);
    RUN_PMC_SDK(adpt_init, (UINT32)digiBase, DIGI_INIT_COLD);
    //pmc_log_verbosity_level_set(PMC_LOG_SEV_MEDIUM);

    new CThread("digi", DigiPeriodAction, NULL);
}

void Digi::UpdateXc(int port, BOOL enable)
{
    if (isTranspondMode)
    {
        digi_switch_mode_t swMode = DIGI_SWITCH_ODUK_CPB;
        if (mPortCfg[11].sigType == PortMode::OTU4 &&
           (mPortCfg[12].sigType == PortMode::OTU4 || mPortCfg[12].sigType == PortMode::ETH100G))
        {
            if (mPortCfg[12].sigType == PortMode::OTU4)
            {
                swMode = DIGI_SWITCH_ODUK;
            }
            RUN_PMC_SDK(adpt_packet_switch, swMode, 0, 1, enable);
            RUN_PMC_SDK(adpt_packet_switch, swMode, 1, 0, enable);
        }
    }
    else
    {
        int start, end;
        if (port == 11)
        {
            start = 1;
            end = 10;
        }
        else if (port >= 1 && port <= 10)
        {
            start = port;
            end = port;
        }
        else
        {
            return;
        }

        for (int i = start; i <= end; i++)
        {
            if (mPortCfg[i].sigType != PortMode::NONE && mPortCfg[11].sigType != PortMode::NONE)
            {
                uint32_t chanId = mPortCfg[i].chanId;
                uint32_t oduId = mPortCfg[i].oduId;
                digi_oduk_t oduk_type = mPortCfg[i].odukType;
                digi_odu_line_payload_t payload_type = DIGI_ODU_TS_1G25;
                digi_otn_mapping_type_t mapping_type = DIGI_OTN_MAP_GMP;
                DOUBLE oduflex_rate = 0;
                BOOL tp_enable = enable;
                UINT32 ts_mask0, ts_mask1, ts_mask2;
                GetTribSlot(i, mPortCfg[i].sigType, ts_mask0, ts_mask1, ts_mask2);

                if (enable == TRUE)
                {
                    RUN_PMC_SDK(adpt_cvt_odu_mux_cfg, 0, oduId, oduk_type, mapping_type, payload_type, ts_mask0, ts_mask1, ts_mask2, tp_enable, oduflex_rate);
                    RUN_PMC_SDK(adpt_packet_switch, DIGI_SWITCH_ODUK_CPB, chanId, oduId, enable);
                    RUN_PMC_SDK(adpt_packet_switch, DIGI_SWITCH_ODUK_CPB, oduId, chanId, enable);
                }
                else
                {
                    RUN_PMC_SDK(adpt_packet_switch, DIGI_SWITCH_ODUK_CPB, chanId, oduId, enable);
                    RUN_PMC_SDK(adpt_packet_switch, DIGI_SWITCH_ODUK_CPB, oduId, chanId, enable);
                    RUN_PMC_SDK(adpt_cvt_odu_mux_cfg, 0, oduId, oduk_type, mapping_type, payload_type, ts_mask0, ts_mask1, ts_mask2, tp_enable, oduflex_rate);
                }
            }
        }
    }
}

void Digi::SetSignalType(int port, PortMode::Enum sigType)
{
    if (port < PORT_NUM_MIN || port > PORT_NUM_MAX)
    {
        APP_LOG("invalid port id %d", port);
        return;
    }

    CMutexLock lock(mDigiLock);

    if (mPortCfg[port].sigType == sigType)
    {
        return;
    }

    APP_LOG("%s %s -> %s\n", __FUNCTION__, PortMode_to_String(mPortCfg[port].sigType),
                                           PortMode_to_String(sigType));

    uint32_t chanId = mPortCfg[port].chanId;
    //uint32_t oduId = mPortCfg[port].oduId;
    uint32_t serdesMask = mPortCfg[port].serdesMask;

    if (sigType == PortMode::NONE)
    {
        UpdateXc(port, FALSE);

        if (mPortCfg[port].trafficMode != LAST_DIGI_TRAFFIC)
        {
        #if 1
            if (mPortCfg[port].mappingType != LAST_DIGI_MAPOTN)
            {
                UINT32 ts_number = 8;
                if (mPortCfg[port].sigType == PortMode::ETH100G)
                {
                    ts_number = 0;
                }
                RUN_PMC_SDK(adpt_client_map_cfg, chanId, mPortCfg[port].mappingType, ts_number, FALSE);
                mPortCfg[port].mappingType = LAST_DIGI_MAPOTN;
            }
        #else
            {
                digi_mapotn_mode_t mapping_modeGet;
                UINT32 ts_numberGet;
                BOOL enableGet = FALSE;
                RUN_PMC_SDK(adpt_client_map_get, chanId, &mapping_modeGet, &ts_numberGet, &enableGet);
                if(enableGet == TRUE)
                {
                    RUN_PMC_SDK(adpt_client_map_cfg, chanId, mapping_modeGet, ts_numberGet, FALSE);
                }
            }
        #endif

            UINT32 ilkn_info = 0;
            RUN_PMC_SDK(adpt_traffic_mode_cfg, chanId, mPortCfg[port].trafficMode, DIGI_OTN_SERVER_FEC_NONE, ilkn_info, FALSE);
            mPortCfg[port].trafficMode = LAST_DIGI_TRAFFIC;
        }

        mPortCfg[port].sigType = PortMode::NONE;
        return;
    }

    if (port == 11 && sigType == PortMode::OTU4)
    {
        RUN_PMC_SDK(adpt_traffic_mode_cfg, chanId, DIGI_SYS_OTU4, DIGI_OTN_SERVER_FEC_NONE, 80, TRUE);
        RUN_PMC_SDK(adpt_serdes_tx_refclk_div_cfg, chanId, serdesMask, DIGI_TX_CLK_DIV_64);
        RUN_PMC_SDK_NO_ERROR(adpt_serdes_start_adaptation, HOSTMSG_ADAPT_INTF_S16_SYS, serdesMask, 0);
        mPortCfg[port].sigType = sigType;
        UpdateXc(port, TRUE);
        mPortCfg[port].trafficMode = DIGI_SYS_OTU4;
        mPortCfg[port].mappingType = LAST_DIGI_MAPOTN;
    }
    else if (port == 12 && (sigType == PortMode::OTU4 || sigType == PortMode::ETH100G))
    {
        if (sigType == PortMode::OTU4)
        {
            RUN_PMC_SDK(adpt_traffic_mode_cfg, chanId, DIGI_LINE_OTU4, DIGI_OTN_SERVER_FEC_NONE, 80, TRUE);
            mPortCfg[port].trafficMode = DIGI_LINE_OTU4;
            mPortCfg[port].mappingType = LAST_DIGI_MAPOTN;
        }
        else
        {
            RUN_PMC_SDK(adpt_traffic_mode_cfg, chanId, DIGI_LINE_100GE_TRANSPARENT_RX_PMON, DIGI_OTN_SERVER_FEC_NONE, 80, TRUE);
            RUN_PMC_SDK(adpt_client_map_cfg, chanId, DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP, 0, TRUE);
            mPortCfg[port].trafficMode = DIGI_LINE_100GE_TRANSPARENT_RX_PMON;
            mPortCfg[port].mappingType = DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP;
        }
        RUN_PMC_SDK(adpt_serdes_tx_refclk_div_cfg, chanId, serdesMask, DIGI_TX_CLK_DIV_64);
        RUN_PMC_SDK_NO_ERROR(adpt_serdes_start_adaptation, HOSTMSG_ADAPT_INTF_S16_LINE, serdesMask, 0);
        write(0x200154, 0x0);   // disable PIN_LOS_EN
        mPortCfg[port].sigType = sigType;
        UpdateXc(port, TRUE);
    }
    else if (port >= 1 && port <= 10)
    {
        UINT32 ilkn_info = 8;
        UINT32 ts_number = 8;

        switch(sigType)
        {
            case PortMode::ETH10G:
                mPortCfg[port].trafficMode = DIGI_LINE_10GE_TRANSPARENT_RX_PMON;
                mPortCfg[port].mappingType = DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP;
                mPortCfg[port].odukType = DIGI_ODUK_ODU2E;
                break;

            case PortMode::OC192: // AMP
                mPortCfg[port].trafficMode = DIGI_LINE_STS192;
                mPortCfg[port].mappingType = DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP;
                mPortCfg[port].odukType = DIGI_ODUK_ODU2;
                break;

            case PortMode::STM64: // AMP
                mPortCfg[port].trafficMode = DIGI_LINE_STM64;
                mPortCfg[port].mappingType = DIGI_MAPOTN_ODU2P_OC_192_CBR_AMP;
                mPortCfg[port].odukType = DIGI_ODUK_ODU2;
                break;

            case PortMode::OTU2:
                mPortCfg[port].trafficMode = DIGI_LINE_OTU2;
                mPortCfg[port].mappingType = LAST_DIGI_MAPOTN;
                mPortCfg[port].odukType = DIGI_ODUK_ODU2;
                break;

            case PortMode::OTU2E:
                mPortCfg[port].trafficMode = DIGI_LINE_OTU2E;
                mPortCfg[port].mappingType = LAST_DIGI_MAPOTN;
                mPortCfg[port].odukType = DIGI_ODUK_ODU2E;
                // ToDo shall use ODUk Switch type instead of CPB switch
                break;

            case PortMode::ETH10G_GFP:
                mPortCfg[port].trafficMode = DIGI_LINE_10GE_MAC;
                mPortCfg[port].mappingType = DIGI_MAPOTN_ODU2_PACKET_GFP;
                mPortCfg[port].odukType = DIGI_ODUK_ODU2;
                // ToDo shall use ODUk Switch type instead of CPB switch
                break;

            //4-TGWAN, //7-FC8G //8-FC10G,

            default:
                break;
        }

        if (mPortCfg[port].trafficMode != LAST_DIGI_TRAFFIC)
        {
            RUN_PMC_SDK(adpt_traffic_mode_cfg, chanId, mPortCfg[port].trafficMode, DIGI_OTN_SERVER_FEC_NONE, ilkn_info, TRUE);
            if (mPortCfg[port].mappingType != LAST_DIGI_MAPOTN)
            {
                RUN_PMC_SDK(adpt_client_map_cfg, chanId, mPortCfg[port].mappingType, ts_number, TRUE);
            }
            RUN_PMC_SDK_NO_ERROR(adpt_serdes_start_adaptation, HOSTMSG_ADAPT_INTF_S16_LINE, serdesMask, 0);
            mPortCfg[port].sigType = sigType;
            UpdateXc(port, TRUE);
        }
    }

    ClearAlarm(port);
    ClearCounter(port);
}

void Digi::SetLoopbackType(int port, LoopbackType::Enum lpbk)
{
    CMutexLock lock(mDigiLock);
    
    if (mPortCfg[port].sigType == PortMode::NONE ||
        mPortCfg[port].lpbkType == lpbk)
    {
        return;
    }

    APP_LOG("%s %s -> %s\n", __FUNCTION__, LoopbackType_to_String(mPortCfg[port].lpbkType),
                                           LoopbackType_to_String(lpbk));
    
    digi_loopback_type_t lpbkType = DIGI_LPBK_DISABLE;
    if (lpbk == LoopbackType::Facility)
    {
        lpbkType = DIGI_LPBK_OUTWARD;
    }
    else if (lpbk == LoopbackType::Terminal)
    {
        lpbkType = DIGI_LPBK_INWARD;
    }
    RUN_PMC_SDK(adpt_loopback_set, mPortCfg[port].chanId, lpbkType);
    mPortCfg[port].lpbkType = lpbk;
}

void Digi::SetFecType(int port, FecType::Enum fecType)
{
    CMutexLock lock(mDigiLock);
    
    if (mPortCfg[port].sigType == PortMode::NONE ||
        mPortCfg[port].fecType == fecType)
    {
        return;
    }
    
    APP_LOG("%s %s -> %s\n", __FUNCTION__, FecType_to_String(mPortCfg[port].fecType),
                                           FecType_to_String(fecType));

    digi_otn_server_fec_t fec_type = DIGI_OTN_SERVER_FEC_NONE;
    switch(fecType)
    {
        case FecType::GFEC:
            fec_type = DIGI_OTN_SERVER_FEC_G709;
            break;
        case FecType::I4:
            fec_type = DIGI_OTN_SERVER_FEC_I4;
            break;
        case FecType::I7:
            fec_type = DIGI_OTN_SERVER_FEC_I7;
            break;
        default:
            break;
    }
    RUN_PMC_SDK(adpt_otn_fec_type_set, mPortCfg[port].chanId, fec_type);
    mPortCfg[port].fecType = fecType;
}

void Digi::SetOtuPrbsEnable(int port, bool enable)
{
    BOOL prbs_enable = enable ? TRUE : FALSE;
    RUN_PMC_SDK(adpt_serdes_prbs_set, mPortCfg[port].chanId, DIGI_SERDES_DIR_RX_TX, DIGI_SERDES_PRBS_31, TRUE, prbs_enable);
    mPortCfg[port].prbsEnable = enable;
}

void Digi::GetOtuPrbsStatus(int port, PrbsStatus::Enum &status)
{
    if (mPortCfg[port].prbsEnable)
    {
        if (port >= 1 && port <= 10)
        {
            UINT32 lane_id = 0, err_cnt = -1;
            util_patt_status_t prbs_status;
            RUN_PMC_SDK(adpt_serdes_lane_rx_state_get, mPortCfg[port].chanId, lane_id, &prbs_status, &err_cnt);
            if (err_cnt < 0xfffffa)
            {
                if (err_cnt == 0)
                {
                    status = PrbsStatus::InSync;
                }
                else
                {
                    status = PrbsStatus::ErrorSync;
                }
            }
            else
            {
                status = PrbsStatus::OutSync;
                RUN_PMC_SDK(adpt_serdes_prbs_set, mPortCfg[port].chanId, DIGI_SERDES_DIR_RX_TX, DIGI_SERDES_PRBS_31, TRUE, FALSE);
                RUN_PMC_SDK(adpt_serdes_prbs_set, mPortCfg[port].chanId, DIGI_SERDES_DIR_RX_TX, DIGI_SERDES_PRBS_31, TRUE, TRUE);
            }
        }
        else
        {
            status = PrbsStatus::OutSync;
        }
    }
    else
    {
        status = PrbsStatus::Disabled;
    }
}

std::string Digi::GetPortName(int port)
{
    if (port >= 1 && port <= 10)
    {
        char name[16] = {0};
        sprintf(name, "Sfp %d", port);
        return name;
    }
    else if (port == 11)
    {
        return "Line CFP";
    }
    else if (port == 12)
    {
        return "Client CFP";
    }

    return "N/A";
}

void Digi::UpdateCounter(int port)
{
    adpt_pmon_trigger(dev_id);

    mEthCnt[port].valid = false;
    mOtuCnt[port].valid = false;
    
    if (mPortCfg[port].sigType == PortMode::ETH10G || mPortCfg[port].sigType == PortMode::ETH100G || mPortCfg[port].sigType == PortMode::ETH10G_GFP)
    {
        uint32_t pmCountL = 0;
        uint32_t pmCountM = 0;

        /* RX_FRAMES */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsPkts += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_OCTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsOctets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));

        /* RX_UNICAST_FRAMES_OK */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_IF_UCAST_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_IF_UCAST_PKTS_MSB, &pmCountM);
        mEthCnt[port].ifInUcastPkts += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_BROADCAST_FRAMES_OK */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_IF_BROADCAST_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_IF_BROADCAST_PKTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsBroadcastPkts += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_MULTICAST_FRAMES_OK */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_IF_MULTICAST_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_IF_MULTICAST_PKTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsMulticastPkts += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));

        /* RX_FCS_ERROR*/
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_FRAME_CHECK_SEQUENCE_ERRORS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_FRAME_CHECK_SEQUENCE_ERRORS_MSB, &pmCountM);
        mEthCnt[port].etherStatsCRCAlignErrors += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_LONG_LENGTH_ERROR */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_OVERSIZE_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_OVERSIZE_PKTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsOversizePkt += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_LONG_LENGTH_CRC_ERROR */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId,  DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_JABBER_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_JABBER_MSB, &pmCountM);
        mEthCnt[port].etherStatsJabbers += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_SHORT_LENGTH_ERROR */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_UNDERSIZE_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_UNDERSIZE_PKTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsUndersizePkts += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_SHORT_LENGTH_CRC_ERROR */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_FRAGMENTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_FRAGMENTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsFragments += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_FRAMES_64_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_64_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_64_OCTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsPkts64Octets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_FRAMES_65_TO_127_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsPkts65to127Octets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_FRAMES_128_TO_255_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsPkts128to255Octets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_FRAMES_256_TO_511_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsPkts256to511Octets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_FRAMES_512_TO_1023_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsPkts512to1023Octets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* RX_FRAMES_1024_TO_1518_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsPkts1024to1518Octets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));

        /* RX_FRAMES_1519_TO_MAX_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_MSB, &pmCountM);
        mEthCnt[port].etherStatsPkts1519toMaxOctets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));

        /* TX_UNICAST_FRAMES_OK */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_IF_UCAST_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_IF_UCAST_PKTS_MSB, &pmCountM);
        mEthCnt[port].ifOutUcastPkts += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* TX_BROADCAST_FRAMES_OK */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_IF_BROADCAST_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_IF_BROADCAST_PKTS_MSB, &pmCountM);
        mEthCnt[port].ifOutBroadcastPkts += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* TX_MULTICAST_FRAMES_OK */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_IF_MULTICAST_PKTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_IF_MULTICAST_PKTS_MSB, &pmCountM);
        mEthCnt[port].ifOutMulticastPkts += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));
        
        /* TX_OCTETS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_ETHER_STATS_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_ETHER_STATS_OCTS_MSB, &pmCountM);
        mEthCnt[port].ifOutOctets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));

        /* TX_PAUSE_MAC_CTRL_FRAMES */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_PAUSE_MAC_CTRL_FRAMES_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_TX, DIGI_PAUSE_MAC_CTRL_FRAMES_MSB, &pmCountM);
        mEthCnt[port].aTxPauseMacCtrlFrames += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));

        /* RX_PAUSE_MAC_CTRL_FRAMES */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_PAUSE_MAC_CTRL_FRAMES_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_PAUSE_MAC_CTRL_FRAMES_MSB, &pmCountM);
        mEthCnt[port].aRxPauseMacCtrlFrames += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));

        /* RX_IF_OCTS */
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_IF_OCTS_LSB, &pmCountL);
        RUN_PMC_SDK(adpt_enet_pm_get, mPortCfg[port].chanId, DIGI_PORT_DIR_RX, DIGI_IF_OCTS_MSB, &pmCountM);
        mEthCnt[port].ifInOctets += ((static_cast<uint64_t>(pmCountM) << 32) + static_cast<uint64_t>(pmCountL));

        mEthCnt[port].valid = true;
    }
    else if (mPortCfg[port].sigType == PortMode::OTU2 || mPortCfg[port].sigType == PortMode::OTU2E || mPortCfg[port].sigType == PortMode::OTU4)
    {
        digi_fec_pm_t fecOnesType = LAST_DIGI_FEC, fecZerosType, fecUncorrType, fecFrameType;
        uint32_t pmCount = 0, fecOnes = 0, fecZeros = 0, fecUncorr = 0, fecCorr = 0, fecFrame = 0;

        RUN_PMC_SDK(adpt_otn_pm_get, mPortCfg[port].chanId, DIGI_OTN_SEG_SM, DIGI_OTN_LINE_SIDE, DIGI_OTN_BIP, &pmCount);
        mOtuCnt[port].bip8 += static_cast<uint32_t>(pmCount);

        RUN_PMC_SDK(adpt_otn_pm_get, mPortCfg[port].chanId, DIGI_OTN_SEG_SM, DIGI_OTN_LINE_SIDE, DIGI_OTN_BEI, &pmCount);
        mOtuCnt[port].bei += static_cast<uint32_t>(pmCount);

        if (mPortCfg[port].fecType == FecType::GFEC)
        {
            fecOnesType = DIGI_GFEC_LPA_STATS_CORR_1S;
            fecZerosType = DIGI_GFEC_LPA_STATS_CORR_0S;
            fecUncorrType = DIGI_GFEC_LPA_STATS_UNCORR_CWDS;
            fecFrameType = DIGI_GFEC_LPA_STATS_FP;
        }
        else if (mPortCfg[port].fecType == FecType::I4)
        {
            fecOnesType = DIGI_I4_FEC_CORR_1S_COUNT;
            fecZerosType = DIGI_I4_FEC_CORR_0S_COUNT;
            fecUncorrType = DIGI_I4_FEC_UNCORR_ERROR_COUNT;
            fecFrameType = DIGI_I4_FEC_RX_FRM_PULSE_COUNT;
        }
        else if (mPortCfg[port].fecType == FecType::I7)
        {
            fecOnesType = DIGI_I7_FEC_CORR_1S_COUNT;
            fecZerosType = DIGI_I7_FEC_CORR_0S_COUNT;
            fecUncorrType = DIGI_I7_FEC_UNCORR_ERROR_COUNT;
            fecFrameType = DIGI_I7_FEC_RX_FRM_PULSE_COUNT;
        }

        if (fecOnesType != LAST_DIGI_FEC)
        {
            RUN_PMC_SDK(adpt_otn_fec_pm_get, mPortCfg[port].chanId, fecOnesType, &fecOnes);
            RUN_PMC_SDK(adpt_otn_fec_pm_get, mPortCfg[port].chanId, fecZerosType, &fecZeros);
            RUN_PMC_SDK(adpt_otn_fec_pm_get, mPortCfg[port].chanId, fecUncorrType, &fecUncorr);
            RUN_PMC_SDK(adpt_otn_fec_pm_get, mPortCfg[port].chanId, DIGI_FEC_TOTAL_CORR_ERRS, &fecCorr);
            RUN_PMC_SDK(adpt_otn_fec_pm_get, mPortCfg[port].chanId, fecFrameType, &fecFrame);

            mOtuCnt[port].correctableOnes  += static_cast<uint32_t>(fecOnes);
            mOtuCnt[port].correctableZeros += static_cast<uint32_t>(fecZeros);
            mOtuCnt[port].correctableBytes =  (mOtuCnt[port].correctableOnes + mOtuCnt[port].correctableZeros)/8;
            mOtuCnt[port].uncorrectables   += static_cast<uint32_t>(fecUncorr);
            mOtuCnt[port].correctBits      += static_cast<uint32_t>(fecCorr);
            mOtuCnt[port].frameCount       += static_cast<uint32_t>(fecFrame);
#if 0
            APP_LOG("correctableOnes  +%d", (int)fecOnes);
            APP_LOG("correctableZeros +%d", (int)fecZeros);
            APP_LOG("uncorrectables   +%d", (int)fecUncorr);
            APP_LOG("correctBits      +%d", (int)fecCorr);
            APP_LOG("frameCount       +%d", (int)fecFrame);
            APP_LOG("    ");
#endif
        }

        mOtuCnt[port].valid = true;
    }
}

void Digi::ClearCounter(int port)
{
    if (port < DigiApi::PORT_NUM)
    {
        memset(&mEthCnt[port], 0, sizeof(EthCounter));
        memset(&mOtuCnt[port], 0, sizeof(OtuCounter));
    }
}

void Digi::DumpCounter(int port)
{
    UpdateCounter(port);
    
    if (mPortCfg[port].sigType == PortMode::ETH10G || mPortCfg[port].sigType == PortMode::ETH100G || mPortCfg[port].sigType == PortMode::ETH10G_GFP)
    {
        //APP_LOG("etherSequenceErrCnt: %llu", mEthCnt[port].etherSequenceErrCnt);
        //APP_LOG("DropEvents: %llu", mEthCnt[port].etherStatsDropEvents);
        APP_LOG("Octets                 : %llu", mEthCnt[port].etherStatsOctets);
        APP_LOG("Pkts                   : %llu", mEthCnt[port].etherStatsPkts);
        APP_LOG("BroadcastPkts          : %llu", mEthCnt[port].etherStatsBroadcastPkts);
        APP_LOG("MulticastPkts          : %llu", mEthCnt[port].etherStatsMulticastPkts);
        APP_LOG("CRCAlignErrors         : %llu", mEthCnt[port].etherStatsCRCAlignErrors);
        APP_LOG("UndersizePkts          : %llu", mEthCnt[port].etherStatsUndersizePkts);
        APP_LOG("OversizePkt            : %llu", mEthCnt[port].etherStatsOversizePkt);
        APP_LOG("Fragments              : %llu", mEthCnt[port].etherStatsFragments);
        APP_LOG("Jabbers                : %llu", mEthCnt[port].etherStatsJabbers);
        APP_LOG("Pkts64Octets           : %llu", mEthCnt[port].etherStatsPkts64Octets);
        APP_LOG("Pkts65to127Octets      : %llu", mEthCnt[port].etherStatsPkts65to127Octets);
        APP_LOG("Pkts128to255Octets     : %llu", mEthCnt[port].etherStatsPkts128to255Octets);
        APP_LOG("Pkts256to511Octets     : %llu", mEthCnt[port].etherStatsPkts256to511Octets);
        APP_LOG("Pkts512to1023Octets    : %llu", mEthCnt[port].etherStatsPkts512to1023Octets);
        APP_LOG("Pkts1024to1518Octets   : %llu", mEthCnt[port].etherStatsPkts1024to1518Octets);
        APP_LOG("Pkts1519toMaxOctets    : %llu", mEthCnt[port].etherStatsPkts1519toMaxOctets);
        //APP_LOG("DroppedFrames: %llu", mEthCnt[port].etherStatsDroppedFrames);
        APP_LOG("ifInUcastPkts          : %llu", mEthCnt[port].ifInUcastPkts);
        //APP_LOG("ifInDiscards: %llu", mEthCnt[port].ifInDiscards);
        //APP_LOG("ifInMulticastPkts      : %llu", mEthCnt[port].ifInMulticastPkts);
        //APP_LOG("ifInBroadcastPkts      : %llu", mEthCnt[port].ifInBroadcastPkts);
        APP_LOG("ifInOctets             : %llu", mEthCnt[port].ifInOctets);
        //APP_LOG("ifInErrors: %llu", mEthCnt[port].ifInErrors);
        APP_LOG("ifOutMulticastPkts     : %llu", mEthCnt[port].ifOutMulticastPkts);
        APP_LOG("ifOutBroadcastPkts     : %llu", mEthCnt[port].ifOutBroadcastPkts);
        APP_LOG("ifOutOctets            : %llu", mEthCnt[port].ifOutOctets);
        APP_LOG("ifOutUcastPkts         : %llu", mEthCnt[port].ifOutUcastPkts);
        //APP_LOG("ifOutDiscards: %llu", mEthCnt[port].ifOutDiscards);
        //APP_LOG("ifOutErrors: %llu", mEthCnt[port].ifOutErrors);
        //APP_LOG("ifInSymbolErrors: %llu", mEthCnt[port].ifInSymbolErrors);
        APP_LOG("aTxPauseMacCtrlFrames  : %llu", mEthCnt[port].aTxPauseMacCtrlFrames);
        APP_LOG("aRxPauseMacCtrlFrames  : %llu", mEthCnt[port].aRxPauseMacCtrlFrames);
    }
    else if (mPortCfg[port].sigType == PortMode::OTU2 || mPortCfg[port].sigType == PortMode::OTU2E || mPortCfg[port].sigType == PortMode::OTU4)
    {
        APP_LOG("correctableBytes   : %llu", mOtuCnt[port].correctableBytes);
        APP_LOG("correctableOnes    : %llu", mOtuCnt[port].correctableOnes);
        APP_LOG("correctableZeros   : %llu", mOtuCnt[port].correctableZeros);
        APP_LOG("uncorrectables     : %llu", mOtuCnt[port].uncorrectables);
        APP_LOG("correctBits        : %llu", mOtuCnt[port].correctBits);
        APP_LOG("frameCount         : %llu", mOtuCnt[port].frameCount);
        APP_LOG("bip8               : %llu", mOtuCnt[port].bip8);
        APP_LOG("bei                : %llu", mOtuCnt[port].bei);
    }
    else
    {
        APP_LOG("Counter not available on this Signal Type");
    }
}

EthCounter* Digi::GetEthCnt(int port)
{
    if (port < DigiApi::PORT_NUM)
    {
        return &mEthCnt[port];
    }
    return NULL;
}

OtuCounter* Digi::GetOtuCnt(int port)
{
    if (port < DigiApi::PORT_NUM)
    {
        return &mOtuCnt[port];
    }
    return NULL;
}

void Digi::UpdateAlarm(int port)
{
    BOOL defect;
    
    pmc_log_sev_t log_sev = pmc_log_verbosity_level_get();
    pmc_log_verbosity_level_set(PMC_LOG_SEV_DISABLE);
    adpt_defect_trigger(dev_id, DIGI_BLK_FULL);
    //adpt_defect_trigger(dev_id, DIGI_BLK_ALL);
    //adpt_defect_trigger(dev_id, DIGI_BLK_MAP);
    //adpt_defect_trigger(dev_id, DIGI_BLK_CBR);
    //adpt_defect_trigger(dev_id, DIGI_BLK_ENET);
    //adpt_defect_trigger(dev_id, DIGI_BLK_OTN);
    //adpt_defect_trigger(dev_id, DIGI_BLK_SIFD);
    pmc_log_verbosity_level_set(log_sev);

    mEthAlarm[port].valid = false;
    mOtuAlarm[port].valid = false;
    mOduAlarm[port].valid = false;
    mStmAlarm[port].valid = false;

    UINT32 chnl_id = mPortCfg[port].chanId;
    
    if (mPortCfg[port].sigType == PortMode::ETH10G || mPortCfg[port].sigType == PortMode::ETH100G || mPortCfg[port].sigType == PortMode::ETH10G_GFP)
    {        
        defect = adpt_cvt_enet_defect_get(dev_id, chnl_id, DIGI_ENET_LOS);
        DIGI_ALARM_UPDATE(mEthAlarm[port].los, AlarmId::ETH_LOS);
        
        defect = adpt_cvt_enet_defect_get(dev_id, chnl_id, DIGI_ENET_LINK_DOWN);
        DIGI_ALARM_UPDATE(mEthAlarm[port].lossync, AlarmId::ETH_LOSYNC);
        
        defect = adpt_cvt_enet_defect_get(dev_id, chnl_id, DIGI_ENET_LF);
        DIGI_ALARM_UPDATE(mEthAlarm[port].lf, AlarmId::ETH_LF);
        
        defect = adpt_cvt_enet_defect_get(dev_id, chnl_id, DIGI_ENET_RF);
        DIGI_ALARM_UPDATE(mEthAlarm[port].rf, AlarmId::ETH_RF);
        
        //defect = adpt_cvt_enet_defect_get(dev_id, chnl_id, DIGI_ENET_HI_BER);
        //DIGI_ALARM_UPDATE(mEthAlarm[port].los, AlarmId::ETH_LOS);
        
        mEthAlarm[port].valid = true;
    }
    else if (mPortCfg[port].sigType == PortMode::OTU2 || mPortCfg[port].sigType == PortMode::OTU2E || mPortCfg[port].sigType == PortMode::OTU4)
    {
        digi_otn_loc_t location = DIGI_OTN_LINE_SIDE;
        if (mPortCfg[port].sigType == PortMode::OTU4)
        {
            location = DIGI_OTN_SYS_SIDE;
        }

        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, location, DIGI_OTN_LOS);
        if (port == LINE_CFP_PORT) defect = MsaCfp::GetInst(0).GetLos() ? TRUE : FALSE; // workaround for line side LOS
        DIGI_ALARM_UPDATE(mOtuAlarm[port].los, AlarmId::OTU_LOS);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, location, DIGI_OTN_LOL);
        DIGI_ALARM_UPDATE(mOtuAlarm[port].lol, AlarmId::OTU_LOL);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, location, DIGI_OTN_LOF);
        DIGI_ALARM_UPDATE(mOtuAlarm[port].lof, AlarmId::OTU_LOF);
        
        //defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, location, DIGI_OTN_LOM);
        //DIGI_ALARM_UPDATE(mOtuAlarm[port].lom, AlarmId::OTU_LOM);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, location, DIGI_OTN_TIM);
        DIGI_ALARM_UPDATE(mOtuAlarm[port].tim, AlarmId::OTU_TIM);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, location, DIGI_OTN_BDI);
        DIGI_ALARM_UPDATE(mOtuAlarm[port].bdi, AlarmId::OTU_BDI);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_SM, location, DIGI_OTN_DEG);
        DIGI_ALARM_UPDATE(mOtuAlarm[port].bersd, AlarmId::OTU_BERSD);

        mOtuAlarm[port].valid = true;

        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, location, DIGI_OTN_DEG);
        DIGI_ALARM_UPDATE(mOduAlarm[port].bersd, AlarmId::ODU_BERSD);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, location, DIGI_OTN_LCK);
        DIGI_ALARM_UPDATE(mOduAlarm[port].lck, AlarmId::ODU_LCK);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, location, DIGI_OTN_OCI);
        DIGI_ALARM_UPDATE(mOduAlarm[port].oci, AlarmId::ODU_OCI);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, location, DIGI_OTN_AIS);
        DIGI_ALARM_UPDATE(mOduAlarm[port].ais, AlarmId::ODU_AIS);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, location, DIGI_OTN_BDI);
        DIGI_ALARM_UPDATE(mOduAlarm[port].bdi, AlarmId::ODU_BDI);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, location, DIGI_OTN_TIM);
        DIGI_ALARM_UPDATE(mOduAlarm[port].tim, AlarmId::ODU_TIM);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, location, DIGI_OTN_PLM);
        DIGI_ALARM_UPDATE(mOduAlarm[port].plm, AlarmId::ODU_PLM);
        
        defect = adpt_cvt_otn_defect_get(dev_id, chnl_id, DIGI_OTN_SEG_PM, location, DIGI_OTN_LOOMFI);
        DIGI_ALARM_UPDATE(mOduAlarm[port].loomfi, AlarmId::ODU_LOOMFI);
        
        mOduAlarm[port].valid = true;
    }
    else if (mPortCfg[port].sigType == PortMode::STM64)
    {
        defect = adpt_cvt_sdh_defect_get(dev_id, chnl_id, DIGI_PORT_DIR_RX, DIGI_SDH_LOS);
        DIGI_ALARM_UPDATE(mStmAlarm[port].los, AlarmId::STM_LOS);

        defect = adpt_cvt_sdh_defect_get(dev_id, chnl_id, DIGI_PORT_DIR_RX, DIGI_SDH_LOF);
        DIGI_ALARM_UPDATE(mStmAlarm[port].lof, AlarmId::STM_LOF);

        defect = adpt_cvt_sdh_defect_get(dev_id, chnl_id, DIGI_PORT_DIR_RX, DIGI_SDH_RS_TIM);
        DIGI_ALARM_UPDATE(mStmAlarm[port].tim_r, AlarmId::STM_TIM_R);

        defect = adpt_cvt_sdh_defect_get(dev_id, chnl_id, DIGI_PORT_DIR_RX, DIGI_SDH_MS_AIS);
        DIGI_ALARM_UPDATE(mStmAlarm[port].ais_l, AlarmId::STM_AIS_L);

        defect = adpt_cvt_sdh_defect_get(dev_id, chnl_id, DIGI_PORT_DIR_RX, DIGI_SDH_MS_RDI);
        DIGI_ALARM_UPDATE(mStmAlarm[port].rfi_l, AlarmId::STM_RFI_L);
        
        mStmAlarm[port].valid = true;
    }
}

void Digi::ClearAlarm(int port)
{
    if (port < DigiApi::PORT_NUM)
    {
        mEthAlarm[port].reset();
        mOtuAlarm[port].reset();
        mOduAlarm[port].reset();
        mStmAlarm[port].reset();
    }
}

void Digi::DumpAlarm(int port)
{
    UpdateAlarm(port);

    EthAlarm* pEth = Digi::GetInst().GetEthAlarm(port);
    OtuAlarm* pOtu = Digi::GetInst().GetOtuAlarm(port);
    OduAlarm* pOdu = Digi::GetInst().GetOduAlarm(port);
    StmAlarm* pStm = Digi::GetInst().GetStmAlarm(port);

    if (pEth->valid)
    {
        APP_LOG("ETH-LOS      : %d", pEth->los);
        APP_LOG("ETH-LOS SYNC : %d", pEth->lossync);
        APP_LOG("ETH-LF       : %d", pEth->lf);
        APP_LOG("ETH-RF       : %d", pEth->rf);
        APP_LOG("ETH-HIGH BER : %d", pEth->highber);
    }

    if (pOtu->valid)
    {
        APP_LOG("OTU-LOS      : %d", pOtu->los);
        APP_LOG("OTU-LOL      : %d", pOtu->lol);
        APP_LOG("OTU-LOF      : %d", pOtu->lof);
        APP_LOG("OTU-LOM      : %d", pOtu->lom);
        APP_LOG("OTU-TIM      : %d", pOtu->tim);
        APP_LOG("OTU-BDI      : %d", pOtu->bdi);
        APP_LOG("OTU-BERSD    : %d", pOtu->bersd);
        APP_LOG("OTU-AIS      : %d", pOtu->ais);
    }

    if (pOdu->valid)
    {
        APP_LOG("ODU-BERSD    : %d", pOdu->bersd);
        APP_LOG("ODU-LCK      : %d", pOdu->lck);
        APP_LOG("ODU-OCI      : %d", pOdu->oci);
        APP_LOG("ODU-AIS      : %d", pOdu->ais);
        APP_LOG("ODU-BDI      : %d", pOdu->bdi);
        APP_LOG("ODU-TIM      : %d", pOdu->tim);
        APP_LOG("ODU-PLM      : %d", pOdu->plm);
        APP_LOG("ODU-LOOMFI   : %d", pOdu->loomfi);
    }

    if (pStm->valid)
    {
        APP_LOG("STM-LOS      : %d", pStm->los);
        APP_LOG("STM-LOF      : %d", pStm->lof);
        APP_LOG("STM-TIM_R    : %d", pStm->tim_r);
        APP_LOG("STM-AIS_L    : %d", pStm->ais_l);
        APP_LOG("STM-RFI_L    : %d", pStm->rfi_l);
    }
}

EthAlarm* Digi::GetEthAlarm(int port)
{
    if (port < DigiApi::PORT_NUM)
    {
        return &mEthAlarm[port];
    }
    return NULL;
}

OtuAlarm* Digi::GetOtuAlarm(int port)
{
    if (port < DigiApi::PORT_NUM)
    {
        return &mOtuAlarm[port];
    }
    return NULL;
}

OduAlarm* Digi::GetOduAlarm(int port)
{
    if (port < DigiApi::PORT_NUM)
    {
        return &mOduAlarm[port];
    }
    return NULL;
}

StmAlarm* Digi::GetStmAlarm(int port)
{
    if (port < DigiApi::PORT_NUM)
    {
        return &mStmAlarm[port];
    }
    return NULL;
}

void Digi::Dump(int port)
{
    APP_LOG("%s:", GetPortName(port).c_str());
    APP_LOG("PortMode: %s", PortMode_to_String(mPortCfg[port].sigType));
    APP_LOG("LoopBack  : %s", LoopbackType_to_String(mPortCfg[port].lpbkType));
    APP_LOG("FecType   : %s", FecType_to_String(mPortCfg[port].fecType));

    DumpCounter(port);
    DumpAlarm(port);
}

void Digi::Menu(int argc, char *argv[])
{
    if (argc == 1 && argv[0][0] == 'i')
    {
        InitCold();
    }
    else if (argc >= 2 && argv[0][0] == 'r')
    {
        int len = 1;
        uint32_t reg, val;
        if (argc == 3) len = atoi(argv[2]);
        sscanf(argv[1], "%x", &reg);
        for (int i = 0; i < len; i++)
        {
            val = read(reg + i);
            APP_LOG("[%08x]: 0x%08x", reg + i, val); 
        }
    }
    else if (argc >= 2 && argv[0][0] == 'x')
    {
        int ret = DigiSdkMenu(argc - 1, argv + 1);
        APP_LOG("ret = 0x%08x", ret);
    }
    else if (argc == 3 && argv[0][0] == 'w')
    {
        uint32_t reg, val;
        sscanf(argv[1], "%x", &reg);
        sscanf(argv[2], "%x", &val);
        write(reg, val);
    }
    else if (argc == 3 && argv[0][0] == 's')
    {
        int port = atoi(argv[1]);
        int type = atoi(argv[2]);
        SetSignalType(port, (PortMode::Enum)type);
    }
    else if (argc == 3 && argv[0][0] == 'l')
    {
        int port = atoi(argv[1]);
        int type = atoi(argv[2]);
        SetLoopbackType(port, (LoopbackType::Enum)type);
    }
    else if (argc == 3 && argv[0][0] == 'f')
    {
        int port = atoi(argv[1]);
        int type = atoi(argv[2]);
        SetFecType(port, (FecType::Enum)type);
    }
    else if (argc == 2 && argv[0][0] == 'd')
    {
        int port = atoi(argv[1]);
        Dump(port);
    }
    else if (argc == 2 && argv[0][0] == 'c')
    {
        int port = atoi(argv[1]);
        ClearCounter(port);
        ClearAlarm(port);
    }
    else if (argc == 2 && argv[0][0] == 'm')
    {
        int port = atoi(argv[1]);
        while (1)
        {
            UpdateCounter(port);
            UpdateAlarm(port);
            sleep(1);
        }
    }
    else if (argc == 2 && argv[0][0] == 'p')
    {
        int port = atoi(argv[1]);
        CheckPortParam(port);
    }
    else if (argc == 5 && argv[0][0] == 't')
    {
        int intf = atoi(argv[1]);
        int amp = atoi(argv[2]);
        int post = atoi(argv[3]);
        int pre = atoi(argv[4]);
        int lane = 0;
        for (lane = 0; lane < 10; lane++)
        {
            RUN_PMC_SDK(adpt_serdes_s16_tx_swing_set, (digi_serdes_intf_t)intf, (UINT32)lane, (UINT8)amp, (UINT8)post, (UINT8)pre);
        }
    }
    else
    {
        APP_LOG("digi");
        APP_LOG("\t i");
        APP_LOG("\t x sdk menu");
        APP_LOG("\t r <reg> [len]");
        APP_LOG("\t w <reg> <val>");
        APP_LOG("\t s <port> <PortMode>");
        APP_LOG("\t l <port> <LoopbackMode>");
        APP_LOG("\t f <port> <FecType>");
        APP_LOG("\t d <port> - dump port cfg");
        APP_LOG("\t c <port> - clear counter");
        APP_LOG("\t m <port> - monitor");
        APP_LOG("\t p <port> - check port param");
        APP_LOG("\t t <0-line,1-sys> <amp> <post> <pre> - set tx swing");
        APP_LOG("\t\t port: SFP 1~10, LineCFP-11, ClientCFP-12");
        APP_LOG("\t\t PortMode: 0-UNKNOWN, 1-ETH10G, 2-STM64, 3-OC192");
        APP_LOG("\t\t             4-TGWAN, 5-OTU2, 6-OTU2E, 7-FC8G");
        APP_LOG("\t\t             8-FC10G, 9-ETH100G, 10-OTU4");
        APP_LOG("\t\t LoopbackMode: 0-Disabled, 1-Facility, 2-Terminal");
        APP_LOG("\t\t FecType: 0-NONE, 1-I4, 2-I7, 3-G709, 4-TGE");
    }
}

void Digi::write(uint32_t reg, uint32_t val)
{
    MemWrite(digiBase + reg, reg_swap(val));
}

uint32_t Digi::read(uint32_t reg)
{
    uint32_t val = MemRead(digiBase + reg);
    return reg_swap(val);
}

void Digi::CheckPortParam(int port)
{
    int nport;
    uint8_t eq = 0, de = 0, drv = 3;
    //uint8_t eq1 = 0, de1 = 0, drv1 = 0;
    //uint64_t bip8 = -1;

    for (drv = 3; drv > 1; drv--)
    {
        for (eq = 0; eq < 32; eq++)
        {
            for (de = 0; de < 3; de++)
            {
                for (nport = 0; nport < 10; nport++)
                {
                    M21036SetRxParam(M21036_LANE_DIGI, nport, eq);
                    M21036SetTxParam(M21036_LANE_CFP, nport, de, drv);
                }
                //UpdateCounter(port);
                //ClearCounter(port);
                //sleep(1);
                //UpdateCounter(port);
                APP_LOG("eq=%d, de=%d, drv=%d, press enter to continue", eq, de, drv);
                //if (mOtuCnt[port].bip8 < bip8)
                //{
                //    bip8 = mOtuCnt[port].bip8;
                //    eq1 = eq;
                //    de1 = de;
                //    drv1 = drv;
                //}
                getchar();
            }
        }
    }
    //APP_LOG("eq1=%d, de1=%d, drv1=%d, bip8=%llu", eq1, de1, drv1, bip8);
}

#if 0
#include <fcntl.h>
#include <sys/ioctl.h>
#define DIGI_IOCTL_MAGIC 0xbb
#define DIGI_IOCTL_READ_DMA_PHY_ADDR         _IOR(DIGI_IOCTL_MAGIC, 1, unsigned long)

void DigiTest(void)
{
#ifdef __TARGET__
    APP_LOG("unsigned long = %d", sizeof(unsigned long));
    APP_LOG("unsigned int = %d", sizeof(unsigned int));
    APP_LOG("uint64_t = %d", sizeof(uint64_t));

    int fd = open("/dev/digi0", O_RDWR | O_SYNC);
    if (fd >= 0)
    {
        int ret = ioctl(fd, DIGI_IOCTL_READ_DMA_PHY_ADDR, &phyAddr);
        if (ret < 0)
        {
            APP_LOG("ERROR: digi ioctl failed");
        }
        else
        {
            APP_LOG("phyAddr = %llx", phyAddr);
        }

        close(fd);
    }
    else
    {
        APP_LOG("ERROR: digi device open failed");
    }

    return;

typedef enum
{
    PMC_LOG_SEV_DISABLE=0,
    PMC_LOG_SEV_HIGHEST,
    PMC_LOG_SEV_FATAL,
    PMC_LOG_SEV_HIGH,
    PMC_LOG_SEV_MEDIUM,
    PMC_LOG_SEV_LOW,
    PMC_LOG_SEV_LOWEST,

    PMC_LOG_SEV_NUM

} pmc_log_sev_t;

#endif
}
#endif
