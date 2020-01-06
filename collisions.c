#include <lcom/lcf.h>
#include "collisions.h"

bool (detectCollision)(Sprite* spr1, Sprite* spr2){
    return !((spr1->x + spr1->width - 50) < spr2->x || (spr2->x + spr2->width - 50) < spr1->x || (spr1->y + spr1->height - 30) < spr2->y || (spr2->y + spr2->height - 30) < spr1->y);
}
