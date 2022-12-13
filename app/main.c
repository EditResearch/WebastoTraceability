#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <version.h>

#include "../src/view.h"


static const Version version = Version(0,1,0);


int
main(int argc, char ** argv)
{
    if(argc > 1 && strcmp(argv[1], "--version") == 0)
    {
        version_show(stdout, version);
        return EXIT_SUCCESS;
    }

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



