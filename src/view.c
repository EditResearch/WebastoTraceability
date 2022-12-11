#include "view.h"
#include "ui.h"

#include <stdio.h>

typedef struct
{
    Ui ui;
}View;

#define View(...)(View){__VA_ARGS__}


static View v;


static void
signals(View * self)
{
    g_signal_connect(self->ui.btn_login, "clicked", G_CALLBACK(ui_set_visible_main_page), &self->ui);
    g_signal_connect(self->ui.btn_logout, "clicked", G_CALLBACK(ui_set_visible_login_page), &self->ui);
}


void
view(GtkApplication * app)
{
    O_Ui t = ui_build("ui/ui.glade");

    if(t.is_value == true)
    {
        v = View(t.value);
        gtk_window_set_application(GTK_WINDOW(v.ui.window), app);
    
        signals(&v);

        gtk_widget_show(GTK_WIDGET(v.ui.window));
    }

    /* treat error */
}


