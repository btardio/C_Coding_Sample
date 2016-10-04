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
#include "cdds_geometry_actionmanager.h"
#include "cdds_geometry_action.h"
#include "cdds_geometry_selection.h"
#include "cdds_geometry_pointmanager.h"
#include "cdds_geometry_point.h"
#include "../cdds_drawingarea.h"
#include "../cdds_window.h"
#include "cdds_geometry_line.h"
#include "cdds_geometry_linemanager.h"


G_DEFINE_TYPE(CdsAreaActionManager, cds_area_actionmanager, G_TYPE_OBJECT)


void
cds_area_actionmanager_undo_add_action ( CdsAreaActionManager* mgr, CDDSGeoAction* a )
{
  
  g_queue_push_head ( mgr->undo, a ); 
  cdds_geoaction_increase_ref_count ( a ); /* the action is referenced by the undo queue */
  
  if ( g_queue_get_length( mgr->undo ) >= mgr->undosize )
  {
    a = g_queue_pop_tail ( mgr->undo ); 
    /* removing an action from undo finalizes geoaction and dereferences points */
    cdds_geoaction_decrease_ref_count ( a ); /* unref the action's undo queue reference */
    cdds_geoaction_decrease_ref_count ( a ); /* unref the action's initial reference, finalize removes references to points */

  }
}


void
cds_area_actionmanager_redo_add_action ( CdsAreaActionManager* mgr, CDDSGeoAction* a )
{
  
  g_queue_push_head ( mgr->redo, a ); 
  cdds_geoaction_increase_ref_count ( a ); /* the action is referenced by the redo queue */
  
  if ( g_queue_get_length( mgr->redo ) >= mgr->redosize )
  {
    a = g_queue_pop_tail ( mgr->redo ); 
    /* removing an action from redo finalizes geoaction and dereferences points */
    cdds_geoaction_decrease_ref_count ( a ); /* unref the action's undo queue reference */
    cdds_geoaction_decrease_ref_count ( a ); /* unref the action's initial reference, finalize removes references to points */
    
  }
}


CDDSGeoAction*
cds_area_actionmanager_undo_peek ( managers* mgrs )
{
  CDDSGeoAction* a;
  a = g_queue_peek_head( mgrs->amgr->undo );
  return a;
}

CDDSGeoAction*
cds_area_actionmanager_undo ( managers* mgrs )
{

  CDDSGeoAction* a; 
  
  /* a = g_queue_pop_head ( mgr->undo ); */
  a = g_queue_pop_head ( mgrs->amgr->undo ); 
  
  if ( a )
  { 
    /* cds_area_actionmanager_redo_add_action ( mgr, a ); */
    cds_area_actionmanager_redo_add_action ( mgrs->amgr, a ); 
    
    cdds_geoaction_decrease_ref_count ( a ); /* removes the action's reference to the undo queue */
    
    /* cdds_geoaction_inverse_action ( a, points, selection ); */
    cdds_geoaction_inverse_action ( mgrs, a );
    
    /* order last */
    /* if the action re initializes or de initializes points, rebuild ref count for initial init reference */
    switch ( a->type ){
      case ACTION_POINT_DELETE:
	//cds_area_selection_increase_ref_count_points ( a->selection ); /* undo a delete re references init reference */
	break;
      case ACTION_POINT_ADD:
	//cds_area_selection_decrease_ref_count_points ( a->selection ); /* undo a add unreferences init reference */
	break;
    }
  }
  
  return a;
  
}

CDDSGeoAction*
cds_area_actionmanager_redo_peek ( managers* mgrs )
{
  CDDSGeoAction* a;
  a = g_queue_peek_head( mgrs->amgr->redo );
  return a;
}


