#include <lcom/lcf.h>
/**
 *  @author Joao Cardoso (jcard@fe.up.pt)
 *  Added by pfs@fe.up.pt
 */

/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/** A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits. 
 */
typedef struct {
  int x, y;           /**< current sprite position */
  int width, height;  /**< sprite dimensions */
  int xspeed, yspeed; /**< current speeds in the x and y direction */
  unsigned char *map; /**< the sprite pixmap (use read_xpm()) */
} Sprite;
/**
 * @brief creates sprites
 * @param pic array with the sprite's information
 * @param x x coordinate
 * @param y y coordinate
 * @param xspeed x coordinate speed
 * @param yspeed y coordinate speed
 * @return sprite
 */
Sprite *create_sprite(xpm_map_t pic, int x, int y, int xspeed, int yspeed);
/**
 * @brief destroys sprite
 * @param sp sprite
 */
void destroy_sprite(Sprite *sp);
/**
 * @brief animates the sprite
 * @param sp sprite
 * @return sprite
 */
Sprite *animate_sprite(Sprite *sp);
/**
 * @brief draws the sprite
 * @param spr sprite
 * @param base buffer
 * @return sprite
 */
Sprite *draw_sprite(Sprite *spr, uint8_t *base);
/**
 * @brief same function as draw_sprite but fills transparency with black
 * @param sp sprite
 * @param base buffer
 * @return sprite
 */
Sprite *draw_sprite_2(Sprite *sp, uint8_t *base);
/**
 * @brief erases sprite from screen
 * @param sp sprite
 * @param base buffer
 * @return sprite
 */
Sprite *erase_sprite_screen(Sprite *sp, uint8_t *base);
/**
 * @brief moves the sprite
 * @param init buffer
 * @param map sprite
 * @return sprite
 */
Sprite *move_sprite(uint8_t* init, Sprite* map);
/**
 * @brief moves sprite using arrow keys
 * @param spr sprite
 * @param init_adress main buffer
 * @param addrs secondary buffer
 * @return sprite
 */
Sprite *move_with_arrows(Sprite* spr, uint8_t *init_adress, uint8_t *addrs);
/**
 * @brief moves sprite using mouse
 * @param spr sprite
 * @param init_adress main buffer
 * @param addrs secondary buffer
 * @param pp packet from mouse
 * @return sprite
 */
Sprite *move_with_mouse(Sprite* spr, uint8_t *init_adress, uint8_t *addrs, struct packet pp);
/**
 * @brief double buffer
 * @param principal main buffer
 * @param secondary secondary buffer
 */
void (updateBuffer)(uint8_t* principal, uint8_t* secondary);
/**
 * @brief moves sans (boss from extreme level)
 * @param spr sprite
 * @param init_adress main buffer
 * @param addrs secondary buffer
 * @return sprite
 */
Sprite *move_sans(Sprite* spr, uint8_t *init_adress, uint8_t *addrs);
/**
 * @brief moves bones if they haven't collided yet
 * @param spr sprite
 * @param init_adress main buffer
 * @param addrs secondary buffer
 * @param pass true in case there has been as collision, false otherwise
 * @return sprite
 */
Sprite *move_bones(Sprite* spr, uint8_t *init_adress, uint8_t *addrs, bool pass);
/**
 * @brief moves sans right (boss from extreme level)
 * @param spr sprite
 * @param init_adress main buffer
 * @param addrs secondary buffer
 * @return sprite
 */
Sprite *move_sans_right(Sprite* spr, uint8_t *init_adress, uint8_t *addrs);
/**
 * @brief moves sans left (boss from extreme level)
 * @param spr sprite
 * @param init_adress main buffer
 * @param addrs secondary buffer
 * @return sprite
 */
Sprite *move_sans_left(Sprite* spr, uint8_t *init_adress, uint8_t *addrs);
/** @} end of sprite */
