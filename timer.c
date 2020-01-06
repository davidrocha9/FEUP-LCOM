#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <lcom/utils.h>

#include <stdint.h>


#include "i8254.h"



int id = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = (uint8_t) id;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &id)) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {

  if(sys_irqrmpolicy(&id)) return 1;

  return 0;
}


uint32_t interrupts = 0; 
void (timer_int_handler)() {
  interrupts++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st)
{
  if (timer < 0 || timer > 2)
    return 1;


  uint8_t mode;
  uint32_t ler = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  if(sys_outb(TIMER_CTRL, ler)) return 1;
  
  switch(timer)
  {
	  case 0:
		if (util_sys_inb(TIMER_0, &mode)) return 1;
		*st = mode;
		return 0;
	  case 1:
		if (util_sys_inb(TIMER_1, &mode)) return 1;
		*st = mode;
		return 0;
	  case 2:
		if (util_sys_inb(TIMER_2, &mode))return 1;
		*st = mode;
		return 0;
	  default:
	  
	  
		return 1;
  }
}



int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

	if (timer < 0 || timer>2) return 1;
	union timer_status_field_val VAR;
	switch (field)
	{
	case tsf_all:
	{
		VAR.byte = st;
		break;
	}
	case tsf_mode: {
		st &= 0x0E;
		st >>= 1;
		if (st == 6) 
			VAR.count_mode = 2; 
		else if (st == 7) 
			VAR.count_mode = 3;
		else
			VAR.count_mode = st;
		break;
	}
	case tsf_initial: {
		st >>= 4;
		st &= 0x30;
	
		VAR.in_mode = st;
		break;
	}
	
	case tsf_base: {
		if (st & TIMER_BCD) 
			VAR.bcd = true;
		else  VAR.bcd = false;
		break;
	}
	default:
		return 1;
	}
	if(timer_print_config(timer, field, VAR)) 
		return 1;
	return 0;
}
