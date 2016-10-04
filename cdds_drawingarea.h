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

#ifndef __CDDS_DRAWINGAREA_H
#define __CDDS_DRAWINGAREA_H

#include "cdds.h"

G_BEGIN_DECLS

#define CDS_AREA_TYPE_DRAWINGAREA (cds_area_drawingarea_get_type ())

G_DECLARE_FINAL_TYPE(CdsAreaDrawingArea, cds_area_drawingarea, CDS_AREA, DRAWINGAREA, GtkWindow)

CdsAreaDrawingArea         *cds_area_drawingarea_new                 ( gint, gint );

void                        cds_area_drawingarea_new_points_treeview ( CdsAreaWindow* win, CdsAreaDrawingArea* area );
void                        cds_area_drawingarea_new_lines_treeview  ( CdsAreaWindow* win, CdsAreaDrawingArea* area );

GType                       cds_area_drawingarea_get_type            (void);

void                        cds_area_drawingarea_destroy             (CdsAreaDrawingArea* area);

void                        toggle_select_callback                   (GtkToggleButton *, gpointer);
void                        toggle_point_callback                    (GtkToggleButton *, gpointer);
void                        toggle_line_callback                     (GtkToggleButton *, gpointer);
void                        toggle_box_callback                      (GtkToggleButton *, gpointer);

void                        cds_area_drawing_area_key_press_event    ( CdsAreaDrawingArea* widget, GdkEventKey *event );
void                        cds_area_drawing_area_key_release_event  ( CdsAreaDrawingArea* widget, GdkEventKey *event );

void                        cds_area_geopointmanager_delete_pointp   ( CdsAreaPointManager* mgr, CDDSGeoPoint* p );
void                        cds_area_geopointmanager_delete_pointxy  ( CdsAreaPointManager* mgr, gint x, gint y );

void                        menuitem_undo_callback                   (GtkMenuItem *menuitem, gpointer data);
void                        menuitem_redo_callback                   (GtkMenuItem *menuitem, gpointer data);

#ifdef CDDS_TEST
void                        cds_area_geometry_test                   ( CdsAreaDrawingArea* area );
void                        menuitem_test_callback               (GtkMenuItem *menuitem, gpointer data);
#endif

GtkMenu*                    toggle_selection_nohover_right_click_menu_create       ( CdsAreaDrawingArea* drawingarea );
GtkMenu*                    toggle_selection_hover_point_right_click_menu_create   ( CdsAreaDrawingArea* drawingarea );
void                        cds_area_drawingarea_right_click_menus_dispose             ( CdsAreaDrawingArea* drawingarea );

xycoord                     cds_area_drawing_area_mouse_to_canvas_xy_d          ( CdsAreaDrawingArea* area, 
										  gdouble x, gdouble y );

xycoord                     cds_area_drawing_area_mouse_to_canvas_xy_i          ( CdsAreaDrawingArea* area, 
										  gint x, gint y );

gint                        cds_area_drawing_area_mouse_to_canvas_d             ( CdsAreaDrawingArea* area, gdouble xory );
gint                        cds_area_drawing_area_mouse_to_canvas_i             ( CdsAreaDrawingArea* area, gint xory );

void                        cds_area_drawingarea_dispose                       ( GObject* obj );
void                        cds_area_drawingarea_finalize                      ( GObject* obj );


G_END_DECLS

/* variable declares */
struct _CdsAreaDrawingArea
{
  GtkWindow parent_instance;
  //GtkApplicationWindow parent;
  
  gint width;
  gint height;
  
  
  gint toggle;
  gboolean t_select;
  gboolean t_point;
  gboolean t_line;
  gboolean t_box;
  
  gdouble last_button_press_x;
  gdouble last_button_press_y;
  
  gboolean key_lshift;
  gboolean key_lctrl;
  
  GtkMenu* toggle_selection_nohover_right_click_menu;
  GtkMenu* toggle_selection_hover_point_right_click_menu;
  GArray* right_click_menu_handlers;
  GPtrArray* right_click_menus;
  gint n_right_click_menu_handlers;
  
  managers* mgrs;
  
  gboolean prop_snap_to_grid;
  gint prop_grid_interval;
  
  GtkListStore* liststore_points;
  GtkTreeView* treeview_points;
  GtkCellRenderer *renderer_points;
  GtkCellRenderer *renderer_lines;
  
  GtkListStore* liststore_lines;
  GtkTreeView* treeview_lines;
  
  GtkTreeViewColumn *column_x;
  GtkTreeViewColumn *column_y;
#ifdef CDDS_DEBUG
  GtkTreeViewColumn *column_refcount;
#endif
  
  
  
  /*
  CdsAreaPointManager* pointsmgr;
  
  CdsAreaSelection* selection;
  
  CdsAreaActionManager* actions;
  */
};


struct _CdsAreaDrawingAreaClass
{
  GtkWindowClass parent_class_instance;
  //GtkApplicationWindowClass parent_class;
  
};


struct _managers{ 
  CdsAreaDrawingArea* area; 
  CdsAreaActionManager* amgr; 
  CdsAreaSelection* smgr; 
  CdsAreaPointManager* pmgr;
  CdsAreaLineManager* lmgr;
#ifdef CDDS_DEBUG
  GPtrArray* allpoints;
  GPtrArray* allactions;
  GPtrArray* alllines;
#endif
};

struct _xycoord{ gint x; gint y; };

struct _xycoordwmgrs{ struct _xycoord xy; struct _managers* mgrs; };

struct _actionwmgrs{ CDDSGeoAction* a; struct _managers* mgrs; };



enum
{
  CDS_AREA_DRAWINGAREA_PROP_WIDTH = 1,
  CDS_AREA_DRAWINGAREA_PROP_HEIGHT,
  CDS_AREA_DRAWINGAREA_PROP_SNAP_TO_GRID,
  CDS_AREA_DRAWINGAREA_PROP_GRID_INTERVAL,
  CDS_AREA_DRAWINGAREA_N_PROPERTIES
};

static GParamSpec *cds_area_drawingarea_properties[CDS_AREA_DRAWINGAREA_N_PROPERTIES] = { NULL, };




enum{
  TOGGLE_SELECT,
  TOGGLE_POINT,
  TOGGLE_LINE,
  TOGGLE_BOX
};


#endif // __CDDS_DRAWINGAREA_H

