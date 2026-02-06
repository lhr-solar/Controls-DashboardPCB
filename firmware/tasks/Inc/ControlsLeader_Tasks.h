#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include <event_groups.h>

//TODO
// /* ------| Event Group for System Faults |------ */
// extern EventGroupHandle_t SystemEventGroup;
// //BPS fault bit
// #define BPS_FAULT_BIT       (1 << 0)
// #define CONTROLS_FAULT_BIT  (1 << 1)

// /* ------| BPS WatchDog timer |------ */
// extern TimerHandle_t BPS_WatchdogTimer;
// void BPS_WatchdogCallback(TimerHandle_t xTimer);


/* ------| Task Priorities |------ */
#define INIT_TASK_PRIORITY                  (tskIDLE_PRIORITY+1)
#define READ_CAR_CAN_PRIORITY               (tskIDLE_PRIORITY+3) //BPS fault detection
#define READ_CONTROLS_CAN_PRIORITY          (tskIDLE_PRIORITY+4) //want to run immidetly after CAN ISR
#define READ_STEERING_ANGLE_CAN_PRIORITY    (tskIDLE_PRIORITY+4) //want to run immidetly after CAN ISR
#define UPDATE_SWITCH_STATES_PRIORITY       (tskIDLE_PRIORITY+2) //general IO updates, BPS fault detection is more important

/* ------| Task Stack Sizes |------ */
//setting stack sizes for each stack to the minimum (128 words)
#define INIT_TASK_STACK_SIZE                configMINIMAL_STACK_SIZE
#define READ_CAR_CAN_STACK_SIZE             configMINIMAL_STACK_SIZE
#define READ_CONTROLS_CAN_STACK_SIZE        configMINIMAL_STACK_SIZE
#define READ_STEERING_ANGLE_CAN_STACK_SIZE  configMINIMAL_STACK_SIZE
#define UPDATE_SWITCH_STATES_STACK_SIZE     configMINIMAL_STACK_SIZE

/* ------| TCBs |------ */
// Task Control Blocks for each task
extern StaticTask_t INIT_TASK_TCB;
extern StaticTask_t READ_CAR_CAN_TCB;
extern StaticTask_t READ_CONTROLS_CAN_TCB;
extern StaticTask_t READ_STEERING_ANGLE_CAN_TCB;
extern StaticTask_t UPDATE_SWITCH_STATES_TCB;

/* ------| Task Functions |------ */
void InitTasks(void *argument);
void ReadCarCAN_task(void *argument);
void ReadControlsCAN_task(void *argument);
void ReadSteeringAngCAN_task(void *argument);
void UpdateSwitchStatesCAN_task(void *argument);

#endif