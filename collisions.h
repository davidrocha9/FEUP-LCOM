#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "sprite.h"
/**
 * @brief detects collisions based on coordinates
 * @param spr1 sprite 1
 * @param spr2 sprite 2
 * @return true if there is collisions between sprites' coordinates
 */
bool (detectCollision)(Sprite* spr1, Sprite* spr2);