CDDSGeoAction*
cds_area_actionmanager_redo ( managers* mgrs )
{
  
  CDDSGeoAction* a;
    
  /* a = g_queue_pop_head ( mgr->redo ); */
  a = g_queue_pop_head ( mgrs->amgr->redo );
  
  if ( a )
  {
    /* cds_area_actionmanager_undo_add_action ( mgr, a ); */
    cds_area_actionmanager_undo_add_action ( mgrs->amgr, a );
    
    cdds_geoaction_decrease_ref_count ( a ); /* removes the action's reference to the redo queue */
    
    /* cdds_geoaction_action ( a, points, selection ); */
    cdds_geoaction_action ( mgrs, a );
    
        
    /* order last */
    /* if the action re initializes or de initializes points, rebuild ref count for initial init reference */
    switch ( a->type ){
      case ACTION_POINT_ADD:      
	//cds_area_selection_increase_ref_count_points ( a->selection ); /* redo an add re references init reference */
	break;
      
      case ACTION_POINT_DELETE:
	//cds_area_selection_decrease_ref_count_points ( a->selection ); /* redo'ing a delete unrefereces initial init reference */
	break;
    }
  }
  
  return a;
  
}




void
cds_area_actionmanager_queue_action_free_foreach ( gpointer item, gpointer data )
{
  
  CDDSGeoAction* a;
  a = item;
  
  cdds_geoaction_decrease_ref_count ( a ); /* unref the action's redo queue reference */
  cdds_geoaction_decrease_ref_count ( a ); /* unref the action's initial reference, finalize removes references to points */
  
}


void
cds_area_actionmanager_clear_redo ( CdsAreaActionManager* mgr )
{
  g_queue_foreach ( mgr->redo, cds_area_actionmanager_queue_action_free_foreach, NULL );
  g_queue_clear ( mgr->redo );  
}

void
cds_area_actionmanager_clear_undo ( CdsAreaActionManager* mgr )
{
  g_queue_foreach ( mgr->undo, cds_area_actionmanager_queue_action_free_foreach, NULL );
  g_queue_clear ( mgr->undo );  
}




void
cds_area_actionmanager_selected_points_move_start ( managers* mgrs, CdsAreaSelection* actionableselection, gint x, gint y)
{
  x = cds_area_drawing_area_mouse_to_canvas_i ( mgrs->area, x );
  y = cds_area_drawing_area_mouse_to_canvas_i ( mgrs->area, y );
  
  /* actioninprogress never reached endaction */
  if ( mgrs->amgr->actioninprogress != NULL ){
    cdds_geoaction_destroy ( mgrs->amgr->actioninprogress );
  }
  
  mgrs->amgr->actioninprogress = cdds_geoaction_new ( mgrs, ACTION_POINT_MOVE, actionableselection );
  mgrs->amgr->actioninprogress->inprogress = TRUE;
  
  /* dereference action points until end action, right clicking brought up pop up menu that kept referencing */
  /* until the move end is reached, ie the action is complete, do not increment refcount */
  cds_area_selection_decrease_ref_count_points ( actionableselection );
  
  mgrs->amgr->actioninprogress->transform->x = mgrs->amgr->actioninprogress->transform_progress_previous->x = 0;
  mgrs->amgr->actioninprogress->transform->y = mgrs->amgr->actioninprogress->transform_progress_previous->y = 0;
  
  mgrs->amgr->actioninprogress->transform_origin->x = x;
  mgrs->amgr->actioninprogress->transform_origin->y = y;
  
}

void
cds_area_actionmanager_selected_points_move_progress ( managers* mgrs, CdsAreaSelection* actionableselection, gint x, gint y )
						      
