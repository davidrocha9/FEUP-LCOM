#include <lcom/lcf.h>
#include "levels1.h"
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
#include "Sprites/hp20.xpm"
#include "Sprites/hp40.xpm"
#include "Sprites/hp60.xpm"
#include "Sprites/hp80.xpm"
#include "Sprites/flowey.xpm"
#include "Sprites/flowey2.xpm"
#include "Sprites/flowey3.xpm"
#include "Sprites/flowey4.xpm"
#include "keyboard.h"
#include "mouse.h"

extern int interrupts;
extern uint8_t scancode;
extern uint8_t BYTE1;

void hitFlowey(int t_irq, int k_irq, int m_irq, uint8_t *init_address, uint8_t *temp_address){
    int ipc_status, r;
    uint8_t elapsedt = 0;
    message msg;
    Sprite *boss;
    boss = create_sprite(flowey3, 410, 50, 10, 0);
    draw_sprite(boss, temp_address);
    Sprite* spr;
    spr = create_sprite(crosshair, 100, 100, 0, 0);
    draw_sprite_2(spr, temp_address);
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

void EasyLevel(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq){
    drawBackground(background1, addrs);
    Sprite *hpSprite;
    hpSprite = create_sprite(hp100, 100, 800, 10, 0);
    draw_sprite(hpSprite, addrs);
    Sprite* spr;
    spr = create_sprite(heart, 490, 678, 0, 0);
    draw_sprite(spr, addrs);
    Sprite *floweySprite;
    floweySprite = create_sprite(flowey1, 410, 50, 10, 0);
    draw_sprite(floweySprite, addrs);
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
                                Fala("howdy!/i_m flowey!/flowey the flower!", addrs, init_addrs, 37, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("see that heart? that/is your soul. the/very culmination of/your being!", addrs, init_addrs, 70, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("your soul starts off/weak but can grow/strong if you gain/a lot of hp.", addrs, init_addrs, 70, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                draw_sprite(balaoFala, addrs);
                                Fala("what_s hp stand for?/happiness of course!", addrs, init_addrs, 41, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 4:
                                draw_sprite(balaoFala, addrs);
                                Fala("you want some/happiness don_t you?", addrs, init_addrs, 35, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 5:
                                draw_sprite(balaoFala, addrs);
                                Fala("don_t worry/i_ll share some/with you!", addrs, init_addrs, 37, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 6:
                                draw_sprite(balaoFala, addrs);
                                Fala("down here happiness/is shared through...", addrs, init_addrs, 40, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 7:
                                draw_sprite(balaoFala, addrs);
                                Fala("little white/_happiness bones_", addrs, init_addrs, 30, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 8:
                                draw_sprite(balaoFala, addrs);
                                Fala("are you ready?", addrs, init_addrs, 14, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 9:
                                draw_sprite(balaoFala, addrs);
                                Fala("move around get/as many as you can!", addrs, init_addrs, 35, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
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

void EasyLevel2(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq){
    drawBackground(background1, addrs);
    Sprite *hpSprite;
    hpSprite = create_sprite(hp100, 100, 800, 10, 0);
    draw_sprite(hpSprite, addrs);
    Sprite* spr;
    spr = create_sprite(heart, 490, 678, 0, 0);
    draw_sprite(spr, addrs);
    Sprite *floweySprite;
    floweySprite = create_sprite(flowey2, 410, 50, 10, 0);
    draw_sprite_2(floweySprite, addrs);
    Sprite *balaoFala;
    Sprite *eyeball1, *eyeball2;
    eyeball1 = create_sprite(eyeballs, 515, 120, 10, 0);
    eyeball2 = create_sprite(eyeballs, 579, 120, 10, 0);
    balaoFala = create_sprite(balaofala, 640, 50, 10, 0);
    draw_sprite(balaoFala, addrs);
    updateBuffer(init_addrs,addrs);
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
                                Fala("hey buddy you/missed them.", addrs, init_addrs, 26, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("you know what_s going/on here don_t you?", addrs, init_addrs, 40, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("you just wanted/to see me suffer.", addrs, init_addrs, 33, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                floweySprite = create_sprite(flowey3, 410, 50, 10, 0);
                                draw_sprite_2(balaoFala, addrs);
                                draw_sprite_2(floweySprite, addrs);
                                draw_sprite(balaoFala, addrs);
                                Fala("die.", addrs, init_addrs, 4, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
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

void EasyLevel3(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq){
    drawBackground(background1, addrs);
    Sprite *hpSprite;
    hpSprite = create_sprite(hp100, 100, 800, 10, 0);
    draw_sprite(hpSprite, addrs);
    Sprite* spr;
    spr = create_sprite(heart, 490, 678, 0, 0);
    draw_sprite(spr, addrs);
    Sprite *floweySprite;
    floweySprite = create_sprite(flowey2, 410, 50, 10, 0);
    //erase_sprite_screen(floweySprite, addrs);
    floweySprite = create_sprite(flowey3, 410, 50, 10, 0);
    draw_sprite_2(floweySprite, addrs);
    Sprite *balaoFala;
    Sprite *eyeball1, *eyeball2;
    eyeball1 = create_sprite(eyeballs, 515, 120, 10, 0);
    eyeball2 = create_sprite(eyeballs, 579, 120, 10, 0);
    balaoFala = create_sprite(balaofala, 640, 50, 10, 0);
    draw_sprite(balaoFala, addrs);
    updateBuffer(init_addrs,addrs);
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
                                Fala("you fool.", addrs, init_addrs, 9, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("in this world/it_s kill or be/killed.", addrs, init_addrs, 37, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                Fala("why would anyone pass/an opportunity like/this?", addrs, init_addrs, 47, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 3:
                                draw_sprite(balaoFala, addrs);
                                Fala("die.", addrs, init_addrs, 4, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                draw_sprite(balaoFala, addrs);
                                erase_sprite_screen(balaoFala, addrs);
                                updateBuffer(init_addrs, addrs);
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

void EasyLevel4(uint8_t *addrs, uint8_t *init_addrs, int t_irq, int k_irq, int m_irq){
    drawBackground(background1, addrs);
    Sprite *hpSprite;
    hpSprite = create_sprite(hp100, 100, 800, 10, 0);
    draw_sprite(hpSprite, addrs);
    Sprite* spr;
    spr = create_sprite(heart, 490, 678, 0, 0);
    draw_sprite(spr, addrs);
    Sprite *floweySprite;
    floweySprite = create_sprite(flowey4, 410, 50, 10, 0);
    draw_sprite(floweySprite, addrs);
    updateBuffer(init_addrs,addrs);
    Sprite* balaoFala;
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
                        if (interrupts % 60)
                            elapsedt++;
                        updateBuffer(init_addrs, addrs);
                        switch (gamestate) {
                            case 0:
                                TwoSec(t_irq, k_irq, m_irq);
                                OneSec(t_irq, k_irq, m_irq);
                                erase_sprite_screen(floweySprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                draw_sprite(floweySprite, addrs);
                                updateBuffer(init_addrs, addrs);
                                gamestate++;
                                break;
                            case 1:
                                draw_sprite(balaoFala, addrs);
                                Fala("what an awful time/to be defeated.", addrs, init_addrs, 34, t_irq, k_irq, m_irq);
                                updateBuffer(init_addrs, addrs);
                                TwoSec(t_irq, k_irq, m_irq);
                                gamestate++;
                                break;
                            case 2:
                                draw_sprite(balaoFala, addrs);
                                FalaDate(date, addrs, init_addrs, 8, t_irq, k_irq, m_irq);
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

void EasyFirstWave(uint8_t *temp_address, uint8_t *init_adress, int k_irq, int t_irq, int m_irq, struct Player p, uint16_t xi, uint16_t yi, Sprite* spr){
    int ipc_status, r;
    u8_t seconds=0;
    message msg;
    struct packet pack;
    uint8_t bytes[3];
    int size = 0;
    draw_sprite(spr, temp_address);
    Sprite* bone1;
    bone1 = create_sprite(bone, 102, 640, 0, 0);
    Sprite* bordas1;
    bordas1 = create_sprite(bordas, 0, 0, 0, 0);
    xpm_map_t hps[] = {hp100, hp80, hp60, hp40, hp20};
    bool bonescol = false;
    Sprite *hpSprite;
    int index = 0;
    hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
    draw_sprite(hpSprite, temp_address);

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
                        if (bone1->x >= 879) {
                            erase_sprite_screen(bone1, temp_address);
                            bone1->x = 0;
                            bone1->y = 0;
                            bonescol = true;
                            EasyLevel2(temp_address, init_adress, t_irq, k_irq, m_irq);
                            return;
                        }
                        updateBuffer(init_adress, temp_address);
                        move_with_arrows(spr, temp_address, init_adress);
                        move_bones(bone1, temp_address, init_adress, bonescol);
                        if(detectCollision(spr, bone1) && bonescol == false){
                            p.life -= 20;
                            index++;
                            erase_sprite_screen(bone1, temp_address);
                            bone1->y = 0;
                            bone1->x = 0;
                            bonescol = true;
                            hpSprite = create_sprite(hps[index], 100, 800, 10, 0);
                            draw_sprite_2(hpSprite, temp_address);
                            EasyLevel3(temp_address, init_adress, t_irq, k_irq, m_irq);
                            return;
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
    return;
}

void FloweyLevel(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t *init_adress, int k_irq, int t_irq, int m_irq){
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

    EasyLevel(temp_address, init_adress, t_irq, k_irq, m_irq);
    updateBuffer(init_adress, temp_address);
    struct Player p1;
    p1.life = 100;
    Sprite* spr;
    spr = create_sprite(heart, xi, yi, 0, 0);
    EasyFirstWave(temp_address, init_adress, k_irq, t_irq, m_irq, p1, xi, yi, spr);
    updateBuffer(init_adress, temp_address);
    hitFlowey(t_irq, k_irq, m_irq, init_adress, temp_address);
    updateBuffer(init_adress, temp_address);
    EasyLevel4(temp_address, init_adress, t_irq, k_irq, m_irq);

    return;
}
