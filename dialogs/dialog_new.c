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

#include "../cdds.h"
#include "dialog_new.h"
#include "../cdds_drawingarea.h"
#include "../cdds_window.h"

G_DEFINE_TYPE(CdsAreaDialog, cds_area_dialog, GTK_TYPE_DIALOG)


void
cds_area_dialog_activate_callback ( GtkMenuItem *menuitem, gpointer data )
{
  CdsAreaWindow* win;
  CdsAreaDialog* dialog_new;
  gint result;  
  win = data;
  
  dialog_new = cds_area_dialog_new( win );
  
  result = gtk_dialog_run ( GTK_DIALOG ( dialog_new ) );
  
  
  switch ( result ) {
    case GTK_RESPONSE_ACCEPT:
      
      if ( win->area != NULL ) {
	//cds_area_drawingarea_destroy(win->area); win->area = NULL;
	cds_area_window_destroy_drawingarea ( win );
      }
  
      cds_area_window_create_drawingarea ( win, 
					   atoi ( gtk_entry_get_text ( dialog_new->entry_width ) ), 
					   atoi ( gtk_entry_get_text ( dialog_new->entry_height ) ) );
      
/*
      win->area = cds_area_drawingarea_new( atoi ( gtk_entry_get_text ( dialog_new->entry_width ) ), 
					atoi ( gtk_entry_get_text ( dialog_new->entry_height ) ) );

      
      
      gtk_container_add( GTK_CONTAINER ( win->viewport ), GTK_WIDGET ( win->area ) );
      
      gtk_widget_set_halign ( GTK_WIDGET ( win->viewport ), GTK_ALIGN_CENTER );
      gtk_widget_set_valign ( GTK_WIDGET ( win->viewport ), GTK_ALIGN_CENTER );
      
      gtk_widget_realize( GTK_WIDGET ( win->area ) );
      gtk_widget_show( GTK_WIDGET ( win->area ) );
*/
      
      
      gtk_toggle_button_set_active ( win->tog_box, FALSE );
      gtk_toggle_button_set_active ( win->tog_select, FALSE );
      gtk_toggle_button_set_active ( win->tog_point, FALSE );
      gtk_toggle_button_set_active ( win->tog_line, FALSE );
            
      break;
    case GTK_RESPONSE_REJECT:
      
      break;
  }
  
  cds_area_dialog_destroy ( dialog_new );
  

}

void
cds_area_dialog_create_clicked_callback ( GtkButton *widget, gpointer data )
{
  GtkDialog* dialog;
  dialog = data;
  
  gtk_dialog_response ( dialog, GTK_RESPONSE_ACCEPT );
  
}

void
cds_area_dialog_cancel_clicked_callback ( GtkButton *widget, gpointer data )
{
  GtkDialog* dialog;
  dialog = data;
    
  gtk_dialog_response ( dialog, GTK_RESPONSE_REJECT );
  
}

/*
 * init function 
 */
static void
cds_area_dialog_init ( CdsAreaDialog *dialog )
{

  gtk_widget_init_template ( GTK_WIDGET ( dialog ) );
    
}



/*
 * creates private member variables for each template variable
 */
static void
cds_area_dialog_class_init ( CdsAreaDialogClass *klass )
{

  GtkWidgetClass *widget_class;
  widget_class = GTK_WIDGET_CLASS(klass);
  
  gtk_widget_class_set_template_from_resource ( GTK_WIDGET_CLASS ( klass ), "/org/gtk/cdds/dialogs/CDDSDialogNew.glade" );
  
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( klass ), CdsAreaDialog, entry_width );
  gtk_widget_class_bind_template_child ( GTK_WIDGET_CLASS ( klass ), CdsAreaDialog, entry_height );
  
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS ( klass ), cds_area_dialog_create_clicked_callback);
  gtk_widget_class_bind_template_callback ( GTK_WIDGET_CLASS ( klass ), cds_area_dialog_cancel_clicked_callback);
  
  
}




CdsAreaDialog
*cds_area_dialog_new ( CdsAreaWindow* win )
{
  CdsAreaDialog* dialog = g_object_new ( CDS_AREA_TYPE_DIALOG, NULL );
  
  gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW ( win ) );
  
  return dialog;
}

void
cds_area_dialog_destroy (CdsAreaDialog *dialog)
{
  
  //gtk_widget_destroy ( GTK_WIDGET (dialog->entry_width) );
  //gtk_widget_destroy ( GTK_WIDGET (dialog->entry_height) );
  
  //gtk_widget_destroy ( GTK_WIDGET (dialog->btn_create) ); 
  //gtk_widget_destroy ( GTK_WIDGET (dialog->btn_cancel) );
  
  gtk_widget_destroy ( GTK_WIDGET (dialog) );
}