{
  
  x = cds_area_drawing_area_mouse_to_canvas_i ( mgrs->area, x );
  y = cds_area_drawing_area_mouse_to_canvas_i ( mgrs->area, y );
  
  mgrs->amgr->actioninprogress->transform->x = x - mgrs->amgr->actioninprogress->transform_origin->x;
  mgrs->amgr->actioninprogress->transform->y = y - mgrs->amgr->actioninprogress->transform_origin->y;
  
  //g_message("transform->x: %d transform->y: %d", 
//	    mgrs->amgr->actioninprogress->transform->x, 
//	    mgrs->amgr->actioninprogress->transform->y );
  
  cdds_geoaction_action ( mgrs, mgrs->amgr->actioninprogress );
  
  mgrs->amgr->actioninprogress->transform_progress_previous->x = mgrs->amgr->actioninprogress->transform->x;
  mgrs->amgr->actioninprogress->transform_progress_previous->y = mgrs->amgr->actioninprogress->transform->y;
}

void
cds_area_actionmanager_selected_points_move_end ( managers* mgrs, CdsAreaSelection* actionableselection, gint x, gint y )
{
  
  x = cds_area_drawing_area_mouse_to_canvas_i ( mgrs->area, x );
  y = cds_area_drawing_area_mouse_to_canvas_i ( mgrs->area, y );
  
  //cds_area_actionmanager_clear_redo ( mgrs->amgr );
      
  mgrs->amgr->actioninprogress->transform->x = x - mgrs->amgr->actioninprogress->transform_origin->x;
  mgrs->amgr->actioninprogress->transform->y = y - mgrs->amgr->actioninprogress->transform_origin->y;

  //g_message("transform->x: %d transform->y: %d", 
//	  mgrs->amgr->actioninprogress->transform->x, 
//	  mgrs->amgr->actioninprogress->transform->y );
  
  /* action selection points are dereferenced until move end, action finished, increment them as part of actionselection*/
  cds_area_selection_increase_ref_count_points ( actionableselection );
  
  cdds_geoaction_action ( mgrs, mgrs->amgr->actioninprogress );
    
  mgrs->amgr->actioninprogress->inprogress = FALSE;
  
  if ( mgrs->amgr->actioninprogress->transform->x != 0 || mgrs->amgr->actioninprogress->transform->y != 0 )
  {
    cds_area_actionmanager_clear_redo ( mgrs->amgr );
    cds_area_actionmanager_undo_add_action ( mgrs->amgr, mgrs->amgr->actioninprogress ); /* add reference to undo queue */
  }
  else /* if the action transform is 0, inclusive of the user clicking on the point without a drag, destroy the action */
  {
    g_message("action destroyed, no points moved");
    cdds_geoaction_destroy ( mgrs->amgr->actioninprogress );
  }
  
  mgrs->amgr->actioninprogress = NULL;
  
  //cds_area_test_actions ( window->area );
  //cds_area_test_points ( window->area );
  
  
}



void
cds_area_actionmanager_add_line_start ( managers* mgrs, gint x, gint y )
{
  /* assume action is complete, disable undo/redo incase user presses ctrl-z during action */
  /* todo: disable undo/redo incase user presses ctrl-z */
  
  gint canx;
  gint cany;
  
  cds_area_actionmanager_clear_redo ( mgrs->amgr );
  
  canx = cds_area_drawing_area_mouse_to_canvas_d ( mgrs->area, x );
  cany = cds_area_drawing_area_mouse_to_canvas_d ( mgrs->area, y );

  mgrs->amgr->actionstartpoint0 = cdds_geopoint_new ( mgrs, canx, cany );
  mgrs->amgr->actionstartpoint1 = cdds_geopoint_new ( mgrs, canx, cany );
  mgrs->amgr->actionstartline = cds_area_line_new ( mgrs, 
						    mgrs->amgr->actionstartpoint0, 
						    mgrs->amgr->actionstartpoint1 );
  
  mgrs->amgr->actionstartselection = cds_area_selection_new ( &mgrs->area->width, &mgrs->area->height );
  
  cds_area_selection_insert_pointp ( mgrs->amgr->actionstartselection, mgrs->amgr->actionstartpoint0 );
  cds_area_selection_insert_pointp ( mgrs->amgr->actionstartselection, mgrs->amgr->actionstartpoint1 );      
  cds_area_selection_insert_linel ( mgrs->amgr->actionstartselection, mgrs->amgr->actionstartline );
  
  cds_area_pointmanager_insert_pointp ( mgrs, mgrs->amgr->actionstartpoint0 );
  cds_area_pointmanager_insert_pointp ( mgrs, mgrs->amgr->actionstartpoint1 );
  cds_area_linemanager_insert_linel ( mgrs, mgrs->amgr->actionstartline );
  
  mgrs->amgr->actioninprogress = cdds_geoaction_new ( mgrs, ACTION_LINE_ADD, mgrs->amgr->actionstartselection ); 

  cds_area_actionmanager_undo_add_action ( mgrs->amgr, mgrs->amgr->actioninprogress ); /* add reference to undo queue */
  
  /* remove init reference */
  cdds_geopoint_unref ( mgrs->amgr->actionstartpoint0 );
  cdds_geopoint_unref ( mgrs->amgr->actionstartpoint1 );
  cds_area_line_unref ( mgrs->amgr->actionstartline );
  
}

