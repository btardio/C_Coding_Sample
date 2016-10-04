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
#include "cdds_application.h"
#include "cdds_drawingarea.h"
#include "cdds_window.h"
#include "geometry/cdds_geometry_actionmanager.h"
#include "geometry/cdds_geometry_pointmanager.h"
#include "geometry/cdds_geometry_selection.h"
#include "geometry/cdds_geometry_point.h"
#include "geometry/cdds_geometry_action.h"

/*
 * if action in allactions is not in undo queue, redo queue, assert error
 */
void
cdds_geometry_test_mallocd_actions_are_referenced ( CdsAreaDrawingArea* area )
{
  
  gint refcount;
  CDDSGeoAction* a;
  GError error;
  error.code = error.domain = 0;
  
  if ( area->mgrs->allactions == NULL )
  {
    error.message = "All actions ptr array is NULL.";
    g_assert_no_error ( &error );
    g_test_fail ();
    return;
  }
  
  for ( guint i = 0; i < area->mgrs->allactions->len; i++ )
  {
    refcount = 0;
    a = g_ptr_array_index ( area->mgrs->allactions, i );
    
    
    /* if action is in undo queue ref count +1 */
    for ( guint i = 0; i < g_queue_get_length ( area->mgrs->amgr->undo ); i++ )
    {
      if ( a == g_queue_peek_nth ( area->mgrs->amgr->undo, i ) )
      {
	refcount += 1;
      }
    }
      
    /* if action is in redo queue ref count +1 */
    for ( guint i = 0; i < g_queue_get_length ( area->mgrs->amgr->redo ); i++ )
    {
      if ( a == g_queue_peek_nth ( area->mgrs->amgr->redo, i ) )
      {
	refcount += 1;
      }
    }

    if ( refcount == 0 )
    {
      
      error.message = "Unreferenced action not destroyed in memory.";
      g_assert_no_error ( &error );
      g_test_fail ();
      
    }
  }
}

/*
 * if point in allpoints is not in undo queue, redo queue or geopointmgr, assert error
 */
void
cdds_geometry_test_mallocd_points_are_referenced ( CdsAreaDrawingArea* area )
{
  gint refcount = 0;
  CDDSGeoAction* a;
  CDDSGeoPoint* p;
  GError error;
  error.code = error.domain = 0;
  
  if ( area->mgrs->allpoints == NULL )
  {
    error.message = "All points ptr array is NULL.";
    g_assert_no_error ( &error );
    g_test_fail ();
    return;
  }
  
  for ( guint i = 0; i < area->mgrs->allpoints->len; i++ )
  {
    refcount = 0;
    p = g_ptr_array_index ( area->mgrs->allpoints, i );
    
    /* if point is in undo queue ref count +1 */
    for ( guint i = 0; i < g_queue_get_length ( area->mgrs->amgr->undo ); i++ )
    {
      a = g_queue_peek_nth ( area->mgrs->amgr->undo, i );
      
      if ( cds_area_selection_contains_point( a->selection, p ) )
      {
	refcount += 1;
      }
    }
      
    /* if point is in redo queue ref count +1 */
    for ( guint i = 0; i < g_queue_get_length ( area->mgrs->amgr->redo ); i++ )
    {
      a = g_queue_peek_nth ( area->mgrs->amgr->redo, i );
      
      if ( cds_area_selection_contains_point( a->selection, p ) )
      {
	refcount += 1;
      }
    }

    /* if point is in geo point mgr ref count +1 */
    /* if ( cds_area_pointmanager_lookup_pointp ( area->pointsmgr, p ) ) */
    if ( cds_area_pointmanager_lookup_pointp ( area->mgrs->pmgr, p ) )
    {
      refcount += 1;
    }

    if ( refcount == 0 )
    {
      
      error.message = "Unreferenced point not destroyed in memory.";
      g_assert_no_error ( &error );
      g_test_fail ();
      
    }
  }
  
}

