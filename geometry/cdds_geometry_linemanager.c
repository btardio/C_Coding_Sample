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

#include "cdds_geometry_linemanager.h"
#include "cdds_geometry_line.h"
#include "cdds_geometry_point.h"
//#include "cdds_geometry_point.h"
#include "../cdds_window.h"
#include "../cdds_drawingarea.h"

G_DEFINE_TYPE(CdsAreaLineManager, cds_area_linemanager, G_TYPE_OBJECT)

/* todo: select a line with same endpoints */

/*
gboolean
cds_area_linemanager_equal (gconstpointer a, gconstpointer b)
{
  return *(gint*)a == *(gint*)b;
}
*/

gint
cds_area_linemanager_compare ( gconstpointer _a, gconstpointer _b, gpointer data )
{
  const gint* a = _a;
  const gint* b = _b;
  
  return *a - *b;
}


void
cds_area_linemanager_direct_hash_key_destroy_func ( gpointer data )
{
  
  CdsAreaLine* l = data;

  cds_area_line_delete_from_liststore ( l );
  
  cds_area_line_unref ( l );  
  
}

void
cds_area_linemanager_direct_hash_value_destroy_func ( gpointer data )
{
  
  //g_free((gint*)data); 
  
}



void
cds_area_linemanager_key_destroy_func ( gpointer data )
{
  g_free((gint*)data); 
}

/*
void
cds_area_linemanager_valueline_destroy_func ( gpointer data )
{
  CdsAreaLine* l = data;
    
  cds_area_line_delete_from_liststore ( l );
  
  cds_area_line_unref ( l );
  
}
*/
void
cds_area_linemanager_valueptrarray_destroy_func ( gpointer data )
{
  
  GPtrArray* ptrarray;
  ptrarray = data;
  
  g_ptr_array_free ( ptrarray, TRUE );
  
}

void
cds_area_linemanager_valuetable_destroy_func ( gpointer data )
{
  g_hash_table_destroy ( ( GHashTable* ) data );
}

void
cds_area_linemanager_valuetree_destroy_func ( gpointer data )
{
  g_tree_destroy ( (GTree*) data );
}

#ifdef TREE
gboolean
#endif
#ifdef HASH
void
#endif
cds_area_linemanager_draw_foreach ( gpointer key, gpointer value, gpointer data )
{
  cairo_t *cr;
  CdsAreaLine* l;
  cr = data;
  l = key;
  
  cairo_save(cr);  
  cairo_set_line_width (cr, 3.0);

  if ( l->selected )
  {
    cairo_set_source_rgba (cr, l->color_r_selected, l->color_g_selected, l->color_b_selected, 1.0);
  }
  else{
    cairo_set_source_rgba (cr, l->color_r, l->color_g, l->color_b, 1.0);
  }
  
  //cairo_arc (cr, l->point_x, l->point_y, 4, 0, M_PI + M_PI);
  cairo_move_to ( cr, l->p1->point_x, l->p1->point_y );
  cairo_line_to ( cr, l->p2->point_x, l->p2->point_y );
  
  cairo_stroke (cr);
  cairo_restore(cr);

#ifdef TREE
  return FALSE;
#endif
}

/*
#ifdef TREE
gboolean
#endif
#ifdef HASH
void
#endif
cds_area_linemanager_foreach_ptrarray ( gpointer key, gpointer value, gpointer data )
{
  
  GPtrArray* ptrarray;
  ptrarray = value;
  
  g_ptr_array_foreach ( ptrarray, cds_area_linemanager_foreach_line, data );
  
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
cds_area_linemanager_foreach_mgr ( gpointer key, gpointer value, gpointer data )
{
#ifdef TREE
  g_tree_foreach ( ( GTree* ) value, cds_area_linemanager_foreach_ptrarray, data);
#endif
#ifdef HASH
  g_hash_table_foreach ( ( GHashTable* ) value, cds_area_linemanager_foreach_ptrarray, data);
#endif
  
#ifdef TREE
  return FALSE;
#endif
}
*/
void
cds_area_linemanager_draw_lines ( CdsAreaLineManager* mgr, cairo_t *cr )
{
/*  
#ifdef TREE  
  g_tree_foreach (mgr->tree, cds_area_linemanager_foreach_mgr, cr);
#endif
#ifdef HASH
  g_hash_table_foreach ( mgr->table, cds_area_linemanager_foreach_mgr, cr);
#endif
*/

  g_hash_table_foreach ( mgr->hash_lines, cds_area_linemanager_draw_foreach, cr );
  
}

