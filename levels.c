#include <lcom/lcf.h>
#include "levels.h"
#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"

#include "Sprites/heart.xpm"
#include "Sprites/background.xpm"
#include "Sprites/background1.xpm"
#include "Sprites/background2.xpm"
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
#include "Sprites/hp20.xpm"
#include "Sprites/hp40.xpm"
#include "Sprites/hp60.xpm"
#include "Sprites/hp80.xpm"

extern int interrupts;
extern uint8_t scancode;
extern uint8_t BYTE1;

void hitSans(int t_irq, int k_irq, int m_irq, uint8_t *init_address, uint8_t *temp_address, Sprite* boss){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    Sprite* spr;
    spr = create_sprite(crosshair, 100, 100, 0, 0);
    draw_sprite_2(spr, temp_address);
    Sprite* bordas;
    bordas = create_sprite(background2, 76, 439, 0, 0);
    draw_sprite(bordas, temp_address);
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    int x = 0;
    while(true) {
        if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if(msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 60) == 0)
                            elapsedt++;
                        if (detectCollision(spr, boss)) {
                            draw_sprite(boss, temp_address);
                            draw_sprite(spr, temp_address);
                        }
                        else if (detectCollision(spr, bordas)) {
                            draw_sprite(bordas, temp_address);
                            draw_sprite(spr, temp_address);
                        }
                        updateBuffer(init_address, temp_address);
                    }
                    if(msg.m_notify.interrupts & k_irq) {
                        kbc_ih();
                    }
                    if (msg.m_notify.interrupts & m_irq){
                        mouse_ih();
                        x++;
                        bytes[size] = BYTE1;
                        size++;
                        if (size != 3) continue;
                        size = 0;
                        packet_processing(bytes, &pack);
                        packet_processing(bytes, &pack);
                        move_with_mouse(spr, init_address, temp_address, pack);
                        if(pack.lb == 1){
                            erase_sprite_screen(spr,temp_address);
                            return;
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }
}

void ExtremeLevel(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq){
    drawBackground(background1, addrs);
    Sprite *hpSprite;
    hpSprite = create_sprite(hp100, 100, 800, 10, 0);
    draw_sprite(hpSprite, addrs);
    Sprite *sansSprite;
    sansSprite = create_sprite(sans1, 430, 50, 10, 0);
    draw_sprite(sansSprite, addrs);
    Sprite *balaoFala;
    Sprite *eyeball1, *eyeball2;
    eyeball1 = create_sprite(eyeballs, 515, 120, 10, 0);
    eyeball2 = create_sprite(eyeballs, 579, 120, 10, 0);
    balaoFala = create_sprite(balaofala, 640, 50, 10, 0);
    draw_sprite(balaoFala, addrs);
    int ipc_status, r;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    uint8_t elapsedt = 0;
    message msg;
    int gamestate = 0;
    bool done = false;
    while(!done) {
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
                    }
                    if (msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if (interrupts % 60)
                            elapsedt++;
                        updateBuffer(init_addrs, addrs);
                        switch (gamestate) {
                            case 0:
                                Fala("it_s a beautiful/day outside.", addrs, init_addrs, 29, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("birds are singing./flowers are/blooming...", addrs, init_addrs, 42, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("on days like these.../kids like you...", addrs, init_addrs, 39, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                destroy_sprite(sansSprite);
                                sansSprite = create_sprite(sans2, 430, 50, 10, 0);
                                draw_sprite_2(sansSprite, addrs);
                                draw_sprite_2(eyeball1, addrs);
                                draw_sprite_2(eyeball2, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("should be/burning in/hell.", addrs, init_addrs, 26, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 4:
                                erase_sprite_screen(eyeball1, addrs);
                                erase_sprite_screen(eyeball2, addrs);
                                draw_sprite_2(sansSprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("here we go.", addrs, init_addrs, 11, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                gamestate++;
                                break;
                            case 5:
                                done = true;
                                break;
                            default:
                                break;
                        }
                    }
            }
        }
    }
}

void ExtremeLevel2(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p){
    drawBackground(background1, addrs);
    Sprite *sansSprite;
    sansSprite = create_sprite(sans2, 430, 50, 10, 0);
    draw_sprite_2(sansSprite, addrs);
    Sprite* hpSprite;
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    int index;
    switch(p.life){
        case 100:
            index = 0;
            break;
        case 80:
            index = 1;
            break;
        case 60:
            index = 2;
            break;
        case 40:
            index = 3;
            break;
        case 20:
            index = 4;
            break;
    }
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite,addrs);
    updateBuffer(init_addrs, addrs);
    hitSans(t_irq, k_irq, m_irq, init_addrs, addrs, sansSprite);
    Sprite *balaoFala;
    balaoFala = create_sprite(balaofala, 682, 52, 10, 0);
    draw_sprite(balaoFala, addrs);
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    int gamestate = 0;
    bool done = false;
    sansSprite->xspeed = -7;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    while(!done) {
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
                    }
                    if (msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 60) == 0)
                            elapsedt++;
                        updateBuffer(init_addrs, addrs);
                        switch (gamestate) {
                            case 0:
                                move_sans_left(sansSprite,init_addrs,addrs);
                                if (sansSprite->x < 300) {
                                    gamestate++;
                                }
                                break;
                            case 1:
                                move_sans_right(sansSprite,init_addrs,addrs);
                                if (sansSprite->x == 430) {
                                    gamestate++;
                                    sansSprite->xspeed = 7;
                                }
                                break;
                            case 2:
                                sansSprite = create_sprite(sans3, 386, 52, 10, 0);
                                draw_sprite_2(sansSprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala2("what? you think i_m/just gonna stand there/and take it?", addrs, init_addrs, 55, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                done = true;
                                break;
                        }
                    }
            }
        }
    }
}

