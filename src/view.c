#include "view.h"
#include "ui.h"
#include "model.h"

#include <stdio.h>

#define LOGOUT_INTERVAL 60*1

typedef struct
{
    Ui ui;
    Model model;
    int cyclic_interrupt_id;
}View;

#define View(...)(View){__VA_ARGS__}


static View v;


static void
view_init_gui(View * self)
{
    self->model.logout_timer = 0;
}
 

static inline void
view_init_logout_timer(View * self)
{
    self->model.logout_timer = LOGOUT_INTERVAL;
}


static void 
view_login(View * self)
{
    view_init_logout_timer(self);
    ui_set_visible_main_page(&self->ui);

    strcpy(
        self->model.logged_user
        , gtk_entry_get_text(GTK_ENTRY(self->ui.entry_user_name)));

    log_debug(
        self->model.log
        , "User %s logged in"
        , self->model.logged_user);
}


static void
view_logout(View * self)
{
    ui_set_visible_login_page(&self->ui);
    view_init_gui(self);

    log_debug(
        self->model.log
        , "User %s logged out"
        , self->model.logged_user);
}


static void
view_combo_table_changed_callback (
  GtkComboBox* widget
  , View * view)
{
    view_init_logout_timer(view);

    Vector * column_list = 
        model_get_table_columns(
            &view->model
            , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(view->ui.combo_column));

    if(column_list != NULL)
    {
        for(size_t i = 0; i < column_list->length; i++)
        {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(view->ui.combo_column), column_list->array[i]);
            free(column_list->array[i]);
        }

        gtk_combo_box_set_active(GTK_COMBO_BOX(view->ui.combo_column), 0);

        free(column_list);
    }
}


static void 
view_btn_login_callback(GtkWidget * widget, View * self)
{
    (void) widget;
    view_login(self);
}


static void 
view_btn_logout_callback(GtkWidget * widget, View * self)
{
    (void) widget;
    view_logout(self);
}


static int
view_cyclic_interupt_callback(gpointer param)
{
    View * self = param;
    char str_timeout[32];

    if(self->model.logout_timer > 0)
    {
        if((--self->model.logout_timer) == 0)
            view_logout(self);      
         
        gtk_progress_bar_set_fraction (
            GTK_PROGRESS_BAR (self->ui.progress_timeout_logout)
            , ((float)self->model.logout_timer)/((float)LOGOUT_INTERVAL));

        sprintf(
            str_timeout
            , "%02ld:%02ld"
            , self->model.logout_timer / 60, self->model.logout_timer % 60);

        gtk_progress_bar_set_text(
            GTK_PROGRESS_BAR(self->ui.progress_timeout_logout)
            , str_timeout);
    }
    
    return true;
}


static void
signals(View * self)
{
    g_signal_connect(self->ui.btn_login, "clicked", G_CALLBACK(view_btn_login_callback), &self->ui);
    g_signal_connect(self->ui.btn_logout, "clicked", G_CALLBACK(view_btn_logout_callback), &self->ui);
    g_signal_connect(self->ui.combo_table, "changed", G_CALLBACK(view_combo_table_changed_callback), self);
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
        int cyclic_interrupt_id = g_timeout_add(1000, view_cyclic_interupt_callback, &v);

        v = View(t.value, m.right, cyclic_interrupt_id);
        gtk_window_set_application(GTK_WINDOW(v.ui.window), app);
 
        view_set_table_list(&v);        

        signals(&v);
        
        gtk_widget_show(GTK_WIDGET(v.ui.window));
    }

    /* treat error */
}









