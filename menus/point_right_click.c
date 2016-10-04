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

//#include "../cdds_drawingarea.h"
//#include "../cdds_winapp.h"

#include "../cdds.h"
#include "../geometry/cdds_geometry_actionmanager.h"
#include "../cdds_drawingarea.h"
#include "../geometry/cdds_geometry_selection.h"
#include "../geometry/cdds_geometry_line.h"
#include "../geometry/cdds_geometry_linemanager.h"
#include "../geometry/cdds_geometry_point.h"

#define DESTROYUSINGREF

void
menus_addpoint_callback (GtkMenuItem *menuitem, gpointer data)
{
  CdsAreaDrawingArea* area;
  area = data;
  
  /* cdds_geoactionmanager_point_new ( area->actions, area->selection, area->pointsmgr, 
				    area->last_button_press_x, area->last_button_press_y ); */
  cds_area_actionmanager_point_new ( area->mgrs, area->last_button_press_x, area->last_button_press_y );
  
  gtk_widget_queue_draw ( GTK_WIDGET ( area ) );
}

void
menus_delete_callback ( GtkMenuItem *menuitem, gpointer data )
{  
  CdsAreaDrawingArea* area;
  area = data;

  /* cdds_geoactionmanager_selected_points_delete ( area->actions, area->selection, area->pointsmgr ); */
  cds_area_actionmanager_selected_points_delete ( area->mgrs );
  
  gtk_widget_queue_draw ( GTK_WIDGET ( area ) );
}

void
menus_addline_callback ( GtkMenuItem *menuitem, gpointer data )
{
  
  CdsAreaDrawingArea* area;
  CdsAreaLine* l;
  CDDSGeoPoint* p1;
  CDDSGeoPoint* p2;
  GPtrArray* ptrarray;
  area = data;
  
  ptrarray = cds_area_selection_points_to_ptrarray ( area->mgrs->smgr );
  
  p1 = g_ptr_array_index ( ptrarray, 0 );
  p2 = g_ptr_array_index ( ptrarray, 1 );
  
  l = cds_area_line_new ( area->mgrs, p1, p2 );
    
  cds_area_linemanager_insert_linel ( area->mgrs, l );
  
  g_ptr_array_free ( ptrarray, TRUE );
  
  gtk_widget_queue_draw ( GTK_WIDGET ( area ) );
  
}

void
menus_menu_activate_callback ( GtkWidget *widget, gpointer data )
{
  
  CdsAreaDrawingArea* area;
  area = data;
  
  if ( cds_area_selection_len ( area->mgrs->smgr ) == 2 )
  {
    
    gtk_widget_show ( GTK_WIDGET ( g_ptr_array_index ( area->right_click_menus, 
						       area->n_right_click_menu_handlers - 2 ) ) );

  }
  else
  {

    gtk_widget_hide ( GTK_WIDGET ( g_ptr_array_index ( area->right_click_menus, 
						       area->n_right_click_menu_handlers - 2 ) ) );
    
  }

}

