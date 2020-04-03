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
  int* dists;

  int y;
  int x;
  int i;
  int j;
  int a;

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

	printf("%d,",dists[y * c_width * palette_len
			 + x * palette_len
		         + i]);
      }

      printf("\t");
    }

    printf("\n");
  }

  free(dists); // clean up
}

int test() {
  char img[48] = {
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
    0xA0,0xA0,0xA0,
    0xB0,0xB0,0xB0,
    0xC0,0x00,0x00,
    0xD0,0x00,0x00,
    0xE0,0x00,0x00,
    0xF0,0x00,0x00
  };

  char palette[9] = {
    0x00,0x00,0x00,
    0xFF,0x00,0x00,
    0xFF,0xFF,0xFF
  };

  pixelate(img,3,4,4,4,4,palette,3);
}

int main() {
  test();
}
