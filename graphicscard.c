#include "graphicscard.h"
#include <lcom/lcf.h>
#include <math.h> 

static char *address;          /* Process (virtual) address to which VRAM is mapped */
static unsigned h_res;           /* Horizontal resolution in pixels */
static unsigned v_res;           /* Vertical resolution in pixels */
static unsigned bits_per_pixel;  /* Number of VRAM bits per pixel */
int bytesPerPixel;
vbe_mode_info_t vmi_p;	


void *(map_address)(uint16_t mode) {
  vbe_mode_info_t info;
  vbe_get_mode_info(mode, &info);
  v_res = info.YResolution;
  h_res = info.XResolution;
  bits_per_pixel = info.BitsPerPixel;
  struct minix_mem_range mr;
  unsigned int vram_size = h_res * v_res * ((bits_per_pixel + 7) / 8);
  unsigned int vram_base = info.PhysBasePtr;
  void *video_mem;
  int r;
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");	
  return video_mem;
}	

int(GraphicsMode)(uint16_t mode) {
  struct minix_mem_range mr;
  struct reg86 r;
  mr.mr_limit = 1 << 20;
  mr.mr_base = 0;
  sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
  memset(&r, 0, sizeof(r));
  r.bx = 1 << 14 | mode;
  r.intno = 0x10;
  r.ax = 0x4F02;
  if (sys_int86(&r) != OK)
    return 1;
  return 0;
}

void *(vg_init)(uint16_t mode) {
  address = map_address(mode);
  if (address == NULL)
    return NULL;
  if (GraphicsMode(mode) == 1) {
    return NULL;
  }
  return address;
}

unsigned get_vres() {
  return v_res;
}

unsigned get_hres() {
  return h_res;
}
