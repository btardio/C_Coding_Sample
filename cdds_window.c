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
#include "cdds_window.h"
#include "cdds_drawingarea.h"
#include "dialogs/dialog_new.h"




G_DEFINE_TYPE(CdsAreaWindow, cds_area_window, GTK_TYPE_APPLICATION_WINDOW)


gboolean
cds_area_window_key_press_event ( GtkWidget *widget, GdkEventKey *event )
{
  CdsAreaWindow* win;
  win = CDS_AREA_WINDOW(widget);
  
  if ( win->area != NULL )
  {
    cds_area_drawing_area_key_press_event ( win->area, event );
  }
    
    
  
    
    
  return GDK_EVENT_PROPAGATE;
}



gboolean
cds_area_window_key_release_event ( GtkWidget *widget, GdkEventKey *event )
{
  CdsAreaWindow* win;
  win = CDS_AREA_WINDOW(widget);
  
  if ( win->area != NULL )
  {
    cds_area_drawing_area_key_release_event ( win->area, event );
  }
  
  return GDK_EVENT_PROPAGATE;
}

CdsAreaDrawingArea*
cds_area_window_create_drawingarea ( CdsAreaWindow* win, gint width, gint height )
{
  
  
  
  if ( CDS_AREA_IS_DRAWINGAREA ( win->area ) )
  {
    
    cds_area_window_destroy_drawingarea ( win );
    
  }
  
  win->area = cds_area_drawingarea_new( width, height );
  
  cds_area_drawingarea_new_points_treeview ( win, win->area );
  cds_area_drawingarea_new_lines_treeview ( win, win->area );
  
  gtk_container_add( GTK_CONTAINER ( win->viewport ), GTK_WIDGET ( win->area ) );
  
  gtk_widget_set_halign ( GTK_WIDGET ( win->viewport ), GTK_ALIGN_CENTER );
  gtk_widget_set_valign ( GTK_WIDGET ( win->viewport ), GTK_ALIGN_CENTER );
  
  gtk_widget_realize( GTK_WIDGET ( win->area ) );
  gtk_widget_show( GTK_WIDGET ( win->area ) );
  
}

void
cds_area_window_destroy_drawingarea ( CdsAreaWindow* win )
{
  gtk_container_remove ( GTK_CONTAINER ( win->viewport ), GTK_WIDGET ( win->area ) );
  
  cds_area_drawingarea_destroy(win->area); 
  win->area = NULL;
  
}

/*
 * init function 
 */
static void
cds_area_window_init ( CdsAreaWindow *win )
{

  gtk_widget_init_template ( GTK_WIDGET ( win ) );
  

#ifdef CDDS_TEST
  
  win->menuitem_toplevel_test = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Test" ) );
  win->menu_test = GTK_MENU ( gtk_menu_new ( ) );
  win->menuitem_test = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Start Test" ) );
  
  gtk_menu_shell_append ( GTK_MENU_SHELL ( win->menubar ), GTK_WIDGET ( win->menuitem_toplevel_test ) );
  gtk_menu_item_set_submenu ( win->menuitem_toplevel_test, GTK_WIDGET ( win->menu_test ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( win->menu_test ), GTK_WIDGET ( win->menuitem_test ) );
  
  gtk_widget_show ( GTK_WIDGET ( win->menuitem_toplevel_test ) );
  gtk_widget_show ( GTK_WIDGET ( win->menu_test ) );
  gtk_widget_show ( GTK_WIDGET ( win->menuitem_test ) );
  
  g_signal_connect ( win->menuitem_test, "activate", G_CALLBACK ( menuitem_test_callback ), win );
  
  
#endif    
}

/*
 * creates private member variables for each template variable
 */
static void
cds_area_window_class_init ( CdsAreaWindowClass *block )
{

  GtkWidgetClass *widget_class;
  GObjectClass   *gobject_class;
  widget_class = GTK_WIDGET_CLASS(block);
  gobject_class = G_OBJECT_CLASS(block);
  
  gtk_widget_class_set_template_from_resource ( GTK_WIDGET_CLASS ( block ), "/org/gtk/cdds/cdds.glade" );
  
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, box_primary );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, eventbox );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, scrolledwindow );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, viewport );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, hadjustment );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, vadjustment );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, tog_select );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, tog_point );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, tog_line );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, tog_box );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, menubar );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, menuitem_edit );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, menuitem_undo );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, menuitem_redo );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, scrolledwindow_points );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( block ), CdsAreaWindow, scrolledwindow_lines );
  
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS ( block ), cds_area_dialog_activate_callback);
  
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS (block), menuitem_undo_callback);
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS (block), menuitem_redo_callback);
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS (block), toggle_select_callback);
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS (block), toggle_line_callback);
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS (block), toggle_point_callback);
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS (block), toggle_box_callback);
  
  widget_class->key_press_event = cds_area_window_key_press_event;
  widget_class->key_release_event = cds_area_window_key_release_event;
  
  gobject_class->dispose = cds_area_window_dispose; /* opposite of init */
  gobject_class->finalize = cds_area_window_finalize; /* opposite of class_init */
  
}


CdsAreaWindow *
cds_area_window_new ( GApplication *app )
{
  CdsAreaWindow *w;
  
  w = g_object_new ( CDS_AREA_TYPE_WINDOW, "application", app, NULL );
    
  return w;
}

void
cds_area_window_open ( CdsAreaWindow *win, GFile *file )
{

  
}

void
cds_area_window_dispose ( GObject* obj )
{
  CdsAreaWindow* win;
  win = CDS_AREA_WINDOW ( obj );
  
  if ( CDS_AREA_IS_DRAWINGAREA( win->area ) )
  {
    gtk_widget_destroy ( GTK_WIDGET ( win->area ) );
  }
  
  G_OBJECT_CLASS ( cds_area_window_parent_class )->dispose ( obj );
}


void
cds_area_window_finalize ( GObject* obj )
{
  
  G_OBJECT_CLASS ( cds_area_window_parent_class )->finalize ( obj );
}

void
cds_area_window_destroy (CdsAreaWindow *win)
{

  gtk_widget_destroy ( GTK_WIDGET ( win ) );

}

