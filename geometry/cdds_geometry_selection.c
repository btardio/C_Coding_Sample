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
#include "cdds_geometry_selection.h"
#include "cdds_geometry_pointmanager.h"
#include "cdds_geometry_point.h"
#include "cdds_geometry_line.h"
#include "../cdds_drawingarea.h"
#include <string.h>

/* optionally turn increment / decrement of points within a selection on */
#define NOREFSELECTION

G_DEFINE_TYPE(CdsAreaSelection, cds_area_selection, G_TYPE_OBJECT)

/*
 * Draw the selection box with dashes
 */
void
cds_area_selection_draw ( CdsAreaSelection* selection, cairo_t *cr )
{
  
  double dashes[] = {2.5, 2.5};
  int    ndash  = 2;
  double offset = 0.0;

  if ( selection->button_press )
  {
    
    cairo_save(cr); 
    
    cairo_set_dash (cr, dashes, ndash, offset);
    cairo_set_line_width (cr, 2.0);
    cairo_set_source_rgba (cr, 0.5, 0.0, 0.0, 0.6);    
    
    cairo_move_to ( cr, selection->button_press_x, selection->button_press_y );
    cairo_line_to ( cr, selection->button_press_x, selection->button_motion_y );
    cairo_line_to ( cr, selection->button_motion_x, selection->button_motion_y );
    cairo_line_to ( cr, selection->button_motion_x, selection->button_press_y );
    cairo_line_to ( cr, selection->button_press_x, selection->button_press_y );
    
    cairo_stroke (cr);
    
    cairo_restore(cr); 
  }  
  
}

/*
 * Hash keys are equal when the address of the CDDSGeoPoint is equal
 */
gboolean
cds_area_selection_hash_key_equal ( gconstpointer a, gconstpointer b )
{
  return a == b;
}

/*
 * Removing hash key callback function, removing keys causes selected to be false and decrement of ref count of point
 */
gboolean
cds_area_selection_hash_foreach_remove (gpointer key, gpointer value, gpointer data)
{
  CDDSGeoPoint* p;
  p = key;

  p->selected = FALSE;
  
#ifndef NOREFSELECTION
  cdds_geopoint_unref ( p );
#endif
  
  return TRUE; /* returning true deletes the key/value */
}

/*
 * Looking up a point in the geopointmanager queries 7 points around the point the user clicks
 */
CDDSGeoPoint
*cds_area_selection_lookup_point ( CdsAreaPointManager* mgr, gint x, gint y )
{
  CDDSGeoPoint* p;
  
  p = cds_area_pointmanager_lookup_pointxy ( mgr, x, y );

  if ( p == NULL )
  {
    for ( int x_offset = -7; p == NULL && x_offset < 8; x_offset++ )
    {
      for ( int y_offset = -7; p == NULL && y_offset < 8; y_offset++ )
      {
	p = cds_area_pointmanager_lookup_pointxy ( mgr, x + x_offset, y + y_offset );	  
      }
    }
  }
}

/*
 * new points inserted onto the canvas cause the selection pixbuf to update with a new channel one value of 0xff
 * points removed cause the selection pixbuf to update with a new channel one value of 0x00
 */
void
cds_area_selection_put_pixel ( CdsAreaSelection* selection, int x, int y, guchar pixel)
{
  
  gint width;
  gint height;
  gint rowstride;
  gint n_channels;
  guchar* pdata;
  guchar* p;
    
  width = gdk_pixbuf_get_width (selection->pixbuf);
  height = gdk_pixbuf_get_height (selection->pixbuf);
  
  n_channels = gdk_pixbuf_get_n_channels (selection->pixbuf);
  
  rowstride = gdk_pixbuf_get_rowstride ( selection->pixbuf );
  
  pdata = gdk_pixbuf_get_pixels ( selection->pixbuf );
  
  p = pdata + y * rowstride + x * n_channels;
  
  p[0] = pixel;
  
}

/*
 * adding points from a selection pixbuf iterates all points in the selection pixbuf
 * if point found in channel one 0xff lookup the point in the geopointmanager through function cds_area_selection_lookup_point
 * add the point to the selection hash table of geoselection if it hasn't been added already
 */
