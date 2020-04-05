#include <stdlib.h>

#include <gtk/gtk.h>

char* arg0;
int   arg1;
int   arg2;
int   arg3;
int   arg4;
int   arg5;
char* arg6;
int   arg7;

GtkWidget* window;
GtkWidget* t_width_in;
GtkWidget* t_height_in;
GtkWidget* submit;
GtkWidget* img;

void changed(GtkEditable* infield,gpointer _0) {
  const gchar* text;
  gchar* ptr;

  text = gtk_entry_get_text(GTK_ENTRY(infield));

  do {
    strtol(text,&ptr,0);

    if (*ptr)
      gtk_editable_delete_text(infield,ptr - text,ptr - text + 1);
  } while (*ptr);
}

void pressed(GtkButton* button,gpointer _0) {
  arg4 = strtol(gtk_entry_get_text(GTK_ENTRY(t_width_in)),NULL,0);
  arg5 = strtol(gtk_entry_get_text(GTK_ENTRY(t_height_in)),NULL,0);

  gtk_widget_hide(t_width_in);
  gtk_widget_hide(t_height_in);
  gtk_widget_hide(submit);
  gtk_widget_show(img);

  gtk_window_resize(GTK_WINDOW(window),arg2,arg3);
}

void activate(GtkApplication* app,gpointer user_data) {
  GtkWidget* box;

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

  submit = gtk_button_new_with_label("Next");
  gtk_container_add(GTK_CONTAINER(box),submit);
  g_signal_connect(submit,"pressed",G_CALLBACK(pressed),NULL);

  img = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_data(arg0,GDK_COLORSPACE_RGB,arg1 > 3,8,arg2,arg3,arg2,NULL,NULL));
  gtk_container_add(GTK_CONTAINER(box),img);

  gtk_widget_show_all(window);
  gtk_widget_hide(img);
}

void pixelate_gui(unsigned char* buf,int channels,int c_width,int c_height) {
  GtkApplication* app;

  arg0 = buf;
  arg1 = channels;
  arg2 = c_width;
  arg3 = c_height;

  app = gtk_application_new("com.pixelate.gui",G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
  g_application_run(G_APPLICATION(app),0,NULL);
  g_object_unref(app);
}
