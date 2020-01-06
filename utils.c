
#include <lcom/lcf.h>
#include <lcom/utils.h>
#include "utils.h"
#include "rtc.h"

#include "Sprites/heart.xpm"
#include "Sprites/background.xpm"
#include "Sprites/background1.xpm"
#include "Sprites/blackbackg.xpm"
#include "Sprites/sans1.xpm"
#include "Sprites/sans2.xpm"
#include "Sprites/sans3.xpm"
#include "Sprites/sans4.xpm"
#include "Sprites/sans5.xpm"
#include "Sprites/hp100.xpm"
#include "Sprites/crosshair.xpm"
#include "Sprites/balaodefala.xpm"
#include "Sprites/eyeballs.xpm"
#include "Sprites/gameover.xpm"
#include "Sprites/blacksquare.xpm"
#include "Sprites/bone.xpm"
#include "Sprites/bone1.xpm"
#include "Sprites/bac.xpm"
#include "Sprites/Letras/a.xpm"
#include "Sprites/Letras/b.xpm"
#include "Sprites/Letras/c.xpm"
#include "Sprites/Letras/d.xpm"
#include "Sprites/Letras/e.xpm"
#include "Sprites/Letras/f.xpm"
#include "Sprites/Letras/g.xpm"
#include "Sprites/Letras/h.xpm"
#include "Sprites/Letras/i.xpm"
#include "Sprites/Letras/j.xpm"
#include "Sprites/Letras/k.xpm"
#include "Sprites/Letras/l.xpm"
#include "Sprites/Letras/m.xpm"
#include "Sprites/Letras/n.xpm"
#include "Sprites/Letras/o.xpm"
#include "Sprites/Letras/p.xpm"
#include "Sprites/Letras/q.xpm"
#include "Sprites/Letras/r.xpm"
#include "Sprites/Letras/s.xpm"
#include "Sprites/Letras/t.xpm"
#include "Sprites/Letras/u.xpm"
#include "Sprites/Letras/v.xpm"
#include "Sprites/Letras/w.xpm"
#include "Sprites/Letras/x.xpm"
#include "Sprites/Letras/y.xpm"
#include "Sprites/Letras/z.xpm"
#include "Sprites/Letras/g2.xpm"
#include "Sprites/Letras/e2.xpm"
#include "Sprites/Letras/t2.xpm"
#include "Sprites/Letras/d2.xpm"
#include "Sprites/Letras/u2.xpm"
#include "Sprites/Letras/n2.xpm"
#include "Sprites/Letras/k2.xpm"
#include "Sprites/Letras/o2.xpm"
#include "Sprites/Letras/exclamacao2.xpm"
#include "Sprites/Letras/apostrofe.xpm"
#include "Sprites/Letras/pontofinal.xpm"
#include "Sprites/Letras/exclamacao.xpm"
#include "Sprites/Letras/interrogacao.xpm"
#include "Sprites/Letras/0.xpm"
#include "Sprites/Letras/1.xpm"
#include "Sprites/Letras/2.xpm"
#include "Sprites/Letras/3.xpm"
#include "Sprites/Letras/4.xpm"
#include "Sprites/Letras/5.xpm"
#include "Sprites/Letras/6.xpm"
#include "Sprites/Letras/7.xpm"
#include "Sprites/Letras/8.xpm"
#include "Sprites/Letras/9.xpm"
#include "Sprites/Letras/doispontos.xpm"
#include "Sprites/hp20.xpm"
#include "Sprites/hp40.xpm"
#include "Sprites/hp60.xpm"
#include "Sprites/hp80.xpm"

#include <stdint.h>

extern int interrupts;
extern uint8_t scancode;
extern uint8_t BYTE1;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  val &= 0x00FF;
  *lsb = val;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  val &= 0xFF00;
  val >>= 8;
  *msb = val;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t new;
  sys_inb(port, &new);
  *value = new;

  return 0;
}

