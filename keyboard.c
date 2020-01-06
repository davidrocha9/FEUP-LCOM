#include "keyboard.h"


u32_t scancode=0;
uint32_t cnt = 0;
int kbc_id = KEYBOARD_IRQ;
uint32_t icalls = 0;
bool status_error = false;
int hook=1;

void (kbc_ih)(void) {
    u32_t stat;
    sys_inb(STAT_REG, &stat);
    if(stat & 0x01)
        sys_inb(OUT_BUF, &scancode);
    else
        status_error = true;
    if ( (stat &(0x80 | 0x40)) != 0 )
        status_error=true;
}


int (keyboard_subscribe_int)(uint8_t *bit_no)
{
    *bit_no=hook;
    if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook)!= OK)
        return 1;
    return 0;
}

int (keyboard_unsubscribe_int)()
{
    if(sys_irqrmpolicy(&hook)!= OK)
        return 1;
    return 0;
}
