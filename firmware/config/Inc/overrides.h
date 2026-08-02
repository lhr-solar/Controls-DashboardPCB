/**
 * Compile-time BPS override TX config (CarCAN 0x67 / 0x69).
 * Change values here; Task_ControlsStatus just calls the two send helpers.
 */
#pragma once

#include <stdint.h>
#include "CarCAN_can_msgs.h"

/* ================= BPS Command (0x67) ================= */
/* 0 = disabled/false, 1 = enabled/true (matches CarCAN value tables) */
#define BPS_DRIVE_PROFILE_ENABLE_MASTER  1
#define BPS_REGEN_ALLOW                  0
#define BPS_ADV_MPPT_CONTROL             1
#define BPS_SOFT_SHDN                    1
#define BPS_VSAG_COMPENSATION            1

/* ================= BPS Module Override (0x69) ================= */
/* Per-module codes (same as BPS_Module_Override value table) */
#define MODULE_OVERRIDE_NORMAL   0
#define MODULE_OVERRIDE_VOLTAGE  1
#define MODULE_OVERRIDE_TEMP     2
#define MODULE_OVERRIDE_ALL      3

#define BPS_MODULE_OVERRIDE_OFF           0
#define BPS_MODULE_OVERRIDE_ALL_MODULES   1
#define BPS_MODULE_OVERRIDE_LIST          2

/* Pick one mode. Default: everything NORMAL_OPERATION. */
#define BPS_MODULE_OVERRIDE_MODE  BPS_MODULE_OVERRIDE_OFF

/* ALL_MODULES: every module gets this value */
#define BPS_MODULE_OVERRIDE_VALUE_ALL  MODULE_OVERRIDE_NORMAL

/* LIST: {module_index, value} pairs. Uncomment MODE=LIST and edit. */
#define BPS_MODULE_OVERRIDE_ENTRIES \
    { 0, MODULE_OVERRIDE_VOLTAGE }, \
    { 5, MODULE_OVERRIDE_TEMP }

/* ================= TX helpers ================= */
void Send_Overrides(void);
