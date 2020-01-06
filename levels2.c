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
#include "Sprites/papyrus1.xpm"
#include "Sprites/papyrus2.xpm"
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

void hitPapyrus(int t_irq, int k_irq, int m_irq, uint8_t *init_address, uint8_t *temp_address, Sprite* boss){
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

void NormalLevel(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq){
    drawBackground(background1, addrs);
    Sprite *hpSprite;
    hpSprite = create_sprite(hp100, 100, 800, 10, 0);
    draw_sprite(hpSprite, addrs);
    Sprite *papyrusSprite;
    papyrusSprite = create_sprite(papyrus1, 430, 50, 10, 0);
    draw_sprite(papyrusSprite, addrs);
    Sprite *balaoFala;
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
                                draw_sprite(balaoFala, addrs);
                                Fala("human.", addrs, init_addrs, 6, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("allow me to tell you/about some complex/feelings.", addrs, init_addrs, 49, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("feelings like...", addrs, init_addrs, 16, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                draw_sprite(balaoFala, addrs);
                                Fala("the joy of finding/another pasta lover.", addrs, init_addrs, 39, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 4:
                                draw_sprite(balaoFala, addrs);
                                Fala("the admiration for/another_s puzzle/solving skills.", addrs, init_addrs, 51, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 5:
                                draw_sprite(balaoFala, addrs);
                                Fala("i can hardly imagine/what it must be like/to feel that way.", addrs, init_addrs, 59, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 6:
                                draw_sprite(balaoFala, addrs);
                                Fala("after all/i am very great.", addrs, init_addrs, 26, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 7:
                                draw_sprite(balaoFala, addrs);
                                Fala("i pity you lonely/human.", addrs, init_addrs, 24, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 8:
                                draw_sprite(balaoFala, addrs);
                                Fala("worry not!!!/you shall be lonely/no longer!", addrs, init_addrs, 43, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 9:
                                draw_sprite(balaoFala, addrs);
                                Fala("nyeh heh heh!", addrs, init_addrs, 13, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 10:
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

void NormalLevel2(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p){
    drawBackground(background1, addrs);
    Sprite *papyrusSprite;
    papyrusSprite = create_sprite(papyrus1, 430, 50, 10, 0);
    draw_sprite_2(papyrusSprite, addrs);
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
    hitPapyrus(t_irq, k_irq, m_irq, init_addrs, addrs, papyrusSprite);
    Sprite *balaoFala;
    balaoFala = create_sprite(balaofala, 640, 50, 10, 0);
    draw_sprite(balaoFala, addrs);
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    int gamestate = 0;
    bool done = false;
    papyrusSprite->xspeed = -7;
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
                                draw_sprite_2(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                HalfSec(t_irq, k_irq, m_irq);
                                erase_sprite_screen(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                HalfSec(t_irq, k_irq, m_irq);
                                draw_sprite_2(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                HalfSec(t_irq, k_irq, m_irq);
                                erase_sprite_screen(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                HalfSec(t_irq, k_irq, m_irq);
                                draw_sprite_2(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                gamestate++;
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("no.../this is all wrong!!!", addrs, init_addrs, 26, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("i can_t be your/friend!!!", addrs, init_addrs, 25, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                draw_sprite(balaoFala, addrs);
                                Fala("i am a skeleton with/very high standards!", addrs, init_addrs, 41, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 4:
                                draw_sprite(balaoFala, addrs);
                                Fala("you are a human!", addrs, init_addrs, 16, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 5:
                                draw_sprite(balaoFala, addrs);
                                Fala("i must capture you!", addrs, init_addrs, 19, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 6:
                                draw_sprite(balaoFala, addrs);
                                Fala("then i can fulfill/my lifelong dream!", addrs, init_addrs, 37, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 7:
                                draw_sprite(balaoFala, addrs);
                                Fala("popular!/powerful!/prestigious!!!", addrs, init_addrs, 33, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 8:
                                draw_sprite(balaoFala, addrs);
                                Fala("that_s papyrus!", addrs, init_addrs, 15, t_irq, k_irq, m_irq);
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

void NormalLevel3(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq, struct Player p){
    drawBackground(background1, addrs);
    Sprite *papyrusSprite;
    papyrusSprite = create_sprite(papyrus1, 430, 50, 10, 0);
    draw_sprite_2(papyrusSprite, addrs);
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
    hitPapyrus(t_irq, k_irq, m_irq, init_addrs, addrs, papyrusSprite);
    Sprite *balaoFala;
    balaoFala = create_sprite(balaofala, 640, 50, 10, 0);
    draw_sprite(balaoFala, addrs);
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    int gamestate = 0;
    bool done = false;
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
                                papyrusSprite = create_sprite(papyrus2, 430, 50, 10, 0);
                                draw_sprite_2(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                HalfSec(t_irq, k_irq, m_irq);
                                erase_sprite_screen(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                HalfSec(t_irq, k_irq, m_irq);
                                draw_sprite_2(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                HalfSec(t_irq, k_irq, m_irq);
                                erase_sprite_screen(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                HalfSec(t_irq, k_irq, m_irq);
                                draw_sprite_2(papyrusSprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                gamestate++;
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("w..well/that_s not what i/expected...", addrs, init_addrs,37, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("but.../st... still!/i believe in you!", addrs, init_addrs,37, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                draw_sprite(balaoFala, addrs);
                                Fala("you can do a little/bit better!", addrs, init_addrs,31, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 4:
                                draw_sprite(balaoFala, addrs);
                                Fala("even if you don_t/think so!", addrs, init_addrs,27, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 5:
                                draw_sprite(balaoFala, addrs);
                                Fala("not a bad time/to be beaten...", addrs, init_addrs,30, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 6:
                                draw_sprite(balaoFala, addrs);
                                FalaDate(date, addrs, init_addrs, 8, t_irq, k_irq, m_irq);
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

extern int mouse_id;
extern uint8_t BYTE1;

struct Player NormalFirstWave(uint8_t *temp_address, uint8_t *init_adress, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr){
    int ipc_status, r;
    u8_t seconds=0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    draw_sprite(spr, temp_address);
    Sprite* bone1;
    bone1 = create_sprite(bone, 102, 450, 0, 0);
    Sprite* bordas1;
    bordas1 = create_sprite(bordas, 0, 0, 0, 0);
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    bool bonescol = false;
    Sprite *hpSprite;
    int index = 0;
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite, temp_address);
    bool pass = true;


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
                        if (bone1->x >= 879 || bonescol == true) {
                            erase_sprite_screen(bone1, temp_address);
                            bonescol = false;
                            bone1->x = 102;
                            bone1->y = 470 + rand() % 250;
                        }
                        move_bones(bone1, temp_address, init_adress, bonescol);
                        if(detectCollision(spr, bone1) && bonescol == false){
                            p.life -= 20;
                            if (p.life == 0){
                                pass = false;
                                EndGame(spr,temp_address,init_adress,t_irq, k_irq, m_irq);
                                return p;
                            }
                            erase_sprite_screen(bone1, temp_address);
                            bone1->y = 0;
                            bone1->x = 0;
                            bonescol = true;
                            index++;
                            hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
                            draw_sprite_2(hpSprite, temp_address);
                            break;
                        }
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

struct Player NormalSecondWave(uint8_t *temp_address, uint8_t *init_adress, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr){
    int ipc_status, r;
    u8_t seconds=0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    NormalLevel2(temp_address, init_adress, t_irq, k_irq, m_irq, p);
    draw_sprite(spr, temp_address);
    Sprite* bone2;
    bone2 = create_sprite(bone, 102, 450, 0, 0);
    Sprite* bone3;
    bone3 = create_sprite(bone1, 102, 450, 0, 0);
    Sprite* bordas1;
    bordas1 = create_sprite(bordas, 0, 0, 0, 0);
    bool bonescol = false;
    Sprite *hpSprite;
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    int switchbone = 0;
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
                        if ((bone2->x >= 879 || bonescol == true) && switchbone == 0) {
                            erase_sprite_screen(bone2, temp_address);
                            bone2 = create_sprite(bone1, 102, 450, 0, 0);
                            bonescol = false;
                            bone2->x = 102;
                            bone2->y = 470 + rand() % 150;
                            switchbone = 1;
                        }
                        if ((bone2->x >= 950 || bonescol == true) && switchbone == 1) {
                            erase_sprite_screen(bone2, temp_address);
                            bone2 = create_sprite(bone, 102, 450, 0, 0);
                            bonescol = false;
                            bone2->x = 102;
                            bone2->y =  470 + rand() % 250;
                            switchbone = 0;
                        }
                        move_bones(bone2, temp_address, init_adress, bonescol);
                        if(detectCollision(spr, bone2) && bonescol == false){
                            p.life -= 20;
                            if (p.life == 0){
                                pass = false;
                                EndGame(spr,temp_address,init_adress,t_irq, k_irq, m_irq);
                                return p;
                            }
                            erase_sprite_screen(bone2, temp_address);
                            bone2->y = 0;
                            bone2->x = 0;
                            bonescol = true;
                            index++;
                            hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
                            draw_sprite_2(hpSprite, temp_address);
                            break;
                        }
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

void PapyrusLevel(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t *init_adress, int k_irq, int t_irq, int m_irq){
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

    NormalLevel(temp_address, init_adress, t_irq, k_irq, m_irq);
    updateBuffer(init_adress, temp_address);
    struct Player p1;
    p1.life = 100;
    Sprite* spr;
    spr = create_sprite(heart, xi, yi, 0, 0);
    p1 = NormalFirstWave(temp_address, init_adress, k_irq, t_irq, m_irq, p1, xi, yi, spr);
    updateBuffer(init_adress, temp_address);
    p1 = NormalSecondWave(temp_address, init_adress, k_irq, t_irq, m_irq, p1, xi, yi, spr);
    updateBuffer(init_adress, temp_address);
    NormalLevel3(temp_address, init_adress, t_irq, k_irq, m_irq, p1);
    updateBuffer(init_adress, temp_address);

    return;
}
