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

#include <gsl/gsl_rng.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_randist.h>

#include <locale.h>
#include <string.h>

#define TEST_N_INIT_POINTSET 300
#define TEST_N_ACTIONS 100 /* execute the specified action N number of times */
#define TEST_N_SELECTED(N_POINTS) ((N_POINTS < 10) ? N_POINTS : 10) /* execute the action selecting N number of points */

#define TEST_DRAWINGAREA_WIDTH 300
#define TEST_DRAWINGAREA_HEIGHT 300

#define TEST_REDRAW_CHECK_WAIT(TIMEWAIT) time = g_get_monotonic_time (); \
					 gtk_test_widget_wait_for_draw ( GTK_WIDGET ( fixture->area ) ); \
					 cds_area_test_actions ( fixture->area ); \
					 cds_area_test_points ( fixture->area ); \
					 while ( g_get_monotonic_time () - time < TIMEWAIT ) {}
					 

/* todo: automatically start tests on window activate and invoke shutdown on test conclusion */
					 

/* todo: test points that are occupying same int coord */


/* note: testing with undo/redo requires undo redo queue reset after initial set creation using point_new */

//CdsAreaApp* test_app;

//CDDSApp *app;

typedef struct {
  CdsAreaDrawingArea *area;
  GPtrArray *ptrarray;
} CDDSDrawingAreaFixture;

typedef struct {
  CdsAreaWindow** win;
  gint width;
  gint height;
} test_setup;


gsl_permutation* p;
gint perm_iter;

void
cdds_geometry_test_perm_reset ( gint n )
{
  
  const size_t N = n;
  const gsl_rng_type* T;
  gsl_rng * r;

  perm_iter = 0;
  
  p = gsl_permutation_alloc (N);
  
  gsl_rng_env_setup();
  
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  
  gsl_rng_set (r, (unsigned long int) g_get_monotonic_time () ) ;
  
  gsl_permutation_init (p);
  
  gsl_ran_shuffle (r, p->data, N, sizeof(size_t));
  
  gsl_rng_free (r);
  
}

void
cdds_geometry_test_perm_free ( void )
{
  gsl_permutation_free (p);
}


gint
cdds_geometry_test_perm_next ( void )
{
  
  //g_message ( "%d", gsl_permutation_get ( p, perm_iter ) );
  
  //perm_iter++;
  
  return gsl_permutation_get ( p, perm_iter++ );
}




const gsl_rng_type * type;
gsl_rng * rng;


void
cdds_geometry_test_random_init ( void )
{
  
  gsl_rng_env_setup();
  type = gsl_rng_default;
  rng = gsl_rng_alloc (type);
  gsl_rng_set (rng, (unsigned long int) g_get_monotonic_time () ) ;
  
}

void
cdds_geometry_test_random_destroy ( void )
{
  gsl_rng_free (rng);
}

gint
cdds_geometry_test_random ( gint max )
{
  gint rval;
  gdouble dmax;
  gdouble rn;
  
  dmax = (gdouble) max;
  
  rn = gsl_rng_uniform (rng);
  
  //rval = lrint ( gsl_rng_uniform (rng) * dmax );
  
  rn = rn * max;
  
  rval = lrint ( rn );
  
  rn = rn - 0.01;
  
  return rval;
  
}


void cdds_geometry_test_add_random_points ( CDDSDrawingAreaFixture *fixture, gint n_points )
{
  gint64 time;
  for ( gint i = 0; i < n_points; i += 1 )
  {
    /* cds_area_actionmanager_point_new ( fixture->area->actions, fixture->area->selection, fixture->area->pointsmgr, 
					  cdds_geometry_test_random( fixture->area->width ), 
					  cdds_geometry_test_random( fixture->area->height )); */
    //cds_area_actionmanager_point_new ( fixture->area->mgrs, 
//				      cdds_geometry_test_random( fixture->area->width ), 
//				      cdds_geometry_test_random( fixture->area->height ) );

    cds_area_actionmanager_point_new ( fixture->area->mgrs, 
				       g_test_rand_int_range ( 0, fixture->area->width ),
				       g_test_rand_int_range ( 0, fixture->area->height ) );

    //TEST_REDRAW_CHECK_WAIT(10)
    
    //gtk_test_widget_wait_for_draw ( GTK_WIDGET ( fixture->area ) );
  }
}




static void
cdds_drawingarea_fixture_set_up (CDDSDrawingAreaFixture *fixture, gconstpointer data)
{
  test_setup* setup;
  CdsAreaWindow** winptr;
  CdsAreaWindow* win;
  
  setup = (test_setup*)data;
  winptr = setup->win;
  win = *winptr;

  cds_area_window_create_drawingarea ( win, setup->width, setup->height );

  fixture->area = win->area;
  
  fixture->ptrarray = g_ptr_array_new();
  
  cdds_geometry_test_random_init ( );
  
  gtk_test_widget_wait_for_draw ( GTK_WIDGET ( win->area ) );
  
}


static void
cdds_drawingarea_fixture_tear_down (CDDSDrawingAreaFixture *fixture, gconstpointer data)
{

  test_setup* setup;
  CdsAreaWindow** winptr;
  CdsAreaWindow* win;
  
  setup = (test_setup*)data;
  winptr = setup->win;
  win = *winptr;

  g_free ( setup );  
  
  cds_area_window_destroy_drawingarea ( win );
  
  g_ptr_array_free ( fixture->ptrarray, TRUE );
  
  cdds_geometry_test_random_destroy ( );
  
  gtk_test_widget_wait_for_draw ( GTK_WIDGET ( win ) );
}





