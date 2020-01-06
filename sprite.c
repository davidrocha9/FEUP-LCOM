#include "collisions.h"
#include "graphicscard.h"
#include "Sprites/heart.xpm"
#include "Sprites/heart2.xpm"
#include "Sprites/background.xpm"
#include "Sprites/background1.xpm"
#include "Sprites/blackbackg.xpm"
#include "i8042.h"
#include "keyboard.h"
#include "Sprites/sans1.xpm"
#include "Sprites/sans2.xpm"
#include "Sprites/balaodefala.xpm"
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
#include "Sprites/Letras/apostrofe.xpm"
#include "Sprites/Letras/pontofinal.xpm"
#include "Sprites/Letras/exclamacao.xpm"
#include "Sprites/Letras/interrogacao.xpm"
#include<stdio.h>
#include<string.h>
#include <lcom/lcf.h>


/** Creates a new sprite with pixmap pic, with specified
* position (within the screen limits) and speed;
* Returns NULL on invalid pixmap.
*/

#define WHITE 0xFF
#define BLACK 0x00

extern int interrupts;
extern bool status_error;
extern uint8_t scancode;

Sprite *create_sprite(xpm_map_t pic, int x, int y, int xspeed, int yspeed) {
  xpm_image_t img;
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  if (sp == NULL)
    return NULL;
  sp->map = xpm_load(pic, XPM_8_8_8_8, &img);
  sp->width = img.width;
  sp->height = img.height;
  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }
  sp->x = x;
  sp->y = y;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  return sp;
}

void destroy_sprite(Sprite *sp) {
    if (sp == NULL)
        return;
    if (sp->map)
        free(sp->map);
    free(sp);
    sp = NULL;
}

Sprite *animate_sprite(Sprite *sp){
    if (sp->xspeed != 0){
        sp->x += sp->xspeed;
    }
    if (sp->yspeed != 0){
        sp->y += sp->yspeed;
    }
    return sp;
}

Sprite *draw_sprite(Sprite *sp, uint8_t *base) {
    uint8_t* map = sp->map;

    for (int i = 0; i < 4*sp->height; i+=4){
        if ((unsigned)(i+sp->y) >=  4*864) break;
        for (int j = 0; j < 4*sp->width; j++){
            if ((unsigned)(sp->x+j) >= 4*1152){
                break;
            }
            else if (map[i*sp->width+j] != (uint8_t) xpm_transparency_color(XPM_8_8_8_8)){
                    uint8_t* ptr = base + ((4*sp->y+i)*1152) + 4*sp->x+j;
                    *ptr = map[i*sp->width+j];
                }
            }
        }
    return sp;
}

Sprite *draw_sprite_2(Sprite *sp, uint8_t *base) {
    uint8_t* map = sp->map;

    for (int i = 0; i < 4*sp->height; i+=4){
        if ((unsigned)(i+sp->y) >=  4*864) break;
        for (int j = 0; j < 4*sp->width; j++){
            if ((unsigned)(sp->x+j) >= 4*1152){
                break;
            }
            else if (map[i*sp->width+j] == (uint8_t) xpm_transparency_color(XPM_8_8_8_8)){
                uint8_t* ptr = base + ((4*sp->y+i)*1152) + 4*sp->x+j;
                *ptr = BLACK;
            }
            else{
                uint8_t* ptr = base + ((4*sp->y+i)*1152) + 4*sp->x+j;
                *ptr = map[i*sp->width+j];
            }
        }
    }
    return sp;
}

Sprite *(erase_sprite_screen)(Sprite* spr, uint8_t* base) {
    //uint8_t* map = spr->map;

    for (int i = 0; i< 4*spr->height; i+=4) {
        if (spr->y + i >= 4 * 864)break;
        for (int j = 0; j < 4 * spr->width; j++) {
            if (spr->x + j >= 4 * 1152) break;
            else if (spr->map[i*spr->width+j] != (uint8_t) xpm_transparency_color(XPM_8_8_8_8)){
                uint8_t *ptr = base + ((4 * spr->y + i) * 1152) + 4 * spr->x + j;
                *ptr = 0x00;

            }
        }
    }
    return spr;
}