void
cds_area_selection_to_hash_from_pixbuf ( CdsAreaSelection* selection, CdsAreaPointManager* mgr, GdkPixbuf* subpbuf, 
					    gint startx, gint starty, gint width, gint height, gboolean add )
{
  CDDSGeoPoint* p;
  gint rowstride;
  gint n_channels;
  guchar* pixbufp;
  guchar* pdata;
  GPtrArray* ptrarray;
  
  pdata = gdk_pixbuf_get_pixels ( subpbuf );

  n_channels = gdk_pixbuf_get_n_channels ( subpbuf );
  rowstride = gdk_pixbuf_get_rowstride ( subpbuf );
  
  for ( int xiter = 0; xiter < width; xiter++ )
  {
    for ( int yiter = 0; yiter < height; yiter++ )
    {
      pixbufp = pdata + yiter * rowstride + xiter * n_channels;
      
      if ( pixbufp[0] == PIXBUF_POINT )
      {
	ptrarray = cds_area_pointmanager_lookup_array_xy ( mgr, xiter + startx, yiter + starty );
	
	if ( ptrarray->len == 0 ) { g_warning ("Selecting a pixbuf point without a valid point in geo point manager."); }
	else
	{
	  for ( guint i = 0; i < ptrarray->len; i++ )
	  {
	    p = g_ptr_array_index ( ptrarray, i );
	    
	    if ( g_hash_table_lookup ( selection->hash_points, p ) == NULL )
	    {
	      if ( add )
	      {
		
		p->selected = TRUE;
		cds_area_selection_insert_pointp ( selection, p );

	      }
	    }
	  }
	}
/*
	p = cds_area_selection_lookup_point ( mgr, xiter + startx, yiter + starty );
	
	if ( p != NULL )
	{
	  if ( g_hash_table_lookup ( selection->hash_points, p ) == NULL )
	  {
	    if ( add )
	    {
	      
	      p->selected = TRUE;
	      cds_area_selection_insert_pointp ( selection, p );

	    }
	  }
	}
	*/

	

      }
      
    }
  }
}

/*
 * begin a selection, store the start of the selection, clear previous selections unless shift modifier
 */
void
cds_area_selection_select_press ( CdsAreaSelection* selection, CdsAreaPointManager* mgr, GdkEventButton *event )
{
  CDDSGeoPoint* p;
  
  selection->button_press = TRUE;
  
  /* set the selection instance variables to the x,y of the event */
  selection->button_press_x = (gint) round ( event->x );
  selection->button_press_y = (gint) round ( event->y );  
  
  /* bounds checking */
  if ( selection->button_press_x > *selection->width )
  {
    selection->button_press_x = *selection->width;
  }
  if ( selection->button_press_y > *selection->width )
  {
    selection->button_press_y = *selection->width;
  }
  
  /* set the selection instance variables to the x,y of the event */
  selection->button_motion_x = selection->button_press_x;
  selection->button_motion_y = selection->button_press_y;

  /* a new selection:  remove all hash_points if 
   *  if the shift button is not pressed and 
   *  if the cursor is not over a point
   *  if the right mouse button was not clicked
   */
/*
  if ( !selection->key_lshift && 
       event->button != 3 && 
       !cds_area_selection_lookup_point ( mgr, selection->button_press_x, selection->button_press_y ) )
  {
    g_hash_table_foreach_remove ( selection->hash_points, cds_area_selection_hash_foreach_remove, NULL );
  }
*/

  if ( !selection->key_lshift && 
       event->button != 3 )
       
  {
    if ( ! ( p = cds_area_selection_lookup_point ( mgr, selection->button_press_x, selection->button_press_y ) ) )
    {
      //g_hash_table_foreach_remove ( selection->hash_points, cds_area_selection_hash_foreach_remove, NULL );
      cds_area_selection_clear_selection ( selection );
    }
    else
    {
      if ( !cds_area_selection_contains_point ( selection, p ) )
      {
	//g_hash_table_foreach_remove ( selection->hash_points, cds_area_selection_hash_foreach_remove, NULL );
	cds_area_selection_clear_selection ( selection );
	p->selected = TRUE;
	cds_area_selection_insert_pointp ( selection, p );
      }
      selection->button_press = FALSE; /* do not start a selection, when the cursor starts over a point, start a move */
    }
  }

  

}





/*
 * required by the drawing function of the selection box
 */
void
cds_area_selection_select_motion ( CdsAreaSelection* selection, CdsAreaPointManager* mgr, GdkEventMotion *event )
{
  selection->button_motion_x = (gint) round ( event->x );
  selection->button_motion_y = (gint) round ( event->y );
}

