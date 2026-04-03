#pragma once

#include "init.h"
#include <event_groups.h>

//convert state_bit_t -> bit mask
#define GET_MASK(bit)   ((uint16_t)(1U << (bit)))


/**
 * @brief Initializes state bitmap
 *
 * @param none
 * @return CL_OK on success, CL_EVEN_GROUP_INIT_FAIL on failure
 */
CL_status_t stateBits_init(void);

/**
 * @brief Set a state in the fault bitmap
 *
 * @param bit 	which fault is being set
 * @param state ON - 1, OFF - 0 = what you want to set the bit to
 * 
 * @return none
 */
void stateBit_set(state_bit_t bit, switch_state_t state);

/**
 * @brief Set a fault in the fault bitmap from an ISR
 *
 * @param bit which fault is being set
 * @param state ON - 1, OFF - 0 = what you want to set the bit to
 * 
 * @return none
 */
void stateBit_set_fromISR(state_bit_t bit, switch_state_t state);