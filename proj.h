#pragma once

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

int main(int argc, char *argv[]);
/**
 * @brief draws menu which shows levels' difficulties
 * @param addrs buffer
 * @param setaSprite menu's arrow
 * @param k_irq keyboard interruptions
 * @param t_irq timer interruptions
 * @param m_irq mouse interruptions
 */
void (draw_menu2)(uint8_t *addrs, Sprite *setaSprite, int k_irq, int t_irq, int m_irq);
/**
 * @brief draws help menu
 * @param addrs buffer
 * @param setaSprite menu's arrow
 * @param k_irq keyboard interruptions
 * @param t_irq timer interruptions
 * @param m_irq mouse interruptions
 */
void (draw_help)(uint8_t *addrs, Sprite *setaSprite, int k_irq, int t_irq, int m_irq);
/**
 * @brief main menu level
 * @param address buffer
 * @return 0 if successful, 1 otherwise
 */
int MainMenu(void *address);
int(proj_main_loop)(int argc, char *argv[]);
