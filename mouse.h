#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <minix/syslib.h>
#include <minix/sysutil.h>

enum state_t {
  INIT,     /* First state, wainting to press the left bottom  */
  DRAW,     /* 2 state, drawing the first line */
  DONELINE, /* 3 state, wainting to press the right bottom */
  DRAW1,    /* 4 state , drawing the second line*/
};

typedef enum { LDOWN,
               LUP,
               RDOWN,
               RUP,
               WRONGKEY } ev_type_t;

/** @defgroup mouse mouse
 * @{
 * Functions related to the mouse
 */

 /**
  * @brief subscribes mouse interruptions
  * @param bit_no bit number pointer set in the interrupt mask
  * @return 0 upon sucess, 1 otherwise
  */
int (mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief unsubscribes mouse notifications
 * @return 0 upon success, 1 otherwise
 */
int (mouse_unsubscribe)();
/**
 * @brief checks for error and writes cmd to the specified port
 * @param port address of the specified port
 * @param cmd command
 * @return 0 upon success, 1 otherwise
 */
int (issueCommand)(port_t port, u32_t cmd);
/**
 * @brief writes the command to the mouse
 * @param command specified command
 * @return 0 upon success, 1 otherwise
 */
int (write_mouse_command)(u32_t command);
/**
 * @briefe processes the packets from the mouse
 * @param bytes bytes from the mouse packets
 * @param pp packet
 */
void (packet_processing)(uint8_t *bytes,struct packet *pp);
/** @} end of mouse */

#endif
