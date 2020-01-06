#pragma once

#include <stdint.h>

/** @defgroup graphicscard graphicscard
 * @{
 * Functions related to the graphics card
 */

/**
 * @brief initializes graphics mode
 * @param mode graphics mode
 */
void *(vg_init)(uint16_t mode);
/**
 * @brief return horizontal resolution
 * @return horizontal resolution
 */
unsigned get_hres();
/**
 * @brief return vertical resolution
 * @return vertical resolution
 */
unsigned get_vres();

/** @} end of graphicscard */