void
cds_area_actionmanager_add_line_progress ( managers* mgrs, gint x, gint y )
{
  gint canx;
  gint cany;

  //g_queue_pop_head ( mgrs->amgr->undo ); 
  
  canx = cds_area_drawing_area_mouse_to_canvas_d ( mgrs->area, x );
  cany = cds_area_drawing_area_mouse_to_canvas_d ( mgrs->area, y );

  /* reinsert */
  cdds_geopoint_ref ( mgrs->amgr->actionstartpoint0 );
  cdds_geopoint_ref ( mgrs->amgr->actionstartpoint1 );
  cds_area_line_ref ( mgrs->amgr->actionstartline );
  cds_area_pointmanager_delete_pointp ( mgrs->pmgr, mgrs->amgr->actionstartpoint0 );
  cds_area_pointmanager_delete_pointp ( mgrs->pmgr, mgrs->amgr->actionstartpoint1 );
  cds_area_linemanager_delete_linel ( mgrs->lmgr, mgrs->amgr->actionstartline );
  
  g_object_set ( mgrs->amgr->actionstartline, "point2-x", canx, NULL );
  g_object_set ( mgrs->amgr->actionstartline, "point2-y", cany, NULL );
  
  cds_area_pointmanager_insert_pointp ( mgrs, mgrs->amgr->actionstartpoint0 );
  cds_area_pointmanager_insert_pointp ( mgrs, mgrs->amgr->actionstartpoint1 );
  cds_area_linemanager_insert_linel ( mgrs, mgrs->amgr->actionstartline );
  cdds_geopoint_unref ( mgrs->amgr->actionstartpoint0 );
  cdds_geopoint_unref ( mgrs->amgr->actionstartpoint1 );
  cds_area_line_unref ( mgrs->amgr->actionstartline );
  
}


void
cds_area_actionmanager_add_line_end ( managers* mgrs, gint x, gint y )
{
  cds_area_actionmanager_add_line_progress ( mgrs, x, y );
}

void
cds_area_actionmanager_startaction ( managers* mgrs, gdouble x, gdouble y, gint toggle )
{
  CDDSGeoPoint* p;


  mgrs->amgr->button_pressed = TRUE;
  
  mgrs->amgr->button_press_start_x = lrint ( x );
  mgrs->amgr->button_press_start_y = lrint ( y );
  
    
  
  switch ( toggle ) {
    case TOGGLE_SELECT:     
	
      mgrs->amgr->actionstartpoint = cds_area_selection_lookup_point ( mgrs->pmgr, 
								       mgrs->amgr->button_press_start_x, 
								       mgrs->amgr->button_press_start_y );
      
      

      if ( mgrs->amgr->actionstartpoint )
      {
	
	mgrs->amgr->actionstartselection = cds_area_selection_selection_from_selection ( mgrs->smgr ); 
	
	cds_area_actionmanager_selected_points_move_start ( mgrs, 
							   mgrs->amgr->actionstartselection, 
							   mgrs->amgr->button_press_start_x, 
							   mgrs->amgr->button_press_start_y );
	
      }
      
      
      break;
      
      
    case TOGGLE_LINE:

      cds_area_actionmanager_add_line_start ( mgrs, x, y );
      
      break;
  
  }
      
  
}


