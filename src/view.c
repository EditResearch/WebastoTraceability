#include "view.h"
#include "ui.h"
#include "model.h"

#include <stdio.h>

typedef struct
{
    Ui ui;
    Model model;
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
view_set_table_list(View * self)
{
    Vector * table_list = model_get_table_list(&v.model);

    if(table_list != NULL)
    {
        for(size_t i = 0; i < table_list->length; i++)
        {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(self->ui.combo_table), table_list->array[i]);
            free(table_list->array[i]);
        }

        free(table_list);
    }
}


void
view(GtkApplication * app)
{
    O_Ui t  = ui_build("ui/ui.glade");
    E_Model m = model_init();      

    if(t.is_value == true 
        && m.either == E_Model_R)
    {
        v = View(t.value, m.right);
        gtk_window_set_application(GTK_WINDOW(v.ui.window), app);
 
        view_set_table_list(&v);        

        signals(&v);

        gtk_widget_show(GTK_WIDGET(v.ui.window));
    }

    /* treat error */
}