/*
 * points init ref count is +1
 * if the point is in the undo queue its ref count is +1
 * if the point is in the redo queue its ref count is +1
 * if the point is in the geopointmgr then its ref count is +1
 * 
 * no point should be > 4 refcount
 */
void
cdds_geometry_test_refcount_correct_for_point ( CdsAreaDrawingArea* area, CDDSGeoPoint* p )
{
  gint refcount = 0;
  CDDSGeoAction* a;
  GError error;
  error.code = error.domain = 0;
  
  /* if point is in undo queue ref count +1 */
  for ( guint i = 0; i < g_queue_get_length ( area->mgrs->amgr->undo ); i++ )
  {
    a = g_queue_peek_nth ( area->mgrs->amgr->undo, i );
    
    if ( cds_area_selection_contains_point( a->selection, p ) )
    {
      refcount += 1;
    }
  }

  /* if point is in redo queue ref count +1 */
  for ( guint i = 0; i < g_queue_get_length ( area->mgrs->amgr->redo ); i++ )
  {
    a = g_queue_peek_nth ( area->mgrs->amgr->redo, i );
    
    if ( cds_area_selection_contains_point( a->selection, p ) )
    {
      refcount += 1;
    }
  }

  /* if point is in geo point mgr ref count +1 */
  if ( cds_area_pointmanager_lookup_pointp ( area->mgrs->pmgr, p ) )
  {
    refcount += 1;
  }
  
  
  if ( p->refcount != refcount )
  {
    
    g_message ( "Point reference count expected: %d, actual: %d", refcount, p->refcount );
    error.message = "Point reference count incorrect.";
    g_assert_no_error ( &error );
    g_test_fail ();
    
  }
  
}

void
cdds_geometry_test_refcount_correct_forall_points ( CdsAreaDrawingArea* area )
{
  
  CDDSGeoPoint* p;
  GError error;
  error.code = error.domain = 0;
  
  if ( area->mgrs->allpoints == NULL )
  {
    error.message = "All points ptr array is NULL.";
    g_assert_no_error ( &error );
    g_test_fail ();
    return;
  }
  
  for ( guint i = 0; i < area->mgrs->allpoints->len; i++ )
  {
    
    p = g_ptr_array_index ( area->mgrs->allpoints, i );
    
    cdds_geometry_test_refcount_correct_for_point ( area, p );

  }
  
}

void
cdds_geometry_test_refcount_correct_for_action ( CdsAreaDrawingArea* area, CDDSGeoAction* action )
{
  
  gint refcount = 1;
  CDDSGeoAction* a;
  GError error;
  error.code = error.domain = 0;
  
  /* if point is in undo queue ref count +1 */
  for ( guint i = 0; i < g_queue_get_length ( area->mgrs->amgr->undo ); i++ )
  {
    if ( action == g_queue_peek_nth ( area->mgrs->amgr->undo, i ) )
    {
      refcount++;
    }
  }

  /* if point is in redo queue ref count +1 */
  for ( guint i = 0; i < g_queue_get_length ( area->mgrs->amgr->redo ); i++ )
  {
    if ( action ==  g_queue_peek_nth ( area->mgrs->amgr->redo, i ) )
    {
      refcount++;
    }
  }

  if ( action->refcount != refcount )
  {
    g_warning ( "Action reference count expected: %d, actual: %d", refcount, action->refcount );
    error.message = "Action reference count incorrect.";
    g_assert_no_error ( &error );
    g_test_fail ();

  }
  
  
}

void
cdds_geometry_test_refcount_correct_forall_actions ( CdsAreaDrawingArea* area )
{
  CDDSGeoAction* action;
  GError error;
  error.code = error.domain = 0;
  
  if ( area->mgrs->allactions == NULL )
  {
    
    error.message = "All actions ptr array is NULL.";
    g_assert_no_error ( &error );
    g_test_fail ();
    return;
  }
  
  for ( guint i = 0; i < area->mgrs->allactions->len; i++ )
  {
    
    action = g_ptr_array_index ( area->mgrs->allactions, i );
    
    cdds_geometry_test_refcount_correct_for_action ( area, action );
    
  }
    

  
}

