#ifndef READ_H
#define READ_H

typedef struct data {
  unsigned char* buf;
  int channels;
  int c_width;
  int c_height;
} data_t;

int read_img(char* fname,data_t* data);

#endif
