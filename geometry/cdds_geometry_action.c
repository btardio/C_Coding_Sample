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
#include "cdds_geometry_action.h"
#include "cdds_geometry_selection.h"
#include "cdds_geometry_pointmanager.h"
#include "cdds_geometry_actionmanager.h"
#include "cdds_geometry_point.h"
#include "../cdds_drawingarea.h"
#include "../cdds_window.h"
/*
void
cdds_geoaction_foreach_point_add_smgr(gpointer key, gpointer value, gpointer data)
{
  CDDSGeoPoint* p = key;
  CdsAreaSelection* s = data;
  
  cds_area_selection_mark_pointp ( s, p );  
}

void
cdds_geoaction_foreach_point_add_pmgr(gpointer key, gpointer value, gpointer data)
{
  CDDSGeoPoint* p = key;
  CdsAreaPointManager* mgr = data;
  
  cds_area_pointmanager_insert_pointp ( mgr, p );
  
  
}
*/
void
cdds_geoaction_foreach_point_add ( gpointer key, gpointer value, gpointer data )
{
  CDDSGeoPoint* p = key;
  managers* mgrs = data;
  
  
  cds_area_pointmanager_insert_pointp ( mgrs, p );
  cds_area_selection_mark_pointp ( mgrs, p );
}

void
cdds_geoaction_foreach_point_delete(gpointer key, gpointer value, gpointer data)
{
  CDDSGeoPoint* p = key;
  managers* mgrs = data;
  gint n_pointsatposition;
  
  
  
  if ( n_pointsatposition = cds_area_pointmanager_lookup_pointp ( mgrs->pmgr, p ) ) /* if there are no other points occupying 
											the same x,y coordinates of the point 
											deleted */
  {
    //g_message ( " n_pointsatposition: %d ", n_pointsatposition );
    if ( n_pointsatposition >= 2 )
    {
      cds_area_pointmanager_delete_pointp ( mgrs->pmgr, p );
    }
    else
    {
      cds_area_pointmanager_delete_pointp ( mgrs->pmgr, p );
      cds_area_selection_unmark_pointp ( mgrs->smgr, p );
    }
  }
  else
  {
    g_warning("Attempting to delete a point that does not exist.");
  }
    
}
/*
void
cdds_geoaction_foreach_point_delete_smgr(gpointer key, gpointer value, gpointer data)
{
  CDDSGeoPoint* p = key;
  CdsAreaSelection* s = data;
  
  cds_area_selection_unmark_pointp ( s, p );
  
}

void
cdds_geoaction_foreach_point_delete_pmgr(gpointer key, gpointer value, gpointer data)
{
  
  CDDSGeoPoint* p = key;
  CdsAreaPointManager* mgr = data;
  
  cds_area_pointmanager_delete_pointp ( mgr, p );
  
  
  
}
*/
void
cdds_geoaction_foreach_point_move( gpointer key, gpointer value, gpointer data )
{
  
  CDDSGeoPoint* p = key;
  actionwmgrs* awmgrs = data;
  gint newx;
  gint newy;
  gint n_pointsatposition;
  
  cdds_geopoint_ref ( p );

  /* todo */
  /* the inprogress move needs inclusion of n_pointsatposition */
  
  if ( n_pointsatposition = cds_area_pointmanager_lookup_pointp ( awmgrs->mgrs->pmgr, p ) ) /* if there are no other points at 
											      the same x,y of the point */
  {
    if ( n_pointsatposition == 1 )
    {      
      cds_area_selection_unmark_pointp ( awmgrs->mgrs->smgr, p );
    }
  }
  cds_area_pointmanager_delete_pointp ( awmgrs->mgrs->pmgr, p );  
  
  if ( awmgrs->a->inprogress )
  {    
    newx = p->point_x + ( awmgrs->a->transform->x - awmgrs->a->transform_progress_previous->x );
    newy = p->point_y + ( awmgrs->a->transform->y - awmgrs->a->transform_progress_previous->y );
  }
  else
  {
    newx = p->point_x + awmgrs->a->transform->x;
    newy = p->point_y + awmgrs->a->transform->y;
  }  
  
  newx = cds_area_drawing_area_mouse_to_canvas_i ( awmgrs->mgrs->area, newx );
  newy = cds_area_drawing_area_mouse_to_canvas_i ( awmgrs->mgrs->area, newy );
  
  if ( newx < 0 ) { newx = 0; }
  if ( newx > awmgrs->mgrs->area->width ) { newx = awmgrs->mgrs->area->width; }
  
  if ( newy < 0 ) { newy = 0; }
  if ( newy > awmgrs->mgrs->area->height ) { newy = awmgrs->mgrs->area->height; }
  
  g_object_set ( G_OBJECT (p), "point-x", newx, NULL);
  
  g_object_set ( G_OBJECT (p), "point-y", newy, NULL);  
  
  cds_area_selection_mark_pointp ( awmgrs->mgrs, p );
  cds_area_pointmanager_insert_pointp ( awmgrs->mgrs, p );
  
  cdds_geopoint_unref ( p );

}