void cdds_geometry_test_unmarked_does_not_exist_in_pointmanager ( CdsAreaDrawingArea* area )
{
  CDDSGeoPoint* p;
  GError error;
  error.code = error.domain = 0;
  gint width;
  gint height;
  gint rowstride;
  gint n_channels;
  guchar* pdata;
  guchar* pixel;

  width = gdk_pixbuf_get_width (area->mgrs->smgr->pixbuf);
  height = gdk_pixbuf_get_height (area->mgrs->smgr->pixbuf);
  
  n_channels = gdk_pixbuf_get_n_channels (area->mgrs->smgr->pixbuf);
  
  rowstride = gdk_pixbuf_get_rowstride ( area->mgrs->smgr->pixbuf );
  
  pdata = gdk_pixbuf_get_pixels ( area->mgrs->smgr->pixbuf );


  for ( gint x = 0; x < width; x ++ )
  {
    for ( gint y = 0; y < height; y ++ )
    {
      
      pixel = pdata + y * rowstride + x * n_channels;
      
      switch ( pixel[0] )
      {
	case PIXBUF_POINT:
	  
	  if ( cds_area_pointmanager_lookup_pointxy ( area->mgrs->pmgr, x, y ) == NULL)
	  {
	    error.message = "Pixel in selection is marked as having a point but no point found at those coordinates.";
	    g_assert_no_error ( &error );
	    g_test_fail ();	    
	  }
	  break;
	  
	case PIXBUF_EMPTY:

	  if ( cds_area_pointmanager_lookup_pointxy ( area->mgrs->pmgr, x, y ) != NULL)
	  {
	    error.message = "Pixel in selection is marked as not having a point but point found in point mgr at coordinates.";
	    g_assert_no_error ( &error );
	    g_test_fail ();	    
	  }
	  break;
	  
	default:
	  
	  error.message = "Pixel in selection has unrecognized value.";
	  g_assert_no_error ( &error );
	  g_test_fail ();
	  
	  break;
      }
    }
  }
}
  


void
cdds_geometry_test_points_marked_in_selection ( CdsAreaDrawingArea* area )
{
  CDDSGeoPoint* p;
  GError error;
  error.code = error.domain = 0;
  gint width;
  gint height;
  gint rowstride;
  gint n_channels;
  guchar* pdata;
  guchar* pixel;

  for ( guint i = 0; i < area->mgrs->allpoints->len; i++ )
  {
    
    p = g_ptr_array_index ( area->mgrs->allpoints, i );
    
    if ( cds_area_pointmanager_lookup_pointp ( area->mgrs->pmgr, p ) )
    {
      width = gdk_pixbuf_get_width (area->mgrs->smgr->pixbuf);
      height = gdk_pixbuf_get_height (area->mgrs->smgr->pixbuf);
      
      n_channels = gdk_pixbuf_get_n_channels (area->mgrs->smgr->pixbuf);
      
      rowstride = gdk_pixbuf_get_rowstride ( area->mgrs->smgr->pixbuf );
      
      pdata = gdk_pixbuf_get_pixels ( area->mgrs->smgr->pixbuf );
      
      pixel = pdata + p->point_y * rowstride + p->point_x * n_channels;
      
      if ( pixel[0] != PIXBUF_POINT ) 
      {
	error.message = "Point in point manager is not marked in selection.";
	g_assert_no_error ( &error );
	g_test_fail ();
      } 
    }
  }
}

void
cds_area_test_actions ( CdsAreaDrawingArea* area )
{
  
  cdds_geometry_test_refcount_correct_forall_actions ( area );
  cdds_geometry_test_mallocd_actions_are_referenced ( area );
  
}


void
cds_area_test_points ( CdsAreaDrawingArea* area )
{
  
  cdds_geometry_test_refcount_correct_forall_points ( area );
  cdds_geometry_test_mallocd_points_are_referenced ( area );
  cdds_geometry_test_points_marked_in_selection ( area );
  cdds_geometry_test_unmarked_does_not_exist_in_pointmanager ( area );
}