Sprite *move_sprite(uint8_t* init, Sprite* map){
    erase_sprite_screen(map, init);
    animate_sprite(map);
    draw_sprite(map, init);
    return map;
}

Sprite *move_sans(Sprite* spr, uint8_t *init_adress, uint8_t *temp_address){
    if(spr->x > 100) {
        spr->xspeed = -5;
    }
    else{
        spr->xspeed = 5;
    }
    spr = move_sprite(init_adress, spr);
    updateBuffer(temp_address, init_adress);
    return spr;
}

Sprite *move_bones(Sprite* spr, uint8_t *init_adress, uint8_t *addrs, bool pass){
    if(!pass) {
        spr->xspeed = 10;
        spr = move_sprite(init_adress, spr);
        updateBuffer(addrs, init_adress);
    }
    else{
        erase_sprite_screen(spr, init_adress);
        spr->x = 102;
    }

    return spr;
}

Sprite *move_sans_left(Sprite* spr, uint8_t *init_adress, uint8_t *addrs){
    spr = move_sprite(init_adress, spr);
    updateBuffer(addrs, init_adress);
    return spr;
}

Sprite *move_sans_right(Sprite* spr, uint8_t *init_adress, uint8_t *addrs){
    spr->xspeed = 7;
    spr = move_sprite(init_adress, spr);
    updateBuffer(addrs, init_adress);
    return spr;
}

Sprite *move_with_arrows(Sprite* spr, uint8_t *init_adress, uint8_t *addrs){
    int newspeed;
    if ((spr->x <= 938 && 42 <= spr->x) && (spr->y <= 686 && 426 <= spr->y)) {
        switch(scancode){
            case RIGHT:
                spr->yspeed = 0;
                spr->xspeed = 14;
                if (spr->xspeed < 0)
                    newspeed = -spr->xspeed;
                else
                    newspeed = spr->xspeed;
                spr->xspeed = newspeed;
                if (spr->x + 14 != 952) {
                    spr = move_sprite(init_adress, spr);
                }
                break;
            case LEFT:
                spr->yspeed = 0;
                spr->xspeed = 14;
                if (spr->xspeed < 0)
                    newspeed = spr->xspeed;
                else
                    newspeed = -spr->xspeed;
                spr->xspeed = newspeed;
                if (spr->x - 14 != 28)
                    spr = move_sprite(init_adress, spr);
                break;
            case UP:
                spr->xspeed = 0;
                spr->yspeed = 14;
                if (spr->yspeed < 0)
                    newspeed = spr->yspeed;
                else
                    newspeed = -spr->yspeed;
                spr->yspeed = newspeed;
                if (spr->y - 14 != 412)
                    spr = move_sprite(init_adress, spr);
                break;
            case DOWN:
                spr->xspeed = 0;
                spr->yspeed = 14;
                if (spr->yspeed < 0)
                    newspeed = -spr->yspeed;
                else
                    newspeed = spr->yspeed;
                spr->yspeed = newspeed;
                if (spr->y + 8 != 686)
                    spr = move_sprite(init_adress, spr);
                break;
            default:
                break;
        }
    }
    updateBuffer(addrs, init_adress);
    return spr;
}

Sprite *move_with_mouse(Sprite* spr, uint8_t *init_adress, uint8_t *addrs, struct packet pp){
    if ((spr->x + pp.delta_x<= 1004 && 0 <= spr->x - pp.delta_x)) {
        spr->yspeed = -pp.delta_y;
        spr->xspeed = pp.delta_x;
        if ((spr->y + pp.delta_y<= 690 && 0 <= spr->y - pp.delta_y)) {
            if ((spr->x + spr->xspeed <= 999) && (5 < spr->x + pp.delta_x)) {
                if ((spr->y + spr->yspeed<= 680 && 0 <= spr->y - pp.delta_y)){
                    spr = move_sprite(init_adress, spr);
                }
            }
        }
    }
    updateBuffer(addrs, init_adress);
    return spr;
}

void (updateBuffer)(uint8_t* principal, uint8_t* secondary){
    memcpy(principal, secondary, 1152*864*4);
}