/*
 * memory test
 * 
 * valgrind --tool=massif --depth=5
 * --alloc-fn=g_malloc --alloc-fn=g_realloc --alloc-fn=g_try_malloc --alloc-fn=g_malloc0 --alloc-fn=g_mem_chunk_alloc 
 * ./click_drag_draw_select_test

 * valgrind --tool=massif [--time-unit=B] --depth=5  
 * --alloc-fn=g_malloc --alloc-fn=g_realloc --alloc-fn=g_try_malloc --alloc-fn=g_malloc0 --alloc-fn=g_mem_chunk_alloc 
 * ./click_drag_draw_select_test
 * 
 * ms_print massif.out
 * 
 * https://developer.gnome.org/optimization-guide/stable/massif.html.en
 * 
 * 
 */

test_setup*
cdds_drawingarea_test_createdestroy_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 500, 1000 );
  setup->height = g_test_rand_int_range ( 500, 1000 );
  return setup;
}

static void
cds_area_drawingarea_test_createdestroy ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  test_setup* setup;
  CdsAreaWindow** winptr;
  CdsAreaWindow* win;
  gint64 time;
  
  setup = (test_setup*)data;
  winptr = setup->win;
  win = *winptr;


  cds_area_window_destroy_drawingarea ( win );
  
  for ( int i = 0; i < 5; i++ )
  {
    
    cds_area_window_create_drawingarea ( win, setup->width, setup->height );
    fixture->area = win->area;
      
    //TEST_REDRAW_CHECK_WAIT(10)
    
    cdds_geometry_test_add_random_points ( fixture, 500 );
        
    cds_area_window_destroy_drawingarea ( win );
    
    
    //TEST_REDRAW_CHECK_WAIT(10)
    
  }
  
  
  cds_area_window_create_drawingarea ( win, setup->width, setup->height );
  fixture->area = win->area;
  
}




test_setup*
cdds_drawingarea_test_add_five_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 10, 100 );
  setup->height = g_test_rand_int_range ( 10, 100 );
  return setup;
}

static void
cdds_drawingarea_test_add_five ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  test_setup* setup;
  CDDSGeoPoint* p;
  gint rn_x;
  gint rn_y;
  gint64 time;
  
  setup = (test_setup*)data;
  
  /* test border regions */
  for ( gint i = 0; i < 5; i++ )
  {
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, 
					   g_test_rand_int_range ( 0, fixture->area->width + 1 ), 
					   g_test_rand_int_range ( 0, fixture->area->height + 1 ) );
    
    TEST_REDRAW_CHECK_WAIT(10)
    
  }
}


test_setup*
cdds_drawingarea_test_add_six_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 10, 100 );
  setup->height = g_test_rand_int_range ( 10, 100 );
  return setup;
}

static void
cdds_drawingarea_test_add_six ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  test_setup* setup;
  CDDSGeoPoint* p;
  gint rn_x;
  gint rn_y;
  gint64 time;

  setup = (test_setup*)data;
  
  /* test border regions */
  for ( gint i = 0; i < 6; i++ )
  {
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, 
					   g_test_rand_int_range ( 0, fixture->area->width + 1 ), 
					   g_test_rand_int_range ( 0, fixture->area->height + 1 ) );
    
    TEST_REDRAW_CHECK_WAIT(10)
    
  }
}


test_setup*
cdds_drawingarea_test_add_seven_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 10, 100 );
  setup->height = g_test_rand_int_range ( 10, 100 );
  return setup;
}

static void
cdds_drawingarea_test_add_seven ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  test_setup* setup;
  CDDSGeoPoint* p;
  gint rn_x;
  gint rn_y;
  gint64 time;
  
  setup = (test_setup*)data;
  
  /* test border regions */
  for ( gint i = 0; i < 7; i++ )
  {
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, 
					   g_test_rand_int_range ( 0, fixture->area->width + 1 ), 
					   g_test_rand_int_range ( 0, fixture->area->height + 1 ) );
    
    
    TEST_REDRAW_CHECK_WAIT(10)
  }
  
  return;
}


test_setup*
cdds_drawingarea_test_add_maxheight_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 10, 100 );
  setup->height = g_test_rand_int_range ( 10, 100 );
  return setup;
}

static void
cdds_drawingarea_test_add_maxheight ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  gint64 time;
  for ( gint x = 0; x <= fixture->area->width; x++ )
  {
    cds_area_actionmanager_point_new ( fixture->area->mgrs, x, fixture->area->height);
    TEST_REDRAW_CHECK_WAIT(10)    
  }
}


test_setup*
cdds_drawingarea_test_add_minheight_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 10, 100 );
  setup->height = g_test_rand_int_range ( 10, 100 );
  return setup;
}

static void
cdds_drawingarea_test_add_minheight ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  gint64 time;
  for ( gint x = 0; x <= fixture->area->width; x++ )
  {
    cds_area_actionmanager_point_new ( fixture->area->mgrs, x, 0);
    TEST_REDRAW_CHECK_WAIT(10)
  }
}


test_setup*
cdds_drawingarea_test_add_maxwidth_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 10, 100 );
  setup->height = g_test_rand_int_range ( 10, 100 );
  return setup;
}

static void
cdds_drawingarea_test_add_maxwidth ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  gint64 time;
  for ( gint y = 0; y <= fixture->area->height; y++ )
  {
    cds_area_actionmanager_point_new ( fixture->area->mgrs, fixture->area->width, y );
    TEST_REDRAW_CHECK_WAIT(10)    
  }
}


test_setup*
cdds_drawingarea_test_add_minwidth_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 10, 100 );
  setup->height = g_test_rand_int_range ( 10, 100 );
  return setup;
}

static void
cdds_drawingarea_test_add_minwidth ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  gint64 time;
  for ( gint y = 0; y <= fixture->area->height; y++ )
  {
    cds_area_actionmanager_point_new ( fixture->area->mgrs, 0, y );
    TEST_REDRAW_CHECK_WAIT(10)
  }
}



test_setup*
cdds_drawingarea_test_add_fillcanvaswiththreepoints_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 5, 10 );
  setup->height = g_test_rand_int_range ( 5, 10 );
  return setup;
}

