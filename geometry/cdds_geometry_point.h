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

#ifndef __CDDS_GEOPOINT_H
#define __CDDS_GEOPOINT_H

//#include <gtk/gtk.h>
//#include "cdds_geometry.h"

#include "../cdds.h"

#define CONC(A, B) CONC_(A, B)
#define CONC_(A, B) A##B

#define MODULEOBJNAME CDDSGeoPoint
#define UMODULEOBJNAME CONC(_, MODULEOBJNAME)

#define MODULEOBJNAME_CLASS CONC(MODULEOBJNAME, Class)
#define UMODULEOBJNAME_CLASS CONC(_, MODULEOBJNAME_CLASS)

#define MODULEOBJNAME_PRIVATE CONC(MODULEOBJNAME, Private)
#define UMODULEOBJNAME_PRIVATE CONC(_, MODULEOBJNAME_PRIVATE)

#define MODULE_OBJ_NAME cdds_geopoint

#define CAPMODULE_OBJ_NAME CDDS_GEOPOINT

#define CDDS_GEOPOINT_TYPE             (cdds_geopoint_get_type ())
#define CDDS_GEOPOINT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CDDS_GEOPOINT_TYPE, CDDSGeoPoint))

typedef struct UMODULEOBJNAME             MODULEOBJNAME;
typedef struct UMODULEOBJNAME_CLASS       MODULEOBJNAME_CLASS;
typedef struct UMODULEOBJNAME_PRIVATE     MODULEOBJNAME_PRIVATE;

/* variable declares */
struct UMODULEOBJNAME
{
  GObject parent;
  
  gboolean selected;
  
  gint point_x;
  gint point_y;
  
  //gint point_x_old;
  //gint point_y_old;
  
  gfloat color_r;
  gfloat color_g;
  gfloat color_b;
  gfloat color_r_selected;
  gfloat color_g_selected;
  gfloat color_b_selected;
  
  GtkTreeIter* iter;
  
  CdsAreaDrawingArea* area;
  
#ifdef CDDS_DEBUG
  gint refcount;
  glong ptractionmgr;
  glong ptrselection;
  glong ptrpointmgr;
#endif

  
};

struct UMODULEOBJNAME_CLASS
{
  GObjectClass parent_class;
};

struct UMODULEOBJNAME_PRIVATE
{
  char nonzero_size;
};

/* func declares */
GType                       CONC(MODULE_OBJ_NAME,_get_type)                   ( void );
MODULEOBJNAME              *CONC(MODULE_OBJ_NAME,_new)                        ( managers* mgrs, gint x, gint y );
void                        CONC(MODULE_OBJ_NAME,_destroy)                    ( MODULEOBJNAME* );
void                        CONC(MODULE_OBJ_NAME,_finalize)                   ( GObject* obj );
//void                        CONC(MODULE_OBJ_NAME,_unref)                      ( MODULEOBJNAME* );

void                        CONC(MODULE_OBJ_NAME,_ref)                        ( CDDSGeoPoint* p );
void                        CONC(MODULE_OBJ_NAME,_unref)                      ( CDDSGeoPoint* p );

void                        CONC(MODULE_OBJ_NAME,_delete_from_liststore)      ( CDDSGeoPoint* p );

void                        CONC(MODULE_OBJ_NAME,_add_to_liststore)           ( managers* mgrs, CDDSGeoPoint* p );

//#ifdef CDDS_TEST
//GPtrArray*                  cdds_geopoint_allpoints                           ( void );
//gint                        cdds_geopoint_n_allpoints                         ( void );
//#endif

/* globals */
static GObjectClass*        CONC(MODULE_OBJ_NAME,_parent_class) = NULL;


enum
{
  CONC(CAPMODULE_OBJ_NAME,_PROP_X) = 1,
  CONC(CAPMODULE_OBJ_NAME,_PROP_Y),
  CONC(CAPMODULE_OBJ_NAME,_PROP_R),
  CONC(CAPMODULE_OBJ_NAME,_PROP_G),
  CONC(CAPMODULE_OBJ_NAME,_PROP_B),
  CONC(CAPMODULE_OBJ_NAME,_PROP_R_SELECTED),
  CONC(CAPMODULE_OBJ_NAME,_PROP_G_SELECTED),
  CONC(CAPMODULE_OBJ_NAME,_PROP_B_SELECTED),
#ifdef CDDS_DEBUG
  CONC(CAPMODULE_OBJ_NAME,_PROP_REFCOUNT),
#endif
  CONC(CAPMODULE_OBJ_NAME,_N_PROPERTIES)
};

static GParamSpec *CONC(MODULE_OBJ_NAME,_properties)[CONC(CAPMODULE_OBJ_NAME,_N_PROPERTIES)] = { NULL, };


enum
{
  COLUMN_POINTER,
  COLUMN_X,
  COLUMN_Y,
#ifdef CDDS_DEBUG
  COLUMN_REFCOUNT,
#endif
  N_COLUMNS
};

#undef CONC
#undef _CONC

#undef MODULEOBJNAME
#undef UMODULEOBJNAME

#undef MODULEOBJNAME_CLASS
#undef UMODULEOBJNAME_CLASS

#undef MODULEOBJNAME_PRIVATE
#undef UMODULEOBJNAME_PRIVATE

#undef MODULE_OBJ_NAME

#undef CAPMODULE_OBJ_NAME

#endif /*__CDDS_GEOPOINT_H */





