// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "Sprites/heart.xpm"
#include "Sprites/menu.xpm"
#include "Sprites/menu2.xpm"
#include "Sprites/seta.xpm"
#include "Sprites/help.xpm"
#include "graphicscard.h"
#include "keyboard.h"
#include "levels.h"
#include "levels1.h"
#include "levels2.h"
#include "mouse.h"
#include "rtc.h"
#include "proj.h"

// Any header files included below this line should have been created by you

extern uint8_t scancode;
extern uint8_t BYTE1;
extern int mouse_id;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

static int print_usage() {
  printf("Usage: <mode - hex>\n");

  return 1;
}

void (draw_menu2)(uint8_t *addrs, Sprite *setaSprite, int k_irq, int t_irq, int m_irq){
    int newy, ipc_status, r;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    message msg;
    drawBackground(menu2, addrs);
    setaSprite->x = 180;
    setaSprite->y = 335;
    draw_sprite(setaSprite, addrs);
    while(scancode != 0x81) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { //received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size] = BYTE1;
                        size++;
                        if (size != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if (msg.m_notify.interrupts & k_irq) {
                        kbc_ih();
                        switch (scancode) {
                            case UP:
                                drawBackground(menu2, addrs);
                                if (setaSprite->y == 335) {
                                    setaSprite->y = 595;
                                    setaSprite->x = 150;
                                }
                                else if (setaSprite->y == 465){
                                    newy = setaSprite->y - 130;
                                    setaSprite->x = 180;
                                    setaSprite->y = newy;
                                }
                                else{
                                    setaSprite->y = 465;
                                    setaSprite->x = 230;
                                }
                                draw_sprite(setaSprite, addrs);
                                newy = setaSprite->y;
                                break;
                            case DOWN:
                                drawBackground(menu2, addrs);
                                if (setaSprite->y == 335) {
                                    setaSprite->y = 465;
                                    setaSprite->x = 250;
                                }
                                else if (setaSprite->y == 465){
                                    setaSprite->y = 595;
                                    setaSprite->x = 150;
                                }
                                else if (setaSprite->y == 595) {
                                    setaSprite->x = 180;
                                    setaSprite->y = 335;
                                }
                                draw_sprite(setaSprite, addrs);
                                newy = setaSprite->y;
                                break;
                            case ENTER:
                                switch(setaSprite->y){
                                    case 335:
                                        FloweyLevel(heart, 490, 678, 500, 678, 10, addrs, k_irq, t_irq, m_irq);
                                        break;
                                    case 465:
                                        PapyrusLevel(heart, 490, 678, 500, 678, 10, addrs, k_irq, t_irq, m_irq);
                                        break;
                                    case 595:
                                        SansLevel(heart, 490, 678, 500, 678, 10, addrs, k_irq, t_irq, m_irq);
                                        return;
                                    default:
                                        break;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    if (msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                    }
            }
        }
    }
}

void (draw_help)(uint8_t *addrs, Sprite *setaSprite, int k_irq, int t_irq, int m_irq){
    int ipc_status, r;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    message msg;
    drawBackground(help, addrs);
    bool pass = true;
    while(pass) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { //received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size] = BYTE1;
                        size++;
                        if (size != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if (msg.m_notify.interrupts & k_irq) {
                        kbc_ih();
                        switch (scancode) {
                            case ESC_BREAKCODE:
                                pass = false;
                                break;
                            default:
                                break;
                        }
                    }
                    if (msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                    }
            }
        }
    }
}

int MainMenu(void *address){
    if (address == NULL)
        return 1;
    uint8_t *addrs = address;

    uint8_t *temp_address = (uint8_t*) malloc(1152*864*4);
    memset(temp_address, 0, 1152*864*4);

    drawBackground(menu, addrs);
    Sprite *setaSprite;
    setaSprite = create_sprite(seta, 230, 335, 10, 0);
    draw_sprite(setaSprite, addrs);

    int k_irq, t_irq, m_irq;
    uint8_t r;
    if(write_mouse_command(EN_DATA_RP)) return 1;
    if(mouse_subscribe_int(&r)) return 1;
    m_irq = BIT(r);
    if (keyboard_subscribe_int(&r) != 0){
        vg_exit();
        return 1;
    }
    k_irq = BIT(r);
    if(timer_subscribe_int(&r)) return 1;
    t_irq = BIT(r);

    int ipc_status;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    bool end = false;
    int newy = setaSprite->y;
    bool pass = false;
    while (!pass) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { //received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & k_irq) {
                        kbc_ih();
                        switch (scancode) {
                            case UP:
                                drawBackground(menu, addrs);
                                if (setaSprite->y == 335)
                                    setaSprite->y = 595;
                                else {
                                    newy = setaSprite->y - 130;
                                    setaSprite->y = newy;
                                }
                                draw_sprite(setaSprite, addrs);
                                newy = setaSprite->y;
                                break;
                            case DOWN:
                                drawBackground(menu, addrs);
                                if (setaSprite->y == 595)
                                    setaSprite->y = 335;
                                else {
                                    newy = setaSprite->y + 130;
                                    setaSprite->y = newy;
                                }
                                draw_sprite(setaSprite, addrs);
                                newy = setaSprite->y;
                                break;
                            case ENTER:
                                switch(setaSprite->y) {
                                    case 335:
                                        draw_menu2(addrs, setaSprite, k_irq, t_irq, m_irq);
                                        break;
                                    case 465:
                                        draw_help(addrs, setaSprite, k_irq, t_irq, m_irq);
                                        break;
                                    case 595:
                                        end = true;
                                        break;
                                }
                                pass = true;
                                break;
                            default:
                                break;
                        }
                    }
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size] = BYTE1;
                        size++;
                        if (size != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if (msg.m_notify.interrupts & t_irq)
                        timer_int_handler();
            }
        }
    }
    if (keyboard_unsubscribe_int())
        return 1;
    if (timer_unsubscribe_int())
        return 1;
    if (mouse_unsubscribe())
        return 1;
    if (!end)
        MainMenu(address);
    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  void *address = vg_init(0x14C);
  MainMenu(address);
  if (vg_exit())
      return 1;
  return 0;

  //
  // if you're interested, try to extend the command line options so that the usage becomes:
  // <mode - hex> <minix3 logo  - true|false> <grayscale - true|false> <delay (secs)>
  //
  bool const minix3_logo = true;
  bool const grayscale = false;
  uint8_t const delay = 5;
  uint16_t mode;

  if (argc != 1)
    return print_usage();

  // parse mode info (it has some limitations for the sake of simplicity)
  if (sscanf(argv[0], "%hx", &mode) != 1) {
    printf("%s: invalid mode (%s)\n", __func__, argv[0]);

    return print_usage();
  }

  return proj_demo(mode, minix3_logo, grayscale, delay);
}
