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

#ifndef __CDDS_GEOSELECTION_H
#define __CDDS_GEOSELECTION_H

#include "../cdds.h"

G_BEGIN_DECLS

#define CDS_AREA_TYPE_SELECTION (cds_area_selection_get_type ())
G_DECLARE_FINAL_TYPE(CdsAreaSelection, cds_area_selection, CDS_AREA, SELECTION, GObject)

/* func declares */
GType                       cds_area_selection_get_type                   	(void);
CdsAreaSelection           *cds_area_selection_new                        	(gint* width, gint* height);
void                        cds_area_selection_destroy                     	(CdsAreaSelection*);

void                        cds_area_selection_init_pixbuf                      ( CdsAreaSelection* selection );

void			    cds_area_selection_draw                        	( CdsAreaSelection*, cairo_t *cr );

void                        cds_area_selection_select_press                	( CdsAreaSelection*, CdsAreaPointManager*, 
									         GdkEventButton *event );
void                        cds_area_selection_select_motion              	( CdsAreaSelection*, CdsAreaPointManager*, 
									         GdkEventMotion *event );
void                        cds_area_selection_select_release              	( CdsAreaSelection*, CdsAreaPointManager*, 
									         GdkEventButton *event );

void			    cds_area_selection_select_keypress             	( CdsAreaSelection*, guint keyval );
void			    cds_area_selection_select_keyrelease          	( CdsAreaSelection*, guint keyval );

void			    cds_area_selection_mark_pointxy		       	( managers*, gint x, gint y );
void			    cds_area_selection_mark_pointp		       	( managers*, CDDSGeoPoint* p );
void			    cds_area_selection_unmark_pointxy	       		( CdsAreaSelection*, gint x, gint y );
void			    cds_area_selection_unmark_pointp	       		( CdsAreaSelection*, CDDSGeoPoint* p );

CDDSGeoPoint		   *cds_area_selection_lookup_point		       	( CdsAreaPointManager* mgr, gint x, gint y );

GHashTable                 *cds_area_selection_copy_points		       	( CdsAreaSelection* selection );

void                        cds_area_selection_put_pixel                   	( CdsAreaSelection* selection, 
									         int x, int y, guchar pixel);

void			    cds_area_selection_insert_pointp               	( CdsAreaSelection* selection, 
									         CDDSGeoPoint* p );

void                        cds_area_selection_insert_linel                     ( CdsAreaSelection* selection, 
										  CdsAreaLine* l );

void			    cds_area_selection_delete_pointp               	( CdsAreaSelection* selection, 
									         CDDSGeoPoint* p );

void                        cds_area_selection_clear_selection                  ( CdsAreaSelection* selection );

gboolean                    cds_area_selection_contains_point              	( CdsAreaSelection* selection, CDDSGeoPoint* p );

guint                       cds_area_selection_len                              ( CdsAreaSelection* selection );

//GPtrArray*                  cds_area_selection_to_ptrarray                      ( CdsAreaSelection* selection );

GPtrArray*                  cds_area_selection_points_to_ptrarray               ( CdsAreaSelection* selection );

GPtrArray*                  cds_area_selection_lines_to_ptrarray                ( CdsAreaSelection* selection );

void                        cds_area_selection_increase_ref_count_points   	( CdsAreaSelection* selection );

void                        cds_area_selection_decrease_ref_count_points   	( CdsAreaSelection* selection );

CdsAreaSelection           *cds_area_selection_selection_from_selection    	( CdsAreaSelection* selection );


G_END_DECLS


/* variable declares */
struct _CdsAreaSelection
{
  GObject parent_instance;
  
  GHashTable* hash_points;
  GHashTable* hash_lines;
  
  gboolean button_press;
  
  gint button_press_x;
  gint button_press_y;
  gint button_motion_x;
  gint button_motion_y;
  
  gboolean key_lshift;
  gboolean key_lctrl;
  
  gint* width;
  gint* height;
  
  GdkPixbuf* pixbuf;
  
};

struct _CdsAreaSelectionClass
{
  GObjectClass parent_class_instance;
};




enum
{
  CDS_AREA_SELECTION_N_PROPERTIES = 1
};

static GParamSpec *cds_area_selection_properties[CDS_AREA_SELECTION_N_PROPERTIES] = { NULL, };


enum{
  PIXBUF_EMPTY,
  PIXBUF_POINT,
  PIXBUF_LINE,
};


#endif /*__CDDS_GEOSELECTION_H */





