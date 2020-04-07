#include <stdlib.h>
#include <string.h>

#include "pixelate.h"

const signed char adj_off[16][2] = {
  {-2,+2},
  {-1,+2},
  {+0,+2},
  {+1,+2},
  {+2,+2},
  {+2,+1},
  {+2,+0},
  {+2,-1},
  {+2,-2},
  {+1,-2},
  {+0,-2},
  {-1,-2},
  {-2,-2},
  {-2,-1},
  {-2,+0},
  {-2,+1}
};

const signed char adj_adj_off[16][3][2] = {
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3},
  {1,2,3}
};

char bit_get(unsigned char* buf,int off) {
  return (buf[off / 8]) & (128 >> (off % 8));
}

void bit_set(unsigned char* buf,int off) {
  buf[off / 8] |= 128 >> (off % 8);
}

void bit_reset(unsigned char* buf,int off) {
  buf[off / 8] &= ~(128 >> (off % 8));
}

void pixelate_auto(unsigned char* buf,int channels,int c_width,int c_height,int t_width,int t_height) {
  char* colored;

  unsigned char* palette;
  int palette_len;

  // create initialized array of booleans
  colored = malloc((c_width * c_height + 7) / 8);
  memset(colored,255,(c_width * c_height + 7) / 8);

  for (int y = 0;y < c_height;y++)
    for (int x = 0;x < c_width;x++) {
      
    }
}
