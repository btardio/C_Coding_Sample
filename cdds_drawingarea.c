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
#include "cdds_drawingarea.h"
#include "geometry/cdds_geometry_pointmanager.h"
#include "geometry/cdds_geometry_linemanager.h"
#include "geometry/cdds_geometry_selection.h"
#include "geometry/cdds_geometry_actionmanager.h"
#include "cdds_window.h"
#include "geometry/cdds_geometry_point.h"
#include "geometry/cdds_geometry_action.h"
#include "geometry/cdds_geometry_line.h"

/* todo/bug: right clicking a point raises the refcount */


G_DEFINE_TYPE(CdsAreaDrawingArea, cds_area_drawingarea, GTK_TYPE_WINDOW)

gboolean
cds_area_drawingarea_draw (GtkWidget *widget, cairo_t *cr)
{
  CdsAreaDrawingArea* area;
  area = CDS_AREA_DRAWINGAREA(widget);
  
  double dashes[] = {2.5,  /* ink */ 2.5,  /* skip */ 2.5,  /* ink */ 2.5   /* skip*/ };
  int    ndash  = sizeof (dashes)/sizeof(dashes[0]);
  double offset = 0.0;
  
  cairo_save(cr); /* draw grid start */
  
  cairo_set_dash (cr, dashes, ndash, offset);
  cairo_set_line_width (cr, 1.0);
  cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.6);
  
  for ( int i_x = 0; i_x < area->width; i_x += 20 ) {
    for ( int i_y = 0; i_y < area->height; i_y += 20 ) {
      
      cairo_move_to ( cr, i_x, i_y);
      cairo_line_to ( cr, i_x + 20, i_y);
      cairo_move_to ( cr, i_x, i_y);
      cairo_line_to ( cr, i_x, i_y + 20);
      
    } 
  }
  
  cairo_move_to (cr, 0.5, 0.5);
  cairo_line_to (cr, 100 - 0.5, 0.5);
  cairo_move_to (cr, 0.5, 0.5);
  cairo_line_to (cr, 0.5, 100 - 0.5);
  
  cairo_stroke (cr);  
  
  cairo_restore(cr); /* draw grid end */
    
  /* cds_area_pointmanager_draw_points ( area->pointsmgr, cr ); */
  cds_area_pointmanager_draw_points ( area->mgrs->pmgr, cr );
  
  cds_area_linemanager_draw_lines ( area->mgrs->lmgr, cr );
  
  /* cds_area_selection_draw ( area->selection, cr ); */
  cds_area_selection_draw ( area->mgrs->smgr, cr );    
  
  return FALSE;
  
}

#ifdef CDDS_TEST
void
menuitem_test_callback (GtkMenuItem *menuitem, gpointer data)
{
  
  CdsAreaWindow* win;
  win = data;

  //cdds_geometry_test ( win->area );
  g_test_run ();
  
//  gtk_widget_queue_draw ( GTK_WIDGET ( win->area ) );
}
#endif


void
menuitem_undo_callback (GtkMenuItem *menuitem, gpointer data)
{
  
  CdsAreaWindow* win;
  win = data;
  
  /* cds_area_actionmanager_undo ( win->area->actions, win->area->selection, win->area->pointsmgr ); */
  cds_area_actionmanager_undo ( win->area->mgrs );
  
  gtk_widget_queue_draw ( GTK_WIDGET ( win->area ) );
}


void
menuitem_redo_callback (GtkMenuItem *menuitem, gpointer data)
{
  
  CdsAreaWindow* win;
  win = data;
  
  /* cds_area_actionmanager_redo ( win->area->actions, win->area->selection, win->area->pointsmgr ); */
  cds_area_actionmanager_redo ( win->area->mgrs );
  gtk_widget_queue_draw ( GTK_WIDGET ( win->area ) );
}


void
toggle_select_callback (GtkToggleButton *togglebutton, gpointer data)
{
  CdsAreaWindow* win;
  win = data;

  if ( gtk_toggle_button_get_active(togglebutton) ){
    gtk_toggle_button_set_active(win->tog_box, FALSE);
    gtk_toggle_button_set_active(win->tog_line, FALSE);
    gtk_toggle_button_set_active(win->tog_point, FALSE);
    
    if ( win->area != NULL )
    {
      win->area->toggle = TOGGLE_SELECT;
    }
  }
  else
  {
    win->area->toggle = -1;
  }  
}