void
cds_area_actionmanager_progressaction ( managers* mgrs, gdouble x, gdouble y, gint toggle )
{
  CDDSGeoPoint* p;
  gint canx;
  gint cany;
  
  gint motion_x = (gint) round ( x );
  gint motion_y = (gint) round ( y );
  
  switch ( toggle ) {
    case TOGGLE_SELECT:
      
      if ( mgrs->amgr->button_pressed && mgrs->amgr->actionstartpoint )
      {

	/* if coordinate position of mouse pointer is inside canvas */
	if ( motion_x >= 0 && motion_x <= mgrs->area->width && motion_y >= 0 && motion_y <= mgrs->area->width )
	{
	  cds_area_actionmanager_selected_points_move_progress ( mgrs, 
								mgrs->amgr->actionstartselection, 
								motion_x, motion_y );
	}
      }
	
      break;
      
    case TOGGLE_LINE:
      
      cds_area_actionmanager_add_line_progress ( mgrs, x, y );
      
      break;

  }
  
}


void
cds_area_actionmanager_endaction ( managers* mgrs, gdouble x, gdouble y, gint toggle )
{
  
  CDDSGeoPoint* p;
  
  gint end_x = cds_area_drawing_area_mouse_to_canvas_d ( mgrs->area, x ); /* (gint) round ( x );*/
  gint end_y = cds_area_drawing_area_mouse_to_canvas_d ( mgrs->area, y ); /* (gint) round ( y ); */
  
  mgrs->amgr->button_pressed = FALSE;
  
  
  
  switch ( toggle ) {
    
    case TOGGLE_POINT:
      
      p = cds_area_actionmanager_point_new ( mgrs, end_x, end_y );
      
      break;
      
    case TOGGLE_SELECT:

      if ( mgrs->amgr->actionstartpoint )
      {
	cds_area_actionmanager_selected_points_move_end ( mgrs, mgrs->amgr->actionstartselection, end_x, end_y );
	mgrs->amgr->actionstartpoint = NULL; /* this point is the point the user clicked initiating the move */
      }
      
      break;
      
    case TOGGLE_LINE:
      
      
      cds_area_actionmanager_add_line_end ( mgrs, x, y );
      
      mgrs->amgr->actionstartselection = NULL;
      mgrs->amgr->actionstartpoint0 = NULL;
      mgrs->amgr->actionstartpoint1 = NULL;
      mgrs->amgr->actionstartline = NULL;
      mgrs->amgr->actioninprogress = NULL;
      
      break;
      
    
  }
  
  // after a successful move the selection will become part of action and free'd when the action is free'd
  mgrs->amgr->actionstartselection = NULL;
}


CdsAreaLine*
cds_area_actionmanager_line_new ( managers* mgrs, CDDSGeoPoint* p1, CDDSGeoPoint* p2 )
{
  
  CdsAreaSelection* new_selection;
  //ActionManagerAction* action;
  CDDSGeoAction* a;
  CdsAreaLine* l;
  
  cds_area_actionmanager_clear_redo ( mgrs->amgr );

  //cds_area_test_actions ( window->area );
  //cds_area_test_points ( window->area );
  
  l = cds_area_line_new ( mgrs, p1, p2 ); /* add initial reference */
  
  new_selection = cds_area_selection_new ( mgrs->smgr->width, mgrs->smgr->height );
  
  cds_area_selection_insert_linel ( new_selection, l ); /* add reference to action->selection */
  
  a = cdds_geoaction_new ( mgrs, ACTION_LINE_ADD, new_selection ); 

  cdds_geoaction_action ( mgrs, a ); /* add reference to geopointmanager */
  
  cds_area_actionmanager_undo_add_action ( mgrs->amgr, a ); /* add reference to undo queue */
  
  cds_area_line_unref ( l ); // remove the init reference
  
  //cds_area_test_actions ( window->area );
  //cds_area_test_points ( window->area );
  
  return l;

  
  
  
}


