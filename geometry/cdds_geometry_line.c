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
#include "cdds_geometry_line.h"
#include "../cdds_drawingarea.h"
#include "../cdds_window.h"
#include "cdds_geometry_point.h"


G_DEFINE_TYPE(CdsAreaLine, cds_area_line, G_TYPE_OBJECT)



void
cds_area_line_ref ( CdsAreaLine* l )
{
  
  g_object_ref ( l );

#ifdef CDDS_DEBUG
  g_object_set ( G_OBJECT (l), "refcount", l->refcount + 1, NULL);
#endif 
  
}

void
cds_area_line_unref ( CdsAreaLine* l )
{
  
#ifdef CDDS_DEBUG
  g_object_set ( G_OBJECT (l), "refcount", l->refcount - 1, NULL);
#endif
  
  g_object_unref ( l );
}

void
cds_area_line_delete_from_liststore ( CdsAreaLine* l )
{
//#ifndef CDDS_DEBUG
//  gtk_list_store_remove ( drawarea->liststore_lines, l->iter);
//  g_free ( l->iter );
//  l->iter = NULL;
//#endif
  
  gtk_container_check_resize ( GTK_CONTAINER ( l->area->treeview_lines ) );
  
}

void
cds_area_line_add_to_liststore ( managers* mgrs, CdsAreaLine* l )
{
  
  if ( l->iter == NULL )
  {
    l->iter = g_malloc ( sizeof( GtkTreeIter ) );
    
    gtk_list_store_insert_with_values ( mgrs->area->liststore_lines,
					l->iter, -1,
					LINE_COLUMN_POINTER, l,
					LINE_COLUMN_POINTER_POINT1X, l->p1->point_x,
					LINE_COLUMN_POINTER_POINT1Y, l->p1->point_y,
					LINE_COLUMN_POINTER_POINT2X, l->p2->point_x,
					LINE_COLUMN_POINTER_POINT2Y, l->p2->point_y,
#ifdef CDDS_DEBUG
					LINE_COLUMN_REFCOUNT, l->refcount,
#endif
					-1 );
  }
  else
  {
    g_warning ( "Adding a line to a list store that is already in a list store." );
  }
  
  gtk_container_check_resize ( GTK_CONTAINER ( mgrs->area->treeview_lines ) );
  
}
  

