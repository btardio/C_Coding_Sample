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
#include "cdds_geometry_pointmanager.h"
#include "cdds_geometry_point.h"
#include "../cdds_window.h"
#include "../cdds_drawingarea.h"

G_DEFINE_TYPE(CdsAreaPointManager, cds_area_pointmanager, G_TYPE_OBJECT)

/*
gboolean
cds_area_pointmanager_equal (gconstpointer a, gconstpointer b)
{
  return *(gint*)a == *(gint*)b;
}
*/

gint
cds_area_pointmanager_compare ( gconstpointer _a, gconstpointer _b, gpointer data )
{
  const gint* a = _a;
  const gint* b = _b;
  
  return *a - *b;
}

void
cds_area_pointmanager_key_destroy_func ( gpointer data )
{
  g_free((gint*)data); 
}

void
cds_area_pointmanager_valuepoint_destroy_func ( gpointer data )
{
  CDDSGeoPoint* p = data;
    
  cdds_geopoint_delete_from_liststore ( p );
  
  cdds_geopoint_unref ( p );
  
}

void
cds_area_pointmanager_valueptrarray_destroy_func ( gpointer data )
{
  
  GPtrArray* ptrarray;
  ptrarray = data;
  
  g_ptr_array_free ( ptrarray, TRUE );
  
}

void
cds_area_pointmanager_valuetable_destroy_func ( gpointer data )
{
  g_hash_table_destroy ( ( GHashTable* ) data );
}

void
cds_area_pointmanager_valuetree_destroy_func ( gpointer data )
{
  g_tree_destroy ( (GTree*) data );
}

#ifdef TREE
gboolean
#endif
#ifdef HASH
void
#endif
cds_area_pointmanager_foreach_point ( gpointer value, gpointer data )
{
  cairo_t *cr;
  CDDSGeoPoint* p;
  cr = data;
  p = value;
  
  cairo_save(cr);  
  cairo_set_line_width (cr, 3.0);

  if ( p->selected )
  {
    cairo_set_source_rgba (cr, p->color_r_selected, p->color_g_selected, p->color_b_selected, 1.0);
  }
  else{
    cairo_set_source_rgba (cr, p->color_r, p->color_g, p->color_b, 1.0);
  }
  
  cairo_arc (cr, p->point_x, p->point_y, 4, 0, M_PI + M_PI);
  
  cairo_stroke (cr);
  cairo_restore(cr);

#ifdef TREE
  return FALSE;
#endif
}

#ifdef TREE
gboolean
#endif
#ifdef HASH
void
#endif
cds_area_pointmanager_foreach_ptrarray ( gpointer key, gpointer value, gpointer data )
{
  
  GPtrArray* ptrarray;
  ptrarray = value;
  
  g_ptr_array_foreach ( ptrarray, cds_area_pointmanager_foreach_point, data );
  
#ifdef TREE
  return FALSE;
#endif
}


#ifdef TREE
gboolean
#endif
#ifdef HASH
void
#endif
cds_area_pointmanager_foreach_mgr ( gpointer key, gpointer value, gpointer data )
{
#ifdef TREE
  /* g_tree_foreach ( ( GTree* ) value, cds_area_pointmanager_foreach_point, data); */
  g_tree_foreach ( ( GTree* ) value, cds_area_pointmanager_foreach_ptrarray, data);
#endif
#ifdef HASH
  /* g_hash_table_foreach ( ( GHashTable* ) value, cds_area_pointmanager_foreach_point, data); */
  g_hash_table_foreach ( ( GHashTable* ) value, cds_area_pointmanager_foreach_ptrarray, data);
#endif
  
#ifdef TREE
  return FALSE;
#endif
}

void
cds_area_pointmanager_draw_points ( CdsAreaPointManager* mgr, cairo_t *cr )
{
  
#ifdef TREE  
  g_tree_foreach (mgr->tree, cds_area_pointmanager_foreach_mgr, cr);
#endif
#ifdef HASH
  g_hash_table_foreach ( mgr->table, cds_area_pointmanager_foreach_mgr, cr);
#endif

  
}


