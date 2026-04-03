#pragma once

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
#define POLLING_WRITE_CAN_PRIORITY       (tskIDLE_PRIORITY+2) //general IO updates, BPS fault detection is more important

/* ------| Task Stack Sizes |------ */
//setting stack sizes for each stack to the minimum (128 words)
#define INIT_TASK_STACK_SIZE                configMINIMAL_STACK_SIZE
#define READ_CAR_CAN_STACK_SIZE             configMINIMAL_STACK_SIZE
#define READ_CONTROLS_CAN_STACK_SIZE        configMINIMAL_STACK_SIZE
#define READ_STEERING_ANGLE_CAN_STACK_SIZE  configMINIMAL_STACK_SIZE
#define POLLING_WRITE_CAN_STACK_SIZE     configMINIMAL_STACK_SIZE

/* ------| TCBs |------ */
// Task Control Blocks for each task
extern StaticTask_t INIT_TASK_TCB;
extern StaticTask_t READ_CAR_CAN_TCB;
extern StaticTask_t READ_CONTROLS_CAN_TCB;
extern StaticTask_t READ_STEERING_ANGLE_CAN_TCB;
extern StaticTask_t POLLING_WRITE_CAN_TCB;

/* ------| Task Functions |------ */

/**
 * @brief   Sets the PWM drive of the specified fan directly. Works only with fan speed control (FSC) disabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   duty_cycle PWM duty cycle percentage (0 to 100)
 * @return  OK if successful, ERR otherwise
 */
void InitTasks(void *argument);

/**
 * @brief   Sets the PWM drive of the specified fan directly. Works only with fan speed control (FSC) disabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   duty_cycle PWM duty cycle percentage (0 to 100)
 * @return  OK if successful, ERR otherwise
 */
void ReadCarCAN_task(void *argument);

/**
 * @brief   Sets the PWM drive of the specified fan directly. Works only with fan speed control (FSC) disabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   duty_cycle PWM duty cycle percentage (0 to 100)
 * @return  OK if successful, ERR otherwise
 */
void ReadControlsCAN_task(void *argument);

/**
 * @brief   Sets the PWM drive of the specified fan directly. Works only with fan speed control (FSC) disabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   duty_cycle PWM duty cycle percentage (0 to 100)
 * @return  OK if successful, ERR otherwise
 */
void ReadSteeringAngCAN_task(void *argument);

/**
 * @brief   Sets the PWM drive of the specified fan directly. Works only with fan speed control (FSC) disabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   duty_cycle PWM duty cycle percentage (0 to 100)
 * @return  OK if successful, ERR otherwise
 */
void Polling_WriteCAN_Task(void *argument);

/* ------| Switch States & Status LED Event Groups |------ */
// extern EventGroupHandle_t SWITCH_STATES_EG;
// extern EventGroupHandle_t STATUS_LED_STATES_EG;