/*
 * when the button is released in boxselect mode 
 * if the button release is same as button press, toggle selection and include point in selection hash table
 * if there is a selection area, create a sub pixbuf of the selection area, add points in that area to the selection
 */
void
cds_area_selection_select_release ( CdsAreaSelection* selection, CdsAreaPointManager* mgr, GdkEventButton *event )
{
  CDDSGeoPoint* p;
  GdkPixbuf* subpbuf;
  GdkPixbuf* oldsubpbuf;
  guint n_pdata;
  
  gint startx;
  gint starty;
  gint width;
  gint height;

  gint button_release_x;
  gint button_release_y;  
  
  /* 
   * Check to see if the button press and release are the same, if so select or deselect based on shift key_lctrl
   * if the selection is a box ie button press != button release, transfer control to cds_area_selection_to_hash_from_pixbuf
   */
  if ( selection->button_press )
  {
    selection->button_press = FALSE;
    
    button_release_x = (gint) round ( event->x );
    button_release_y = (gint) round ( event->y );      
    
    if ( button_release_x == selection->button_press_x && button_release_y == selection->button_press_y )
    {
      p = cds_area_selection_lookup_point ( mgr, button_release_x, button_release_y );
      
      if ( p != NULL && selection->key_lshift )
      {

	if ( p->selected )
	{
	  p->selected = FALSE;
	  cds_area_selection_delete_pointp ( selection, p );
	}
	else
	{
	  p->selected = TRUE;
	  cds_area_selection_insert_pointp ( selection, p );
	}
      }
      else if ( p != NULL && g_hash_table_size ( selection->hash_points ) == 0 )
      {
	
	p->selected = TRUE;
	cds_area_selection_insert_pointp ( selection, p );
	
      }
    }
    else
    {

      if ( button_release_x > *selection->width )
      {
	button_release_x = *selection->width;
      }
      if ( button_release_y > *selection->width )
      {
	button_release_y = *selection->width;
      }
      if ( button_release_x < 0)
      {
	button_release_x = 0;
      }
      if ( button_release_y < 0 )
      {
	button_release_y = 0;
      }
      
      startx = MIN( selection->button_press_x, button_release_x );
      starty = MIN( selection->button_press_y, button_release_y );
      width = MAX( selection->button_press_x, button_release_x ) - startx;
      height = MAX( selection->button_press_y, button_release_y ) - starty;
      
      if ( width == 0 ) 
      {
	width = 1; 
      }
      
      if ( height == 0 )
      {
	height = 1;
      }

      subpbuf = gdk_pixbuf_new_subpixbuf ( selection->pixbuf, startx, starty, width, height );
	  
      cds_area_selection_to_hash_from_pixbuf ( selection, mgr, subpbuf, 
					      startx, starty, width, height, TRUE );
      
      g_object_unref ( subpbuf );
    }
    
  }  
}


/*
 * toggle selection variables based on key pressed
 */
void
cds_area_selection_select_keypress ( CdsAreaSelection* selection, guint keyval )
{
  switch ( keyval )
  {
    case GDK_KEY_Shift_L:
      selection->key_lshift = TRUE;
      break;
    case GDK_KEY_Control_L:
      selection->key_lctrl = TRUE;
      break;
  }
}

/*
 * toggle selection variables based on key released
 */
void
cds_area_selection_select_keyrelease ( CdsAreaSelection* selection, guint keyval )
{
  switch ( keyval )
  {
    case GDK_KEY_Shift_L:
      selection->key_lshift = FALSE;
      break;
    case GDK_KEY_Control_L:
      selection->key_lctrl = FALSE;
      break;
  }
}


void
cds_area_selection_clear_selection ( CdsAreaSelection* selection )
{
  
  g_hash_table_foreach_remove ( selection->hash_points, cds_area_selection_hash_foreach_remove, NULL );
  g_hash_table_foreach_remove ( selection->hash_lines, cds_area_selection_hash_foreach_remove, NULL );
}

void
cds_area_selection_points_clear_selection ( CdsAreaSelection* selection )
{
  g_hash_table_foreach_remove ( selection->hash_points, cds_area_selection_hash_foreach_remove, NULL );
}

void
cds_area_selection_lines_clear_selection ( CdsAreaSelection* selection )
{
  g_hash_table_foreach_remove ( selection->hash_lines, cds_area_selection_hash_foreach_remove, NULL );
}


void
cds_area_selection_insert_linel ( CdsAreaSelection* selection, CdsAreaLine* l )
{
  g_hash_table_add ( selection->hash_lines, l );
  if ( selection->pixbuf != NULL )
  {
    l->selected = TRUE;
  }
}

