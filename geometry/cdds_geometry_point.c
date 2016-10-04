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
#include "cdds_geometry_point.h"
#include "../cdds_drawingarea.h"
#include "../cdds_window.h"


void
cdds_geopoint_ref ( CDDSGeoPoint* p )
{
  
  g_object_ref ( p );

#ifdef CDDS_DEBUG
  g_object_set ( G_OBJECT (p), "refcount", p->refcount + 1, NULL);
#endif 
  
}

void
cdds_geopoint_unref ( CDDSGeoPoint* p )
{
  
#ifdef CDDS_DEBUG
  g_object_set ( G_OBJECT (p), "refcount", p->refcount - 1, NULL);
#endif
  
  g_object_unref ( p );  
}

void
cdds_geopoint_delete_from_liststore ( CDDSGeoPoint* p )
{

//#ifndef CDDS_DEBUG
//  gtk_list_store_remove ( drawarea->liststore_points, p->iter);
//  g_free ( p->iter );
//  p->iter = NULL;
//#endif
  
  gtk_container_check_resize ( GTK_CONTAINER ( p->area->treeview_points ) );
  
}

void
cdds_geopoint_add_to_liststore ( managers* mgrs, CDDSGeoPoint* p )
{
  if ( p->iter == NULL )
  {
    p->iter = g_malloc ( sizeof( GtkTreeIter ) );
    
    gtk_list_store_insert_with_values ( mgrs->area->liststore_points,
					p->iter, -1,
					COLUMN_POINTER, p,
					COLUMN_X, p->point_x,
					COLUMN_Y, p->point_y,
#ifdef CDDS_DEBUG
					COLUMN_REFCOUNT, p->refcount,
#endif
					-1 );
  }
  
  gtk_container_check_resize ( GTK_CONTAINER ( mgrs->area->treeview_points ) );
  
}
  

void
cdds_geopoint_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  CDDSGeoPoint *self = CDDS_GEOPOINT (object);

  switch (property_id)
  {
  case CDDS_GEOPOINT_PROP_X:
    self->point_x = g_value_get_int (value);
    if ( self->iter != NULL )
    {
      gtk_list_store_set_value ( self->area->liststore_points, self->iter, COLUMN_X, ( GValue * ) value );
    }
    break;

  case CDDS_GEOPOINT_PROP_Y:
    self->point_y = g_value_get_int (value);
    if ( self->iter != NULL )
    {
      gtk_list_store_set_value ( self->area->liststore_points, self->iter, COLUMN_Y, ( GValue * ) value );
    }
    break;

  case CDDS_GEOPOINT_PROP_R:
    self->color_r = g_value_get_float (value);
    break;

  case CDDS_GEOPOINT_PROP_G:
    self->color_g = g_value_get_float (value);
    break;

  case CDDS_GEOPOINT_PROP_B:
    self->color_b = g_value_get_float (value);
    break;

  case CDDS_GEOPOINT_PROP_R_SELECTED:
    self->color_r_selected = g_value_get_float (value);
    break;

  case CDDS_GEOPOINT_PROP_G_SELECTED:
    self->color_g_selected = g_value_get_float (value);
    break;

  case CDDS_GEOPOINT_PROP_B_SELECTED:
    self->color_b_selected = g_value_get_float (value);
    break;
    
#ifdef CDDS_DEBUG
  case CDDS_GEOPOINT_PROP_REFCOUNT:
    self->refcount = g_value_get_int (value);
    if ( self->iter != NULL )
    {
      gtk_list_store_set_value ( self->area->liststore_points, self->iter, COLUMN_REFCOUNT, ( GValue * ) value );
    }
    break;
#endif
    
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    break;
  }
}

void
cdds_geopoint_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  CDDSGeoPoint *self = CDDS_GEOPOINT (object);

  switch (property_id)
  {
  case CDDS_GEOPOINT_PROP_X:
    g_value_set_int (value, self->point_x);
    break;

  case CDDS_GEOPOINT_PROP_Y:
    g_value_set_int (value, self->point_y);
    break;
    
  case CDDS_GEOPOINT_PROP_R:
    g_value_set_float (value, self->color_r);
    break;

  case CDDS_GEOPOINT_PROP_G:
    g_value_set_float (value, self->color_g);
    break;

  case CDDS_GEOPOINT_PROP_B:
    g_value_set_float (value, self->color_b);
    break;

  case CDDS_GEOPOINT_PROP_R_SELECTED:
    g_value_set_float (value, self->color_r_selected);
    break;

  case CDDS_GEOPOINT_PROP_G_SELECTED:
    g_value_set_float (value, self->color_g_selected);
    break;

  case CDDS_GEOPOINT_PROP_B_SELECTED:
    g_value_set_float (value, self->color_b_selected);
    break;
    
#ifdef CDDS_DEBUG
  case CDDS_GEOPOINT_PROP_REFCOUNT:
    g_value_set_int (value, self->refcount);
    break;    
#endif

  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    break;
  }
}



