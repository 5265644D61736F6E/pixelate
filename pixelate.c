#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pixelate.h"

/*
  pixelate bitmap image buf (in place)

  c_width	- current width of bitmap image
  c_height	- current height of bitmap image
  t_width	- target level-of-detail width of output image
  t_height	- target level-of-detail height of output image
  palette	- list of colors to snap to
  pallete_len	- length of the color list
*/

void pixelate(unsigned char* buf,int channels,int c_width,int c_height,int t_width,int t_height,unsigned char* palette,int palette_len) {
  int* dists; // distance between pixel colors and palette colors
  int* colorcount; // count of colors for the current output pixel

  int y; // y coordinate iterator
  int x; // x coordinate iterator
  int y1; // y coordinate iterator
  int x1; // x coordinate iterator
  int y2; // y coordinate iterator
  int x2; // x coordinate iterator
  int i; // primary iterator (over palette)
  int j; // secondary iterator (over channels)
  int a; // square root of additive
  int l; // palette index of lowest distance to current pixel
  int outcolor; // palette index to output

  if (c_width < t_width || c_height < t_height) {
    fprintf(stderr,"pixelate can only downscale images. Target dimensions must be smaller than current dimensions.\n");
    return;
  }

  // initialize heap objects

  dists = malloc(palette_len * sizeof(int));
  colorcount = malloc(palette_len * sizeof(int));

  for (y1 = 0;y1 < t_height;y1++)
    for (x1 = 0;x1 < t_width;x1++) {
      // initialize colorcount
      memset(colorcount,0,palette_len * sizeof(int));

      for (y2 = y1 * c_height / t_height;y2 < (y1 + 1) * c_height / t_height;y2++)
	for (x2 = x1 * c_width / t_width;x2 < (x1 + 1) * c_width / t_width;x2++) {
	  // find 'closest' palette color to represent the current color

	  l = 0;

	  for (i = 0;i < palette_len;i++) {
	    // initialize value
	    dists[i] = 0;

	    for (j = 0;j < channels;j++) {
	      // add square of difference

	      a = (int) buf[y2 * c_width * channels + x2 * channels + j] - (int) palette[i * channels + j];
	      dists[i] += a * a;
	    }

	    if (dists[i] < dists[l])
	      l = i;
	  }

	  colorcount[l]++;
	}

      // find color with the highest amount of pixels within the new output pixel

      outcolor = 0;

      for (i = 0;i < palette_len;i++)
	if (colorcount[i] > colorcount[outcolor])
	  outcolor = i;

      // output the resulting image to the buffer

      for (y2 = y1 * c_height / t_height;y2 < (y1 + 1) * c_height / t_height;y2++)
	for (x2 = x1 * c_width / t_width;x2 < (x1 + 1) * c_width / t_width;x2++)
	  memcpy(buf + y2 * c_width * channels + x2 * channels,palette + outcolor * channels,channels);
    }

  // clean up

  free(dists);
  free(colorcount);
}
