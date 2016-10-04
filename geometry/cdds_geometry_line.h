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

#ifndef __CDDS_GEOLINE_H
#define __CDDS_GEOLINE_H

#include "../cdds.h"

G_BEGIN_DECLS

#define CDS_AREA_TYPE_LINE (cds_area_line_get_type ())
G_DECLARE_FINAL_TYPE(CdsAreaLine, cds_area_line, CDS_AREA, LINE, GObject)

/* func declares */
GType                       cds_area_line_get_type                      ( void );

CdsAreaLine                *cds_area_line_new                           ( managers* mgrs, 
									  CDDSGeoPoint* a, CDDSGeoPoint* b );

void                        cds_area_line_destroy                       ( CdsAreaLine* );

void                        cds_area_line_finalize                      ( GObject* obj );

void                        cds_area_line_ref                           ( CdsAreaLine* l );

void                        cds_area_line_unref                         ( CdsAreaLine* l );

void                        cds_area_line_delete_from_liststore         ( CdsAreaLine* l );

void                        cds_area_line_add_to_liststore              ( managers* mgrs, CdsAreaLine* l );

G_END_DECLS


/* variable declares */
struct _CdsAreaLine
{
  GObject parent_instance;
  
  gboolean selected;
  
  CDDSGeoPoint* p1;
  CDDSGeoPoint* p2;
  
  gfloat color_r;
  gfloat color_g;
  gfloat color_b;
  gfloat color_r_selected;
  gfloat color_g_selected;
  gfloat color_b_selected;
  
  gint point1_x;
  gint point1_y;
  gint point2_x;
  gint point2_y;
  
  GtkTreeIter* iter;
  
  CdsAreaDrawingArea* area;
  
#ifdef CDDS_DEBUG
  gint refcount;
#endif

  
};

struct _CdsAreaLineClass
{
  GObjectClass parent_class_instance;
};




enum
{
  CDS_AREA_LINE_PROP_POINT1X = 1,
  CDS_AREA_LINE_PROP_POINT1Y,
  CDS_AREA_LINE_PROP_POINT2X,
  CDS_AREA_LINE_PROP_POINT2Y,
  CDS_AREA_LINE_PROP_R,
  CDS_AREA_LINE_PROP_G,
  CDS_AREA_LINE_PROP_B,
  CDS_AREA_LINE_PROP_R_SELECTED,
  CDS_AREA_LINE_PROP_G_SELECTED,
  CDS_AREA_LINE_PROP_B_SELECTED,
#ifdef CDDS_DEBUG
  CDS_AREA_LINE_PROP_REFCOUNT,
#endif

  CDS_AREA_LINE_N_PROPERTIES
};

static GParamSpec *cds_area_line_properties[CDS_AREA_LINE_N_PROPERTIES] = { NULL, };

enum
{
  LINE_COLUMN_POINTER,
  LINE_COLUMN_POINTER_POINT1X,
  LINE_COLUMN_POINTER_POINT1Y,
  LINE_COLUMN_POINTER_POINT2X,
  LINE_COLUMN_POINTER_POINT2Y,
#ifdef CDDS_DEBUG
  LINE_COLUMN_REFCOUNT,
#endif
  LINE_N_COLUMNS
};



#endif /*__CDDS_GEOLINE_H */





