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

#ifndef __CDDS_GEOACTION_H
#define __CDDS_GEOACTION_H

#include "../cdds.h"

#define CONC(A, B) CONC_(A, B)
#define CONC_(A, B) A##B

#define MODULEOBJNAME CDDSGeoAction
#define UMODULEOBJNAME CONC(_, MODULEOBJNAME)

#define MODULEOBJNAME_CLASS CONC(MODULEOBJNAME, Class)
#define UMODULEOBJNAME_CLASS CONC(_, MODULEOBJNAME_CLASS)

#define MODULEOBJNAME_PRIVATE CONC(MODULEOBJNAME, Private)
#define UMODULEOBJNAME_PRIVATE CONC(_, MODULEOBJNAME_PRIVATE)

#define MODULE_OBJ_NAME cdds_geoaction

#define CAPMODULE_OBJ_NAME CDDS_GEOACTION

#define CDDS_GEOACTION_TYPE             (cdds_geoaction_get_type ())
#define CDDS_GEOACTION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), CDDS_GEOACTION_TYPE, CDDSGeoAction))

typedef struct UMODULEOBJNAME             MODULEOBJNAME;
typedef struct UMODULEOBJNAME_CLASS       MODULEOBJNAME_CLASS;
typedef struct UMODULEOBJNAME_PRIVATE     MODULEOBJNAME_PRIVATE;

/* variable declares */
struct UMODULEOBJNAME
{
  GObject parent;
  
  CdsAreaSelection* selection;
  
  xycoord* transform;
  xycoord* transform_origin;
  xycoord* transform_progress_previous;
  
  gboolean inprogress;
  
  gint type;
  
  CdsAreaDrawingArea* area;
  
#ifdef CDDS_DEBUG
  gint refcount;
#endif
  
};

struct UMODULEOBJNAME_CLASS
{
  GObjectClass parent_class;
};





/* func declares */
GType                       CONC(MODULE_OBJ_NAME,_get_type)             ( void );
MODULEOBJNAME              *CONC(MODULE_OBJ_NAME,_new)                  ( managers* mgrs, gint type, 
									  CdsAreaSelection* selection );
void                        cdds_geoaction_destroy                      ( CDDSGeoAction* action );
void                        CONC(MODULE_OBJ_NAME,_finalize)             ( GObject* );

void                        CONC(MODULE_OBJ_NAME,_action)               ( managers* mgrs,
									  CDDSGeoAction* action );

void                        CONC(MODULE_OBJ_NAME,_inverse_action)       ( managers* mgrs,
									  CDDSGeoAction* action );

void                        CONC(MODULE_OBJ_NAME,_increase_ref_count)   ( CDDSGeoAction* action );

void                        CONC(MODULE_OBJ_NAME,_decrease_ref_count)   ( CDDSGeoAction* action );


/* globals */
static GObjectClass*        CONC(MODULE_OBJ_NAME,_parent_class) = NULL;


enum
{
  CONC(CAPMODULE_OBJ_NAME,_N_PROPERTIES) = 1,
};

static GParamSpec *CONC(MODULE_OBJ_NAME,_properties)[CONC(CAPMODULE_OBJ_NAME,_N_PROPERTIES)] = { NULL, };



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

#endif /*__CDDS_GEOACTION_H */