/*
 * Actions Order:
 * 1. clear redo
 * 2. get/prep selection
 * 3. create action
 * 4. add action to undo queue
 * 5. call action
 * 6. handle init references depending on action
 */
CDDSGeoPoint
*cds_area_actionmanager_point_new ( managers* mgrs, gint x, gint y )
{
  CdsAreaSelection* new_selection;
  CDDSGeoPoint* p;
  //ActionManagerAction* action;
  CDDSGeoAction* a;
  
  cds_area_actionmanager_clear_redo ( mgrs->amgr );

  //cds_area_test_actions ( window->area );
  //cds_area_test_points ( window->area );
  
  p = cdds_geopoint_new ( mgrs, x, y ); /* add initial reference */
  
  new_selection = cds_area_selection_new ( mgrs->smgr->width, mgrs->smgr->height );
  
  cds_area_selection_insert_pointp ( new_selection, p ); /* add reference to action->selection */
  
  a = cdds_geoaction_new ( mgrs, ACTION_POINT_ADD, new_selection ); 

  cdds_geoaction_action ( mgrs, a ); /* add reference to geopointmanager */
  
  cds_area_actionmanager_undo_add_action ( mgrs->amgr, a ); /* add reference to undo queue */
  
  cdds_geopoint_unref ( p ); // remove the init reference
  
  //cds_area_test_actions ( window->area );
  //cds_area_test_points ( window->area );
  
  return p;
}

CDDSGeoPoint
*cds_area_actionmanager_point_new_xy ( managers* mgrs, xycoord xy )
{
  return cds_area_actionmanager_point_new ( mgrs, xy.x, xy.y );
}

/*
 * Actions Order:
 * 1. clear redo
 * 2. get/prep selection
 * 3. create action
 * 4. add action to undo queue
 * 5. call action
 * 6. handle init references depending on action
 */
void
cds_area_actionmanager_selected_points_delete ( managers* mgrs )
{
  CdsAreaSelection* new_selection;
  CDDSGeoAction* a;
  
  cds_area_actionmanager_clear_redo ( mgrs->amgr );
  
  //cds_area_test_actions ( window->area );
  //cds_area_test_points ( window->area );
  
  new_selection = cds_area_selection_selection_from_selection ( mgrs->smgr ); 
  
  a = cdds_geoaction_new ( mgrs, ACTION_POINT_DELETE, new_selection ); /* increases refcount of points */
  
  cds_area_actionmanager_undo_add_action ( mgrs->amgr, a ); /* increases refcount of points */
                                                          
  cdds_geoaction_action ( mgrs, a ); /* remove action reference, possibly finalizing point */
				     /* it is required that action comes last b/c delete removes all refs */
  
  /* points following this operation are left with a reference from undo and a reference from action */
  
  //cds_area_test_actions ( window->area );
  //cds_area_test_points ( window->area );
}


/*
 * Actions Order:
 * 1. clear redo
 * 2. get/prep selection
 * 3. create action
 * 4. add action to undo queue
 * 5. call action
 * 6. handle init references depending on action
 */
