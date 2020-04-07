#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pixelate.h"

// a linked list is needed for the user interface to record colors
typedef struct _list {
  char* val;

  struct _list* next;
} list_t;

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
