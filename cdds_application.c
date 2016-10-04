/*
 *    Resume Vector
 *    Copyright (C) 2016 Brandon C Tardio
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *    
 *   Contact: BTardio@gmail.com
 *            818 424 6838
 *
 */

#include "cdds.h"
#include "cdds_application.h"
#include "cdds_window.h"
#include "cdds_drawingarea.h"



G_DEFINE_TYPE(CdsAreaApp, cds_area_app, GTK_TYPE_APPLICATION)


static void
cds_area_app_init (CdsAreaApp *app)
{


  
}



static void
cds_area_app_activate (GApplication *app)
{

  //CDS_AREA_APP(app)->win = window = cds_area_window_new ( app );
  CDS_AREA_APP(app)->win = cds_area_window_new ( app );

  gtk_window_present (GTK_WINDOW (CDS_AREA_APP(app)->win));

  
  if ( g_test_initialized() )
  {
    g_test_run();
    g_application_quit (app);
  }

  
  //g_test_run ();
}

static void
cds_area_app_class_init (CdsAreaAppClass *block)
{
  
  G_APPLICATION_CLASS (block)->shutdown = cds_area_app_shutdown;
  G_APPLICATION_CLASS (block)->activate = cds_area_app_activate;
  G_APPLICATION_CLASS (block)->startup = cds_area_app_startup;
  G_APPLICATION_CLASS (block)->open = cds_area_app_open;
}

CdsAreaApp*
cds_area_app_new (void)
{
  return g_object_new (CDS_AREA_TYPE_APP,
                       "application-id", "org.gtk.cdds",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}


static void
cds_area_app_startup (GApplication *app)
{

  G_APPLICATION_CLASS (cds_area_app_parent_class)->startup (app);
  
}

void
cds_area_app_shutdown ( GApplication *app )
{
  
  G_APPLICATION_CLASS (cds_area_app_parent_class)->shutdown (app);
  
}

static void
cds_area_app_open (GApplication  *app,
                   GFile        **files,
                   gint           n_files,
                   const gchar   *hint)
{
  GList *windows;
  CdsAreaWindow *win;
  int i;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows)
    win = CDS_AREA_WINDOW (windows->data);
  else
    win = cds_area_window_new ( app );

  for (i = 0; i < n_files; i++)
    cds_area_window_open (win, files[i]);

  gtk_window_present (GTK_WINDOW (win));
}
