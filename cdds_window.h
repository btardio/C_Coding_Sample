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

#ifndef __CDDS_WINDOW_H
#define __CDDS_WINDOW_H

#include "cdds.h"

//#include <gtk/gtk.h>
//#include "cdds_application.h"
//#include "cdds_drawingarea.h"


G_BEGIN_DECLS

#define CDS_AREA_TYPE_WINDOW (cds_area_window_get_type ())

G_DECLARE_FINAL_TYPE(CdsAreaWindow, cds_area_window, CDS_AREA, WINDOW, GtkApplicationWindow)

GType                   cds_area_window_get_type             (void);
CdsAreaWindow          *cds_area_window_new                  (GApplication *app);
void                    cds_area_window_open                 (CdsAreaWindow *win, GFile *file);
void                    cds_area_window_destroy              (CdsAreaWindow *win);

CdsAreaDrawingArea     *cds_area_window_create_drawingarea   ( CdsAreaWindow* win, gint width, gint height);
void                    cds_area_window_destroy_drawingarea  ( CdsAreaWindow* win );

void                    cds_area_window_dispose              ( GObject* obj );
void                    cds_area_window_finalize             ( GObject* obj );

G_END_DECLS


struct _CdsAreaWindow
{
  GtkApplicationWindow parent_instance;
  
  GtkWidget* box_primary;
  
  GtkAdjustment* hadjustment;
  GtkAdjustment* vadjustment;
  
  GtkEventBox* eventbox;
  GtkScrolledWindow* scrolledwindow;
  GtkViewport* viewport;
  
  GtkToggleButton *tog_select;
  GtkToggleButton *tog_point;
  GtkToggleButton *tog_line;
  GtkToggleButton *tog_box;
  
  GtkMenuBar* menubar;
  GtkMenuItem* menuitem_edit;
  GtkImageMenuItem* menuitem_undo;
  GtkImageMenuItem* menuitem_redo;
  
#ifdef CDDS_TEST
  GtkMenuItem* menuitem_toplevel_test;
  GtkMenu* menu_test;
  GtkMenuItem* menuitem_test;
#endif
  
  GtkScrolledWindow* scrolledwindow_points;
  GtkScrolledWindow* scrolledwindow_lines;
  
  CdsAreaDrawingArea* area;
  
};

struct _CdsAreaWindowClass
{
  GtkApplicationWindowClass parent_class_instance;
};


#endif // __CDDS_WINDOW_H