static void
cdds_drawingarea_test_add_fillcanvaswiththreepoints ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  gint64 time;
  
  for ( gint i = 0; i < 3; i ++ )
  {
    for ( gint x = 0; x <= fixture->area->width; x++ )
    {
      for ( gint y = 0; y <= fixture->area->height; y++ )
      {
	cds_area_actionmanager_point_new ( fixture->area->mgrs, x, y );
	TEST_REDRAW_CHECK_WAIT(10)
      }
    }  
  }
}

test_setup*
cdds_drawingarea_test_add_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 5, 10 );
  setup->height = g_test_rand_int_range ( 5, 10 );
  return setup;
}

static void
cdds_drawingarea_test_add ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  gint64 time;
  CDDSGeoPoint* p;

  /* test border regions */
  for ( gint x = 0; x <= fixture->area->width; x++ )
  {
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, x, 0);
    TEST_REDRAW_CHECK_WAIT(10)
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, x, fixture->area->height);
    TEST_REDRAW_CHECK_WAIT(10)
  }
  
  for ( gint y = 0; y <= fixture->area->height; y++ )
  {
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, 0, y);
    TEST_REDRAW_CHECK_WAIT(10)
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, fixture->area->width, y);
    TEST_REDRAW_CHECK_WAIT(10)
  }
  
  /* fill every pixel on the canvas with a point */
  for ( gint x = 0; x < fixture->area->width; x++ )
  {
    for ( gint y = 0; y < fixture->area->height; y++ )
    {
      p = cds_area_actionmanager_point_new ( fixture->area->mgrs, x, y);
      TEST_REDRAW_CHECK_WAIT(10)
    }
  }

  /* randomly insert points numbering the number of pixels */

  for ( gint i = 0; i < (fixture->area->width * fixture->area->height); i++ )
  {
        
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, 
					   g_test_rand_int_range ( 0, fixture->area->width ),
					   g_test_rand_int_range ( 0, fixture->area->height ) );
    TEST_REDRAW_CHECK_WAIT(10)
    
  }
  
  
  TEST_REDRAW_CHECK_WAIT(10)
}


test_setup*
cdds_drawingarea_test_add_undo_redo_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 5, 10 );
  setup->height = g_test_rand_int_range ( 5, 10 );
  return setup;
}

static void
cdds_drawingarea_test_add_undo_redo ( CDDSDrawingAreaFixture *fixture, gconstpointer data )
{
  CDDSGeoPoint* p;
  gint64 time;
  
  for ( gint i = 0; i < 300; i += 1 )
  {
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, 
					  g_test_rand_int_range ( 0, fixture->area->width ), 
					  g_test_rand_int_range ( 0, fixture->area->height ) );
    
    
    while ( g_test_rand_bit() + g_test_rand_bit() )
    {

      cds_area_actionmanager_undo ( fixture->area->mgrs );
      TEST_REDRAW_CHECK_WAIT(10)
      
    }
    
    while ( g_test_rand_bit() )
    {

      cds_area_actionmanager_redo ( fixture->area->mgrs );
      TEST_REDRAW_CHECK_WAIT(10)
      
    }

    TEST_REDRAW_CHECK_WAIT(10)
    
  }  
  
  
  TEST_REDRAW_CHECK_WAIT(10)
  

}


test_setup*
cdds_drawingarea_test_delete_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 400, 600 );
  setup->height = g_test_rand_int_range ( 400, 600 );
  return setup;
}

static void
cdds_drawingarea_test_delete (CDDSDrawingAreaFixture *fixture, gconstpointer data)
{
  
  CDDSGeoPoint* p;
  CdsAreaSelection* s;
  gint64 time;
  gint randrange;
  
  gint index;
  
  for ( gint i = 0; i < 300; i ++ )
  {
    
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, 
					   g_test_rand_int_range ( 0, fixture->area->width ), 
					   g_test_rand_int_range ( 0, fixture->area->height ) );
  }
  
  TEST_REDRAW_CHECK_WAIT(10)
  
  for ( gint j = 0; j < 300; j ++ )
  {
    
    s = fixture->area->mgrs->smgr;
    cds_area_selection_clear_selection ( s );
    
    randrange = g_test_rand_int_range ( 0, 100 );
    
    for ( gint i = 0; i < randrange; i++ )
    {
      if ( fixture->area->mgrs->allpoints->len > 0 )
      {
	
	p = g_ptr_array_index ( fixture->area->mgrs->allpoints, 
				g_test_rand_int_range ( 0, fixture->area->mgrs->allpoints->len ) );
	
	
	if ( !cds_area_selection_contains_point ( s, p ) && 
	     cds_area_pointmanager_lookup_pointp ( fixture->area->mgrs->pmgr, p ) )
	{
	  cds_area_selection_insert_pointp ( s, p );
	  
	  gtk_widget_queue_draw ( GTK_WIDGET ( fixture->area ) );
	  
	}
      }
    }
    
    TEST_REDRAW_CHECK_WAIT(10)
    
    cds_area_actionmanager_selected_points_delete ( fixture->area->mgrs );
    
    TEST_REDRAW_CHECK_WAIT(10)
  
    if ( fixture->area->mgrs->allpoints->len == 0 )
    {
      return;
    }

  }
  
}



test_setup*
cdds_drawingarea_test_delete_undo_redo_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 400, 600 );
  setup->height = g_test_rand_int_range ( 400, 600 );
  return setup;
}

