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

#ifndef __CDDS_GEOACTIONMANAGER_H
#define __CDDS_GEOACTIONMANAGER_H

#include "../cdds.h"

G_BEGIN_DECLS

#define CDS_AREA_TYPE_ACTIONMANAGER (cds_area_actionmanager_get_type ())

G_DECLARE_FINAL_TYPE(CdsAreaActionManager, cds_area_actionmanager, CDS_AREA, ACTIONMANAGER, GObject)


/* func declares */
GType                       cds_area_actionmanager_get_type         ( void );
CdsAreaActionManager       *cds_area_actionmanager_new              ( void );
void                        cds_area_actionmanager_destroy          ( CdsAreaActionManager* );

void                        cds_area_actionmanager_startaction      ( managers* mgrs, gdouble x, gdouble y, gint toggle );

void                        cds_area_actionmanager_endaction        ( managers* mgrs, gdouble x, gdouble y, gint toggle );

void			    cds_area_actionmanager_progressaction   ( managers* mgrs, gdouble x, gdouble y, gint toggle );


CDDSGeoAction		   *cds_area_actionmanager_undo             ( managers* mgrs );

CDDSGeoAction              *cds_area_actionmanager_undo_peek        ( managers* mgrs );

void                        cds_area_actionmanager_clear_undo       ( CdsAreaActionManager* mgr );


CDDSGeoAction		   *cds_area_actionmanager_redo             ( managers* mgrs );

CDDSGeoAction              *cds_area_actionmanager_redo_peek        ( managers* mgrs );

void                        cds_area_actionmanager_clear_redo       ( CdsAreaActionManager* mgr );


CDDSGeoPoint               *cds_area_actionmanager_point_new        ( managers* mgrs, gint x, gint y );

CDDSGeoPoint               *cds_area_actionmanager_point_new_xy     ( managers* mgrs, xycoord xy );

void                        cds_area_actionmanager_selected_points_delete ( managers* mgrs );

void                        cds_area_actionmanager_selected_points_move   ( managers* mgrs, CdsAreaSelection* selection, 
									    gint xchange, gint ychange );

G_END_DECLS

enum
{
  CDS_AREA_ACTIONMANAGER_PROP_UNDOSIZE = 1,
  CDS_AREA_ACTIONMANAGER_PROP_REDOSIZE,
  CDS_AREA_ACTIONMANAGER_N_PROPERTIES
};

static GParamSpec *cds_area_actionmanager_properties[CDS_AREA_ACTIONMANAGER_N_PROPERTIES] = { NULL, };



/* variable declares */
struct _CdsAreaActionManager
{
  GObject parent_instance;
  
  GQueue *undo;
  GQueue *redo;
  
  gint undosize;
  gint redosize;
    
  gint button_press_start_x;
  gint button_press_start_y;
  gboolean button_pressed;
  
  CDDSGeoPoint* actionstartpoint;
  CDDSGeoPoint* actionstartpoint0;
  CDDSGeoPoint* actionstartpoint1;
  CdsAreaLine* actionstartline;
  CdsAreaSelection *actionstartselection;
  CDDSGeoAction* actioninprogress;
  
};

struct _CdsAreaActionManagerClass
{
  GObjectClass parent_class;
};


enum {
  ACTION_POINT_ADD,
  ACTION_POINT_DELETE,
  ACTION_POINT_MOVE,
  ACTION_LINE_ADD,
  ACTION_LINE_DELETE,
  ACTION_LINE_MOVE
};


typedef struct _ActionManagerAction ActionManagerAction;


#endif /*__CDDS_GEOACTIONMANAGER_H */





