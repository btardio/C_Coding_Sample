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
#include "../cdds_drawingarea.h"
#include "cdds_geometry_actionmanager.h"
#include "cdds_geometry_action.h"
#include "cdds_geometry_point.h"
#include "cdds_geometry_pointmanager.h"
#include "cdds_geometry_selection.h"

#ifdef CDDS_TEST
#ifdef CDDS_DEBUG

gint
cdds_geometry_test_random ( void )
{
  const gsl_rng_type * type;
  gsl_rng * rng;
  gint rval;
  
  gsl_rng_env_setup();

  type = gsl_rng_default;
  rng = gsl_rng_alloc (type);
  
  gsl_rng_set (rng, (unsigned long int) g_get_monotonic_time () ) ;
  
  rval = (gint) ( gsl_rng_uniform (rng) * 500.0 );
  
  gsl_rng_free (rng);
  
  return rval;
  
}

/*
 * all points that are part of undo or redo queue have a +1 reference count
 */
gboolean
cdds_geometry_test_is_part_of_undoredo_queue ( CdsAreaDrawingArea* area, CDDSGeoPoint* p )
{
  
}

/*
 * points init ref count is +1
 * if the point is in the undo queue its ref count is +1
 * if the point is in the redo queue its ref count is +1
 * if the point is in the geopointmgr then its ref count is +1
 * 
 * no point should be > 4 refcount
 */


gboolean
cdds_geometry_test_refcount_correct ( CdsAreaDrawingArea* area, CDDSGeoPoint* p )
{
  gint refcount = 1; /* initialize to 1, all points init reference count at 1 */
  CDDSGeoAction* a;
  
  /* if point is in undo queue ref count +1 */
  for ( guint i = 0; i < g_queue_get_length ( area->actions->undo ); i++ )
  {
    a = g_queue_peek_nth ( area->actions->undo, i );
    
    if ( cds_area_selection_contains_point( a->selection, p ) )
    {
      refcount += 1;
    }
  }

  /* if point is in undo queue ref count +1 */
  for ( guint i = 0; i < g_queue_get_length ( area->actions->redo ); i++ )
  {
    a = g_queue_peek_nth ( area->actions->redo, i );
    
    if ( cds_area_selection_contains_point( a->selection, p ) )
    {
      refcount += 1;
    }
  }

  /* if point is in geo point mgr ref count +1 */
  if ( cds_area_pointmanager_lookup_pointp ( area->pointsmgr, p ) )
  {
    refcount += 1;
  }
  
  
  if ( p->refcount != refcount )
  {
    g_warning ( "Reference count expected: %d, actual: %d", refcount, p->refcount );
  }
  
}

/*
 * all points added should have the same reference count after undo followed by redo
 */
void
cdds_geometry_test_add_undo_redo ( CdsAreaDrawingArea* area )
{
  CDDSGeoPoint* p;
  
  for ( gint x = 0; x < 500; x += cdds_geometry_test_random() / 2 )
  {
    for ( gint y = 0; y < 500; y += cdds_geometry_test_random() / 2 )
    {
      p = cds_area_actionmanager_point_new ( area->actions, area->selection, area->pointsmgr, 
					    cdds_geometry_test_random(), 
					    cdds_geometry_test_random());
            
      if ( x == 0 )
      {
	p->refcount++;
      }
      
    }
  }
    
  for ( gint i = 0; i < allpoints->len; i++ )
  {
    p = g_ptr_array_index ( allpoints , i);
    
    cdds_geometry_test_refcount_correct ( area, p );
    
  }
}


void
cdds_geometry_test_delete_undo_redo ( CdsAreaDrawingArea* area )
{
  GdkEventButton event;
  event.x = cdds_geometry_test_random();
  event.y = cdds_geometry_test_random();
  
  cds_area_selection_select_press ( area->selection, area->pointsmgr, &event );

  event.x = cdds_geometry_test_random();
  event.y = cdds_geometry_test_random();
  
  cds_area_selection_select_release ( area->selection, area->pointsmgr, &event );
  
  
  
}


void
cdds_geometry_test ( CdsAreaDrawingArea* area )
{
  g_message ( "Begin Test" );
  
  cdds_geometry_test_add_undo_redo ( area );
  
/*  for ( gint x = 0; x < 500; x += cdds_geometry_test_random() )
  {
    for ( gint y = 0; y < 500; y += cdds_geometry_test_random() )
    {
      cds_area_actionmanager_point_new ( area->actions, area->selection, area->pointsmgr, 
					cdds_geometry_test_random(), 
					cdds_geometry_test_random());
    }
  }
  */
}


#endif
#endif