void
toggle_point_callback (GtkToggleButton *togglebutton, gpointer data)
{
  CdsAreaWindow* win;
  win = data;
  
  if ( gtk_toggle_button_get_active(togglebutton) ){
    gtk_toggle_button_set_active(win->tog_box, FALSE);
    gtk_toggle_button_set_active(win->tog_line, FALSE);
    gtk_toggle_button_set_active(win->tog_select, FALSE);
    if ( win->area != NULL )
    {
      win->area->toggle = TOGGLE_POINT;
    }
  }
  else
  {
    win->area->toggle = -1;
  }  


  
}

void
toggle_line_callback (GtkToggleButton *togglebutton, gpointer data)
{
  CdsAreaWindow* win;
  win = data;
  
  if ( gtk_toggle_button_get_active(togglebutton) ){
    gtk_toggle_button_set_active(win->tog_box, FALSE);
    gtk_toggle_button_set_active(win->tog_point, FALSE);
    gtk_toggle_button_set_active(win->tog_select, FALSE);
    if ( win->area != NULL )
    {
      win->area->toggle = TOGGLE_LINE;
    }
  }
  else
  {
    win->area->toggle = -1;
  }  

  
}

void
toggle_box_callback (GtkToggleButton *togglebutton, gpointer data)
{
  CdsAreaWindow* win;
  win = data;
  
  if ( gtk_toggle_button_get_active(togglebutton) ){
    gtk_toggle_button_set_active(win->tog_line, FALSE);
    gtk_toggle_button_set_active(win->tog_point, FALSE);
    gtk_toggle_button_set_active(win->tog_select, FALSE);
    if ( win->area != NULL )
    {
      win->area->toggle = TOGGLE_BOX;
    }
  }
  else
  {
    win->area->toggle = -1;
  }  

}

void
cds_area_drawing_area_key_press_event ( CdsAreaDrawingArea* area, GdkEventKey *event )
{
  
  switch ( event->keyval )
  {
    case GDK_KEY_Shift_L:
      area->key_lshift = TRUE;
      break;
    case GDK_KEY_Control_L:
      area->key_lctrl = TRUE;
      break;
      
    case GDK_KEY_z:
    case GDK_KEY_Z:
      
      if ( area->key_lctrl )
      {
	if ( area->key_lshift )
	{
	  /* cds_area_actionmanager_redo ( area->actions, area->selection, area->pointsmgr ); */
	  cds_area_actionmanager_redo ( area->mgrs );
	}
	else
	{
	  /* cds_area_actionmanager_undo ( area->actions, area->selection, area->pointsmgr ); */
	  cds_area_actionmanager_undo ( area->mgrs );
	}
      }
      break;
  }

  
  switch ( area->toggle )
  {
    case TOGGLE_SELECT:
      cds_area_selection_select_keypress ( area->mgrs->smgr, event->keyval );
            
      switch ( event->keyval )
      {
	case GDK_KEY_Delete:
	  /* cds_area_actionmanager_selected_points_delete ( area->actions, area->selection, area->pointsmgr ); */
	  cds_area_actionmanager_selected_points_delete ( area->mgrs );
	  break;
      }
      break;
  }
  
  gtk_widget_queue_draw ( GTK_WIDGET ( area ) );
}



void
cds_area_drawing_area_key_release_event ( CdsAreaDrawingArea* area, GdkEventKey *event )
{
  
  switch ( event->keyval )
  {
    case GDK_KEY_Shift_L:
      area->key_lshift = FALSE;
      break;
    case GDK_KEY_Control_L:
      area->key_lctrl = FALSE;
      break;
  }
  
  
  switch ( area->toggle )
  {
    case TOGGLE_SELECT:
      /*cds_area_selection_select_keyrelease ( area->selection, event->keyval ); */
      cds_area_selection_select_keyrelease ( area->mgrs->smgr, event->keyval );
      
      
      break;
  }
  
  gtk_widget_queue_draw ( GTK_WIDGET ( area ) );
}