static void
cdds_drawingarea_test_delete_undo_redo (CDDSDrawingAreaFixture *fixture, gconstpointer data)
{
  
  CDDSGeoPoint* p;
  CdsAreaSelection* s;
  gint64 time;
  
  for ( gint i = 0; i < 300; i += 1 )
  {
    
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, 
					   g_test_rand_int_range ( 0, fixture->area->width ), 
					   g_test_rand_int_range ( 0, fixture->area->height ) );
  }
  
  TEST_REDRAW_CHECK_WAIT(10)
  
  cds_area_actionmanager_clear_redo ( fixture->area->mgrs->amgr );
  cds_area_actionmanager_clear_undo ( fixture->area->mgrs->amgr );
  
  TEST_REDRAW_CHECK_WAIT(10)
  
  for ( gint j = 0; j < 300; j ++ )
  {
    
    s = fixture->area->mgrs->smgr;
    cds_area_selection_clear_selection ( s );
    
    for ( gint i = 0; i < g_test_rand_int_range ( 0, 100 ); i++ )
    {
      if ( fixture->area->mgrs->allpoints->len > 0 )
      {
	
	p = g_ptr_array_index ( fixture->area->mgrs->allpoints, 
				g_test_rand_int_range ( 0, fixture->area->mgrs->allpoints->len ) );
	
	if ( !cds_area_selection_contains_point ( s, p ) && 
	     cds_area_pointmanager_lookup_pointp ( fixture->area->mgrs->pmgr, p ) )
	{
	  cds_area_selection_insert_pointp ( s, p );
	  gtk_widget_queue_draw ( GTK_WIDGET ( fixture->area ) );
	}
      }
    }
    
    TEST_REDRAW_CHECK_WAIT(10)
    
    cds_area_actionmanager_selected_points_delete ( fixture->area->mgrs );
    
    TEST_REDRAW_CHECK_WAIT(10)
  
    if ( fixture->area->mgrs->allpoints->len == 0 )
    {
      return;
    }

    
    while ( g_test_rand_bit() )
    {

      cds_area_actionmanager_undo ( fixture->area->mgrs );
      TEST_REDRAW_CHECK_WAIT(10)
      
    }
    
    while ( g_test_rand_bit()  + g_test_rand_bit() )
    {

      cds_area_actionmanager_redo ( fixture->area->mgrs );
      TEST_REDRAW_CHECK_WAIT(10)
      
    }

    TEST_REDRAW_CHECK_WAIT(10)

    
    
  }
  
}



struct moveforeach {
  CDDSGeoPoint* p;
  gint x;
  gint y;
};

void
cdds_drawingarea_test_move_foreach (gpointer key, gpointer value, gpointer data)
{
  
  CDDSGeoPoint *p;
  struct moveforeach *p_move;
  GPtrArray* ptrarray;
  
  p = key;
  ptrarray = data;
  
  p_move = g_malloc ( sizeof ( struct moveforeach ) );
  
  //p_copy = g_memdup (p, sizeof ( CDDSGeoPoint ) );
  
  //p_copy = g_malloc ( sizeof ( CDDSGeoPoint ) );
  
  //memcpy ( p_copy, p, sizeof ( CDDSGeoPoint ) );
  
  //p_copy = cdds_geopoint_new ( p->point_x, p->point_y );
  
  //p_copy = g_object_new (CDDS_GEOPOINT_TYPE, NULL);
  //p_copy->point_x = p->point_x;
  //p_copy->point_y = p->point_y;
  
  p_move->x = p->point_x;
  p_move->y = p->point_y;
  p_move->p = p;
  
  g_ptr_array_add ( ptrarray, p_move );
  
  
}

void
cdds_drawingarea_test_movefree_foreach (gpointer data, gpointer user_data)
{
  
  struct moveforeach *p_move;  
  p_move = data;
  
  //g_object_unref ( p ) ;
  //cdds_geopoint_destroy ( p );
  
  g_free ( p_move );
  
}


void
cdds_drawingarea_test_moveptrarray_free_foreach ( gpointer data )
{
  
  struct moveforeach *p_move;  
  p_move = data;
    
  g_free ( p_move );
  
}

test_setup*
cdds_drawingarea_test_move_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 400, 600 );
  setup->width = setup->width - ( setup->width % 20 );
  setup->height = g_test_rand_int_range ( 400, 600 );
  setup->height = setup->height - ( setup->height % 20 );
  return setup;
}

static void
cdds_drawingarea_test_move (CDDSDrawingAreaFixture *fixture, gconstpointer data)
{
  gint64 time;
  CDDSGeoPoint* p;
  CdsAreaSelection* s;
  gboolean perminit = FALSE;
  guint iter;
  
  struct moveforeach* cmp_p_a;
  struct moveforeach* cmp_p_b;
  
  GPtrArray* beforemove;
  GPtrArray* aftermove;
  
  gint p_x;
  gint p_y;
  gint r_x;
  gint r_y;
  
  gint interv = fixture->area->prop_grid_interval;
  
  for ( gint i = 0; i < TEST_N_INIT_POINTSET; i += 1 )
  {
    
    // accomodate snap to grid, if it is on add points based on the interval, this makes move checking sane
    if ( fixture->area->prop_snap_to_grid )
    {
      r_x = g_test_rand_int_range ( 0, fixture->area->width );
      p_x = r_x - ( r_x % fixture->area->prop_grid_interval);
      r_y = g_test_rand_int_range ( 0, fixture->area->height );
      p_y = r_y - ( r_y % fixture->area->prop_grid_interval);
    }
    else
    {
      p_x = g_test_rand_int_range ( 0, fixture->area->width );
      p_y = g_test_rand_int_range ( 0, fixture->area->height );
    }
    
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, p_x, p_y );
  }
  
  for ( gint j = 0; j < TEST_N_ACTIONS; j ++ )
  {
    
    perminit = FALSE;
    beforemove = g_ptr_array_new ( );
    aftermove = g_ptr_array_new ( );
    
    /* this selection is deleted during dispose of action */
    s = cds_area_selection_new ( &fixture->area->width, &fixture->area->height );    

    iter = 1;
    while ( iter <= TEST_N_SELECTED ( fixture->area->mgrs->pmgr->n_points ) &&
            cds_area_selection_len( s ) < TEST_N_SELECTED ( fixture->area->mgrs->pmgr->n_points ) )
    {
      
      if ( !perminit )
      {
	cdds_geometry_test_perm_reset ( fixture->area->mgrs->pmgr->n_points );
	perminit = TRUE;
      }
      
      p = g_ptr_array_index ( fixture->area->mgrs->allpoints, cdds_geometry_test_perm_next() );
      if ( cds_area_pointmanager_lookup_pointp ( fixture->area->mgrs->pmgr, p ) ) 
      {
	cds_area_selection_insert_pointp ( s, p );
      }
      iter++;
    }

    g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, beforemove );
    
    cds_area_actionmanager_selected_points_move ( fixture->area->mgrs, s, 
						 (g_test_rand_bit() == 0 ? -interv : interv),
						 (g_test_rand_bit() == 0 ? -interv : interv) );

    TEST_REDRAW_CHECK_WAIT(10)
    
    g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, aftermove );


    for ( int i = 0; i < beforemove->len; i++ )
    {
      cmp_p_a = g_ptr_array_index ( beforemove, i );
      cmp_p_b = g_ptr_array_index ( aftermove, i );
      
      if ( cmp_p_a->p != cmp_p_b->p ) { g_message ("Hash order changed"); g_test_fail(); }
      
      if ( cmp_p_a->x != cmp_p_b->x - interv && cmp_p_a->x != cmp_p_b->x + interv )
      {
	if ( cmp_p_a->x != 0 && cmp_p_a->x != fixture->area->width )
	{
	  g_message ("Point did not move. beforemovex %d aftermovex %d interv %d", cmp_p_a->x, cmp_p_b->x, interv);
	  g_test_fail();
	}
	
      }

      if ( cmp_p_a->y != cmp_p_b->y - interv && cmp_p_a->y != cmp_p_b->y + interv )
      {
	if ( cmp_p_a->y != 0 && cmp_p_a->y != fixture->area->height )
	{
	  g_message ("Point did not move. beforemovey %d aftermovey %d interv %d", cmp_p_a->y, cmp_p_b->y, interv);
	  g_test_fail();
	}
      }

    }
  
    g_ptr_array_foreach ( aftermove, cdds_drawingarea_test_movefree_foreach, NULL );
    g_ptr_array_foreach ( beforemove, cdds_drawingarea_test_movefree_foreach, NULL );
    
    g_ptr_array_free ( aftermove, TRUE );
    g_ptr_array_free ( beforemove, TRUE );
  
  }

  
}

