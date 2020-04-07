#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pixelate.h"

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

  for (int y1 = 0;y1 < c_height;y1++)
    for (int x1 = 0;x1 < c_width;x1++) {
      printf("%u:%u\n",x1,y1);

      for (int y2 = -2;y2 <= 2;y2++)
	for (int x2 = -2;x2 <= 2;x2++)
	  if ((x2 || y2) && x1 + x2 >= 0 && y1 + y2 >= 0 && x1 + x2 < c_width && y1 + y2 < c_height) {
	    unsigned char* color_min;
	    unsigned char* color_max;

	    color_min = malloc(channels);
	    color_max = malloc(channels);

	    printf("%d:%d:\t",x2,y2);

	    for (int i = 0;i < channels;i++) {
	      color_min[i] = (buf[(y1 * c_width + x1) * channels + i]
			 + buf[((y1 + y2) * c_width + (x1 + x2)) * channels + i]) / 2;
	      color_max[i] = (buf[(y1 * c_width + x1) * channels + i]
			 + buf[((y1 + y2) * c_width + (x1 + x2)) * channels + i] + 1) / 2;

	      printf("%u\t",color_min[i]);
	    }

	    printf("\n");

	    for (int y3 = -1;y3 < 1;y3++)
	      for (int x3 = -1;x3 < 1;x3++)
		if (abs(y2 + y3) == 1 && abs(x2 + x3) == 1 && (x3 != 0 || y3 != 0)) {
		  
		}
	  }

      printf("\n");
    }
}