void
cds_area_drawingarea_right_click_menus_dispose ( CdsAreaDrawingArea* drawingarea )
{
  
  GList* menuitems;
  gulong handler_id;
  
  if ( drawingarea != NULL && drawingarea->right_click_menus != NULL && drawingarea->right_click_menu_handlers != NULL )
  {  
    for ( int i = 0; i < drawingarea->n_right_click_menu_handlers; i++ )
    {

      g_signal_handler_disconnect ( g_ptr_array_index ( drawingarea->right_click_menus, i ),
				    g_array_index (drawingarea->right_click_menu_handlers, gulong, i) );
    }
    
    g_ptr_array_free ( drawingarea->right_click_menus, TRUE );
    g_array_free ( drawingarea->right_click_menu_handlers, TRUE );
    drawingarea->right_click_menus = NULL;
    drawingarea->right_click_menu_handlers = NULL;
    
  }

  if ( drawingarea != NULL && GTK_IS_CONTAINER ( drawingarea->toggle_selection_hover_point_right_click_menu ) )
  {
    menuitems = gtk_container_get_children ( GTK_CONTAINER ( drawingarea->toggle_selection_hover_point_right_click_menu ) );
    
    while ( menuitems != NULL )
    {
      
      if ( GTK_IS_MENU_ITEM ( GTK_WIDGET ( menuitems->data ) ) )
      {
	gtk_widget_destroy ( GTK_WIDGET ( menuitems->data ) );
      }
      else
      {
	g_warning ( "GTK_IS_MENU_ITEM ( menuitems->data ) == FALSE");
      }
      
      menuitems = menuitems->next;
    }
    
    gtk_widget_destroy ( GTK_WIDGET ( drawingarea->toggle_selection_hover_point_right_click_menu ) );
    drawingarea->toggle_selection_hover_point_right_click_menu = NULL;
  }

  
  if ( drawingarea != NULL && GTK_IS_CONTAINER ( drawingarea->toggle_selection_nohover_right_click_menu ) )
  {
    menuitems = gtk_container_get_children ( GTK_CONTAINER ( drawingarea->toggle_selection_nohover_right_click_menu ) );
    
    while ( menuitems != NULL )
    {
      
      if ( GTK_IS_MENU_ITEM ( GTK_WIDGET ( menuitems->data ) ) )
      {
	gtk_widget_destroy ( GTK_WIDGET ( menuitems->data ) );
      }
      else
      {
	g_warning ( "GTK_IS_MENU_ITEM ( menuitems->data ) == FALSE");
      }
      
      menuitems = menuitems->next;
    }
    
    gtk_widget_destroy ( GTK_WIDGET ( drawingarea->toggle_selection_nohover_right_click_menu ) );
    drawingarea->toggle_selection_nohover_right_click_menu = NULL;
    
  }
  
}


GtkMenu*
toggle_selection_nohover_right_click_menu_create ( CdsAreaDrawingArea* drawingarea )
{

  GtkMenu *menu;
  GtkMenuItem *menuitem_undo;
  GtkMenuItem *menuitem_redo;
  GtkMenuItem *menuitem_addpoint;
  GtkSeparatorMenuItem *menuitem_separator;
  gulong handler_id;
  
  menu = GTK_MENU ( gtk_menu_new ( ) );
  
  gtk_menu_attach_to_widget ( menu, GTK_WIDGET( drawingarea ), NULL );
  
  menuitem_undo = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Undo" ) );
  menuitem_redo = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Redo" ) );
  menuitem_addpoint = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Add Point" ) );
  menuitem_separator = GTK_SEPARATOR_MENU_ITEM ( gtk_separator_menu_item_new() );
  
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_addpoint ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_separator ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_undo ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_redo ) );

  gtk_widget_show ( GTK_WIDGET ( menuitem_separator ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_addpoint ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_undo ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_redo ) );
  
  if ( drawingarea->right_click_menu_handlers == NULL )
  {
    drawingarea->right_click_menu_handlers = g_array_new ( FALSE, FALSE, sizeof ( gulong )  );
    drawingarea->right_click_menus = g_ptr_array_new ( );
    drawingarea->n_right_click_menu_handlers = 0;
  }
  
  handler_id = g_signal_connect ( GTK_WIDGET ( menuitem_addpoint ), "activate", 
				  G_CALLBACK ( menus_addpoint_callback ), drawingarea );
  
  g_array_append_val ( drawingarea->right_click_menu_handlers, handler_id );
  g_ptr_array_add ( drawingarea->right_click_menus, menuitem_addpoint );
  drawingarea->n_right_click_menu_handlers ++;
  
  return menu;
  
}