void
cdds_drawingarea_test_points_moved_correctly ( CDDSDrawingAreaFixture *fixture, 
					       GPtrArray* beforemove, GPtrArray* aftermove,
					       gboolean moved )
{

  struct moveforeach* cmp_p_a;
  struct moveforeach* cmp_p_b;
  gint interv = fixture->area->prop_grid_interval;
  gint64 calltime = g_get_monotonic_time ();
  
  for ( int i = 0; i < beforemove->len; i++ )
  {
    cmp_p_a = g_ptr_array_index ( beforemove, i );
    cmp_p_b = g_ptr_array_index ( aftermove, i );
    
    if ( cmp_p_a->p != cmp_p_b->p ) { g_message ("Hash order changed"); g_test_fail(); }
    
    if ( cmp_p_a->x != cmp_p_b->x - interv && cmp_p_a->x != cmp_p_b->x + interv )
    {
      if ( cmp_p_a->x != 0 && cmp_p_a->x != fixture->area->width )
      {
	if ( moved == TRUE )
	{
	  g_message ("%d Point did not move. beforemovex %d aftermovex %d interv %d", 
		      calltime, cmp_p_a->x, cmp_p_b->x, interv);
	  g_test_fail();
	}
      }
      
    }
    else if ( cmp_p_a->x == cmp_p_b->x && cmp_p_a->x != 0 && cmp_p_a->x != fixture->area->width )
    {
      if ( moved == TRUE )
      {
	g_message ("Point remained moved after a undo.");
	g_test_fail();
      }
      else
      {
	g_message ("Point moved somewhere else.");
      }
    }
    else
    {
      if ( moved == FALSE )
      {
	g_message ("Point moved in an operation that was not supposed to move.");
      }
    }
    

    if ( cmp_p_a->y != cmp_p_b->y - interv && cmp_p_a->y != cmp_p_b->y + interv )
    {
      if ( cmp_p_a->y != 0 && cmp_p_a->y != fixture->area->height )
      {
	if ( moved == TRUE )
	{
	  g_message ("%d Point did not move. beforemovey %d aftermovey %d interv %d", 
		      calltime, cmp_p_a->y, cmp_p_b->y, interv);
	  g_test_fail();
	}
      }
    }
    else if ( cmp_p_a->y == cmp_p_b->y && cmp_p_a->y != 0 && cmp_p_a->y != fixture->area->height )
    {
      if ( moved == TRUE )
      {
	g_message ("Point remained moved after a undo.");
	g_test_fail();
      }
      else
      {
	g_message("Point moved somewhere else.");
      }
    }
    else
    {
      if ( moved == FALSE )
      {
	g_message ("Point moved in an operation that was not supposed to move.");
      }
    }
  }
}

test_setup*
cdds_drawingarea_test_move_undo_redo_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  //setup->width = g_test_rand_int_range ( 400, 600 );
  //setup->width = setup->width - ( setup->width % 20 );
  //setup->height = g_test_rand_int_range ( 400, 600 );
  //setup->height = setup->height - ( setup->height % 20 );
  
  setup->width = 500;
  setup->height = 500;
  
  return setup;
}