//found = g_ptr_array_new_with_free_func ( cds_area_linemanager_valueline_destroy_func );
// insertions are done multiple times for every line
// everytime the free func is called it is being called on the same line
// this causes finalization thread disjointment upon subsequent calls of free for the same line
// solution is a g_ptr_array without free func, and a separate hash table for lines which are called for finalization
// order: lookup line address in hash(key_x:hash) then look up line in hash(address:address) for deletion
void cds_area_linemanager_insert_xy_to_structure ( managers* mgrs, CdsAreaLine* l, int x, int y )
{
  
#ifdef TREE
  GTree* foundx;
#endif
#ifdef HASH
  GHashTable* foundx;
#endif
  
  GPtrArray* found;
  gint* key_x;
  gint* key_y;
    
  foundx = NULL;
  found = NULL;

  key_x = g_malloc(sizeof(gint));
  key_y = g_malloc(sizeof(gint));
  *key_x = x;
  *key_y = y;
  
#ifdef TREE
  if ( foundx = g_tree_lookup ( mgrs->lmgr->tree, key_x ) ) // if the x tree contains line 
  {
    if ( found = g_tree_lookup ( foundx, key_y ) ) // if the y tree contains line 
#endif
#ifdef HASH
  if ( foundx = g_hash_table_lookup ( mgrs->lmgr->table, key_x ) )
  {
    if ( found = g_hash_table_lookup ( foundx, key_y ) )
#endif
    {            
      g_ptr_array_add ( found, l );
      //mgrs->lmgr->n_lines++;
    }
    else
    {
#ifdef TREE
      //g_tree_insert ( foundx, key_y, p ); 
      //found = g_ptr_array_new_with_free_func ( cds_area_linemanager_valueline_destroy_func );
      found = g_ptr_array_new ( );
      
      g_tree_insert ( foundx, key_y, found );
      
      g_ptr_array_add ( found, l ); // line has been added to the x's tree ptrarray
      //mgrs->lmgr->n_lines++;
      
#endif
#ifdef HASH
      //g_hash_table_insert ( foundx, key_y, p );
      //found = g_ptr_array_new_with_free_func ( cds_area_linemanager_valueline_destroy_func );
      found = g_ptr_array_new ( );
      
      g_hash_table_insert ( foundx, key_y, found );
      
      g_ptr_array_add ( found, l ); // line has been added to the x's tree ptrarray 
      //mgrs->lmgr->n_lines++;
      
#endif
      
    }
  }
  
  else
  {
#ifdef TREE
    
    foundx = g_tree_new_full ( cds_area_linemanager_compare, NULL,
			       cds_area_linemanager_key_destroy_func, 
			       cds_area_linemanager_valueptrarray_destroy_func ) );
    
    g_tree_insert ( mgrs->lmgr->tree, key_x, foundx );
    
    //found = g_ptr_array_new_with_free_func ( cds_area_linemanager_valueline_destroy_func );
    found = g_ptr_array_new ( );
    
    g_tree_insert ( foundx, key_y, found );
    
    g_ptr_array_add ( found, l );
    //mgrs->lmgr->n_lines++;
    
#endif
#ifdef HASH
    
    foundx = g_hash_table_new_full ( g_int_hash, 
				     g_int_equal, //cds_area_linemanager_equal,
				     cds_area_linemanager_key_destroy_func, 
				     cds_area_linemanager_valueptrarray_destroy_func );
    
    g_hash_table_insert ( mgrs->lmgr->table, key_x, foundx );
    
    //found = g_ptr_array_new_with_free_func ( cds_area_linemanager_valueline_destroy_func );
    found = g_ptr_array_new ( );
    
    g_hash_table_insert ( foundx, key_y, found );
    
    g_ptr_array_add ( found, l );
    //mgrs->lmgr->n_lines++;
    
    
#endif
      
  }
  
}



