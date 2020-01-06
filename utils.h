#pragma once
#include "keyboard.h"
#include "mouse.h"
#include "collisions.h"
#include "graphicscard.h"
#include "rtc.h"
/** @defgroup utils utils
 * @{
 *
 * LCOM's utility functions
 */

#include <stdint.h>

#ifndef BIT
#define BIT(n) (1 << (n))
#endif
/**
 * @brief struct Player with players info
 */
struct Player{
    /**
     * @brief player's life, bool checks if the players has lost;
     */
    int life;
    bool dead;
};

/**
 * @brief Returns the LSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param lsb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Returns the MSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param msb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 *
 * @param port the input port that is to be read
 * @param value address of 8-bit variable to be update with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value);

/**
 * @brief function that displays endgame menu
 * @param spr player's sprite
 * @param addrs secondary buffer
 * @param init_address main buffer
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 * @return true
 */
bool EndGame(Sprite* spr, uint8_t *addrs, uint8_t *init_address, int t_irq, int k_irq, int m_irq);

/**
 * @brief draws ingame background
 * @param map array with the background information
 * @param addrs buffer
 */
void drawBackground(xpm_map_t map, uint8_t *addrs);
/**
 * @brief counts two seconds
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void TwoSec(int t_irq, int k_irq, int m_irq);
/**
 * @brief counts one second
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void OneSec(int t_irq, int k_irq, int m_irq);
/**
 * @brief counts half of a second
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void HalfSec(int t_irq, int k_irq, int m_irq);
/**
 * @brief function that allows bosses to talk
 * @param frase contains the information regarding what the boss will say
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param size array size
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void Fala(char frase[], uint8_t *addrs, uint8_t *init_addrs, int size, int t_irq, int k_irq, int m_irq);
/**
 * @brief same function as Fala but receives an object from struct Date instead of an array
 * @param frase contains the information regarding what the boss will say
 * @param addrs secondary buffer
 * @param init_addrs main buffer
 * @param size array size
 * @param t_irq timer interruptions
 * @param k_irq keyboard interruptions
 * @param m_irq mouse interruptions
 */
void FalaDate(Date data, uint8_t *addrs, uint8_t *init_addrs, int size, int t_irq, int k_irq, int m_irq);
/**
 * @brief same function as Fala but on different coordinates
 * @param frase
 * @param addrs
 * @param init_addrs
 * @param size
 * @param t_irq
 * @param k_irq
 * @param m_irq
 */
void Fala2(char frase[], uint8_t *addrs, uint8_t *init_addrs, int size, int t_irq, int k_irq, int m_irq);
/**
 * @brief same function as Fala but with white letters
 * @param frase
 * @param addrs
 * @param init_addrs
 * @param size
 * @param t_irq
 * @param k_irq
 * @param m_irq
 */
void FalaBranco(char frase[], uint8_t *addrs, uint8_t *init_addrs,int size, int t_irq, int k_irq, int m_irq);
/** @} end of utils */