gboolean
//cds_area_pointmanager_insert_pointp ( CdsAreaPointManager* mgr, CDDSGeoPoint* p )
cds_area_pointmanager_insert_pointp ( managers* mgrs, CDDSGeoPoint* p )
{
#ifdef TREE
  GTree* foundx;
#endif
#ifdef HASH
  GHashTable* foundx;
#endif
  //CDDSGeoPoint* found;
  GPtrArray* found;
  gint* key_x;
  gint* key_y;
  
  foundx = NULL;
  found = NULL;
  
  key_x = g_malloc(sizeof(gint));
  key_y = g_malloc(sizeof(gint));
  *key_x = p->point_x;
  *key_y = p->point_y;
  
#ifdef TREE
  if ( foundx = g_tree_lookup ( mgrs->pmgr->tree, key_x ) ) /* if the x tree contains point */
  {
    if ( found = g_tree_lookup ( foundx, key_y ) ) /* if the y tree contains point */
#endif
#ifdef HASH
  if ( foundx = g_hash_table_lookup ( mgrs->pmgr->table, key_x ) )
  {
    if ( found = g_hash_table_lookup ( foundx, key_y ) )
#endif
    {            
      g_ptr_array_add ( found, p );
      mgrs->pmgr->n_points++;
    }
    else
    {
#ifdef TREE
      //g_tree_insert ( foundx, key_y, p ); 
      found = g_ptr_array_new_with_free_func ( cds_area_pointmanager_valuepoint_destroy_func );
      
      g_tree_insert ( foundx, key_y, found );
      
      g_ptr_array_add ( found, p ); /* point has been added to the x's tree ptrarray */
      mgrs->pmgr->n_points++;
      
#endif
#ifdef HASH
      //g_hash_table_insert ( foundx, key_y, p );
      found = g_ptr_array_new_with_free_func ( cds_area_pointmanager_valuepoint_destroy_func );
      
      g_hash_table_insert ( foundx, key_y, found );
      
      g_ptr_array_add ( found, p ); /* point has been added to the x's tree ptrarray */
      mgrs->pmgr->n_points++;
      
#endif
      
    }
  }
  
  else
  {
#ifdef TREE
    /*
    g_tree_insert( mgr->tree, key_x, foundx = g_tree_new_full ( cds_area_pointmanager_compare, 
								NULL,
								cds_area_pointmanager_key_destroy_func, 
								cds_area_pointmanager_valuepoint_destroy_func ) );
        
    g_tree_insert( foundx, key_y, p );
    */
    
    foundx = g_tree_new_full ( cds_area_pointmanager_compare, NULL,
			       cds_area_pointmanager_key_destroy_func, 
			       cds_area_pointmanager_valueptrarray_destroy_func ) );
    
    g_tree_insert ( mgrs->pmgr->tree, key_x, foundx );
    
    found = g_ptr_array_new_with_free_func ( cds_area_pointmanager_valuepoint_destroy_func );
    
    g_tree_insert ( foundx, key_y, found );
    
    g_ptr_array_add ( found, p );
    mgrs->pmgr->n_points++;
    
#endif
#ifdef HASH
    
    foundx = g_hash_table_new_full ( g_int_hash, 
				     g_int_equal, //cds_area_pointmanager_equal,
				     cds_area_pointmanager_key_destroy_func, 
				     cds_area_pointmanager_valueptrarray_destroy_func );
    
    g_hash_table_insert ( mgrs->pmgr->table, key_x, foundx );
    
    found = g_ptr_array_new_with_free_func ( cds_area_pointmanager_valuepoint_destroy_func );
    
    g_hash_table_insert ( foundx, key_y, found );
    
    g_ptr_array_add ( found, p );
    mgrs->pmgr->n_points++;
    
    /*
    g_hash_table_insert ( mgr->table, key_x, foundx = g_hash_table_new_full ( g_int_hash, 
									     cds_area_pointmanager_equal,
									     cds_area_pointmanager_key_destroy_func, 
									     cds_area_pointmanager_valuepoint_destroy_func ) );
    g_hash_table_insert ( foundx, key_y, p );
    */
    
#endif
      
  }
  
  cdds_geopoint_ref ( p );
    
  cdds_geopoint_add_to_liststore ( mgrs, p );
  
  return TRUE;  
  
}

