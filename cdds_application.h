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

#ifndef __CDDS_APPLICATION_H
#define __CDDS_APPLICATION_H

#include "cdds.h"

G_BEGIN_DECLS

#define CDS_AREA_TYPE_APP (cds_area_app_get_type ())
G_DECLARE_FINAL_TYPE(CdsAreaApp, cds_area_app, CDS_AREA, APP, GtkApplication)

GType             cds_area_app_get_type    ( void );
CdsAreaApp       *cds_area_app_new         ( void );
static void       cds_area_app_startup     ( GApplication *app );
void              cds_area_app_shutdown    ( GApplication *app );
static void       cds_area_app_open        ( GApplication *app, GFile **files, gint n_files, const gchar *hint );
G_END_DECLS


struct _CdsAreaApp
{
  GtkApplication parent_instance;
  CdsAreaWindow* win;
};

struct _CdsAreaAppClass
{
  GtkApplicationClass parent_class_instance;
};



#endif // __CDDS_APPLICATION_H

