#include <math.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include "pixelate.h"

// a linked list is needed for the user interface to record colors
typedef struct _list {
  char* val;

  struct _list* next;
} list_t;

// arguments to pass to pixelate

char* arg0; // buf
int   arg1; // channels
int   arg2; // c_width
int   arg3; // c_height
int   arg4; // t_width
int   arg5; // t_height
char* arg6; // palette
int   arg7; // palette_len

// interface variables

list_t* list;
list_t* l_curr;
int state;

// GTK+ widgets

GtkWidget* window;
GtkWidget* t_width_in;
GtkWidget* t_height_in;
GtkWidget* submit;
GtkWidget* img;

void button_press(GtkWidget* _0,GdkEventButton* evt,gpointer _1) {
  // initialize current list node (the last is ignored so initializing next is fine)

  l_curr->val = malloc(arg1);
  l_curr->next = malloc(sizeof(list_t));

  // add pixel color to palette
  memcpy(l_curr->val,arg0 + (int) floor(evt->y) * arg1 * arg2 + (int) floor(evt->x) * arg1,arg1);

  // advance the pointer for the next selection
  l_curr = l_curr->next;
}

void changed(GtkEditable* infield,gpointer _0) {
  const gchar* text;
  gchar* ptr;

  // get the new input text
  text = gtk_entry_get_text(GTK_ENTRY(infield));

  do {
    strtol(text,&ptr,0);

    // check if the first invalid character is NULL
    if (*ptr)
      // the invalid character was entered so it should be deleted
      gtk_editable_delete_text(infield,ptr - text,ptr - text + 1);
  } while (*ptr);
}

void pressed(GtkButton* button,gpointer _0) {
  if (state) { // state 1 is for selecting palette colors and state 0 is for specifying target dimensions
    int l_count;

    // pass once to find length

    l_curr = list;
    l_count = 0;

    while (l_curr->next != NULL) {
      l_curr = l_curr->next;
      l_count++;
    }

    arg6 = malloc(l_count * arg1);
    arg7 = l_count;

    // pass again to convert list to array

    l_curr = list;

    for (int i = 0;i < l_count;i++) {
      memcpy(arg6 + i * arg1,l_curr->val,arg1);
      l_curr = l_curr->next;
    }

    // perform the operation
    pixelate(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7);
  } else {
    // prepare input to pass to function
    arg4 = strtol(gtk_entry_get_text(GTK_ENTRY(t_width_in)),NULL,0);
    arg5 = strtol(gtk_entry_get_text(GTK_ENTRY(t_height_in)),NULL,0);

    // reuse the button for state 1
    gtk_button_set_label(GTK_BUTTON(submit),"Submit");

    // show new set of widgets

    gtk_widget_hide(t_width_in);
    gtk_widget_hide(t_height_in);
    gtk_widget_show(submit);
    gtk_widget_show(img);

    // fit the window to the image size
    gtk_window_resize(GTK_WINDOW(window),arg2,arg3);

    // initialize list and iterator
    list = malloc(sizeof(list_t));
    l_curr = list;

    // change state
    state = 1;
  }
}

void activate(GtkApplication* app,gpointer user_data) {
  GtkWidget* box;
  GtkWidget* imgcont;

  // set button state to 0
  state = 0;

  // create GTK3+ widgets

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window),"pixelate");
  gtk_window_set_default_size(GTK_WINDOW(window),200,100);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
  gtk_container_add(GTK_CONTAINER(window),box);

  t_width_in = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(t_width_in),"Output Width");
  gtk_container_add(GTK_CONTAINER(box),t_width_in);
  g_signal_connect(t_width_in,"changed",G_CALLBACK(changed),NULL);

  t_height_in = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(t_height_in),"Output Height");
  gtk_container_add(GTK_CONTAINER(box),t_height_in);
  g_signal_connect(t_height_in,"changed",G_CALLBACK(changed),NULL);

  imgcont = gtk_event_box_new();
  gtk_container_add(GTK_CONTAINER(box),imgcont);

  img = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_data(arg0,GDK_COLORSPACE_RGB,arg1 > 3,8,arg2,arg3,arg2,NULL,NULL));
  gtk_container_add(GTK_CONTAINER(imgcont),img);

  g_signal_connect(imgcont,"button_press_event",G_CALLBACK(button_press),img);

  gtk_widget_set_halign(img,GTK_ALIGN_START);

  submit = gtk_button_new_with_label("Next");
  g_signal_connect(submit,"pressed",G_CALLBACK(pressed),NULL);
  gtk_container_add(GTK_CONTAINER(box),submit);

  // show the widgets
  gtk_widget_show_all(window);
  gtk_widget_hide(img);
}

void pixelate_gui(unsigned char* buf,int channels,int c_width,int c_height) {
  GtkApplication* app;

  arg0 = buf;
  arg1 = channels;
  arg2 = c_width;
  arg3 = c_height;

  // create the GTK3+ gui
  app = gtk_application_new("com.pixelate.gui",G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
  g_application_run(G_APPLICATION(app),0,NULL);
  g_object_unref(app);
}
