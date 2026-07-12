#include "overrides.h"
#include "CarCAN.h"
#include <stddef.h>

#define BPS_MODULE_COUNT 32

static void Overrides_Send_BPS_Command(void)
{
    uint8_t data[CAN_DLC_BPS_COMMAND] = {0};

    data[0] |= (uint8_t)((BPS_DRIVE_PROFILE_ENABLE_MASTER & 0x1u) << 0);
    data[0] |= (uint8_t)((BPS_REGEN_ALLOW                 & 0x1u) << 1);
    data[0] |= (uint8_t)((BPS_ADV_MPPT_CONTROL            & 0x1u) << 2);
    data[0] |= (uint8_t)((BPS_SOFT_SHDN                   & 0x1u) << 3);
    data[0] |= (uint8_t)((BPS_VSAG_COMPENSATION           & 0x1u) << 4);

    (void)CarCAN_Send(CAN_ID_BPS_COMMAND, CAN_DLC_BPS_COMMAND, data, 0);
}

static void Overrides_Send_BPS_Module_Override(void)
{
    uint8_t data[CAN_DLC_BPS_MODULE_OVERRIDE] = {0};

#if BPS_MODULE_OVERRIDE_MODE == BPS_MODULE_OVERRIDE_ALL_MODULES
    {
        const uint8_t v = (uint8_t)(BPS_MODULE_OVERRIDE_VALUE & 0x3u);
        for (uint8_t m = 0; m < BPS_MODULE_COUNT; m++) {
            data[m >> 2] |= (uint8_t)(v << ((m & 0x3u) * 2u));
        }
    }
#elif BPS_MODULE_OVERRIDE_MODE == BPS_MODULE_OVERRIDE_LIST
    {
        static const uint8_t entries[][2] = {
            BPS_MODULE_OVERRIDE_ENTRIES
        };
        for (size_t i = 0; i < (sizeof(entries) / sizeof(entries[0])); i++) {
            const uint8_t m = entries[i][0];
            const uint8_t v = (uint8_t)(entries[i][1] & 0x3u);
            if (m >= BPS_MODULE_COUNT) {
                continue;
            }
            data[m >> 2] |= (uint8_t)(v << ((m & 0x3u) * 2u));
        }
    }
#else
    /* BPS_MODULE_OVERRIDE_OFF: leave all NORMAL_OPERATION (0) */
#endif

    (void)CarCAN_Send(CAN_ID_BPS_MODULE_OVERRIDE, CAN_DLC_BPS_MODULE_OVERRIDE, data, 0);
}

void Send_Overrides(void)
{
    Overrides_Send_BPS_Command();
    Overrides_Send_BPS_Module_Override();
}