void
cds_area_line_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  CdsAreaLine *self = CDS_AREA_LINE (object);

  switch (property_id)
  {
    case CDS_AREA_LINE_PROP_POINT1X:
      self->point1_x = g_value_get_int ( value );

      if ( self->iter != NULL )
      {
	gtk_list_store_set_value ( self->area->liststore_lines, self->iter, 
				   LINE_COLUMN_POINTER_POINT1X, ( GValue * ) value );
      }
      
      break;

    case CDS_AREA_LINE_PROP_POINT1Y:
      self->point1_y = g_value_get_int ( value );
      
      if ( self->iter != NULL )
      {
	gtk_list_store_set_value ( self->area->liststore_lines, self->iter, 
				   LINE_COLUMN_POINTER_POINT1Y, ( GValue * ) value );
      }
      
      break;
    
    case CDS_AREA_LINE_PROP_POINT2X:
      self->point2_x = g_value_get_int ( value );
      
      if ( self->iter != NULL )
      {
	gtk_list_store_set_value ( self->area->liststore_lines, self->iter, 
				   LINE_COLUMN_POINTER_POINT2X, ( GValue * ) value );
      }

      break;

    case CDS_AREA_LINE_PROP_POINT2Y:
      self->point2_y = g_value_get_int ( value );

      if ( self->iter != NULL )
      {
	gtk_list_store_set_value ( self->area->liststore_lines, self->iter, 
				   LINE_COLUMN_POINTER_POINT2Y, ( GValue * ) value );
      }
      
      break;  
    
    case CDS_AREA_LINE_PROP_R:
      self->color_r = g_value_get_float (value);
      break;

    case CDS_AREA_LINE_PROP_G:
      self->color_g = g_value_get_float (value);
      break;

    case CDS_AREA_LINE_PROP_B:
      self->color_b = g_value_get_float (value);
      break;

    case CDS_AREA_LINE_PROP_R_SELECTED:
      self->color_r_selected = g_value_get_float (value);
      break;

    case CDS_AREA_LINE_PROP_G_SELECTED:
      self->color_g_selected = g_value_get_float (value);
      break;

    case CDS_AREA_LINE_PROP_B_SELECTED:
      self->color_b_selected = g_value_get_float (value);
      break;
      
  #ifdef CDDS_DEBUG
    case CDS_AREA_LINE_PROP_REFCOUNT:
      self->refcount = g_value_get_int (value);
      if ( self->iter != NULL )
      {
	gtk_list_store_set_value ( self->area->liststore_lines, self->iter, LINE_COLUMN_REFCOUNT, ( GValue * ) value );
      }
      break;
  #endif
      
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
cds_area_line_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  CdsAreaLine *self = CDS_AREA_LINE (object);

  switch (property_id)
  {
    
    case CDS_AREA_LINE_PROP_POINT1X:
      g_value_set_int ( value, self->point1_x );
      break;

    case CDS_AREA_LINE_PROP_POINT1Y:
      g_value_set_int ( value, self->point1_y );
      break;
    
    case CDS_AREA_LINE_PROP_POINT2X:
      g_value_set_int ( value, self->point2_x ); 
      break;

    case CDS_AREA_LINE_PROP_POINT2Y:
      g_value_set_int ( value, self->point2_y );
      break;  
      
    case CDS_AREA_LINE_PROP_R:
      g_value_set_float (value, self->color_r);
      break;

    case CDS_AREA_LINE_PROP_G:
      g_value_set_float (value, self->color_g);
      break;

    case CDS_AREA_LINE_PROP_B:
      g_value_set_float (value, self->color_b);
      break;

    case CDS_AREA_LINE_PROP_R_SELECTED:
      g_value_set_float (value, self->color_r_selected);
      break;

    case CDS_AREA_LINE_PROP_G_SELECTED:
      g_value_set_float (value, self->color_g_selected);
      break;

    case CDS_AREA_LINE_PROP_B_SELECTED:
      g_value_set_float (value, self->color_b_selected);
      break;
      
  #ifdef CDDS_DEBUG
    case CDS_AREA_LINE_PROP_REFCOUNT:
      g_value_set_int (value, self->refcount);
      break;    
  #endif

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}


static void
cds_area_line_init ( CdsAreaLine *l )
{
  GValue gval_int = G_VALUE_INIT;
  GValue gval_float = G_VALUE_INIT;
  
  l->selected = FALSE;
  
  g_value_init ( &gval_int, G_TYPE_INT );
  g_value_init ( &gval_float, G_TYPE_FLOAT );
  
  g_value_set_float ( &gval_float, 0.4 );
  g_object_set_property ( G_OBJECT ( l ), "color-r", &gval_float );  

  g_value_set_float ( &gval_float, 0.7 );
  g_object_set_property ( G_OBJECT ( l ), "color-g", &gval_float );  

  g_value_set_float ( &gval_float, 0.1 );
  g_object_set_property ( G_OBJECT ( l ), "color-b", &gval_float );
  
  g_value_set_float ( &gval_float, 0.7 );
  g_object_set_property ( G_OBJECT ( l ), "color-r-selected", &gval_float );  

  g_value_set_float ( &gval_float, 0.2 );
  g_object_set_property ( G_OBJECT ( l ), "color-g-selected", &gval_float );  

  g_value_set_float ( &gval_float, 0.5 );
  g_object_set_property ( G_OBJECT ( l ), "color-b-selected", &gval_float );  
  
  g_value_unset ( &gval_float );
  g_value_unset ( &gval_int );
  
}


static void
cds_area_line_class_init (CdsAreaLineClass *klass)
{

  GObjectClass   *gobject_class;
  
  gobject_class = G_OBJECT_CLASS (klass);
  
  gobject_class->finalize = cds_area_line_finalize;
  
  gobject_class->set_property = cds_area_line_set_property;
  gobject_class->get_property = cds_area_line_get_property;

  
  cds_area_line_properties[CDS_AREA_LINE_PROP_POINT1X] = g_param_spec_int ("point1-x", "x", 
									   "Integer value of x coordinate.",
                                                                           0, G_MAXINT, 0, G_PARAM_READWRITE);

  cds_area_line_properties[CDS_AREA_LINE_PROP_POINT1Y] = g_param_spec_int ("point1-y", "y", 
									   "Integer value of y coordinate.",
									   0, G_MAXINT, 0, G_PARAM_READWRITE);

  cds_area_line_properties[CDS_AREA_LINE_PROP_POINT2X] = g_param_spec_int ("point2-x", "x", 
									   "Integer value of x coordinate.",
                                                                           0, G_MAXINT, 0, G_PARAM_READWRITE);

  cds_area_line_properties[CDS_AREA_LINE_PROP_POINT2Y] = g_param_spec_int ("point2-y", "y", 
									   "Integer value of y coordinate.",
									   0, G_MAXINT, 0, G_PARAM_READWRITE);
  
  cds_area_line_properties[CDS_AREA_LINE_PROP_R] = g_param_spec_float ("color-r", "r", "Color value red.",
                                                                       0, 1.0, 0, G_PARAM_READWRITE);
  
  cds_area_line_properties[CDS_AREA_LINE_PROP_G] = g_param_spec_float ("color-g", "g", "Color value green.",
                                                                       0, 1.0, 0, G_PARAM_READWRITE);
  
  cds_area_line_properties[CDS_AREA_LINE_PROP_B] = g_param_spec_float ("color-b", "b", "Color value blue.",
                                                                       0, 1.0, 0, G_PARAM_READWRITE);
  
  cds_area_line_properties[CDS_AREA_LINE_PROP_R_SELECTED] = g_param_spec_float ("color-r-selected", "r-selected", 
                                                                                "Color value selected red.",
                                                                                0, 1.0, 0, G_PARAM_READWRITE);
  
  cds_area_line_properties[CDS_AREA_LINE_PROP_G_SELECTED] = g_param_spec_float ("color-g-selected", "g-selected", 
                                                                                "Color value selected green.",
                                                                                0, 1.0, 0, G_PARAM_READWRITE);
  
  cds_area_line_properties[CDS_AREA_LINE_PROP_B_SELECTED] = g_param_spec_float ("color-b-selected", "b-selected", 
                                                                                "Color value selected blue.",
                                                                                0, 1.0, 0, G_PARAM_READWRITE);
  
#ifdef CDDS_DEBUG
  cds_area_line_properties[CDS_AREA_LINE_PROP_REFCOUNT] = g_param_spec_int ("refcount", "refcount", 
									    "Reference count of point.",
									    0, G_MAXINT, 0, G_PARAM_READWRITE);  
#endif
  
  g_object_class_install_properties (gobject_class,
                                     CDS_AREA_LINE_N_PROPERTIES,
                                     cds_area_line_properties);  
  
}




CdsAreaLine
*cds_area_line_new ( managers* mgrs, CDDSGeoPoint* a, CDDSGeoPoint* b )
{
  
  CdsAreaLine* l;  

  l = g_object_new (CDS_AREA_TYPE_LINE, NULL);
  
  l->area = mgrs->area;
  
  l->p1 = a;
  l->p2 = b;

  
  g_object_bind_property (l, "point1-x", a, "point-x", G_BINDING_BIDIRECTIONAL);
  g_object_bind_property (l, "point1-y", a, "point-y", G_BINDING_BIDIRECTIONAL);
  g_object_bind_property (l, "point2-x", b, "point-x", G_BINDING_BIDIRECTIONAL);
  g_object_bind_property (l, "point2-y", b, "point-y", G_BINDING_BIDIRECTIONAL);
  
  l->iter = g_malloc ( sizeof( GtkTreeIter ) );
  
  gtk_list_store_insert_with_values ( mgrs->area->liststore_lines,
				      l->iter, -1,
				      LINE_COLUMN_POINTER, l,
				      LINE_COLUMN_POINTER_POINT1X, l->p1->point_x,
				      LINE_COLUMN_POINTER_POINT1Y, l->p1->point_y,
				      LINE_COLUMN_POINTER_POINT2X, l->p2->point_x,
				      LINE_COLUMN_POINTER_POINT2Y, l->p2->point_y,
#ifdef CDDS_DEBUG
				      LINE_COLUMN_REFCOUNT, l->refcount,
#endif
				      -1 );

#ifdef CDDS_DEBUG

  g_object_set ( G_OBJECT ( l ), "refcount", 1, NULL);

  g_ptr_array_add ( mgrs->alllines, l );
  
#endif
  
  return l;
}


void
cds_area_line_finalize ( GObject* obj )
{
  
  CdsAreaLine* l;
  l = CDS_AREA_LINE ( obj );
  
  //g_message( "geopoint finalize" );
  
  gtk_list_store_remove ( l->area->liststore_lines, l->iter );

  g_free ( l->iter );

  l->iter = NULL;

#ifdef CDDS_DEBUG
  
  if ( !g_ptr_array_remove ( l->area->mgrs->alllines, l ) )
  {
    g_message("Removed a line that could not be found in alllines ptr array.");
  }
  
#endif
  
  G_OBJECT_CLASS ( cds_area_line_parent_class )->finalize ( obj );
  
}
  


void
cds_area_line_destroy (CdsAreaLine* l)
{

  g_object_unref ( l );
  
}