gboolean
cds_area_drawing_area_button_press_event ( GtkWidget *widget, GdkEventButton *event )
{
  CdsAreaDrawingArea* area;  
  area = CDS_AREA_DRAWINGAREA(widget);
  
  area->last_button_press_x = event->x;
  area->last_button_press_y = event->y;
  
  /* right mouse button */
  if ( event->button == 3 )
  {
    /* if ( cds_area_selection_lookup_point ( area->pointsmgr, (gint) round ( event->x ), (gint) round ( event->y ) ) ) */
    if ( cds_area_selection_lookup_point ( area->mgrs->pmgr, (gint) round ( event->x ), (gint) round ( event->y ) ) )
    {
      gtk_menu_popup (area->toggle_selection_hover_point_right_click_menu, 
		      NULL, NULL, NULL, NULL, event->button, event->time);
    }
    else
    {
      gtk_menu_popup (area->toggle_selection_nohover_right_click_menu, 
		      NULL, NULL, NULL, NULL, event->button, event->time);
    }
  }
  

  
  switch ( area->toggle )
  {
    case TOGGLE_SELECT:    
      
      cds_area_selection_select_press ( area->mgrs->smgr, area->mgrs->pmgr, event );
      //cds_area_actionmanager_startaction ( area->mgrs, event->x, event->y, area->toggle ); 
      break;
  }
  
  cds_area_actionmanager_startaction ( area->mgrs, event->x, event->y, area->toggle ); 
  
  gtk_widget_queue_draw ( widget );
  return GDK_EVENT_PROPAGATE;
}


gboolean
cds_area_drawing_area_motion_notify_event ( GtkWidget *widget, GdkEventMotion *event )
{
  CdsAreaDrawingArea* area;  
  area = CDS_AREA_DRAWINGAREA(widget);
  
  switch ( area->toggle )
  {
    case TOGGLE_SELECT:
      
      cds_area_selection_select_motion ( area->mgrs->smgr, area->mgrs->pmgr, event );
      
      //cds_area_actionmanager_progressaction ( area->mgrs, event->x, event->y, area->toggle );
      break;
  }  
  cds_area_actionmanager_progressaction ( area->mgrs, event->x, event->y, area->toggle );
  gtk_widget_queue_draw ( widget );
  return GDK_EVENT_PROPAGATE;
}

gboolean
cds_area_drawing_area_button_release_event ( GtkWidget *widget, GdkEventButton *event )
{

  CdsAreaDrawingArea* area;
  CDDSGeoPoint* p;
  
  area = CDS_AREA_DRAWINGAREA(widget);
    
  switch ( area->toggle )
  {
    case TOGGLE_POINT:
      
      //cds_area_actionmanager_endaction ( area->mgrs, event->x, event->y, area->toggle );
      break;
    case TOGGLE_SELECT:    
      
      cds_area_selection_select_release ( area->mgrs->smgr, area->mgrs->pmgr, event );
      
      //cds_area_actionmanager_endaction ( area->mgrs, event->x, event->y, area->toggle );
      break;   
  }  
  
  cds_area_actionmanager_endaction ( area->mgrs, event->x, event->y, area->toggle );
  
  gtk_widget_queue_draw ( widget );
  return GDK_EVENT_PROPAGATE;
}

gint
cds_area_drawing_area_mouse_to_canvas_d ( CdsAreaDrawingArea* area, gdouble xory )
{
  return lrint ( ((double)xory) / ((double)area->prop_grid_interval) ) * area->prop_grid_interval;
}

gint
cds_area_drawing_area_mouse_to_canvas_i ( CdsAreaDrawingArea* area, gint xory )
{
  return lrint ( ((double)xory) / ((double)area->prop_grid_interval) ) * area->prop_grid_interval;
}


xycoord
cds_area_drawing_area_mouse_to_canvas_xy_d ( CdsAreaDrawingArea* area, gdouble x, gdouble y )
{
  xycoord xy;

  xy.x = lrint ( x );
  xy.y = lrint ( y );
  
  if ( area->prop_snap_to_grid )
  {
    /* xy.x = lrint ( ((double)xy.x) / ((double)area->prop_grid_interval) ) * area->prop_grid_interval; */
    /* xy.y = lrint ( ((double)xy.y) / ((double)area->prop_grid_interval) ) * area->prop_grid_interval; */
    xy.x = cds_area_drawing_area_mouse_to_canvas_d ( area, x );
    xy.y = cds_area_drawing_area_mouse_to_canvas_d ( area, y );
  }
  
  return xy;
}

