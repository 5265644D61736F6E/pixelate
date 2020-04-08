#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pixelate.h"
#include "imgio.h"

#define HELP_MSG							\
  "pixelate is a program to generate high-contrast, pixelated images\n"	\
  "\n"									\
  "Usage:\tpixelate FILE\n"						\
  "\tpixelate { -u | -a } FILE WIDTH HEIGHT\n"				\
  "\tpixelate -a FILE WIDTH HEIGHT\n"					\
  "\n"									\
  "Options:\n"								\
  "\t-u\t- run Unix-based CLI instead of GUI\n"				\
  "\t-a\t- automatically detect colors in image (instead of asking)\n"	\
  "\t-h,--help\n"							\
  "\t\t- print this help message\n"

int main(int argc,char** argv) {
  data_t* img;

  int t_width;
  int t_height;
  char* fname;
  int j;

  char unix_mode;
  char automatic;

  // initialize variables and heap pointers
  j = 0;
  img = malloc(sizeof(data_t));

  for (int i = 1;i < argc;i++) {
    if (*(argv[i]) == '-')
      if (argv[i][1] == '-')
	if (!strcmp(argv[i] + 2,"help")) {
	  printf(HELP_MSG);
	  return 0;
	} else {
	  printf("Unrecognized option\n");
	  return 2;
	}
      else if (argv[i][1] == 'h') {
	printf(HELP_MSG);
	return 0;
      } else if (argv[i][1] == 'u')
	unix_mode = 1;
      else if (argv[i][1] == 'a')
	automatic = 1;
      else {
	printf("Unrecognized option\n");
	return 2;
      }
    else {
      if (j == 0)
	fname = argv[i];
      else if (j == 1)
	t_width = strtol(argv[i],NULL,0);
      else if (j == 2)
	t_height = strtol(argv[i],NULL,0);

      j++;
    }
  }

  if (read_img(fname,img))
    return 1;

  if (automatic)
    pixelate_auto(img->buf,img->channels,img->c_width,img->c_height,t_width,t_height);
  else if (unix_mode)
    pixelate_cli(img->buf,img->channels,img->c_width,img->c_height,t_width,t_height);
  else
    pixelate_gui(img->buf,img->channels,img->c_width,img->c_height);

  char* outfname;

  outfname = malloc(strlen(fname) + 5);
  memcpy(outfname,fname,strlen(fname));
  memcpy(outfname + strlen(fname) - 4,".new.png",9);

  if (write_img(outfname,img))
    return 1;

  return 0;
}
