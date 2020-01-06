#ifndef _KEYBOARD_H
#define _KEYBOARD_H


#pragma once
#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"

/** @defgroup keyboard keyboard
 * @{
 * Functions related to the keyboard
 */
/**
 * @brief keyboard handler
 */
void (kbc_ih)(void);
/**
 * @brief subscribes mouse interrupts
 * @param bit_no bit number pointer set in the interrupt mask
 * @return 0 upon success, 1 otherwise
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);
/**
 * @brief unsubscribes mouse interrupts
 * @return 0 upon success, 1 otherwise
 */
int (keyboard_unsubscribe_int)();
/** @} end of keyboard */

#endif /*_KEYBOARD_H */