void
cdds_geoaction_action ( managers* mgrs, CDDSGeoAction* action )
{
  
  actionwmgrs awmgrs;
  
  if ( action == NULL )
  {
    return;
  }
  
  switch ( action->type )
  {
    case ( ACTION_POINT_ADD ):
      
      //g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_add_pmgr, mgrs->pmgr);
      //g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_add_smgr, mgrs->smgr);
      g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_add, mgrs);
      break;
      
    case ( ACTION_POINT_DELETE ):
      
      g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_delete, mgrs);
      /*g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_delete_pmgr , mgrs->pmgr);
      g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_delete_smgr, mgrs->smgr);*/
      break;
      
    case ( ACTION_POINT_MOVE ):
      
      awmgrs.a = action;
      awmgrs.mgrs = mgrs;
      
      g_hash_table_foreach ( action->selection->hash_points, cdds_geoaction_foreach_point_move, &awmgrs );
      break;
  }
  
  gtk_widget_queue_draw ( GTK_WIDGET ( mgrs->area ) );
  
}


void
cdds_geoaction_inverse_action ( managers* mgrs, CDDSGeoAction* action )
{					       
  actionwmgrs awmgrs;


  if ( action == NULL )
  {
    return;
  }
  
  switch ( action->type )
  {
    case ( ACTION_POINT_ADD ):
      
      g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_delete, mgrs);
      /*g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_delete_pmgr , mgrs->pmgr);
      g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_delete_smgr, mgrs->smgr);*/
      break;
      
    case ( ACTION_POINT_DELETE ):
      g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_add, mgrs);
      //g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_add_pmgr, mgrs->pmgr);
      //g_hash_table_foreach (action->selection->hash_points, cdds_geoaction_foreach_point_add_smgr, mgrs->smgr);
      break;

    case ( ACTION_POINT_MOVE ):
      
      awmgrs.a = action;
      awmgrs.mgrs = mgrs;
      
      action->transform->x *= -1;
      action->transform->y *= -1;
      
      g_hash_table_foreach ( action->selection->hash_points, cdds_geoaction_foreach_point_move, &awmgrs );
      
      action->transform->x *= -1;
      action->transform->y *= -1;
      
      break;
      
  }
  
  gtk_widget_queue_draw ( GTK_WIDGET ( mgrs->area ) );  
  
}


void
cdds_geoaction_increase_ref_count ( CDDSGeoAction* action )
{
  
  g_object_ref ( action );
  
#ifdef CDDS_DEBUG
  action->refcount += 1;
  
#endif
}

void
cdds_geoaction_decrease_ref_count ( CDDSGeoAction* action )
{
  
  g_object_unref ( action );

#ifdef CDDS_DEBUG
  action->refcount -= 1;
  
#endif  
}