xycoord
cds_area_drawing_area_mouse_to_canvas_xy_i ( CdsAreaDrawingArea* area, gint x, gint y )
{
  xycoord xy;

  xy.x = x;
  xy.y = y;
  
  if ( area->prop_snap_to_grid )
  {
    /* xy.x = lrint ( ((float)xy.x) / ((float)area->prop_grid_interval) ) * area->prop_grid_interval; */
    /* xy.y = lrint ( ((float)xy.y) / ((float)area->prop_grid_interval) ) * area->prop_grid_interval; */
    xy.x = cds_area_drawing_area_mouse_to_canvas_i ( area, x );
    xy.y = cds_area_drawing_area_mouse_to_canvas_i ( area, y );

  }
  
  return xy;
}

/*
 * any event causing a preferred width query is sent CDDSDrawingArea->width
 */
void
cds_area_drawingarea_get_preferred_width (GtkWidget *widget, gint *minimum_width, gint *natural_width)
{
  CdsAreaDrawingArea *area;
  area = CDS_AREA_DRAWINGAREA(widget);
  *minimum_width = *natural_width = area->width;
}


/*
 * any event causing a preferred height query is sent CDDSDrawingArea->height
 */
void
cds_area_drawingarea_get_preferred_height (GtkWidget *widget, gint *minimum_height, gint *natural_height)
{
  CdsAreaDrawingArea *area;
  area = CDS_AREA_DRAWINGAREA(widget);
  *minimum_height = *natural_height = area->height;
}



