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

#ifndef __CDDS_GEOLINEMANAGER_H
#define __CDDS_GEOLINEMANAGER_H

#define HASH
//#define TREE

#include "../cdds.h"

G_BEGIN_DECLS

#define CDS_AREA_TYPE_LINEMANAGER (cds_area_linemanager_get_type ())

G_DECLARE_FINAL_TYPE(CdsAreaLineManager, cds_area_linemanager, CDS_AREA, LINEMANAGER, GObject)

/* func declares */
GType                      cds_area_linemanager_get_type          ( void );
CdsAreaLineManager        *cds_area_linemanager_new               ( void );
void                       cds_area_linemanager_destroy           ( CdsAreaLineManager* );

gboolean                   cds_area_linemanager_insert_linel      ( managers* mgrs, CdsAreaLine* l );

GPtrArray*                 cds_area_linemanager_lookup_array_xy   ( CdsAreaLineManager* mgr, gint x, gint y );
gint                       cds_area_linemanager_lookup_linel      ( CdsAreaLineManager* mgr, CdsAreaLine* l );
CdsAreaLine               *cds_area_linemanager_lookup_linexy     ( CdsAreaLineManager* mgr, gint x, gint y );

void                       cds_area_linemanager_draw_lines        ( CdsAreaLineManager* mgr, cairo_t *cr );

void                       cds_area_linemanager_delete_linel      ( CdsAreaLineManager* mgr, CdsAreaLine* l );

G_END_DECLS


/* variable declares */
struct _CdsAreaLineManager
{
  GObject parent_instance;
  
#ifdef TREE
  GTree* tree;
#endif
  
#ifdef HASH
  GHashTable* table;
#endif
  
  GHashTable* hash_lines;
  
  guint n_lines;
};

struct _CdsAreaLineManagerClass
{
  GObjectClass parent_class;
};


enum
{
  CDS_AREA_LINEMANAGER_N_PROPERTIES = 1
};

static GParamSpec *cds_area_linemanager_properties[CDS_AREA_LINEMANAGER_N_PROPERTIES] = { NULL, };


#endif /*__CDDS_GEOLINEMANAGER_H */





