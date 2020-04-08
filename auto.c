#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pixelate.h"

char bit_get(unsigned char* buf,int off) {
  return (buf[off / 8]) & (128 >> (off % 8));
}

void bit_reset(unsigned char* buf,int off) {
  buf[off / 8] &= ~(128 >> (off % 8));
}

void pixelate_auto(unsigned char* buf,int channels,int c_width,int c_height,int t_width,int t_height) {
  unsigned char* colored; // boolean array to mark exportable colors
  unsigned char* color_min; // minimum mean for each channel
  unsigned char* color_max; // maximum mean for each channel
  unsigned char* palette; // palette to pass to pixelate

  char reset;

  int palette_len; // length of palette
  int i; // primary iterator
  int j; // conditionally incremented iterator
  int x; // x coordinate iterator
  int y; // y coordinate iterator
  int x1; // primary x coordinate iterator
  int y1; // primary y coordinate iterator
  int x2; // secondary x coordinate iterator
  int y2; // secondary y coordinate iterator
  int x3; // tertiary x coordinate iterator
  int y3; // tertiary y coordinate iterator

  // initialize reusable arrays
  color_min = malloc(channels);
  color_max = malloc(channels);

  // initialize variable values
  palette_len = 0;
  j = 0;

  // create initialized array of booleans
  colored = malloc((c_width * c_height + 7) / 8);
  memset(colored,255,(c_width * c_height + 7) / 8);

  // pass 1 of buf to choose colors to be included
  for (y1 = 0;y1 < c_height;y1++)
    for (x1 = 0;x1 < c_width;x1++)
      for (y2 = -2;y2 <= 2;y2++)
	for (x2 = -2;x2 <= 2;x2++)
	  if ((x2 || y2) && x1 + x2 >= 0 && y1 + y2 >= 0 && x1 + x2 < c_width && y1 + y2 < c_height) {
	    // create range for the condition to disable the color export
	    for (i = 0;i < channels;i++) {
	      color_min[i] = (buf[(y1 * c_width + x1) * channels + i]
			    + buf[((y1 + y2) * c_width + (x1 + x2)) * channels + i]) / 2;
	      color_max[i] = (buf[(y1 * c_width + x1) * channels + i]
			    + buf[((y1 + y2) * c_width + (x1 + x2)) * channels + i] + 1) / 2;
	    }

	    for (y3 = -1;y3 <= 1;y3++)
	      for (x3 = -1;x3 <= 1;x3++)
		if (abs(y2 + y3) < 2 && abs(x2 + x3) < 2
		 && (x2 + x3 != 0 || y2 + y3 != 0)
		 && (x3 != 0 || y3 != 0)
		 && x1 + x2 + x3 >= 0 && y1 + y2 + y3 >= 0
		 && x1 + x2 + x3 < c_width && y1 + y2 + y3 < c_height) {
		  reset = 1;

		  for (i = 0;i < channels;i++)
		    // check if the pixels aren't a gradient
		    if (buf[((y1 + y2 + y3) * c_width + x1 + x2 + x3) * channels + i] < color_min[i]
		     || buf[((y1 + y2 + y3) * c_width + x1 + x2 + x3) * channels + i] > color_max[i])
		      reset = 0; // there is no gradient on this channel so the color shouldn't be disabled

		  // disable exporting the color
		  if (reset)
		    bit_reset(colored,(y1 + y2 + y3) * c_width + x1 + x2 + x3);
		}
	  }

  // pass 1 cleanup
  free(color_min);
  free(color_max);

  // pass 2 to count colors to export
  for (y = 0;y < c_height;y++)
    for (x = 0;x < c_width;x++)
      if (bit_get(colored,y * c_width + x))
	palette_len++;

  // pass 3 to export colors to array

  palette = malloc(palette_len * channels);

  for (i = 0;i < c_width * c_height;i++)
    if (bit_get(colored,i)) {
      memcpy(palette + j * channels,buf + i * channels,channels);
      j++;
    }

  // function call
  pixelate(buf,channels,c_width,c_height,t_width,t_height,palette,palette_len);
}