bool EndGame(Sprite* spr, uint8_t *addrs, uint8_t *init_address, int t_irq, int k_irq, int m_irq){
    Sprite* blackbackground;
    blackbackground = create_sprite(blackbackg, 0, 0, 0, 0);
    draw_sprite_2(blackbackground, addrs);
    erase_sprite_screen(spr, addrs);
    Sprite* blackbox1;
    blackbox1 = create_sprite(blackbox, spr->x, spr->y, 0, 0);
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    int next = 0;
    bool pass = false;
    draw_sprite(spr, addrs);
    int y = spr->y;

    while(!pass) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size] = BYTE1;
                        size++;
                        if (size != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if(msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 60) == 0) {
                            elapsedt++;
                        }
                        updateBuffer(init_address, addrs);
                        if(elapsedt == 1) {
                            switch (next) {
                                case 0:
                                    blackbox1->y = y - 110;
                                    draw_sprite_2(blackbox1, addrs);
                                    next++;
                                    elapsedt = 0;
                                    break;
                                case 1:
                                    blackbox1->y = y - 80;
                                    draw_sprite_2(blackbox1, addrs);
                                    elapsedt = 0;
                                    next++;
                                    break;
                                case 2:
                                    draw_sprite_2(blackbackground, addrs);
                                    pass = true;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    if(msg.m_notify.interrupts & k_irq) {
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
    updateBuffer(init_address, addrs);
    OneSec(t_irq, k_irq, m_irq);
    Sprite* gameover1;
    gameover1 = create_sprite(gameover, 270, 100, 0, 0);
    draw_sprite_2(gameover1, addrs);
    updateBuffer(init_address, addrs);
    FalaBranco("geeet dunked on !!!!!!", addrs, init_address, 22, t_irq, k_irq, m_irq);
    updateBuffer(init_address, addrs);
    TwoSec(t_irq, k_irq, m_irq);
    return true;
}

void drawBackground(xpm_map_t map, uint8_t *addrs){
    xpm_map_t menup = map;
    xpm_image_t menup_info;
    uint8_t *menu = xpm_load(menup, XPM_8_8_8_8, &menup_info);
    if (menu == NULL) {
        if (vg_exit())
            return;
        return;
    }
    for (uint16_t i = 0; i < 4 * menup_info.height; i += 4) {
        if ((i) >= 4*get_vres())
            break;
        for (uint16_t j = 0; j < 4 * menup_info.width; j++) {
            if ((j) >= 4*get_hres())
                break;
            uint8_t *pixel = addrs + ((i) *get_hres() + j);
            *pixel = menu[i * menup_info.width + j];
        }
    }
}

void TwoSec(int t_irq, int k_irq, int m_irq){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    while(elapsedt != 2) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size] = BYTE1;
                        size++;
                        if (size != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if(msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 60) == 0) {
                            elapsedt++;
                        }
                    }
                    if(msg.m_notify.interrupts & k_irq) {
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
}

void OneSec(int t_irq, int k_irq, int m_irq){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    while(elapsedt != 1) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size] = BYTE1;
                        size++;
                        if (size != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if(msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 60) == 0) {
                            elapsedt++;
                        }
                    }
                    if(msg.m_notify.interrupts & k_irq) {
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
}

void HalfSec(int t_irq, int k_irq, int m_irq){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    while(elapsedt != 1) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size] = BYTE1;
                        size++;
                        if (size != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if(msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 10) == 0) {
                            elapsedt++;
                        }
                    }
                    if(msg.m_notify.interrupts & k_irq) {
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
}