static void
cdds_drawingarea_test_move_undo_redo (CDDSDrawingAreaFixture *fixture, gconstpointer data)
{
  gint64 time;
  CDDSGeoPoint* p;
  CdsAreaSelection* s;
  gboolean perminit = FALSE;
  guint iter;
  gboolean undo;
  
  struct moveforeach* cmp_p_a;
  struct moveforeach* cmp_p_b;
  
  CDDSGeoAction* a;
  
  GPtrArray* beforemove;
  GPtrArray* aftermove;
  
  gint p_x;
  gint p_y;
  gint r_x;
  gint r_y;
  
  gint interv = fixture->area->prop_grid_interval;
  
  for ( gint i = 0; i < TEST_N_INIT_POINTSET; i += 1 )
  {
    
    // accomodate snap to grid, if it is on add points based on the interval, this makes move checking sane
    if ( fixture->area->prop_snap_to_grid )
    {
      r_x = g_test_rand_int_range ( 0, fixture->area->width );
      p_x = r_x - ( r_x % fixture->area->prop_grid_interval);
      r_y = g_test_rand_int_range ( 0, fixture->area->height );
      p_y = r_y - ( r_y % fixture->area->prop_grid_interval);
    }
    else
    {
      p_x = g_test_rand_int_range ( 0, fixture->area->width );
      p_y = g_test_rand_int_range ( 0, fixture->area->height );
    }
    
    p = cds_area_actionmanager_point_new ( fixture->area->mgrs, p_x, p_y );
  }
  
  cds_area_actionmanager_clear_redo ( fixture->area->mgrs->amgr );
  cds_area_actionmanager_clear_undo ( fixture->area->mgrs->amgr );
  
  for ( gint j = 0; j < TEST_N_ACTIONS; j ++ )
  {
    
    perminit = FALSE;
    
    /* this selection is deleted during dispose of action */
    s = cds_area_selection_new ( &fixture->area->width, &fixture->area->height );    
    iter = 1;
    while ( iter <= TEST_N_SELECTED ( fixture->area->mgrs->pmgr->n_points ) && 
            cds_area_selection_len( s ) < TEST_N_SELECTED ( fixture->area->mgrs->pmgr->n_points ) )
    {
      
      if ( !perminit )
      {
	cdds_geometry_test_perm_reset ( fixture->area->mgrs->pmgr->n_points );
	perminit = TRUE;
      }
      
      p = g_ptr_array_index ( fixture->area->mgrs->allpoints, cdds_geometry_test_perm_next() );
      if ( cds_area_pointmanager_lookup_pointp ( fixture->area->mgrs->pmgr, p ) ) 
      {
	cds_area_selection_insert_pointp ( s, p );
      }
      
      iter++;
    
    }

    beforemove = g_ptr_array_new_with_free_func ( cdds_drawingarea_test_moveptrarray_free_foreach );
    aftermove = g_ptr_array_new_with_free_func ( cdds_drawingarea_test_moveptrarray_free_foreach );
    
    g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, beforemove );
    
    cds_area_actionmanager_selected_points_move ( fixture->area->mgrs, s, 
						 (g_test_rand_bit() == 0 ? -interv : interv),
						 (g_test_rand_bit() == 0 ? -interv : interv) );

    g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, aftermove );
    
    cdds_drawingarea_test_points_moved_correctly ( fixture, beforemove, aftermove, TRUE );
    
    g_ptr_array_free ( beforemove, TRUE );
    g_ptr_array_free ( aftermove, TRUE );
        
    TEST_REDRAW_CHECK_WAIT(10)
    
    
    /* undo-redo nothing moved */
    while ( g_test_rand_bit() )
    {
      
      beforemove = g_ptr_array_new_with_free_func ( cdds_drawingarea_test_moveptrarray_free_foreach );
      aftermove = g_ptr_array_new_with_free_func ( cdds_drawingarea_test_moveptrarray_free_foreach );
      
      a = cds_area_actionmanager_undo_peek ( fixture->area->mgrs );
      
      if ( a != NULL )
      {
	s = a->selection;
	g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, beforemove );
      }
      
      a = cds_area_actionmanager_undo ( fixture->area->mgrs );
      if ( a != cds_area_actionmanager_redo_peek ( fixture->area->mgrs ) )
      {
	g_message ("return value from undo does not match return value from redo_peek");
	g_test_fail();
      }
      a = cds_area_actionmanager_redo ( fixture->area->mgrs );
            
      if ( a != NULL )
      {
	if ( s != a->selection ) 
	{ 
	  g_message ("return values from peek_undo and redo do not correspond"); 
	  g_test_fail();
	}
	s = a->selection;
	g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, aftermove );
	
      }
      
      cdds_drawingarea_test_points_moved_correctly ( fixture, beforemove, aftermove, FALSE );
      
      g_ptr_array_free ( beforemove, TRUE );
      g_ptr_array_free ( aftermove, TRUE );
      
      TEST_REDRAW_CHECK_WAIT(10)
      
    }
    
    
    /* undo */
    while ( g_test_rand_bit() )
    {
      
      beforemove = g_ptr_array_new_with_free_func ( cdds_drawingarea_test_moveptrarray_free_foreach );
      aftermove = g_ptr_array_new_with_free_func ( cdds_drawingarea_test_moveptrarray_free_foreach );
      
      a = cds_area_actionmanager_undo_peek ( fixture->area->mgrs );
      
      if ( a != NULL )
      {
	s = a->selection;
	g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, beforemove );
      }
      
      a = cds_area_actionmanager_undo ( fixture->area->mgrs );
            
      if ( a != NULL )
      {
	if ( s != a->selection ) 
	{
	  g_message ("return values from peek_undo and undo do not correspond"); 
	  g_test_fail();
	}
	s = a->selection;
	g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, aftermove );
	
      }
      
      cdds_drawingarea_test_points_moved_correctly ( fixture, beforemove, aftermove, TRUE );
      
      g_ptr_array_free ( beforemove, TRUE );
      g_ptr_array_free ( aftermove, TRUE );
      
      TEST_REDRAW_CHECK_WAIT(10)
      
    }

    /* redo */
    while ( g_test_rand_bit() )
    {
      
      beforemove = g_ptr_array_new_with_free_func ( cdds_drawingarea_test_moveptrarray_free_foreach );
      aftermove = g_ptr_array_new_with_free_func ( cdds_drawingarea_test_moveptrarray_free_foreach );
      
      a = cds_area_actionmanager_redo_peek ( fixture->area->mgrs );
      
      if ( a != NULL )
      {
	s = a->selection;
	g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, beforemove );
      }
      
      a = cds_area_actionmanager_redo ( fixture->area->mgrs );
            
      if ( a != NULL )
      {
	if ( s != a->selection ) 
	{
	  g_message ("return values from peek_redo and redo do not correspond"); 
	  g_test_fail();
	}

	s = a->selection;
	g_hash_table_foreach ( s->hash_points, cdds_drawingarea_test_move_foreach, aftermove );
      }
      
      cdds_drawingarea_test_points_moved_correctly ( fixture, beforemove, aftermove, TRUE );
      
      g_ptr_array_free ( beforemove, TRUE );
      g_ptr_array_free ( aftermove, TRUE );
      
      TEST_REDRAW_CHECK_WAIT(10)

    }

    
  
  }

  
}


