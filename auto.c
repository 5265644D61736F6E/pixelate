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

  // pass 1 of buf to choose colors to be included
  for (int y1 = 0;y1 < c_height;y1++)
    for (int x1 = 0;x1 < c_width;x1++) {
      printf("\n%d:%d:",x1,y1);

      for (int y2 = -2;y2 <= 2;y2++)
	for (int x2 = -2;x2 <= 2;x2++)
	  if ((x2 || y2) && x1 + x2 >= 0 && y1 + y2 >= 0 && x1 + x2 < c_width && y1 + y2 < c_height) {
	    unsigned char* color_min;
	    unsigned char* color_max;

	    color_min = malloc(channels);
	    color_max = malloc(channels);

	    // create range for the condition to disable the color export
	    for (int i = 0;i < channels;i++) {
	      color_min[i] = (buf[(y1 * c_width + x1) * channels + i]
			    + buf[((y1 + y2) * c_width + (x1 + x2)) * channels + i]) / 2;
	      color_max[i] = (buf[(y1 * c_width + x1) * channels + i]
			    + buf[((y1 + y2) * c_width + (x1 + x2)) * channels + i] + 1) / 2;
	    }

	    printf("\n\t%d:%d:\t",x1 + x2,y1 + y2);

	    for (int y3 = -1;y3 <= 1;y3++)
	      for (int x3 = -1;x3 <= 1;x3++)
		if (abs(y2 + y3) < 2 && abs(x2 + x3) < 2
		 && (x2 + x3 != 0 || y2 + y3 != 0) && (x3 != 0 || y3 != 0)
		 && x1 + x2 + x3 >= 0 && y1 + y2 + y3 >= 0
		 && x1 + x2 + x3 < c_width && y1 + y2 + y3 < c_height) {
		  char reset = 1;
		  printf("%d:%d ",x1 + x2 + x3,y1 + y2 + y3);

		  for (int i = 0;i < channels;i++)
		    // check if the pixels aren't a gradient
		    if (buf[((y1 + y2 + y3) * c_width + x1 + x2 + x3) * channels + i] < color_min[i]
		     || buf[((y1 + y2 + y3) * c_width + x1 + x2 + x3) * channels + i] > color_max[i])
		      reset = 0; // there is no gradient on this channel so the color shouldn't be disabled

		  // disable exporting the color
		  if (reset) {
		    bit_reset(colored,(y1 + y2 + y3) * c_width + x1 + x2 + x3);
		  }
		}
	  }
    }

  printf("\n");

  // pass 2 to count colors to export

  int colored_count = 0;

  for (int y = 0;y < c_height;y++)
    for (int x = 0;x < c_width;x++)
      if (bit_get(colored,y * c_width + x)) {
	colored_count++;

	printf("%u:%u ",x,y);
      }

  printf("\n%u\n",colored_count);
}