void Fala(char frase[], uint8_t *addrs, uint8_t *init_addrs, int size, int t_irq, int k_irq, int m_irq){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size1 = 0;
    char ch = {'a'};
    Sprite *letra;
    int index = 0, x = 710, y = 95;
    while(index < size) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size1] = BYTE1;
                        size1++;
                        if (size1 != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if(msg.m_notify.interrupts & t_irq){
                        timer_int_handler();
                        if ((interrupts % 3) == 0) {
                            elapsedt++;
                        }
                        updateBuffer(init_addrs, addrs);
                        if (elapsedt == 1){
                            ch = frase[index];
                            if (strncmp(frase, "should be/burning in/hell.",26) == 0)
                                HalfSec(t_irq, k_irq, m_irq);
                            switch(ch){
                                case '1':
                                    letra = create_sprite(um, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '2':
                                    letra = create_sprite(dois, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '3':
                                    letra = create_sprite(tres, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 4;
                                    break;
                                case '4':
                                    letra = create_sprite(quatro, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 4;
                                    break;
                                case '5':
                                    letra = create_sprite(cinco, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '6':
                                    letra = create_sprite(seis, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '7':
                                    letra = create_sprite(sete, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 2;
                                    break;
                                case '8':
                                    letra = create_sprite(oito, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 2;
                                    break;
                                case '9':
                                    letra = create_sprite(nove, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 2;
                                    break;
                                case ':':
                                    letra = create_sprite(doispontos, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '!':
                                    letra = create_sprite(exclamacao, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '?':
                                    letra = create_sprite(interrogacao, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '_':
                                    letra = create_sprite(apostrofe, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'a':
                                    letra = create_sprite(a, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'b':
                                    letra = create_sprite(b1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'c':
                                    letra = create_sprite(c, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'd':
                                    letra = create_sprite(d, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'e':
                                    letra = create_sprite(e, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'f':
                                    letra = create_sprite(f, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'g':
                                    y += 14;
                                    letra = create_sprite(g, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 14;
                                    break;
                                case 'h':
                                    letra = create_sprite(h, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'i':
                                    letra = create_sprite(i, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'j':
                                    y += 10;
                                    letra = create_sprite(j, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'k':
                                    letra = create_sprite(k, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'l':
                                    letra = create_sprite(l, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'm':
                                    letra = create_sprite(m, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x+=3;
                                    break;
                                case 'n':
                                    letra = create_sprite(n, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'o':
                                    letra = create_sprite(o, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'p':
                                    y += 10;
                                    letra = create_sprite(p, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'q':
                                    y += 10;
                                    letra = create_sprite(q, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'r':
                                    letra = create_sprite(r1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 's':
                                    letra = create_sprite(s, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 't':
                                    letra = create_sprite(t, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'u':
                                    letra = create_sprite(u, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'v':
                                    letra = create_sprite(v, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'w':
                                    letra = create_sprite(w, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 3;
                                    break;
                                case 'x':
                                    letra = create_sprite(x1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'y':
                                    y += 14;
                                    letra = create_sprite(y1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 14;
                                    break;
                                case 'z':
                                    letra = create_sprite(z, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '/':
                                    x = 695;
                                    y += 40;
                                    break;
                                case '.':
                                    letra = create_sprite(pontofinal, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                default:
                                    break;
                            }
                            x += 15;
                            index++;
                            elapsedt = 0;
                        }
                    }
                    if(msg.m_notify.interrupts & k_irq){
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
    return;
}

void FalaDate(Date data, uint8_t *addrs, uint8_t *init_addrs, int size, int t_irq, int k_irq, int m_irq){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size1 = 0;
    char ch = {'a'};
    char nr = {'a'};
    char frase[8];
    switch(data.hour / 10){
        case 0:
            nr = '0';
            break;
        case 1:
            nr = '1';
            break;
        case 2:
            nr = '2';
            break;
        default:
            break;
    }
    frase[0] = nr;
    switch(data.hour % 10){
        case 0:
            nr = '0';
            break;
        case 1:
            nr = '1';
            break;
        case 2:
            nr = '2';
            break;
        case 3:
            nr = '3';
            break;
        case 4:
            nr = '4';
            break;
        case 5:
            nr = '5';
            break;
        case 6:
            nr = '6';
            break;
        case 7:
            nr = '7';
            break;
        case 8:
            nr = '8';
            break;
        case 9:
            nr = '9';
            break;
        default:
            nr = '0';
            break;
    }
    frase[1] = nr;
    frase[2] = ':';
    switch(data.minute / 10){
        case 0:
            nr = '0';
            break;
        case 1:
            nr = '1';
            break;
        case 2:
            nr = '2';
            break;
        case 3:
            nr = '3';
            break;
        case 4:
            nr = '4';
            break;
        case 5:
            nr = '5';
            break;
        case 6:
            nr = '6';
            break;
        case 7:
            nr = '7';
            break;
        case 8:
            nr = '8';
            break;
        case 9:
            nr = '9';
            break;
        default:
            break;
    }
    frase[3] = nr;
    switch(data.minute % 10){
        case 0:
            nr = '0';
            break;
        case 1:
            nr = '1';
            break;
        case 2:
            nr = '2';
            break;
        case 3:
            nr = '3';
            break;
        case 4:
            nr = '4';
            break;
        case 5:
            nr = '5';
            break;
        case 6:
            nr = '6';
            break;
        case 7:
            nr = '7';
            break;
        case 8:
            nr = '8';
            break;
        case 9:
            nr = '9';
            break;
        default:
            break;
    }
    frase[4] = nr;
    frase[5] = ':';
    switch(data.second / 10){
        case 0:
            nr = '0';
            break;
        case 1:
            nr = '1';
            break;
        case 2:
            nr = '2';
            break;
        case 3:
            nr = '3';
            break;
        case 4:
            nr = '4';
            break;
        case 5:
            nr = '5';
            break;
        case 6:
            nr = '6';
            break;
        case 7:
            nr = '7';
            break;
        case 8:
            nr = '8';
            break;
        case 9:
            nr = '9';
            break;
        default:
            break;
    }
    frase[6] = nr;
    switch(data.second % 10){
        case 0:
            nr = '0';
            break;
        case 1:
            nr = '1';
            break;
        case 2:
            nr = '2';
            break;
        case 3:
            nr = '3';
            break;
        case 4:
            nr = '4';
            break;
        case 5:
            nr = '5';
            break;
        case 6:
            nr = '6';
            break;
        case 7:
            nr = '7';
            break;
        case 8:
            nr = '8';
            break;
        case 9:
            nr = '9';
            break;
        default:
            break;
    }
    frase[7] = nr;
    Sprite *letra;
    int index = 0, x = 710, y = 95;
    while(index < size) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size1] = BYTE1;
                        size1++;
                        if (size1 != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                    }
                    if(msg.m_notify.interrupts & t_irq){
                        timer_int_handler();
                        if ((interrupts % 3) == 0) {
                            elapsedt++;
                        }
                        updateBuffer(init_addrs, addrs);
                        if (elapsedt == 1){
                            ch = frase[index];
                            if (strncmp(frase, "should be/burning in/hell.",26) == 0)
                                HalfSec(t_irq, k_irq, m_irq);
                            switch(ch){
                                case '0':
                                    letra = create_sprite(zero, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x +=2 ;
                                    break;
                                case '1':
                                    letra = create_sprite(um, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '2':
                                    letra = create_sprite(dois, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '3':
                                    letra = create_sprite(tres, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 4;
                                    break;
                                case '4':
                                    letra = create_sprite(quatro, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 4;
                                    break;
                                case '5':
                                    letra = create_sprite(cinco, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '6':
                                    letra = create_sprite(seis, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '7':
                                    letra = create_sprite(sete, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 2;
                                    break;
                                case '8':
                                    letra = create_sprite(oito, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 2;
                                    break;
                                case '9':
                                    y -=2;
                                    letra = create_sprite(nove, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 2;
                                    y +=2;
                                    break;
                                case ':':
                                    letra = create_sprite(doispontos, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '!':
                                    letra = create_sprite(exclamacao, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '?':
                                    letra = create_sprite(interrogacao, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '_':
                                    letra = create_sprite(apostrofe, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'a':
                                    letra = create_sprite(a, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'b':
                                    letra = create_sprite(b1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'c':
                                    letra = create_sprite(c, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'd':
                                    letra = create_sprite(d, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'e':
                                    letra = create_sprite(e, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'f':
                                    letra = create_sprite(f, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'g':
                                    y += 14;
                                    letra = create_sprite(g, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 14;
                                    break;
                                case 'h':
                                    letra = create_sprite(h, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'i':
                                    letra = create_sprite(i, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'j':
                                    y += 10;
                                    letra = create_sprite(j, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'k':
                                    letra = create_sprite(k, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'l':
                                    letra = create_sprite(l, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'm':
                                    letra = create_sprite(m, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x+=3;
                                    break;
                                case 'n':
                                    letra = create_sprite(n, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'o':
                                    letra = create_sprite(o, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'p':
                                    y += 10;
                                    letra = create_sprite(p, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'q':
                                    y += 10;
                                    letra = create_sprite(q, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'r':
                                    letra = create_sprite(r1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 's':
                                    letra = create_sprite(s, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 't':
                                    letra = create_sprite(t, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'u':
                                    letra = create_sprite(u, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'v':
                                    letra = create_sprite(v, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'w':
                                    letra = create_sprite(w, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 3;
                                    break;
                                case 'x':
                                    letra = create_sprite(x1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'y':
                                    y += 14;
                                    letra = create_sprite(y1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 14;
                                    break;
                                case 'z':
                                    letra = create_sprite(z, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '/':
                                    x = 695;
                                    y += 40;
                                    break;
                                case '.':
                                    letra = create_sprite(pontofinal, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                default:
                                    break;
                            }
                            x += 15;
                            index++;
                            elapsedt = 0;
                        }
                    }
                    if(msg.m_notify.interrupts & k_irq){
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
    return;
}

void Fala2(char frase[], uint8_t *addrs, uint8_t *init_addrs, int size, int t_irq, int k_irq, int m_irq){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size1 = 0;
    char ch = {'a'};
    Sprite *letra;
    int index = 0, x = 745, y = 95;
    while(index < size) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & m_irq) {
                        mouse_ih();
                        bytes[size1] = BYTE1;
                        size1++;
                        if (size1 != 3) continue;
                        size1 = 0;
                        packet_processing(bytes, &pack);
                    }
                    if(msg.m_notify.interrupts & t_irq){
                        timer_int_handler();
                        if ((interrupts % 3) == 0) {
                            elapsedt++;
                        }
                        updateBuffer(init_addrs, addrs);
                        if (elapsedt == 1){
                            ch = frase[index];
                            if (strncmp(frase, "should be/burning in/hell.",26) == 0)
                                HalfSec(t_irq, k_irq, m_irq);
                            switch(ch){
                                case '!':
                                    letra = create_sprite(exclamacao, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '?':
                                    letra = create_sprite(interrogacao, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '_':
                                    letra = create_sprite(apostrofe, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'a':
                                    letra = create_sprite(a, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'b':
                                    letra = create_sprite(b1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'c':
                                    letra = create_sprite(c, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'd':
                                    letra = create_sprite(d, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'e':
                                    letra = create_sprite(e, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'f':
                                    letra = create_sprite(f, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'g':
                                    y += 14;
                                    letra = create_sprite(g, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 14;
                                    break;
                                case 'h':
                                    letra = create_sprite(h, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'i':
                                    letra = create_sprite(i, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'j':
                                    y += 10;
                                    letra = create_sprite(j, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'k':
                                    letra = create_sprite(k, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'l':
                                    letra = create_sprite(l, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'm':
                                    letra = create_sprite(m, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x+=3;
                                    break;
                                case 'n':
                                    letra = create_sprite(n, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'o':
                                    letra = create_sprite(o, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'p':
                                    y += 10;
                                    letra = create_sprite(p, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'q':
                                    y += 10;
                                    letra = create_sprite(q, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 10;
                                    break;
                                case 'r':
                                    letra = create_sprite(r1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 's':
                                    letra = create_sprite(s, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 't':
                                    letra = create_sprite(t, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'u':
                                    letra = create_sprite(u, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'v':
                                    letra = create_sprite(v, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'w':
                                    letra = create_sprite(w, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    x += 3;
                                    break;
                                case 'y':
                                    y += 14;
                                    letra = create_sprite(y1, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 14;
                                    break;
                                case 'z':
                                    letra = create_sprite(z, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '/':
                                    x = 730;
                                    y += 40;
                                    break;
                                case '.':
                                    letra = create_sprite(pontofinal, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                default:
                                    break;
                            }
                            x += 15;
                            index++;
                            elapsedt = 0;
                        }
                    }
                    if(msg.m_notify.interrupts & k_irq){
                        kbc_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
    return;
}

void FalaBranco(char frase[], uint8_t *addrs, uint8_t *init_addrs,int size, int t_irq, int k_irq, int m_irq){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;

    char ch = {'a'};
    Sprite *letra;
    int index = 0, x = 300, y = 500;
    while(index < size) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if(msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 6) == 0) {
                            elapsedt++;
                        }
                        updateBuffer(init_addrs, addrs);
                        if (elapsedt == 1){
                            ch = frase[index];
                            switch(ch){
                                case 'd':
                                    letra = create_sprite(d2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'e':
                                    letra = create_sprite(e2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'g':
                                    y += 14;
                                    letra = create_sprite(g2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    y -= 14;
                                    break;
                                case 'k':
                                    letra = create_sprite(k2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'n':
                                    letra = create_sprite(n2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'o':
                                    letra = create_sprite(o2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 't':
                                    letra = create_sprite(t2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case 'u':
                                    letra = create_sprite(u2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                case '/':
                                    x = 695;
                                    y += 40;
                                    break;
                                case '!':
                                    letra = create_sprite(exclamacao2, x, y, 10, 0);
                                    draw_sprite_2(letra, addrs);
                                    break;
                                default:
                                    break;
                            }
                            x += 20;
                            index++;
                            elapsedt = 0;
                        }
                        if(msg.m_notify.interrupts & k_irq){
                            kbc_ih();
                        }
                        if(msg.m_notify.interrupts & m_irq){
                            mouse_ih();
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
    updateBuffer(init_addrs, addrs);
    return;
}
