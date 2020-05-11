#ifndef _ALARM_ID_H_
#define _ALARM_ID_H_

namespace AlarmId
{
    enum Enum
    {
        TEMP_LT,
        TEMP_HT,
        FAN_FAULT,
        PWR_FAULT,
        
        ETH_LOS,
        ETH_LOSYNC,
        ETH_LF,
        ETH_RF,

        OTU_LOS,
        OTU_LOL,
        OTU_LOF,
        OTU_LOM,
        OTU_TIM,
        OTU_BDI,
        OTU_BERSD,

        ODU_BERSD,
        ODU_LCK,
        ODU_OCI,
        ODU_AIS,
        ODU_BDI,
        ODU_TIM,
        ODU_PLM,
        ODU_LOOMFI,
        ODU_MSIM,
        ODU_LOFLOM,
        
        OPU_CSF,

        STM_LOS,
        STM_LOF,
        STM_TIM_R,
        STM_AIS_L,
        STM_RFI_L,

        INVALID,
    };
}

const char* GetAlarmName(AlarmId::Enum aid);

#endif