void ExtremeLevel3(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p){
    drawBackground(background1, addrs);
    Sprite *sansSprite;
    sansSprite = create_sprite(sans2, 430, 50, 10, 0);
    draw_sprite_2(sansSprite, addrs);
    Sprite* hpSprite;
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    int index;
    switch(p.life){
        case 100:
            index = 0;
            break;
        case 80:
            index = 1;
            break;
        case 60:
            index = 2;
            break;
        case 40:
            index = 3;
            break;
        case 20:
            index = 4;
            break;
    }
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite,addrs);
    updateBuffer(init_addrs, addrs);
    hitSans(t_irq, k_irq, m_irq, init_addrs, addrs, sansSprite);
    Sprite *balaoFala;
    balaoFala = create_sprite(balaofala, 640, 50, 10, 0);
    draw_sprite(balaoFala, addrs);
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    int gamestate = 0;
    bool done = false;
    sansSprite->xspeed = -7;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    while(!done) {
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
                    }
                    if (msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 60) == 0)
                            elapsedt++;
                        updateBuffer(init_addrs, addrs);
                        switch (gamestate) {
                            case 0:
                                move_sans_left(sansSprite,init_addrs,addrs);
                                if (sansSprite->x < 300) {
                                    gamestate++;
                                }
                                break;
                            case 1:
                                move_sans_right(sansSprite,init_addrs,addrs);
                                if (sansSprite->x == 430) {
                                    gamestate++;
                                    sansSprite->xspeed = 7;
                                }
                                break;
                            case 2:
                                draw_sprite_2(sansSprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("our reports showed/a massive anomaly in/the timespace/continuum.", addrs, init_addrs,64, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                draw_sprite(balaoFala, addrs);
                                Fala("timelines jumping/left and right/stopping and starting.", addrs, init_addrs,56, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 4:
                                sansSprite = create_sprite(sans1, 430, 50, 10, 0);
                                draw_sprite(sansSprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("until suddenly/everything ends...", addrs, init_addrs,33, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 5:
                                draw_sprite(balaoFala, addrs);
                                Fala("heh heh heh...", addrs, init_addrs,14, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 6:
                                sansSprite = create_sprite(sans2, 430, 50, 10, 0);
                                draw_sprite_2(sansSprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("that_s your fault,/isn_t it?", addrs, init_addrs,28, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 7:
                                done = true;
                                break;
                            default:
                                break;
                        }
                    }
            }
        }
    }
}

void ExtremeLevel5(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p){
    drawBackground(background1, addrs);
    Sprite *sansSprite;
    sansSprite = create_sprite(sans1, 430, 50, 10, 0);
    draw_sprite_2(sansSprite, addrs);
    Sprite* hpSprite;
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    int index;
    switch(p.life){
        case 100:
            index = 0;
            break;
        case 80:
            index = 1;
            break;
        case 60:
            index = 2;
            break;
        case 40:
            index = 3;
            break;
        case 20:
            index = 4;
            break;
    }
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite,addrs);
    updateBuffer(init_addrs, addrs);
    hitSans(t_irq, k_irq, m_irq, init_addrs, addrs, sansSprite);
    Sprite *balaoFala;
    balaoFala = create_sprite(balaofala, 640, 50, 10, 0);
    //draw_sprite(balaoFala, addrs);
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    int gamestate = 0;
    bool done = false;
    sansSprite->xspeed = -7;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    Date date = read_date();

    while(!done) {
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
                    }
                    if (msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 60) == 0)
                            elapsedt++;
                        updateBuffer(init_addrs, addrs);
                        switch (gamestate) {
                            case 0:
                                erase_sprite_screen(sansSprite, addrs);
                                sansSprite = create_sprite(sans4, 430, 100, 10, 0);
                                draw_sprite_2(sansSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                gamestate++;
                                TwoSec(t_irq, k_irq, m_irq);
                                TwoSec(t_irq, k_irq, m_irq);
                                break;
                            case 1:
                                sansSprite = create_sprite(sans5, 420, 102, 10, 0);
                                draw_sprite_2(sansSprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("...", addrs, init_addrs,3, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("so...", addrs, init_addrs,5, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                draw_sprite(balaoFala, addrs);
                                Fala("guess that_s/it, huh?", addrs, init_addrs,21, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 4:
                                draw_sprite(balaoFala, addrs);
                                Fala("just...", addrs, init_addrs,7, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 5:
                                draw_sprite(balaoFala, addrs);
                                Fala("don_t say i/didn_t warn you.", addrs, init_addrs,28, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 6:
                                draw_sprite(balaoFala, addrs);
                                Fala("welp. you did it.", addrs, init_addrs, 17, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 7:
                                draw_sprite(balaoFala, addrs);
                                Fala("you defeated me at", addrs, init_addrs, 18, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 8:
                                draw_sprite(balaoFala, addrs);
                                FalaDate(date, addrs, init_addrs, 8, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 9:
                                done = true;
                                break;
                            default:
                                break;
                        }
                    }
            }
        }
    }
}

void ExtremeLevel4(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p){
    drawBackground(background1, addrs);
    Sprite *sansSprite;
    sansSprite = create_sprite(sans2, 430, 50, 10, 0);
    draw_sprite_2(sansSprite, addrs);
    Sprite* hpSprite;
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    int index;
    switch(p.life){
        case 100:
            index = 0;
            break;
        case 80:
            index = 1;
            break;
        case 60:
            index = 2;
            break;
        case 40:
            index = 3;
            break;
        case 20:
            index = 4;
            break;
    }
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite,addrs);
    updateBuffer(init_addrs, addrs);
    Sprite *balaoFala;
    balaoFala = create_sprite(balaofala, 640, 50, 10, 0);
    draw_sprite(balaoFala, addrs);
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    int gamestate = 0;
    bool done = false;
    sansSprite->xspeed = -7;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    while(!done) {
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
                    }
                    if (msg.m_notify.interrupts & t_irq) {
                        timer_int_handler();
                        if ((interrupts % 60) == 0)
                            elapsedt++;
                        updateBuffer(init_addrs, addrs);
                        switch (gamestate) {
                            case 0:
                                draw_sprite_2(sansSprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("huff...puff...", addrs, init_addrs, 14, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                gamestate++;
                                TwoSec(t_irq, k_irq, m_irq);
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("i know i can_t/beat you.", addrs, init_addrs,24, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("one of your turns/you_re just gonna/kill me.", addrs, init_addrs,44, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                draw_sprite(balaoFala, addrs);
                                Fala("so uh.../i_ve decided...", addrs, init_addrs,24, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 4:
                                draw_sprite(balaoFala, addrs);
                                Fala("it_s not gonna/be your turn./ever.", addrs, init_addrs,34, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 5:
                                draw_sprite(balaoFala, addrs);
                                Fala("i_m just gonna keep/having my turn/until you give up.", addrs, init_addrs,53, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 6:
                                draw_sprite(balaoFala, addrs);
                                Fala("even if it means/we have to stand here/until the end/of time.", addrs, init_addrs,61, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 7:
                                draw_sprite(balaoFala, addrs);
                                Fala("capiche?", addrs, init_addrs,8, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 8:
                                sansSprite = create_sprite(sans1, 430, 50, 10, 0);
                                draw_sprite_2(sansSprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("this was fun...", addrs, init_addrs,15, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                sansSprite = create_sprite(sans1, 430, 50, 10, 0);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 9:
                                draw_sprite(balaoFala, addrs);
                                Fala("zzz... zzz...", addrs, init_addrs,13, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                done = true;
                                break;
                            default:
                                break;
                        }
                    }
            }
        }
    }
}

extern int mouse_id;
extern uint8_t BYTE1;

struct Player FirstWave(uint8_t *temp_address, uint8_t *init_adress, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr){
    int ipc_status, r;
    u8_t seconds=0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    draw_sprite(spr, temp_address);
    Sprite* bone1;
    bone1 = create_sprite(bone, 102, 450, 0, 0); // 640
    Sprite* bone2;
    bone2 = create_sprite(bone, 102, 720, 0, 0);
    Sprite* bordas1;
    bordas1 = create_sprite(bordas, 0, 0, 0, 0);
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    Sprite* bones[] = {bone1, bone2};
    bool bonescol[] = {false, false, false, false};
    Sprite *hpSprite;
    int index = 0;
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite, temp_address);
    bool pass = true;
    int updown = 0;


    //int x = 0;
    while(seconds < 15)
    {
        //receiving the notification
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        //verifying the type of the notification
        if (is_ipc_notify(ipc_status))
        {
            switch (_ENDPOINT_P(msg.m_source))
            {
                case HARDWARE:
                {
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
                    }
                    if (msg.m_notify.interrupts & t_irq)
                    {
                        timer_int_handler();
                        if(interrupts%60==0)
                            seconds++;
                        updateBuffer(init_adress, temp_address);
                        move_with_arrows(spr, temp_address, init_adress);
                        if (bone1->x >= 879 || bone2->x >= 879 || (bonescol[0] == true && bonescol[1] == true)) {
                            erase_sprite_screen(bones[0], temp_address);
                            erase_sprite_screen(bones[1], temp_address);
                            bonescol[0] = false;
                            bonescol[1] = false;
                            switch (updown) {
                                case 0:
                                    bone1->x = 102;
                                    bone2->x = 102;
                                    bone1->y = 470;
                                    bone2->y = 550;
                                    updown = 1;
                                    break;
                                case 1:
                                    bone1->x = 102;
                                    bone2->x = 102;
                                    bone1->y = 640;
                                    bone2->y = 720;
                                    updown = 0;
                                    break;
                            }
                        }
                        for (unsigned int x = 0; x < sizeof(bones)/sizeof(bones[0]); x++){
                            move_bones(bones[x], temp_address, init_adress, bonescol[x]);
                        }
                        for (unsigned int x = 0; x < sizeof(bones)/sizeof(bones[0]); x++){
                            if(detectCollision(spr, bones[x]) && bonescol[x] == false){
                                p.life -= 20;
                                printf("%d\n", p.life);
                                if (p.life == 0){
                                    pass = false;
                                    EndGame(spr,temp_address,init_adress,t_irq, k_irq, m_irq);
                                    return p;
                                }
                                erase_sprite_screen(bones[x], temp_address);
                                bones[x]->y = 0;
                                bones[x]->x = 0;
                                bonescol[x] = true;
                                index++;
                                hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
                                draw_sprite_2(hpSprite, temp_address);
                                break;
                            }
                        }
                        draw_sprite(bordas1,temp_address);
                    }
                }
                default:
                    break; // no other notifications expected: do nothing
            }
        }
        else
        {
            // received a standard message, not a notification, no standard messages expected: do nothing
        }
    }
    updateBuffer(init_adress, temp_address);
    return p;
}

struct Player SecondWave(uint8_t *temp_address, uint8_t *init_adress, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr){
    int ipc_status, r;
    u8_t seconds=0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    ExtremeLevel2(temp_address, init_adress, t_irq, k_irq, m_irq, p);
    draw_sprite(spr, temp_address);
    Sprite* bone2;
    bone2 = create_sprite(bone1, 102, 620, 0, 0);
    Sprite* bone3;
    bone3 = create_sprite(bone1, 102, 460, 0, 0);
    Sprite* bordas1;
    bordas1 = create_sprite(bordas, 0, 0, 0, 0);
    Sprite* bones[] = {bone2, bone3};
    bool bonescol[] = {false, false};
    Sprite *hpSprite;
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    int index;
    switch(p.life){
        case 100:
            index = 0;
            break;
        case 80:
            index = 1;
            break;
        case 60:
            index = 2;
            break;
        case 40:
            index = 3;
            break;
        case 20:
            index = 4;
            break;
    }
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite,temp_address);
    updateBuffer(init_adress, temp_address);
    bool pass = true;
    int release = 0;
    bool passmove = false;
    while(seconds < 15)
    {
        //receiving the notification
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        //verifying the type of the notification
        if (is_ipc_notify(ipc_status))
        {
            switch (_ENDPOINT_P(msg.m_source))
            {
                case HARDWARE:
                {
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
                    }
                    if (msg.m_notify.interrupts & t_irq)
                    {
                        timer_int_handler();
                        if(interrupts%60==0) {
                            seconds++;
                            release++;
                        }
                        updateBuffer(init_adress, temp_address);
                        if (bone2->x > 960 || bonescol[0] == true) {
                            erase_sprite_screen(bones[0], temp_address);
                            bonescol[0] = false;
                            bone2->x = 102;
                            bone2->y = 620;
                            pass = false;
                            release = 0;
                        }
                        else if (bone3->x > 960 || bonescol[1] == true){
                            erase_sprite_screen(bones[1], temp_address);
                            bonescol[1] = false;
                            bone3->x = 102;
                            bone3->y = 460;
                        }
                        move_bones(bone2, temp_address, init_adress, bonescol[0]);
                        move_with_arrows(spr, temp_address, init_adress);
                        if (release >= 1 || passmove == true) {
                            move_bones(bone3, temp_address, init_adress, bonescol[1]);
                            passmove = true;
                        }
                        for (unsigned int x = 0; x < sizeof(bones)/sizeof(bones[0]); x++){
                            if(detectCollision(spr, bones[x]) && bonescol[x] == false){
                                p.life -= 20;
                                if (p.life == 20){
                                    EndGame(spr, temp_address, init_adress, t_irq, k_irq, m_irq);
                                    pass = false;
                                    return p;
                                }
                                erase_sprite_screen(bones[x], temp_address);
                                bones[x]->y = 0;
                                bones[x]->x = 0;
                                bonescol[x] = true;
                                index++;
                                hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
                                draw_sprite_2(hpSprite, temp_address);
                                break;
                            }
                        }
                        draw_sprite(bordas1,temp_address);
                    }
                }
                default:
                    break; // no other notifications expected: do nothing
            }
        }
        else
        {
            // received a standard message, not a notification, no standard messages expected: do nothing
        }
    }
    return p;
}

struct Player ThirdWave(uint8_t *temp_address, uint8_t *init_adress, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr){
    int ipc_status, r;
    u8_t seconds=0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    ExtremeLevel3(temp_address, init_adress, t_irq, k_irq, m_irq, p);
    draw_sprite(spr, temp_address);
    Sprite* bone2;
    bone2 = create_sprite(bone1, 102, 620, 0, 0);
    Sprite* bone3;
    bone3 = create_sprite(bone1, 102, 460, 0, 0);
    Sprite* bordas1;
    bordas1 = create_sprite(bordas, 0, 0, 0, 0);
    Sprite* bones[] = {bone2, bone3};
    bool bonescol[] = {false, false};
    Sprite *hpSprite;
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    int index;
    switch(p.life){
        case 100:
            index = 0;
            break;
        case 80:
            index = 1;
            break;
        case 60:
            index = 2;
            break;
        case 40:
            index = 3;
            break;
        case 20:
            index = 4;
            break;
    }
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite,temp_address);
    updateBuffer(init_adress, temp_address);
    bool pass = true;
    int release = 0;
    bool passmove = false;
    while(seconds < 15)
    {
        //receiving the notification
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        //verifying the type of the notification
        if (is_ipc_notify(ipc_status))
        {
            switch (_ENDPOINT_P(msg.m_source))
            {
                case HARDWARE:
                {
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
                    }
                    if (msg.m_notify.interrupts & t_irq)
                    {
                        timer_int_handler();
                        if(interrupts%60==0) {
                            seconds++;
                            release++;
                        }
                        updateBuffer(init_adress, temp_address);
                        if (bone2->x > 960 || bonescol[0] == true) {
                            erase_sprite_screen(bones[0], temp_address);
                            bonescol[0] = false;
                            bone2->x = 102;
                            bone2->y = 620;
                            pass = false;
                            release = 0;
                        }
                        else if (bone3->x > 960 || bonescol[1] == true){
                            erase_sprite_screen(bones[1], temp_address);
                            bonescol[1] = false;
                            bone3->x = 102;
                            bone3->y = 460;
                        }
                        move_bones(bone2, temp_address, init_adress, bonescol[0]);
                        move_with_arrows(spr, temp_address, init_adress);
                        if (release >= 1 || passmove == true) {
                            move_bones(bone3, temp_address, init_adress, bonescol[1]);
                            passmove = true;
                        }
                        for (unsigned int x = 0; x < sizeof(bones)/sizeof(bones[0]); x++){
                            if(detectCollision(spr, bones[x]) && bonescol[x] == false){
                                p.life -= 20;
                                if (p.life == 20){
                                    EndGame(spr, temp_address, init_adress, t_irq, k_irq, m_irq);
                                    pass = false;
                                    return p;
                                }
                                erase_sprite_screen(bones[x], temp_address);
                                bones[x]->y = 0;
                                bones[x]->x = 0;
                                bonescol[x] = true;
                                index++;
                                hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
                                draw_sprite_2(hpSprite, temp_address);
                                break;
                            }
                        }
                        //draw_sprite(bordas1,temp_address);
                    }
                }
                default:
                    break; // no other notifications expected: do nothing
            }
        }
        else
        {
            // received a standard message, not a notification, no standard messages expected: do nothing
        }
    }
    return p;
}

void SansLevel(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t *init_adress, int k_irq, int t_irq, int m_irq){
    uint8_t *temp_address = (uint8_t*) malloc(1152*864*4);
    memset(temp_address, 0, 1152*864*4);
    scancode = 0;
    if (abs(xi-xf)!=0 && abs(yi-yf)!=0)
        return;
    bool speed_neg=false;
    //inicializar o modo gráfico
    if (speed<0){
        speed_neg=true;
        speed=1;
    }
    speed=abs(speed);

    struct Player p1;
    p1.life = 100;
    ExtremeLevel(temp_address, init_adress, t_irq, k_irq, m_irq);
    updateBuffer(init_adress, temp_address);
    Sprite* spr;
    spr = create_sprite(heart, xi, yi, 0, 0);
    p1 = FirstWave(temp_address, init_adress, k_irq, t_irq, m_irq, p1, xi, yi, spr);
    updateBuffer(init_adress, temp_address);
    p1 = SecondWave(temp_address, init_adress, k_irq, t_irq, m_irq, p1, xi, yi, spr);
    updateBuffer(init_adress, temp_address);
    p1 = ThirdWave(temp_address, init_adress, k_irq, t_irq, m_irq, p1, xi, yi, spr);
    updateBuffer(init_adress, temp_address);
    ExtremeLevel4(temp_address, init_adress, t_irq, k_irq, m_irq, p1);
    ExtremeLevel5(temp_address, init_adress, t_irq, k_irq, m_irq, p1);
    updateBuffer(init_adress, temp_address);
}