void
cds_area_pointmanager_delete_pointp ( CdsAreaPointManager* mgr, CDDSGeoPoint* p )
{
  
#ifdef TREE
  GTree* foundx;
#endif
#ifdef HASH
  GHashTable* foundx;
#endif
  GPtrArray* found;
  //CDDSGeoPoint* found;
  
  
  found = NULL;
  foundx = NULL;
    
#ifdef TREE
/*  if ( foundx = g_tree_lookup ( mgr->tree, &p->point_x; ) ) 
  {
    cds_area_pointmanager_liststore_point_delete ( mgr, p );
    if ( !g_tree_remove ( foundx, &p->point_y ) )
    {
      g_warning ( "Did not find point in geopoint manager.");
    }
*/
  if ( foundx = g_tree_lookup ( mgr->tree, &p->point_x ) )
  {
    if ( found = g_tree_lookup ( foundx, &p->point_y ) )
    {
      if ( !g_ptr_array_remove ( found, p ) )
      {
	mgr->n_points--;
      }
      else
      {
	g_warning ( "Did not find point in geopoint manager.");
      }
      
      if ( found->len == 0 )
      {
	g_tree_remove ( foundx, &p->point_y );
      }
    }
    else { g_warning ( "Did not find point in geopoint manager."); }
  }
  else { g_warning ( "Did not find point in geopoint manager."); }
#endif
#ifdef HASH
/*
  if ( foundx = g_hash_table_lookup ( mgr->table, &p->point_x ) )
  {
    if ( !g_hash_table_remove( foundx, &p->point_y ) )
    {
      g_warning ( "Did not find point in geopoint manager.");
    }
  }
*/
  if ( foundx = g_hash_table_lookup ( mgr->table, &p->point_x ) )
  {
    if ( found = g_hash_table_lookup ( foundx, &p->point_y ) )
    {
      if ( g_ptr_array_remove ( found, p ) )
      {
	mgr->n_points--;
      }
      else
      {
	g_warning ( "Did not find point in geopoint manager.");
      }
      
      if ( found->len == 0 )
      {
	g_hash_table_remove ( foundx, &p->point_y );
      }
    }
    else { g_warning ( "Did not find point in geopoint manager."); }
  }
  else { g_warning ( "Did not find point in geopoint manager."); }
  

    
#endif
  
}

GPtrArray*
cds_area_pointmanager_lookup_array_xy ( CdsAreaPointManager* mgr, gint x, gint y )
{

#ifdef TREE
  GTree* foundx;
#endif
#ifdef HASH
  GHashTable* foundx;
#endif
  GPtrArray* found;
  
#ifdef TREE
  foundx = g_tree_lookup ( mgr->tree, &x );
#endif
#ifdef HASH
  foundx = g_hash_table_lookup ( mgr->table, &x );
#endif
  
  if ( foundx == NULL )
  {
    return NULL;
  }
  else
  {
#ifdef TREE
    found = g_tree_lookup ( foundx, &y );
#endif
#ifdef HASH
    found = g_hash_table_lookup ( foundx, &y );
#endif
  }
  
  if ( found == NULL )
  {
    return NULL;
  }
  
  return found;
  
  
}

