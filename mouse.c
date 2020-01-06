#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"
#include "mouse.h"

int ms_hook_id = 2;
int mouse_id = KBD_AUX_IRQ;
uint8_t BYTE1; // output byte
u32_t scan_code_mouse=0;
uint32_t num_packets=0;
uint8_t byte;

int (mouse_subscribe_int)(uint8_t *bit_no) //Subscribed mouse interrupts
{
    *bit_no = (uint8_t) ms_hook_id;

    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &ms_hook_id)) return 1;

    return 0;
}

int (mouse_unsubscribe)()  //Unsubscribed mouse interrupts
{
    if(sys_irqrmpolicy(&ms_hook_id)) return 1;

    return 0;
}

void (mouse_ih)()
{
    uint8_t status;
    if (util_sys_inb(STAT_REG,&status)!=0) return;
    if ((status & OBF)==1)
    {
        if ((((status & BIT(5))<<2)>>7)==1) {
            if (util_sys_inb(OUT_BUF, &BYTE1) != 0) return;
            if (((status & BIT(7)) >> 7) == 1)BYTE1 = 0;
            if ((((status & BIT(6)) << 1) >> 7) == 1)BYTE1 = 0;
        }
    }
    else BYTE1=0;
}


int (issueCommand)(port_t port, u32_t cmd) //Reads the status register to check for errors and writes to port specified in first argument the command in the second argument
{
    uint32_t stat;

    while( true ) {
        if(sys_inb(STAT_REG, &stat)) return 1;
        /* loop while 8042 input buffer is not empty */
        if( (stat & BIT(1)) == 0 ) {
            if(sys_outb(port, cmd)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

int (write_mouse_command)(u32_t command) //Writes the command specified in its argument to the mouse and checks the acknowledgement
    {
    uint32_t response;

    while(true) { //If the response to the command is a NACK the function tries to write it again
        if( issueCommand(0x64, 0xD4)) return 1;
        if( issueCommand(0x60, command)) return 1;

        if( sys_inb(0x60, &response)) return 1;

        if(response == ERROR) return 1; //If the response is an error, the function returns 1
        else if (response == ACK) return 0; //If the response is OK the function return 0

        tickdelay(micros_to_ticks(DELAY_US));
    }
}


void (packet_processing)(uint8_t *bytes,struct packet *pp){
    pp->bytes[0]=bytes[0];
    pp->bytes[1]=bytes[1];
    pp->bytes[2]=bytes[2];

    pp->lb=(bytes[0] & LB);
    pp->rb=((bytes[0] & RB)>>1);
    pp->mb=((bytes[0] & MB)>>2);
    pp->x_ov=((bytes[0] & OVF_X)>>4);
    pp->y_ov=((bytes[0] & OVF_Y)>>5);
    pp->delta_x=(int16_t)bytes[1];
    pp->delta_y=(int16_t)bytes[2];

    if((bytes[0]&MSB_X)!=0) pp->delta_x |= (0x00FF<<8);
    if((bytes[0]&MSB_Y)!=0) pp->delta_y |= (0x00FF<<8);
}