static void
cdds_geopoint_init ( CDDSGeoPoint *point )
{
  GValue gval_int = G_VALUE_INIT;
  GValue gval_float = G_VALUE_INIT;
  
  point->selected = FALSE;
  
  g_value_init ( &gval_int, G_TYPE_INT );
  g_value_init ( &gval_float, G_TYPE_FLOAT );
  
  g_value_set_float ( &gval_float, 0.4 );
  g_object_set_property ( G_OBJECT ( point ), "color-r", &gval_float );  

  g_value_set_float ( &gval_float, 0.7 );
  g_object_set_property ( G_OBJECT ( point ), "color-g", &gval_float );  

  g_value_set_float ( &gval_float, 0.1 );
  g_object_set_property ( G_OBJECT ( point ), "color-b", &gval_float );
  
  g_value_set_float ( &gval_float, 0.7 );
  g_object_set_property ( G_OBJECT ( point ), "color-r-selected", &gval_float );  

  g_value_set_float ( &gval_float, 0.2 );
  g_object_set_property ( G_OBJECT ( point ), "color-g-selected", &gval_float );  

  g_value_set_float ( &gval_float, 0.5 );
  g_object_set_property ( G_OBJECT ( point ), "color-b-selected", &gval_float );  
  
  g_value_unset ( &gval_float );
  g_value_unset ( &gval_int );
}


static void
cdds_geopoint_class_init (CDDSGeoPointClass *klass)
{

  GObjectClass   *gobject_class;
  
  gobject_class = G_OBJECT_CLASS (klass);
  
  gobject_class->finalize = cdds_geopoint_finalize;
  
  cdds_geopoint_parent_class = g_type_class_peek_parent (klass);
  
  gobject_class->set_property = cdds_geopoint_set_property;
  gobject_class->get_property = cdds_geopoint_get_property;
  
  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_X] = g_param_spec_int ("point-x", "x", "Integer value of x coordinate.",
                                                                     0, G_MAXINT, 0, G_PARAM_READWRITE);

  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_Y] = g_param_spec_int ("point-y", "y", "Integer value of y coordinate.",
                                                                     0, G_MAXINT, 0, G_PARAM_READWRITE);
  
  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_R] = g_param_spec_float ("color-r", "r", "Color value red.",
                                                                       0, 1.0, 0, G_PARAM_READWRITE);
  
  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_G] = g_param_spec_float ("color-g", "g", "Color value green.",
                                                                       0, 1.0, 0, G_PARAM_READWRITE);
  
  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_B] = g_param_spec_float ("color-b", "b", "Color value blue.",
                                                                       0, 1.0, 0, G_PARAM_READWRITE);
  
  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_R_SELECTED] = g_param_spec_float ("color-r-selected", "r_selected", 
                                                                                "Color value selected red.",
                                                                                0, 1.0, 0, G_PARAM_READWRITE);
  
  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_G_SELECTED] = g_param_spec_float ("color-g-selected", "g_selected", 
                                                                                "Color value selected green.",
                                                                                0, 1.0, 0, G_PARAM_READWRITE);
  
  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_B_SELECTED] = g_param_spec_float ("color-b-selected", "b_selected", 
                                                                                "Color value selected blue.",
                                                                                0, 1.0, 0, G_PARAM_READWRITE);
  
#ifdef CDDS_DEBUG
  cdds_geopoint_properties[CDDS_GEOPOINT_PROP_REFCOUNT] = g_param_spec_int ("refcount", "refcount", 
									    "Reference count of point.",
									    0, G_MAXINT, 0, G_PARAM_READWRITE);  
#endif
  
  g_object_class_install_properties (gobject_class,
                                     CDDS_GEOPOINT_N_PROPERTIES,
                                     cdds_geopoint_properties);  
  
  
  
  
}



GType cdds_geopoint_get_type (void)
{
  static GType cdds_geopoint_type = 0;
  
  if (!cdds_geopoint_type)
  {
    const GTypeInfo cdds_geopoint_info =
    {
      sizeof (CDDSGeoPointClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) cdds_geopoint_class_init, /* class_init */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (CDDSGeoPoint),
      0, /* n_preallocs */
      (GInstanceInitFunc) cdds_geopoint_init, /* instance_init */
    };

    cdds_geopoint_type = g_type_register_static (G_TYPE_OBJECT, "CDDSGeoPoint", &cdds_geopoint_info, 0);
  }
  
  return cdds_geopoint_type;
}






CDDSGeoPoint
*cdds_geopoint_new ( managers* mgrs, gint x, gint y )
{
  
  CDDSGeoPoint* point;  

  point = g_object_new (CDDS_GEOPOINT_TYPE, NULL);
  
  point->area = mgrs->area;
  
  point->point_x = x;
  point->point_y = y;

  point->iter = g_malloc ( sizeof( GtkTreeIter ) );
  
  gtk_list_store_insert_with_values ( mgrs->area->liststore_points,
				      point->iter, -1,
				      COLUMN_POINTER, point,
				      COLUMN_X, point->point_x,
				      COLUMN_Y, point->point_y,
#ifdef CDDS_DEBUG
				      COLUMN_REFCOUNT, point->refcount,
#endif
				      -1 );

#ifdef CDDS_DEBUG

  g_object_set ( G_OBJECT ( point ), "refcount", 1, NULL);

  g_ptr_array_add ( mgrs->allpoints, point );
  
#endif
  

  
  

  
  return point;
}


void
cdds_geopoint_finalize ( GObject* obj )
{
  
  CDDSGeoPoint* point;
  point = CDDS_GEOPOINT ( obj );
  
  //g_message( "geopoint finalize" );
  
  gtk_list_store_remove ( point->area->liststore_points, point->iter );

  g_free ( point->iter );

  point->iter = NULL;

#ifdef CDDS_DEBUG
  
  if ( !g_ptr_array_remove ( point->area->mgrs->allpoints, point ) )
  {
    g_message("Removed a point that could not be found in allpoints ptr array.");
  }
  
#endif
  
  cdds_geopoint_parent_class->finalize ( obj );
  
}
  


void
cdds_geopoint_destroy (CDDSGeoPoint* point)
{

  g_object_unref ( point );
  
}