gint
cds_area_pointmanager_lookup_pointp ( CdsAreaPointManager* mgr, CDDSGeoPoint* p )
{
    
#ifdef TREE
  GTree* foundx;
#endif
#ifdef HASH
  GHashTable* foundx;
#endif
  
  GPtrArray* found;
  
#ifdef TREE
  foundx = g_tree_lookup ( mgr->tree, &p->point_x );
#endif
#ifdef HASH  
  foundx = g_hash_table_lookup ( mgr->table, &p->point_x );
#endif
  
  if ( foundx == NULL )
  {
    return 0;
  }
  else
  {
#ifdef TREE
    if ( found = g_tree_lookup ( foundx, &p->point_y ) )
#endif
#ifdef HASH
    if ( found = g_hash_table_lookup ( foundx, &p->point_y ) )
#endif
    {
      for ( guint i = 0; i < found->len; i++ )
      {
	if ( p == g_ptr_array_index (found, i) )
	{
	  return found->len;
	}
      }      
    }
  }
  
  return 0;
  
}


CDDSGeoPoint
*cds_area_pointmanager_lookup_pointxy ( CdsAreaPointManager* mgr, gint x, gint y )
{
  
#ifdef TREE
  GTree* foundx;
#endif
#ifdef HASH
  GHashTable* foundx;
#endif
  GPtrArray* found;
  CDDSGeoPoint* foundp;
  
#ifdef TREE
  foundx = g_tree_lookup ( mgr->tree, &x );
#endif
#ifdef HASH
  foundx = g_hash_table_lookup ( mgr->table, &x );
#endif
  
  if ( foundx == NULL )
  {
    return NULL;
  }
  else
  {
#ifdef TREE
    found = g_tree_lookup ( foundx, &y );
#endif
#ifdef HASH
    found = g_hash_table_lookup ( foundx, &y );
#endif
  }
  
  if ( found == NULL )
  {
    return NULL;
  }
  
  return g_ptr_array_index (found, 0);
  
}


void
cds_area_pointmanager_set_property (GObject      *object,
                                   guint         property_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
  CdsAreaPointManager *self = CDS_AREA_POINTMANAGER (object);

  switch (property_id)
  {

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
cds_area_pointmanager_get_property (GObject    *object,
                                   guint       property_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
  CdsAreaPointManager *self = CDS_AREA_POINTMANAGER (object);

  switch (property_id)
  {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}



void
cds_area_pointmanager_init (CdsAreaPointManager *mgr)
{  

#ifdef TREE
  mgr->tree = g_tree_new_full ( cds_area_pointmanager_compare, 
				NULL,
				cds_area_pointmanager_key_destroy_func, 
				cds_area_pointmanager_valuetree_destroy_func );
#endif /* TREE */

#ifdef HASH  
  mgr->table = g_hash_table_new_full ( g_int_hash, 
				       g_int_equal, // cds_area_pointmanager_equal,
				       cds_area_pointmanager_key_destroy_func, 
				       cds_area_pointmanager_valuetable_destroy_func );
#endif /* HASH */

  mgr->n_points = 0;
}


void
cds_area_pointmanager_class_init (CdsAreaPointManagerClass *klass)
{

  GObjectClass   *gobject_class;
  gobject_class = G_OBJECT_CLASS (klass);
  
  //cds_area_pointmanager_parent_class = g_type_class_peek_parent (klass);
  
  gobject_class->set_property = cds_area_pointmanager_set_property;
  gobject_class->get_property = cds_area_pointmanager_get_property;
  
  /*
  g_object_class_install_properties (gobject_class,
                                     CDDS_GEOPOINTMANAGER_N_PROPERTIES,
                                     cds_area_pointmanager_properties);  
  */
}




CdsAreaPointManager
*cds_area_pointmanager_new ( void )
{
    
  CdsAreaPointManager* mgr = g_object_new (CDS_AREA_TYPE_POINTMANAGER, NULL);
    
  return mgr;
}


void
cds_area_pointmanager_destroy (CdsAreaPointManager* mgr)
{
#ifdef TREE
  g_tree_destroy ( mgr->tree );
#endif
#ifdef HASH
  g_hash_table_destroy ( mgr->table );
#endif
  
  g_object_unref ( mgr );
  
  
}