GtkMenu*
toggle_selection_hover_point_right_click_menu_create ( CdsAreaDrawingArea* drawingarea )
{

  GtkMenu *menu;
  GtkMenuItem *menuitem_undo;
  GtkMenuItem *menuitem_redo;
  GtkMenuItem *menuitem_delete;
  GtkMenuItem *menuitem_addline;
  GtkSeparatorMenuItem *menuitem_separator1;
  GtkSeparatorMenuItem *menuitem_separator2;
  GtkMenuItem *menuitem_addpoint;
  gulong handler_id;  
  
  menu = GTK_MENU ( gtk_menu_new ( ) );
  
  gtk_menu_attach_to_widget ( menu, GTK_WIDGET( drawingarea ), NULL );
  
  menuitem_delete = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Delete" ) );
  menuitem_separator1 = GTK_SEPARATOR_MENU_ITEM ( gtk_separator_menu_item_new() );
  menuitem_separator2 = GTK_SEPARATOR_MENU_ITEM ( gtk_separator_menu_item_new() );
  menuitem_addpoint = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Add Point" ) );
  menuitem_addline = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Add Line" ) );
  menuitem_undo = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Undo" ) );
  menuitem_redo = GTK_MENU_ITEM ( gtk_menu_item_new_with_label ( "Redo" ) );
  
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_delete ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_separator1 ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_addpoint ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_addline ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_separator2 ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_undo ) );
  gtk_menu_shell_append ( GTK_MENU_SHELL ( menu ), GTK_WIDGET ( menuitem_redo ) );
  
  //g_object_unref ( menuitem_delete ); // reference held by GTK_MENU_SHELL ( menu )
    
  gtk_widget_show ( GTK_WIDGET ( menuitem_separator1 ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_separator2 ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_undo ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_redo ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_delete ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_addpoint ) );
  gtk_widget_show ( GTK_WIDGET ( menuitem_addline ) );


  if ( drawingarea->right_click_menu_handlers == NULL )
  {
    drawingarea->right_click_menu_handlers = g_array_new ( FALSE, FALSE, sizeof ( gulong )  );
    drawingarea->right_click_menus = g_ptr_array_new ( );
    drawingarea->n_right_click_menu_handlers = 0;
  }
  
  
  
  
  
  handler_id = g_signal_connect ( GTK_WIDGET ( menuitem_addpoint ), "activate", 
				  G_CALLBACK ( menus_addpoint_callback ), drawingarea );
  g_array_append_val ( drawingarea->right_click_menu_handlers, handler_id );
  g_ptr_array_add ( drawingarea->right_click_menus, menuitem_addpoint );
  drawingarea->n_right_click_menu_handlers++;
  
  
  handler_id = g_signal_connect ( GTK_WIDGET ( menuitem_delete ), "activate", 
				  G_CALLBACK ( menus_delete_callback ), drawingarea );
  g_array_append_val ( drawingarea->right_click_menu_handlers, handler_id );
  g_ptr_array_add ( drawingarea->right_click_menus, menuitem_delete );
  drawingarea->n_right_click_menu_handlers++;

  
  /* required second to last, referenced in ptr array in show/hide */
  handler_id = g_signal_connect ( GTK_WIDGET ( menuitem_addline ), "activate",
				  G_CALLBACK ( menus_addline_callback ), drawingarea );
  g_array_append_val ( drawingarea->right_click_menu_handlers, handler_id );
  g_ptr_array_add ( drawingarea->right_click_menus, menuitem_addline );
  drawingarea->n_right_click_menu_handlers++;
  
  
  /* required last, menu destroys references it holds to menuitems */
  handler_id = g_signal_connect ( GTK_WIDGET ( menu ), "map",
				  G_CALLBACK ( menus_menu_activate_callback ), drawingarea );
  g_array_append_val ( drawingarea->right_click_menu_handlers, handler_id );
  g_ptr_array_add ( drawingarea->right_click_menus, menu );
  drawingarea->n_right_click_menu_handlers++;
  
  return menu;
  
}