void
cdds_geoaction_set_property (GObject      *object,
                                guint         property_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
  CDDSGeoAction *self = CDDS_GEOACTION (object);

  switch (property_id)
  {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
cdds_geoaction_get_property (GObject    *object,
                                guint       property_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  CDDSGeoAction *self = CDDS_GEOACTION (object);

  switch (property_id)
  {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}


static void
cdds_geoaction_init (CDDSGeoAction *action)
{
  GValue gval_int = G_VALUE_INIT;
  GValue gval_float = G_VALUE_INIT;
  
  g_value_init ( &gval_int, G_TYPE_INT );
  g_value_init ( &gval_float, G_TYPE_FLOAT );

  g_value_unset ( &gval_float );
  g_value_unset ( &gval_int );
  
#ifdef CDDS_DEBUG
  action->refcount = 1;
#endif
  

  
  
  
}


static void
cdds_geoaction_class_init (CDDSGeoActionClass *klass)
{

  GObjectClass   *gobject_class;
  gobject_class = G_OBJECT_CLASS (klass);
  
  gobject_class->finalize = cdds_geoaction_finalize;
  
  cdds_geoaction_parent_class = g_type_class_peek_parent (klass);
  
  gobject_class->set_property = cdds_geoaction_set_property;
  gobject_class->get_property = cdds_geoaction_get_property;
  
  
  
  
/*  
  g_object_class_install_properties (gobject_class,
                                     CDDS_GEOACTION_N_PROPERTIES,
                                     cdds_geoaction_properties);  
*/  
  
  
  
}



GType cdds_geoaction_get_type (void)
{
  static GType cdds_geoaction_type = 0;
  
  if (!cdds_geoaction_type)
  {
    const GTypeInfo cdds_geoaction_info =
    {
      sizeof (CDDSGeoActionClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) cdds_geoaction_class_init, /* class_init */
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (CDDSGeoAction),
      0, /* n_preallocs */
      (GInstanceInitFunc) cdds_geoaction_init, /* instance_init */
    };

    cdds_geoaction_type = g_type_register_static (G_TYPE_OBJECT, "CDDSGeoAction", &cdds_geoaction_info, 0);
  }
  
  return cdds_geoaction_type;
}


CDDSGeoAction
*cdds_geoaction_new ( managers* mgrs, gint type, CdsAreaSelection* selection )
{
  
  CDDSGeoAction* action;
  
  action = g_object_new (CDDS_GEOACTION_TYPE, NULL);
  
  action->area = mgrs->area;
  
  action->selection = selection;
  action->type = type;
  
  cds_area_selection_increase_ref_count_points ( action->selection );
  
#ifdef CDDS_DEBUG
    
  g_ptr_array_add ( mgrs->allactions, action );
  
#endif
  
  if ( type == ACTION_POINT_MOVE )
  {
    action->transform = g_malloc ( sizeof ( xycoord ) );
    action->transform_origin = g_malloc ( sizeof ( xycoord ) );
    action->transform_progress_previous = g_malloc ( sizeof ( xycoord ) );
  }
  
  return action;
}

void
cdds_geoaction_finalize ( GObject* obj )
{
  CDDSGeoAction* action;
  action = CDDS_GEOACTION ( obj );
  
#ifdef CDDS_DEBUG
  
  if ( !g_ptr_array_remove( action->area->mgrs->allactions, action ) )
  {
    g_message("Removed an action that could not be found in allactions ptr array.");
  }
  
#endif
  
  if ( action->transform != NULL )
  {
    g_free ( action->transform );
  }
  
  if ( action->transform_origin != NULL )
  {
    g_free ( action->transform_origin );
  }
  
  if ( action->transform_progress_previous != NULL )
  {
    g_free ( action->transform_progress_previous );
  }
  
  cds_area_selection_decrease_ref_count_points ( action->selection );
  
  cds_area_selection_destroy ( action->selection );
  
  cdds_geoaction_parent_class->finalize ( obj );
}

void
cdds_geoaction_destroy ( CDDSGeoAction* action )
{
    
  g_object_unref ( action );
  
}