void
cds_area_selection_insert_pointp ( CdsAreaSelection* selection, CDDSGeoPoint* p )
{  
  g_hash_table_add ( selection->hash_points, p );
  
  /* selection is used in actionmanager's undo redo queue, these points shouldnt stay selected, 
   * if there is a pixbuf it indicates the selection is the geopointmanager primary selection and selection should show
   */
  if ( selection->pixbuf != NULL )
  {
    p->selected = TRUE;
  }
}

void
cds_area_selection_delete_pointp ( CdsAreaSelection* selection, CDDSGeoPoint* p )
{  
  g_hash_table_remove ( selection->hash_points, p );
}

void
cds_area_selection_delete_linep ( CdsAreaSelection* selection, CdsAreaLine* l )
{
  g_hash_table_remove ( selection->hash_lines, l );
}


gboolean
cds_area_selection_contains_point ( CdsAreaSelection* selection, CDDSGeoPoint* p )
{
  return g_hash_table_contains ( selection->hash_points, p );
}

gboolean
cds_area_selection_contains_line ( CdsAreaSelection* selection, CdsAreaLine* l )
{
  return g_hash_table_contains ( selection->hash_lines, l );
}

guint
cds_area_selection_len ( CdsAreaSelection* selection )
{
  return g_hash_table_size ( selection->hash_points ) + g_hash_table_size ( selection->hash_lines );
}
  
guint
cds_area_selection_points_len ( CdsAreaSelection* selection )
{
  return g_hash_table_size ( selection->hash_points );
}

guint
cds_area_selection_lines_len ( CdsAreaSelection* selection )
{
  return g_hash_table_size ( selection->hash_lines );
}

  
GPtrArray*
cds_area_selection_points_to_ptrarray ( CdsAreaSelection* selection )
{
  gpointer *array;
  guint length;
  GPtrArray* ptrarray;
  gpointer p;
  
  array = g_hash_table_get_keys_as_array ( selection->hash_points, &length );
  ptrarray = g_ptr_array_sized_new ( length );
  
  for ( int i = 0; i < length; i++ )
  {
    p = array[i];
    g_ptr_array_add ( ptrarray, p );
  }
  g_free ( array );
  return ptrarray;
}

GPtrArray*
cds_area_selection_lines_to_ptrarray ( CdsAreaSelection* selection )
{
  gpointer *array;
  guint length;
  GPtrArray* ptrarray;
  gpointer l;
  
  array = g_hash_table_get_keys_as_array ( selection->hash_lines, &length );
  ptrarray = g_ptr_array_sized_new ( length );
  
  for ( int i = 0; i < length; i++ )
  {
    l = array[i];
    g_ptr_array_add ( ptrarray, l );
  }
  g_free ( array );
  return ptrarray;
}

  

/*
 * called by cdds_actionmanager, new points update the selection pixbuf
 */
void
cds_area_selection_mark_pointp ( managers* mgrs, CDDSGeoPoint* p )
{
        
    cds_area_selection_put_pixel ( mgrs->smgr, p->point_x, p->point_y, PIXBUF_POINT );
}

/*
 * called by cdds_actionmanager, new points update the selection pixbuf
 */
void
cds_area_selection_mark_pointxy ( managers* mgrs, gint x, gint y )
{
        
    cds_area_selection_put_pixel ( mgrs->smgr, x, y, PIXBUF_POINT );
}

/*
 * called by cdds_actionmanager, discard points update the selection pixbuf
 */
void
cds_area_selection_unmark_pointp (CdsAreaSelection* selection, CDDSGeoPoint* p)
{
    /* todo: lookup point and check if there are other points occupying the same coordinate */
    
    cds_area_selection_put_pixel ( selection, p->point_x, p->point_y, PIXBUF_EMPTY );
}

/*
 * called by cdds_actionmanager, discard points update the selection pixbuf
 */
void
cds_area_selection_unmark_pointxy (CdsAreaSelection* selection, gint x, gint y)
{
    /* todo: lookup point and check if there are other points occupying the same coordinate */
        
    cds_area_selection_put_pixel ( selection, x, y, PIXBUF_EMPTY );
}


void
cds_area_selection_ref_count_increase_foreach (gpointer key, gpointer value, gpointer data)
{
  cdds_geopoint_ref ( (CDDSGeoPoint*)key );
}

