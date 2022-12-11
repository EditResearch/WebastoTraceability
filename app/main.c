#include <stdio.h>
#include <stdlib.h>
#include "../src/view.h"
#include <gtk/gtk.h>


int
main(int argc, char ** argv)
{
    GtkApplication * app = 
        gtk_application_new(
            "if42.Webasto.WebastoTraceability"
            , G_APPLICATION_FLAGS_NONE);

    g_signal_connect(
        G_OBJECT(app)
        , "activate"
        , G_CALLBACK(view)
        , NULL);

    int result = 
        g_application_run(
            G_APPLICATION(app)
            , argc
            , argv);

    g_object_unref(app);
    
    return result;
}



