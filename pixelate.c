/*
  pixelate bitmap image buf (in place)

  c_width	- current width of bitmap image
  c_height	- current height of bitmap image
  t_width	- target level-of-detail width of output image
  t_height	- target level-of-detail height of output image
  palette	- list of colors to snap to
  pallete_len	- length of the color list
*/

void pixelate(char* buf,int channels,int c_width,int c_height,int t_width,int t_height,char* palette,int palette_len) {
  int* dists = malloc(c_width * c_height * palette_len * sizeof(int));

  for (int y = 0;y < c_height;y++)
    for (int x = 0;x < c_width;x++)
      for (int i = 0;i < palette_len;i++) {
	// initialize value
	dists[y * c_width * palette_len
	    + x * palette_len
	    + i] = 0;

	for (int j = 0;j < channels;j++) {
	  // add square of difference

	  int a = palette[i * channels + j] - buf[y * c_width * channels + x * channels];

	  a *= a;

	  dists[y * c_width * palette_len
	      + x * palette_len
	      + i] += a;
	}
      }
}