test_setup*
cdds_drawingarea_test_n_points_count_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 300, 500 );
  setup->height = g_test_rand_int_range ( 300, 500 );
    
  return setup;
}

static void
cdds_drawingarea_test_n_points_count ( CDDSDrawingAreaFixture* fixture, gconstpointer data )
{

  gint64 time;
  CDDSGeoPoint* p;
  CdsAreaSelection* s;
  
  int testcount;
  
  for ( int i = 0; i < 500; i ++ )
  {
    
    switch ( g_test_rand_int_range ( 0, 3 ) )
    {
      case 0:
    
	s = fixture->area->mgrs->smgr;
	cds_area_selection_clear_selection ( s );
	
	for ( gint i = 0; i < g_test_rand_int_range ( 0, 10 ); i++ )
	{
	  if ( fixture->area->mgrs->allpoints->len > 0 )
	  {
	    
	    p = g_ptr_array_index ( fixture->area->mgrs->allpoints, 
				    g_test_rand_int_range ( 0, fixture->area->mgrs->allpoints->len ) );
	    
	    if ( !cds_area_selection_contains_point ( s, p ) && 
		cds_area_pointmanager_lookup_pointp ( fixture->area->mgrs->pmgr, p ) )
	    {
	      cds_area_selection_insert_pointp ( s, p );
	      gtk_widget_queue_draw ( GTK_WIDGET ( fixture->area ) );
	    }
	  }
	}
	
	testcount -= cds_area_selection_len ( s );
	
	cds_area_actionmanager_selected_points_delete ( fixture->area->mgrs );
	
	TEST_REDRAW_CHECK_WAIT(10)
	break;
	
      default:
	
	cds_area_actionmanager_point_new ( fixture->area->mgrs,
					  g_test_rand_int_range ( 0,  fixture->area->width ), 
					  g_test_rand_int_range ( 0,  fixture->area->height ) );
	testcount++;
	TEST_REDRAW_CHECK_WAIT(10)
    
	break;
    }
    TEST_REDRAW_CHECK_WAIT(10)
    
    if ( testcount != fixture->area->mgrs->pmgr->n_points )
    {
      g_message("n_points does not match add delete operations count.");
      g_test_fail();
    }
    
  }
}




test_setup*
cdds_drawingarea_test_random_action_setup ( CdsAreaWindow** win )
{
  test_setup* setup;
  setup = g_malloc ( sizeof ( test_setup ) );
  setup->win = win;
  setup->width = g_test_rand_int_range ( 50, 600 );
  setup->height = g_test_rand_int_range ( 50, 600 );
    
  return setup;
}

static void
cdds_drawingarea_test_random_action ( CDDSDrawingAreaFixture* fixture, gconstpointer data )
{
  CdsAreaSelection* s;
  CDDSGeoPoint* p;
  gint64 time;
  guint iter;
  gboolean perminit;
  
  
  for ( int i = 0; i < 500; i++ )
  {
    switch ( g_test_rand_int_range ( 1, 9 ) )
    {
      case 1: /* delete point */
	    
	s = fixture->area->mgrs->smgr;
	cds_area_selection_clear_selection ( s );
	
	for ( gint i = 0; i < g_test_rand_int_range ( 0, 10 ); i++ )
	{
	  if ( fixture->area->mgrs->allpoints->len > 0 )
	  {
	    
	    p = g_ptr_array_index ( fixture->area->mgrs->allpoints, 
				    g_test_rand_int_range ( 0, fixture->area->mgrs->allpoints->len ) );
	    
	    if ( !cds_area_selection_contains_point ( s, p ) && 
		cds_area_pointmanager_lookup_pointp ( fixture->area->mgrs->pmgr, p ) )
	    {
	      cds_area_selection_insert_pointp ( s, p );
	      gtk_widget_queue_draw ( GTK_WIDGET ( fixture->area ) );
	    }
	  }
	}
	
	TEST_REDRAW_CHECK_WAIT(10)
	
	cds_area_actionmanager_selected_points_delete ( fixture->area->mgrs );
	
	TEST_REDRAW_CHECK_WAIT(10)
	
	break;

      case 2:
	
	cds_area_actionmanager_undo ( fixture->area->mgrs );
	TEST_REDRAW_CHECK_WAIT(10)
	break;
	
      case 3:
	
	cds_area_actionmanager_redo ( fixture->area->mgrs );
	TEST_REDRAW_CHECK_WAIT(10)
	break;


      case 4: /* move */

	perminit = FALSE;
	
	/* this selection is deleted during dispose of action */
	s = cds_area_selection_new ( &fixture->area->width, &fixture->area->height );    

	iter = 1;

	while ( iter <= TEST_N_SELECTED ( fixture->area->mgrs->pmgr->n_points ) && 
		cds_area_selection_len( s ) < TEST_N_SELECTED ( fixture->area->mgrs->pmgr->n_points ) )
	{
	  
	  if ( !perminit )
	  {
	    cdds_geometry_test_perm_reset ( fixture->area->mgrs->pmgr->n_points );
	    perminit = TRUE;
	  }

	  p = g_ptr_array_index ( fixture->area->mgrs->allpoints, cdds_geometry_test_perm_next() );
	  if ( cds_area_pointmanager_lookup_pointp ( fixture->area->mgrs->pmgr, p ) ) 
	  {
	    cds_area_selection_insert_pointp ( s, p );
	  }
	  iter++;
	}
	      
	cds_area_actionmanager_selected_points_move ( fixture->area->mgrs, s, 
				              g_test_rand_int_range ( -fixture->area->width, fixture->area->width ),
				              g_test_rand_int_range ( -fixture->area->height, fixture->area->height ) );
	    
	TEST_REDRAW_CHECK_WAIT(10)	
	
	
	break;
	
      default:
	
	cds_area_actionmanager_point_new ( fixture->area->mgrs,
					  cdds_geometry_test_random( fixture->area->width ), 
					  cdds_geometry_test_random( fixture->area->height ));
	TEST_REDRAW_CHECK_WAIT(10)
	
	break;

    }
    
    TEST_REDRAW_CHECK_WAIT(10)
      
  }
  
}







