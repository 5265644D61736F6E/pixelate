#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>

#define READ_LIBPNG_ERROR 1

typedef struct data {
  unsigned char* buf;
  int channels;
  int c_width;
  int c_height;
} data_t;

int read_img(char* fname,data_t* data) {
  FILE* fp;

  char sig[8];

  png_structp png;
  png_infop info;
  png_infop endinfo;

  fp = fopen(fname,"rb");

  if (!fp) {
    printf("Failed to open %s",fname);
    return 1;
  }

  fread(sig,1,8,fp);

  if (!png_sig_cmp(sig,0,8)) {
    // parse PNG

    fseek(fp,0,SEEK_SET);

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

    if (!png) {
      fclose(fp);
      return 1;
    }

    info = png_create_info_struct(png);

    if (!info) {
      png_destroy_read_struct(&png,NULL,NULL);
      fclose(fp);
      return 1;
    }

    endinfo = png_create_info_struct(png);

    if (!endinfo) {
      png_destroy_read_struct(&png,&info,NULL);
      fclose(fp);
      return 1;
    }

    setjmp(png_jmpbuf(png));
    png_init_io(png,fp);

    png_read_png(png,info,0,NULL); // parse the png

    data->channels = png_get_channels(png,info);
    data->c_width = png_get_image_width(png,info);
    data->c_height = png_get_image_height(png,info);
    data->buf = malloc(data->c_width * data->c_height * data->channels);

    png_bytepp buf;

    buf = png_get_rows(png,info);

    for (int i = 0;i < data->c_height;i++)
      memcpy(data->buf + i * data->c_width * data->channels,buf[i],data->c_width * data->channels);
  } else
    return 1;
}
