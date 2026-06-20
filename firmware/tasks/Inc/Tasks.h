#pragma once

#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "event_groups.h"


#define READ_CARCAN_TASK_DELAY_TICKS 		pdMS_TO_TICKS(150)
#define CONTROLS_CAN_TASK_DELAY_TICKS 		pdMS_TO_TICKS(300)
#define SEND_SWITCH_STATES_TASK_DELAY_TICKS pdMS_TO_TICKS(300)


/* ------| Task Priorities |------ */
#define INIT_TASK_PRIORITY                  (tskIDLE_PRIORITY+1)
#define READ_CAR_CAN_PRIORITY               (tskIDLE_PRIORITY+2) //BPS fault detection
#define READ_CONTROLS_CAN_PRIORITY          (tskIDLE_PRIORITY+4) //want to run immidetly after CAN ISR
#define SEND_SWITCH_STATES_PRIORITY       	(tskIDLE_PRIORITY+3) //general IO updates + CAN

/* ------| Task Stack Sizes |------ */
//setting stack sizes for each stack to the minimum (128 words)
#define INIT_TASK_STACK_SIZE                configMINIMAL_STACK_SIZE
#define READ_CAR_CAN_STACK_SIZE             configMINIMAL_STACK_SIZE
#define READ_CONTROLS_CAN_STACK_SIZE        configMINIMAL_STACK_SIZE
#define SEND_SWITCH_STATES_STACK_SIZE     	configMINIMAL_STACK_SIZE

#define NUM_STATUS_LEDS		6
#define BPS_WATCHDOG_TIMEOUT_MS	   1000

#define HAZARD_PERIOD_TICKS				pdMS_TO_TICKS(500)

/* ------| TCBs |------ */
// Task Control Blocks for each task
extern StaticTask_t INIT_TASK_TCB;
extern StaticTask_t READ_CAR_CAN_TCB;
extern StaticTask_t READ_CONTROLS_CAN_TCB;
extern StaticTask_t SEND_SWITCH_STATES_TCB;

extern TaskHandle_t ReadControlsCAN_TaskHandle;


/* ------| Tasks |------ */

void InitTasks(void *argument);

void ReadCarCAN_task(void *argument);

void ReadControlsCAN_task(void *argument);

void Task_Send_Switch_States(void *argument);