void
cds_area_actionmanager_selected_points_move ( managers* mgrs, CdsAreaSelection* selection, gint xchange, gint ychange )
{
  //CdsAreaSelection* new_selection;
  CDDSGeoAction* a;
  
  if ( xchange == 0 && ychange == 0 )
  {
    g_message ( "Call to move a point with a 0 transform." );
    return;
  }
  
  cds_area_actionmanager_clear_redo ( mgrs->amgr );
    
  //new_selection = cds_area_selection_selection_from_selection ( mgrs->smgr ); 
  
  a = cdds_geoaction_new ( mgrs, ACTION_POINT_MOVE, selection ); /* increases refcount of points */
  
  a->transform->x = xchange;
  a->transform->y = ychange;
  
  cds_area_actionmanager_undo_add_action ( mgrs->amgr, a ); /* increases refcount of points */
  						   
  cdds_geoaction_action ( mgrs, a ); 
				     /* it is required that action comes last b/c delete removes all refs */
    
  //cds_area_test_actions ( window->area );
  //cds_area_test_points ( window->area );
}

void
cds_area_actionmanager_set_property (GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
  CdsAreaActionManager *self = CDS_AREA_ACTIONMANAGER (object);

  switch (property_id)
  {
    case CDS_AREA_ACTIONMANAGER_PROP_UNDOSIZE:
      self->undosize = g_value_get_int (value);
      break;

    case CDS_AREA_ACTIONMANAGER_PROP_REDOSIZE:
      self->redosize = g_value_get_int (value);
      break;
    
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
cds_area_actionmanager_get_property (GObject    *object,
                                guint       property_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  CdsAreaActionManager *self = CDS_AREA_ACTIONMANAGER (object);

  switch (property_id)
  {
    case CDS_AREA_ACTIONMANAGER_PROP_UNDOSIZE:
      g_value_set_int (value, self->undosize);
      break;
    case CDS_AREA_ACTIONMANAGER_PROP_REDOSIZE:
      g_value_set_int (value, self->redosize);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}



static void
cds_area_actionmanager_init (CdsAreaActionManager *mgr)
{
  GValue gval_int = G_VALUE_INIT;
  GValue gval_float = G_VALUE_INIT;
  
  g_value_init ( &gval_int, G_TYPE_INT );
  g_value_init ( &gval_float, G_TYPE_FLOAT );  
  
  g_value_unset ( &gval_float );
  g_value_unset ( &gval_int );
  
  mgr->undo = g_queue_new ( );
  mgr->redo = g_queue_new ( );
}


static void
cds_area_actionmanager_class_init (CdsAreaActionManagerClass *klass)
{

  GObjectClass   *gobject_class;
  
  gobject_class = G_OBJECT_CLASS (klass);
  
  //cds_area_actionmanager_parent_class = g_type_class_peek_parent (klass);
  
  gobject_class->set_property = cds_area_actionmanager_set_property;
  gobject_class->get_property = cds_area_actionmanager_get_property;

  cds_area_actionmanager_properties[CDS_AREA_ACTIONMANAGER_PROP_UNDOSIZE] = g_param_spec_int ("undosize", "undosize", 
										    "Undo queue size.",
										    0, G_MAXINT, 0, G_PARAM_READWRITE);

  cds_area_actionmanager_properties[CDS_AREA_ACTIONMANAGER_PROP_REDOSIZE] = g_param_spec_int ("redosize", "redosize", 
										    "Redo queue size.",
										    0, G_MAXINT, 0, G_PARAM_READWRITE);  
  
  g_object_class_install_properties (gobject_class,
                                     CDS_AREA_ACTIONMANAGER_N_PROPERTIES,
                                     cds_area_actionmanager_properties);  
  
  
  
  
}



CdsAreaActionManager
*cds_area_actionmanager_new ( void )
{
  
  CdsAreaActionManager* mgr;
  
  mgr = g_object_new (CDS_AREA_TYPE_ACTIONMANAGER, "undosize", 5, "redosize", 5, NULL);
    
  return mgr;
}


void
cds_area_actionmanager_destroy (CdsAreaActionManager* mgr)
{
  cds_area_actionmanager_clear_undo ( mgr );
  cds_area_actionmanager_clear_redo ( mgr );
  
  g_queue_free ( mgr->redo );
  g_queue_free ( mgr->undo );
  
  g_object_unref ( mgr );
}



