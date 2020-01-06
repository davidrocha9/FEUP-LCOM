    #pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "graphicscard.h"
#include "i8042.h"
#include "keyboard.h"
#include "utils.h"

/** @defgroup levels levels
 * @{
 * Functions related to the extreme level
 */

/**
 * @brief funtion for attacking the level's boss
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 * @param init_address main buffer
 * @param temp_address secondary buffer
 * @param boss boss's sprite
 */
void hitSans(int t_irq, int k_irq, int m_irq, uint8_t *init_address, uint8_t *temp_address, Sprite* boss);
/**
 * @brief first dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void ExtremeLevel(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq);
/**
 * @brief second dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 * @param p object from struct Player
 */
void ExtremeLevel2(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p);
/**
 * @brief third dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 * @param p object from struct Player
 */
void ExtremeLevel3(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p);
/**
 * @brief fourth dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 * @param p object from struct Player
 */
void ExtremeLevel4(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p);
/**
 * @brief fifth dialogue of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 * @param p object from struct Player
 */
void ExtremeLevel5(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p);
/**
 * @brief first game wave of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param k_irq keyboard interruptions
 * @param t_irq timer interruptions
 * @param m_irq mouse interruptions
 * @param p object from struct Player
 * @param xi initial x coordinates
 * @param yi initial y coordinates
 * @param spr player's sprite
 * @param p object from struct Player
 * @return object from struct Player
 */
struct Player FirstWave(uint8_t *addrs, uint8_t *init_addrs, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr);
/**
 * @brief second game wave of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param k_irq keyboard interruptions
 * @param t_irq timer interruptions
 * @param m_irq mouse interruptions
 * @param p object from struct Player
 * @param xi initial x coordinates
 * @param yi initial y coordinates
 * @param spr player's sprite
 * @return object from struct Player
 */
struct Player SecondWave(uint8_t *temp_address, uint8_t *init_adress, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr);
/**
 * @brief third game wave of the level
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param k_irq keyboard interruptions
 * @param t_irq timer interruptions
 * @param m_irq mouse interruptions
 * @param p object from struct Player
 * @param xi initial x coordinates
 * @param yi initial y coordinates
 * @param spr player's sprite
 * @return object from struct Player
 */
struct Player ThirdWave(uint8_t *temp_address, uint8_t *init_adress, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr);
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
void SansLevel(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t *init_adress, int k_irq, int t_irq, int m_irq);

/** @} end of levels */
