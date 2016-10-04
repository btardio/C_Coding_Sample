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

#ifndef __CDDS_GEOPOINTMANAGER_H
#define __CDDS_GEOPOINTMANAGER_H

#define HASH
//#define TREE

#include "../cdds.h"

G_BEGIN_DECLS

#define CDS_AREA_TYPE_POINTMANAGER (cds_area_pointmanager_get_type ())

G_DECLARE_FINAL_TYPE(CdsAreaPointManager, cds_area_pointmanager, CDS_AREA, POINTMANAGER, GObject)

/* func declares */
GType                       cds_area_pointmanager_get_type       ( void );
CdsAreaPointManager        *cds_area_pointmanager_new            ( void );
void                        cds_area_pointmanager_destroy        ( CdsAreaPointManager* );

gboolean                    cds_area_pointmanager_insert_pointp  ( managers* mgrs, CDDSGeoPoint* p );

GPtrArray*                  cds_area_pointmanager_lookup_array_xy( CdsAreaPointManager* mgr, gint x, gint y );
gint                        cds_area_pointmanager_lookup_pointp  ( CdsAreaPointManager* mgr, CDDSGeoPoint* p );
CDDSGeoPoint               *cds_area_pointmanager_lookup_pointxy ( CdsAreaPointManager* mgr, gint x, gint y );

void                        cds_area_pointmanager_draw_points    ( CdsAreaPointManager* mgr, cairo_t *cr );

void                        cds_area_pointmanager_delete_pointp  ( CdsAreaPointManager* mgr, CDDSGeoPoint* p );

G_END_DECLS


/* variable declares */
struct _CdsAreaPointManager
{
  GObject parent_instance;
  
#ifdef TREE
  GTree* tree;
#endif
  
#ifdef HASH
  GHashTable* table;
#endif
    
  guint n_points;
};

struct _CdsAreaPointManagerClass
{
  GObjectClass parent_class;
};


enum
{
  CDS_AREA_POINTMANAGER_N_PROPERTIES = 1
};

static GParamSpec *cds_area_pointmanager_properties[CDS_AREA_POINTMANAGER_N_PROPERTIES] = { NULL, };


#endif /*__CDDS_GEOPOINTMANAGER_H */





