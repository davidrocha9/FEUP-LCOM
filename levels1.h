    #pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "graphicscard.h"
#include "i8042.h"
#include "keyboard.h"
#include "collisions.h"
#include "utils.h"

/** @defgroup levels1 levels1
 * @{
 * Functions related to the extreme level
 */

/**
 * @brief function for attacking the level's boss
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 * @param init_address main buffer
 * @param temp_address secondary buffer
 */
void hitFlowey(int t_irq, int k_irq, int m_irq, uint8_t *init_address, uint8_t *temp_address);
/**
 * @brief first dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void EasyLevel(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq);
/**
 * @brief second dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void EasyLevel2(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq);
/**
 * @brief third dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void EasyLevel3(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq);
/**
 * @brief fourth dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void EasyLevel4(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq);
/**
 * @brief first game wave of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void EasyFirstWave(uint8_t *addrs, uint8_t *init_addrs, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr);
/**
 * @brief main level's loop
 * @param xpm player's xpm
 * @param xi initial x coordinates
 * @param yi initial y coordinates
 * @param xf final x coordinates
 * @param yf final y coordinates
 * @param speed sprite's speed
 * @param init_adress main buffer
 * @param k_irq keyboard interruptions
 * @param t_irq timer interruptions
 * @param m_irq mouse interruptions
 */
void FloweyLevel(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t *init_adress, int k_irq, int t_irq, int m_irq);
/** @} end of levels1 */
