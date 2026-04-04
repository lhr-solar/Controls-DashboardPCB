#pragma once

#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include <event_groups.h>


/* ------| Task Priorities |------ */
#define INIT_TASK_PRIORITY                  (tskIDLE_PRIORITY+1)
#define READ_CAR_CAN_PRIORITY               (tskIDLE_PRIORITY+3) //BPS fault detection
#define READ_CONTROLS_CAN_PRIORITY          (tskIDLE_PRIORITY+4) //want to run immidetly after CAN ISR
#define POLLING_WRITE_CAN_PRIORITY       (tskIDLE_PRIORITY+2) //general IO updates, BPS fault detection is more important

/* ------| Task Stack Sizes |------ */
//setting stack sizes for each stack to the minimum (128 words)
#define INIT_TASK_STACK_SIZE                configMINIMAL_STACK_SIZE
#define READ_CAR_CAN_STACK_SIZE             configMINIMAL_STACK_SIZE
#define READ_CONTROLS_CAN_STACK_SIZE        configMINIMAL_STACK_SIZE
#define POLLING_WRITE_CAN_STACK_SIZE     configMINIMAL_STACK_SIZE

/* ------| TCBs |------ */
// Task Control Blocks for each task
extern StaticTask_t INIT_TASK_TCB;
extern StaticTask_t READ_CAR_CAN_TCB;
extern StaticTask_t READ_CONTROLS_CAN_TCB;
extern StaticTask_t POLLING_WRITE_CAN_TCB;

/* ------| Task Functions |------ */


void InitTasks(void *argument);

void ReadCarCAN_task(void *argument);

void ReadControlsCAN_task(void *argument);

void Polling_WriteCAN_Task(void *argument);