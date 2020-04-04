#include <stdio.h>
#include <stdlib.h>

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
  int* closest; // palette indicies of lowest distance to all pixels
  int* outcolors; // palette indicies to output
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

  if (c_width < t_width || c_height < t_height) {
    fprintf(stderr,"pixelate can only downscale images. Target dimensions must be smaller than current dimensions.\n");
    return;
  }

  dists = malloc(c_width * c_height * palette_len * sizeof(int));

  for (y = 0;y < c_height;y++) {
    for (x = 0;x < c_width;x++) {
      for (i = 0;i < palette_len;i++) {
	// initialize value
	dists[y * c_width * palette_len
	    + x * palette_len
	    + i] = 0;

	for (j = 0;j < channels;j++) {
	  // add square of difference

	  a = (int) buf[y * c_width * channels + x * channels + j] - (int) palette[i * channels + j];
	  dists[y * c_width * palette_len
	      + x * palette_len
	      + i] += a * a;
	}

	//printf("%d,",dists[y * c_width * palette_len
	//		 + x * palette_len
	//	         + i]);
      }

      //printf("\t");
    }

    //printf("\n");
  }

  closest = malloc(c_width * c_height * sizeof(int));

  for (y = 0;y < c_height;y++) {
    for (x = 0;x < c_width;x++) {
      l = 0;

      for (i = 0;i < palette_len;i++)
	if (dists[y * c_width * palette_len
		+ x * palette_len
		+ i]
	  < dists[y * c_width * palette_len
		+ x * palette_len
		+ l])
	  l = i;

      closest[y * c_width + x] = l;

      //printf("%u ",l);
    }

    //printf("\n");
  }

  outcolors = malloc(t_height * t_width * sizeof(int));
  colorcount = malloc(palette_len * sizeof(int));

  for (y1 = 0;y1 < t_height;y1++)
    for (x1 = 0;x1 < t_width;x1++) {
      for (i = 0;i < palette_len;i++)
	colorcount[i] = 0;

      for (y2 = y1 * c_height / t_height;y2 < (y1 + 1) * c_height / t_height;y2++)
	for (x2 = x1 * c_width / t_width;x2 < (x1 + 1) * c_width / t_width;x2++)
	  colorcount[closest[y2 * c_width + x2]]++;

      outcolors[y1 * t_width + x1] = 0;

      for (i = 0;i < palette_len;i++) {
	if (colorcount[i]
	  > colorcount[outcolors[y1 * t_width + x1]])
	  outcolors[y1 * t_width + x1] = i;

	printf("%u ",colorcount[i]);
      }

      printf("%u\n",outcolors[y1 * t_width + x1]);
    }

  for (y1 = 0;y1 < t_height;y1++)
    for (x1 = 0;x1 < t_width;x1++)
      for (y2 = y1 * c_height / t_height;y2 < (y1 + 1) * c_height / t_height;y2++)
	for (x2 = x1 * c_width / t_width;x2 < (x1 + 1) * c_width / t_width;x2++)
	  memcpy(buf + y2 * c_width * channels + x2 * channels,palette + outcolors[y1 * t_width + x1] * channels,channels);

  free(dists);
  free(closest);
}

int test() {
  unsigned char img[48] = {
    0x00,0x00,0x00,
    0x10,0x10,0x10,
    0x20,0x20,0x20,
    0x30,0x30,0x30,
    0x40,0x40,0x40,
    0x50,0x50,0x50,
    0x60,0x60,0x60,
    0x70,0x70,0x70,
    0x80,0x80,0x80,
    0x90,0x90,0x90,
    0xA0,0x00,0x00,
    0xB0,0x00,0x00,
    0xC0,0xC0,0xC0,
    0xD0,0xD0,0xD0,
    0xE0,0x00,0x00,
    0xF0,0x00,0x00
  };

  char palette[9] = {
    0x00,0x00,0x00,
    0xFF,0x00,0x00,
    0xFF,0xFF,0xFF
  };

  pixelate(img,3,4,4,2,2,palette,3);

  for (int i = 0;i < 48;i++)
    printf("%u,",img[i]);

  printf("\b \n");
}

int main() {
  test();
}
