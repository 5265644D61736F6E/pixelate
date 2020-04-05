#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pixelate.h"

// a linked list is needed for the user interface to record colors
typedef struct _list {
  char* val;

  struct _list* next;
} list_t;

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
      for (i = 0;i < palette_len;i++)
	colorcount[i] = 0;

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

void pixelate_cli(unsigned char* buf,int channels,int c_width,int c_height,int t_width,int t_height) {
  char* input = malloc(channels * 5 + 1);
  char* ptr;
  list_t* list = malloc(sizeof(list_t));
  list_t* l_curr = list;
  list_t* l_prev;
  char* palette;
  int l_count = 0;
  int i;

  fprintf(stderr,"Enter the palette colors: \nred,green,blue\nor\n(x:y)\n");

  while (1) {
    for (i = 0;input[i - 1] != '\n';i++)
      fread(input + i,1,1,stdin);

    if (*input != '\n' && *input != '\r') {
      ptr = input;
      l_curr->val = malloc(channels);

      if (*input == '(')
	memcpy(l_curr->val,buf + (strtol(ptr + 1,&ptr,0) * c_width + strtol(ptr + 1,&ptr,0)) * channels,channels);
      else
	for (int i = 0;i < channels;i++)
	  l_curr->val[i] = strtol(ptr,&ptr,0);

      l_curr->next = malloc(sizeof(list_t));
      l_curr = l_curr->next;

      l_count++;
    } else {
      free(l_curr); // free residual node
      break;
    }
  }

  palette = malloc(l_count * channels);
  l_curr = list;

  for (i = 0;i < l_count;i++) {
    memcpy(palette + i * channels,l_curr->val,channels);

    l_prev = l_curr;
    l_curr = l_curr->next;
    free(l_prev->val);
    free(l_prev);
  }

  pixelate(buf,channels,c_width,c_height,t_width,t_height,palette,l_count);
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
    0x00,0x00,0x90,
    0xA0,0x00,0x00,
    0xB0,0x00,0x00,
    0xC0,0xC0,0xC0,
    0x00,0x00,0xD0,
    0xF0,0x00,0x00,
    0xF0,0x00,0x00
  };

  char palette[12] = {
    0x00,0x00,0x00,
    0xFF,0x00,0x00,
    0x00,0x00,0xFF,
    0xFF,0xFF,0xFF
  };

  pixelate_gui(img,3,4,4);

  for (int i = 0;i < 48;i++)
    printf("%u,",img[i]);

  printf("\b \n");
}

int main() {
  test();
}