void
cds_area_selection_ref_count_decrease_foreach (gpointer key, gpointer value, gpointer data)
{
  cdds_geopoint_unref ( (CDDSGeoPoint*)key );
}


/*
 * Increase reference count of each point in selection, referrer is actionmanager, undo redo
 */
void
cds_area_selection_increase_ref_count_points ( CdsAreaSelection* selection )
{

  g_hash_table_foreach ( selection->hash_points, cds_area_selection_ref_count_increase_foreach, NULL );
  
}

/*
 * Decrease reference count of each point in selection, referrer is actionmanager, undo redo
 */
void
cds_area_selection_decrease_ref_count_points ( CdsAreaSelection* selection )
{

  g_hash_table_foreach ( selection->hash_points, cds_area_selection_ref_count_decrease_foreach, NULL );
  
}

void
cds_area_selection_foreach_copy_points (gpointer key, gpointer value, gpointer data)
{
  GHashTable* selection_copy;
  selection_copy = data;
  
  g_hash_table_add ( selection_copy, key );

#ifndef NOREFSELECTION
  cdds_geopoint_ref ( (CDDSGeoPoint*) key );
#endif
  
}

CdsAreaSelection
*cds_area_selection_selection_from_selection ( CdsAreaSelection* selection )
{
  
  CdsAreaSelection* selectionnew;
  
  selectionnew = g_object_new (CDS_AREA_TYPE_SELECTION, NULL);
  
  selectionnew->width = selection->width;
  selectionnew->height = selection->height;
  
  g_hash_table_foreach ( selection->hash_points, cds_area_selection_foreach_copy_points, selectionnew->hash_points );
  
  return selectionnew;
}


void
cds_area_selection_init_pixbuf ( CdsAreaSelection* selection )
{
  
  selection->pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, *selection->width+1, *selection->height+1);
  gdk_pixbuf_fill (selection->pixbuf, 0x000000);

  
}


void
cds_area_selection_set_property ( GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec )
{
  CdsAreaSelection *self = CDS_AREA_SELECTION ( object );

  switch ( property_id )
  {
    
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    break;
  }
}

void
cds_area_selection_get_property ( GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec )
{
  CdsAreaSelection *self = CDS_AREA_SELECTION ( object );

  switch (property_id)
  {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}



static void
cds_area_selection_init (CdsAreaSelection *selection)
{
  GValue gval_int = G_VALUE_INIT;
  GValue gval_float = G_VALUE_INIT;

  g_value_init ( &gval_int, G_TYPE_INT );
  g_value_init ( &gval_float, G_TYPE_FLOAT );
  
  g_value_unset ( &gval_float );
  g_value_unset ( &gval_int );
  
  selection->hash_points = g_hash_table_new ( g_direct_hash, cds_area_selection_hash_key_equal );
  
  selection->hash_lines = g_hash_table_new ( g_direct_hash, cds_area_selection_hash_key_equal );
  
}


static void
cds_area_selection_class_init (CdsAreaSelectionClass *klass)
{

  GObjectClass   *gobject_class;
  
  gobject_class = G_OBJECT_CLASS (klass);
  
  //cds_area_selection_parent_class = g_type_class_peek_parent (klass);
  
  gobject_class->set_property = cds_area_selection_set_property;
  gobject_class->get_property = cds_area_selection_get_property;
  
  /*
  g_object_class_install_properties (gobject_class,
                                     CDDS_GEOSELECTION_N_PROPERTIES,
                                     cds_area_selection_properties);  
  */
}







CdsAreaSelection
*cds_area_selection_new ( gint* width, gint* height )
{
  
  CdsAreaSelection* selection;
  
  selection = g_object_new (CDS_AREA_TYPE_SELECTION, NULL);
  
  selection->width = width;
  selection->height = height;
  
  //selection->pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, *selection->width, *selection->height);
  //gdk_pixbuf_fill (selection->pixbuf, 0x000000);
  
  
  
  return selection;
}


void
cds_area_selection_destroy (CdsAreaSelection* selection)
{
#ifndef NOREFSELECTION
  cds_area_selection_decrease_ref_count_points ( selection );
  // g_object_unref ( selection );
#endif
  
  g_hash_table_destroy ( selection->hash_lines );
  
  g_hash_table_destroy ( selection->hash_points );
  
  if ( selection->pixbuf != NULL )
  {
    g_object_unref ( selection->pixbuf );
    
  }
  
  g_object_unref ( selection ); 
  
}










