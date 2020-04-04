#ifndef PIXELATE_H
#define PIXELATE_H

void pixelate(unsigned char* buf,int channels,int c_width,int c_height,int t_width,int t_height,unsigned char* palette,int palette_len);

void pixelate_gui(unsigned char* buf,int channels,int c_width,int c_height);
void pixelate_cli(unsigned char* buf,int channels,int c_width,int c_height,int t_width,int t_height);

#endif