void cds_area_linemanager_insert_key_value_for_line(managers* mgrs, CdsAreaLine* l) {
 
  gint x0 = l->p1->point_x;
  gint y0 = l->p1->point_y;
  gint x1 = l->p2->point_x;
  gint y1 = l->p2->point_y;
  
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    
    cds_area_linemanager_insert_xy_to_structure ( mgrs, l, x0, y0 );
    
    //g_message ( "%d, %d", x0, y0 );
    
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

gboolean
cds_area_linemanager_insert_linel ( managers* mgrs, CdsAreaLine* l )
{

  cds_area_linemanager_insert_key_value_for_line ( mgrs, l );

  cds_area_line_ref ( l );
    
  g_hash_table_add ( mgrs->lmgr->hash_lines, l );
  
  mgrs->lmgr->n_lines++;
  

}



void
cds_area_linemanager_delete_xy_in_structure ( CdsAreaLineManager* mgr, CdsAreaLine* l, int x, int y )
{

#ifdef TREE
  GTree* foundx;
#endif
#ifdef HASH
  GHashTable* foundx;
#endif
  GPtrArray* found;
  //CDDSGeoLine* found;
  
  
  found = NULL;
  foundx = NULL;
    
#ifdef TREE
  if ( foundx = g_tree_lookup ( mgr->tree, &x ) )
  {
    if ( found = g_tree_lookup ( foundx, &y ) )
    {
      if ( !g_ptr_array_remove ( found, l ) )
      {
	mgr->n_lines--;
      }
      else
      {
	g_warning ( "Did not find line in geoline manager.");
      }
      
      if ( found->len == 0 )
      {
	g_tree_remove ( foundx, &y );
      }
    }
    else { g_warning ( "Did not find line in geoline manager."); }
  }
  else { g_warning ( "Did not find line in geoline manager."); }
#endif
#ifdef HASH
  if ( foundx = g_hash_table_lookup ( mgr->table, &x ) )
  {
    if ( found = g_hash_table_lookup ( foundx, &y ) )
    {
      if ( g_ptr_array_remove ( found, l ) )
      {
	mgr->n_lines--;
      }
      else
      {
	g_warning ( "Did not find line in geoline manager.");
      }
      
      if ( found->len == 0 )
      {
	g_hash_table_remove ( foundx, &y );
      }
    }
    else { g_warning ( "Did not find line in geoline manager."); }
  }
  else { g_warning ( "Did not find line in geoline manager."); }
  

    
#endif
  
  
}
  
  
void
cds_area_linemanager_delete_key_value_for_line(CdsAreaLineManager* mgr, CdsAreaLine* l) {

  gint x0 = l->p1->point_x;
  gint y0 = l->p1->point_y;
  gint x1 = l->p2->point_x;
  gint y1 = l->p2->point_y;
  
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    
    cds_area_linemanager_delete_xy_in_structure ( mgr, l, x0, y0 );
    
    //g_message ( "%d, %d", x0, y0 );
    
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}


void
cds_area_linemanager_delete_linel ( CdsAreaLineManager* mgr, CdsAreaLine* l )
{

  cds_area_linemanager_delete_key_value_for_line ( mgr, l );

  //cds_area_line_ref ( l );
    
  g_hash_table_remove ( mgr->hash_lines, l );
  
  mgr->n_lines--;
  
}

GPtrArray*
cds_area_linemanager_lookup_array_xy ( CdsAreaLineManager* mgr, gint x, gint y )
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
cds_area_linemanager_lookup_linel ( CdsAreaLineManager* mgr, CdsAreaLine* l )
{
/*    
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
  */
}


CdsAreaLine
*cds_area_linemanager_lookup_linexy ( CdsAreaLineManager* mgr, gint x, gint y )
{
  
#ifdef TREE
  GTree* foundx;
#endif
#ifdef HASH
  GHashTable* foundx;
#endif
  GPtrArray* found;
  CdsAreaLine* foundl;
  
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
cds_area_linemanager_set_property (GObject      *object,
                                   guint         property_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
  CdsAreaLineManager *self = CDS_AREA_LINEMANAGER (object);

  switch (property_id)
  {

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
cds_area_linemanager_get_property (GObject    *object,
                                   guint       property_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
  CdsAreaLineManager *self = CDS_AREA_LINEMANAGER (object);

  switch (property_id)
  {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}



void
cds_area_linemanager_init (CdsAreaLineManager *mgr)
{  

#ifdef TREE
  mgr->tree = g_tree_new_full ( cds_area_linemanager_compare, 
				NULL,
				cds_area_linemanager_key_destroy_func, 
				cds_area_linemanager_valuetree_destroy_func );
#endif /* TREE */

#ifdef HASH  
  mgr->table = g_hash_table_new_full ( g_int_hash, 
				       g_int_equal, // cds_area_linemanager_equal,
				       cds_area_linemanager_key_destroy_func, 
				       cds_area_linemanager_valuetable_destroy_func );
#endif /* HASH */

  mgr->hash_lines = g_hash_table_new_full ( g_direct_hash,
					    g_direct_equal,
					    cds_area_linemanager_direct_hash_key_destroy_func,
					    cds_area_linemanager_direct_hash_value_destroy_func );
  
  mgr->n_lines = 0;
}


void
cds_area_linemanager_class_init (CdsAreaLineManagerClass *klass)
{

  GObjectClass   *gobject_class;
  gobject_class = G_OBJECT_CLASS (klass);
  
  //cds_area_linemanager_parent_class = g_type_class_peek_parent (klass);
  
  gobject_class->set_property = cds_area_linemanager_set_property;
  gobject_class->get_property = cds_area_linemanager_get_property;
  
  /*
  g_object_class_install_properties (gobject_class,
                                     CDDS_GEOLINEMANAGER_N_PROPERTIES,
                                     cds_area_linemanager_properties);  
  */
}




CdsAreaLineManager
*cds_area_linemanager_new ( void )
{
    
  CdsAreaLineManager* mgr = g_object_new (CDS_AREA_TYPE_LINEMANAGER, NULL);
    
  return mgr;
}


void
cds_area_linemanager_destroy (CdsAreaLineManager* mgr)
{
#ifdef TREE
  g_tree_destroy ( mgr->tree );
#endif
#ifdef HASH
  g_hash_table_destroy ( mgr->table );
#endif
  
  g_hash_table_destroy ( mgr->hash_lines );
  
  g_object_unref ( mgr );
  
  
}