void
cds_area_drawingarea_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
  CdsAreaDrawingArea *self = CDS_AREA_DRAWINGAREA (object);

  switch (property_id)
  {
    case CDS_AREA_DRAWINGAREA_PROP_WIDTH:
      self->width = g_value_get_int ( value );
      break;

    case CDS_AREA_DRAWINGAREA_PROP_HEIGHT:
      self->height = g_value_get_int ( value );
      break;
      
    case CDS_AREA_DRAWINGAREA_PROP_SNAP_TO_GRID:
      self->prop_snap_to_grid = g_value_get_boolean ( value );
      break;
      
    case CDS_AREA_DRAWINGAREA_PROP_GRID_INTERVAL:
      self->prop_grid_interval = g_value_get_int ( value );
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
cds_area_drawingarea_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
  CdsAreaDrawingArea *self = CDS_AREA_DRAWINGAREA (object);

  switch (property_id)
  {
    case CDS_AREA_DRAWINGAREA_PROP_WIDTH:
      g_value_set_int ( value, self->width );
      break;

    case CDS_AREA_DRAWINGAREA_PROP_HEIGHT:
      g_value_set_int ( value, self->height );
      break;
      
    case CDS_AREA_DRAWINGAREA_PROP_SNAP_TO_GRID:
      g_value_set_boolean ( value, self->prop_snap_to_grid );
      break;
      
    case CDS_AREA_DRAWINGAREA_PROP_GRID_INTERVAL:
      g_value_set_int ( value, self->prop_grid_interval );
      break;
      
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}


static void
cds_area_drawingarea_init (CdsAreaDrawingArea *area)
{
  
  /* area->actions = cds_area_actionmanager_new ( ); */
  
  
  

  area->toggle_selection_nohover_right_click_menu = toggle_selection_nohover_right_click_menu_create ( area );
  area->toggle_selection_hover_point_right_click_menu = toggle_selection_hover_point_right_click_menu_create ( area );


  g_object_set ( G_OBJECT (area), "snaptogrid", TRUE, NULL );
  g_object_set ( G_OBJECT (area), "gridinterval", 20, NULL );

}

static void
cds_area_drawingarea_class_init (CdsAreaDrawingAreaClass *klass)
{

  GtkWidgetClass *widget_class;
  GObjectClass   *gobject_class;
  widget_class = GTK_WIDGET_CLASS (klass);
  gobject_class = G_OBJECT_CLASS (klass);
  
  //cdds_drawingarea_parent_class = g_type_class_peek_parent (klass);

  
  widget_class->get_preferred_height = cds_area_drawingarea_get_preferred_height;
  widget_class->get_preferred_width = cds_area_drawingarea_get_preferred_width;
  
  widget_class->draw = cds_area_drawingarea_draw;
  
  widget_class->button_press_event = cds_area_drawing_area_button_press_event;
  widget_class->motion_notify_event = cds_area_drawing_area_motion_notify_event;
  widget_class->button_release_event = cds_area_drawing_area_button_release_event;
    
  gobject_class->set_property = cds_area_drawingarea_set_property;
  gobject_class->get_property = cds_area_drawingarea_get_property;
  
  cds_area_drawingarea_properties[CDS_AREA_DRAWINGAREA_PROP_WIDTH] = g_param_spec_int ("width", "w", "Width value of area.",
                                                                            0, G_MAXINT, 0, G_PARAM_READWRITE);

  cds_area_drawingarea_properties[CDS_AREA_DRAWINGAREA_PROP_HEIGHT] = g_param_spec_int ("height", "h", "Height value of area.",
                                                                             0, G_MAXINT, 0, G_PARAM_READWRITE);

  cds_area_drawingarea_properties[CDS_AREA_DRAWINGAREA_PROP_SNAP_TO_GRID] = g_param_spec_boolean ("snaptogrid", "snaptogrid", 
												  "Snap to grid toggle.",
												  TRUE, G_PARAM_READWRITE);
  
  cds_area_drawingarea_properties[CDS_AREA_DRAWINGAREA_PROP_GRID_INTERVAL] = g_param_spec_int ("gridinterval", "gridinterval",
											       "Interval between grid lines.",
											       5, G_MAXINT, 20, 
											       G_PARAM_READWRITE);
  
  
  g_object_class_install_properties (gobject_class,
                                     CDS_AREA_DRAWINGAREA_N_PROPERTIES,
                                     cds_area_drawingarea_properties);

  gobject_class->dispose = cds_area_drawingarea_dispose; /* opposite of init */
  gobject_class->finalize = cds_area_drawingarea_finalize; /* opposite of class_init */
  
}

void
cds_area_drawingarea_new_points_treeview ( CdsAreaWindow* win, CdsAreaDrawingArea* area )
{
  
  area->treeview_points = GTK_TREE_VIEW ( gtk_tree_view_new_with_model ( GTK_TREE_MODEL ( area->liststore_points ) ) );
  
  
  area->renderer_points = gtk_cell_renderer_text_new ();
  
  area->column_x = gtk_tree_view_column_new_with_attributes ("X", area->renderer_points, "text", COLUMN_X, NULL);
  gtk_tree_view_append_column ( area->treeview_points, area->column_x );
  area->column_y = gtk_tree_view_column_new_with_attributes ("Y", area->renderer_points, "text", COLUMN_Y, NULL);
  gtk_tree_view_append_column ( area->treeview_points, area->column_y );
#ifdef CDDS_DEBUG
  area->column_refcount = gtk_tree_view_column_new_with_attributes ("Count", area->renderer_points, "text", 
								    COLUMN_REFCOUNT, NULL);
  gtk_tree_view_append_column ( area->treeview_points, area->column_refcount );
#endif
  
  //CDS_AREA_WINDOW ( gtk_widget_get_parent ( GTK_WIDGET ( area ) ) )->scrolledwindow_points
  
  //gtk_container_add ( GTK_CONTAINER ( window->scrolledwindow_points ), GTK_WIDGET ( area->treeview_points ) );
  //gtk_container_add ( GTK_CONTAINER ( CDS_AREA_WINDOW ( gtk_widget_get_parent ( GTK_WIDGET ( area ) ) )->scrolledwindow_points ), 
//		      GTK_WIDGET ( area->treeview_points ) );
  
  gtk_container_add ( GTK_CONTAINER ( win->scrolledwindow_points ), GTK_WIDGET ( area->treeview_points ) );
  
  
  gtk_widget_show ( GTK_WIDGET ( area->treeview_points ) );

}

void
cds_area_drawingarea_new_lines_treeview ( CdsAreaWindow* win, CdsAreaDrawingArea* area )
{
  
  GtkTreeViewColumn *column;
  
  area->treeview_lines = GTK_TREE_VIEW ( gtk_tree_view_new_with_model ( GTK_TREE_MODEL ( area->liststore_lines ) ) );
  
  area->renderer_lines = gtk_cell_renderer_text_new ();
  
  column = gtk_tree_view_column_new_with_attributes ("X1", area->renderer_lines, "text", 
						     LINE_COLUMN_POINTER_POINT1X, NULL);
  gtk_tree_view_append_column ( area->treeview_lines, column );
  column = gtk_tree_view_column_new_with_attributes ("Y1", area->renderer_lines, "text", 
						     LINE_COLUMN_POINTER_POINT1Y, NULL);
  gtk_tree_view_append_column ( area->treeview_lines, column );
  column = gtk_tree_view_column_new_with_attributes ("X2", area->renderer_lines, "text", 
						     LINE_COLUMN_POINTER_POINT2X, NULL);
  gtk_tree_view_append_column ( area->treeview_lines, column );
  column = gtk_tree_view_column_new_with_attributes ("Y2", area->renderer_lines, "text", 
						     LINE_COLUMN_POINTER_POINT2Y, NULL);
  gtk_tree_view_append_column ( area->treeview_lines, column );

#ifdef CDDS_DEBUG
  column = gtk_tree_view_column_new_with_attributes ("Count", area->renderer_lines, "text", 
						     LINE_COLUMN_REFCOUNT, NULL);
  gtk_tree_view_append_column ( area->treeview_lines, column );
#endif
  
  //CDS_AREA_WINDOW ( gtk_widget_get_parent ( GTK_WIDGET ( area ) ) )->scrolledwindow_points
  
  //gtk_container_add ( GTK_CONTAINER ( window->scrolledwindow_points ), GTK_WIDGET ( area->treeview_points ) );
  //gtk_container_add ( GTK_CONTAINER ( CDS_AREA_WINDOW ( gtk_widget_get_parent ( GTK_WIDGET ( area ) ) )->scrolledwindow_points ), 
//		      GTK_WIDGET ( area->treeview_points ) );
  
  // todo: points change to lines, glade edits
  gtk_container_add ( GTK_CONTAINER ( win->scrolledwindow_lines ), GTK_WIDGET ( area->treeview_lines ) );
  
  
  gtk_widget_show ( GTK_WIDGET ( area->treeview_lines ) );

}


CdsAreaDrawingArea
*cds_area_drawingarea_new ( gint width, gint height )
{
  CdsAreaDrawingArea* area;
  
  area = g_object_new ( CDS_AREA_TYPE_DRAWINGAREA, "width", width, "height", height, NULL);
  
  //drawarea = area;
  
  area->mgrs = g_malloc ( sizeof ( managers ) );
  
  area->mgrs->area = area;
  
  area->mgrs->pmgr = cds_area_pointmanager_new ( );
  
  area->mgrs->lmgr = cds_area_linemanager_new ( );
  
  area->mgrs->smgr = cds_area_selection_new( &area->width, &area->height );
  cds_area_selection_init_pixbuf ( area->mgrs->smgr ); /* selection needs a pix buf */
  
  area->mgrs->amgr = cds_area_actionmanager_new ( ); 
  
  /* prepare list store and tree view */
#ifdef CDDS_DEBUG
  area->liststore_points = gtk_list_store_new (4, G_TYPE_POINTER, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
  area->liststore_lines = gtk_list_store_new (6, G_TYPE_POINTER, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, 
					      G_TYPE_INT);
#else
  area->liststore_points = gtk_list_store_new (3, G_TYPE_POINTER, G_TYPE_INT, G_TYPE_INT);
  area->liststore_lines = gtk_list_store_new (5, G_TYPE_POINTER, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT );  
#endif
  
#ifdef CDDS_DEBUG
  area->mgrs->allactions = g_ptr_array_new ( );
  area->mgrs->allpoints = g_ptr_array_new ( );
  area->mgrs->alllines = g_ptr_array_new ( );
#endif
  
  //cds_area_drawingarea_new_treeview ( area );
  
  return area;
}


void
cds_area_drawingarea_dispose ( GObject* obj )
{

  CdsAreaDrawingArea* area;
  
  area = CDS_AREA_DRAWINGAREA ( obj );
  
  cds_area_drawingarea_right_click_menus_dispose ( area );
  
  if ( CDS_AREA_IS_DRAWINGAREA ( area ) )
  {
    
    if ( area->mgrs != NULL && area->mgrs->lmgr != NULL && CDS_AREA_IS_LINEMANAGER ( area->mgrs->lmgr ) )
    {
      cds_area_linemanager_destroy ( area->mgrs->lmgr );
      area->mgrs->lmgr = NULL;
    }
    
    if ( area->mgrs != NULL && area->mgrs->pmgr != NULL && CDS_AREA_IS_POINTMANAGER ( area->mgrs->pmgr ) )
    {
      cds_area_pointmanager_destroy ( area->mgrs->pmgr );
      area->mgrs->pmgr = NULL;
    }
    
    if ( area->mgrs != NULL && area->mgrs->smgr != NULL && CDS_AREA_IS_SELECTION ( area->mgrs->smgr ) )
    {
      cds_area_selection_destroy ( area->mgrs->smgr );
      area->mgrs->smgr = NULL;
    }
    
    if ( area->mgrs != NULL && area->mgrs->amgr != NULL && CDS_AREA_IS_ACTIONMANAGER ( area->mgrs->amgr ) )
    {
      cds_area_actionmanager_destroy ( area->mgrs->amgr );
      area->mgrs->amgr = NULL;
    }
    
    if ( area->treeview_points != NULL && GTK_IS_TREE_VIEW ( area->treeview_points ) )
    {
      gtk_widget_destroy ( GTK_WIDGET ( area->treeview_points ) );
      area->treeview_points = NULL;
    }

    if ( area->liststore_points != NULL && GTK_IS_LIST_STORE ( area->liststore_points ) )
    {
      g_object_unref ( area->liststore_points );
      area->liststore_points = NULL;
    }

    if ( area->renderer_points != NULL && GTK_IS_CELL_RENDERER ( area->renderer_points ) )
    {
      g_object_unref ( area->renderer_points );
      area->renderer_points = NULL;
    }

    
    if ( area->treeview_lines != NULL && GTK_IS_TREE_VIEW ( area->treeview_lines ) )
    {
      gtk_widget_destroy ( GTK_WIDGET ( area->treeview_lines ) );
      area->treeview_lines = NULL;
    }

    if ( area->liststore_lines != NULL && GTK_IS_LIST_STORE ( area->liststore_lines ) )
    {
      g_object_unref ( area->liststore_lines );
      area->liststore_lines = NULL;
    }

    if ( area->renderer_lines != NULL && GTK_IS_CELL_RENDERER ( area->renderer_lines ) )
    {
      g_object_unref ( area->renderer_lines );
      area->renderer_lines = NULL;
    }
    
    
    
  }

#ifdef CDDS_DEBUG

  if ( area->mgrs != NULL && area->mgrs->allactions != NULL )
  {
    g_ptr_array_free ( area->mgrs->allactions, TRUE ); area->mgrs->allactions = NULL;
  }
  
  if ( area->mgrs != NULL && area->mgrs->allpoints != NULL )
  {
    g_ptr_array_free ( area->mgrs->allpoints, TRUE ); area->mgrs->allpoints = NULL;
  }
  
  if ( area->mgrs != NULL && area->mgrs->alllines != NULL )
  {
    g_ptr_array_free ( area->mgrs->alllines, TRUE ); area->mgrs->alllines = NULL;
  }
  
#endif
  
  if ( CDS_AREA_IS_DRAWINGAREA ( area ) )
  {
    if ( area->mgrs != NULL )
    {
      g_free ( area->mgrs );
      area->mgrs = NULL;
    }
  }
  
  if ( gtk_widget_get_parent ( GTK_WIDGET ( area ) ) != NULL )
  {
    gtk_container_remove ( GTK_CONTAINER ( gtk_widget_get_parent ( GTK_WIDGET ( area ) ) ), GTK_WIDGET ( area ) );
  }
  
    
  G_OBJECT_CLASS ( cds_area_drawingarea_parent_class )->dispose ( obj );
}


void
cds_area_drawingarea_finalize ( GObject* obj )
{
  

  G_OBJECT_CLASS ( cds_area_drawingarea_parent_class )->finalize ( obj );
}


void
cds_area_drawingarea_destroy (CdsAreaDrawingArea* area)
{
  
  
  
  
  //area->toggle_selection_nohover_right_click_menu
  
  //cdds_drawingarea_right_click_menus_destroy ( area );
  
  
  gtk_widget_destroy ( GTK_WIDGET ( area ) );
  
  
}




/*
  if ( area->mgrs != NULL && area->mgrs->allactions == NULL )
  {
    g_message ( "Destroying a drawing area without a allactions array." );
  }
  else
  {  
    while ( area->mgrs->allactions->len > 0 )
    {
      cdds_geoaction_destroy ( g_ptr_array_index(area->mgrs->allactions, area->mgrs->allactions->len-1) );
    }
  }
      
  
  if ( area->mgrs != NULL && area->mgrs->allpoints == NULL )
  {
    g_message ( "Destroying a drawing area without a allpoints array." );
  }
  else
  {
    while ( area->mgrs->allpoints->len > 0 )
    {    
      cdds_geopoint_destroy ( g_ptr_array_index(area->mgrs->allpoints, area->mgrs->allpoints->len-1) );
    }
  }  
*/