int main( int argc, char *argv[] ){
  
  
  
  CdsAreaApp* test_app;
  test_setup* setup;
  
  //setlocale (LC_ALL, "");
  g_setenv ("GSETTINGS_SCHEMA_DIR", ".", FALSE);  
  
  test_app = cds_area_app_new ();
  
  //g_test_init (&argc, &argv, NULL);
  //g_test_bug_base ("http://127.0.0.1");
  
  gtk_test_init ( &argc, &argv, NULL );
  
  //test_app->win
  

  setup = cdds_drawingarea_test_add_five_setup ( &test_app->win );
  g_test_add ("/drawingarea/addfive", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_five,
              cdds_drawingarea_fixture_tear_down);
  
  setup = cdds_drawingarea_test_add_six_setup ( &test_app->win );
  g_test_add ("/drawingarea/addsix", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_six,
              cdds_drawingarea_fixture_tear_down);
  
  setup = cdds_drawingarea_test_add_seven_setup ( &test_app->win );
  g_test_add ("/drawingarea/addseven", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_seven,
              cdds_drawingarea_fixture_tear_down);

  setup = cdds_drawingarea_test_add_maxheight_setup ( &test_app->win );
  g_test_add ("/drawingarea/addmaxheight", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_maxheight,
              cdds_drawingarea_fixture_tear_down);
  
  setup = cdds_drawingarea_test_add_minheight_setup ( &test_app->win );
  g_test_add ("/drawingarea/addminheight", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_minheight,
              cdds_drawingarea_fixture_tear_down);

  setup = cdds_drawingarea_test_add_maxwidth_setup ( &test_app->win );
  g_test_add ("/drawingarea/addmaxwidth", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_maxwidth,
              cdds_drawingarea_fixture_tear_down);

  setup = cdds_drawingarea_test_add_minwidth_setup ( &test_app->win );
  g_test_add ("/drawingarea/addminwidth", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_minwidth,
              cdds_drawingarea_fixture_tear_down);

  setup = cdds_drawingarea_test_add_fillcanvaswiththreepoints_setup ( &test_app->win );
  g_test_add ("/drawingarea/addfillcanvaswiththreepoints", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_fillcanvaswiththreepoints,
              cdds_drawingarea_fixture_tear_down);

  setup = cdds_drawingarea_test_add_setup ( &test_app->win );
  g_test_add ("/drawingarea/add", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add,
              cdds_drawingarea_fixture_tear_down);

  
  
  setup = cdds_drawingarea_test_createdestroy_setup ( &test_app->win );
  g_test_add ("/drawingarea/createdestroy", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up,
	      cds_area_drawingarea_test_createdestroy,
              cdds_drawingarea_fixture_tear_down);

  
  setup = cdds_drawingarea_test_add_undo_redo_setup ( &test_app->win );
  g_test_add ("/drawingarea/add_undo_redo", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up, 
              cdds_drawingarea_test_add_undo_redo,
              cdds_drawingarea_fixture_tear_down);

  setup = cdds_drawingarea_test_delete_setup ( &test_app->win );
  g_test_add ("/drawingarea/delete", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up, 
              cdds_drawingarea_test_delete,
              cdds_drawingarea_fixture_tear_down);
  
  setup = cdds_drawingarea_test_delete_undo_redo_setup ( &test_app->win );
  g_test_add ("/drawingarea/delete_undo_redo", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up, 
              cdds_drawingarea_test_delete_undo_redo,
              cdds_drawingarea_fixture_tear_down);

  setup = cdds_drawingarea_test_move_setup ( &test_app->win );
  g_test_add ("/drawingarea/move", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up, 
              cdds_drawingarea_test_move,
              cdds_drawingarea_fixture_tear_down);


  setup = cdds_drawingarea_test_move_undo_redo_setup ( &test_app->win );
  g_test_add ("/drawingarea/move_undo_redo", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up, 
              cdds_drawingarea_test_move_undo_redo,
              cdds_drawingarea_fixture_tear_down);

  


  setup = cdds_drawingarea_test_random_action_setup ( &test_app->win );
  g_test_add ("/drawingarea/random_action", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up, 
              cdds_drawingarea_test_random_action,
              cdds_drawingarea_fixture_tear_down);



  setup = cdds_drawingarea_test_n_points_count_setup ( &test_app->win );
  g_test_add ("/drawingarea/npointscount", CDDSDrawingAreaFixture, 
	      setup,
              cdds_drawingarea_fixture_set_up, 
              cdds_drawingarea_test_n_points_count,
              cdds_drawingarea_fixture_tear_down);
  
  
  
  
  /*
  g_test_add ("/drawingarea/addborderareas", CDDSDrawingAreaFixture, 
	      &test_app->win,
              cdds_drawingarea_fixture_set_up,
	      cdds_drawingarea_test_add_borderareas,
              cdds_drawingarea_fixture_tear_down);
  
*/

  //return g_application_run (G_APPLICATION (cds_area_app_new ()), argc, argv);
  
  
  
  return g_application_run (G_APPLICATION (test_app), argc, argv);
  
  
}




