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

#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

#define CDDS_TEST
#define CDDS_DEBUG

//typedef struct _CDDSApp                         CDDSApp;

typedef struct _CdsAreaApp                         CdsAreaApp;

typedef struct _CdsAreaDialog                      CdsAreaDialog;

typedef struct _CdsAreaWindow                      CdsAreaWindow;

typedef struct _CdsAreaDrawingArea                 CdsAreaDrawingArea;

typedef struct _CdsAreaActionManager               CdsAreaActionManager;

typedef struct _CDDSGeoAction                   CDDSGeoAction;

typedef struct _CdsAreaSelection                   CdsAreaSelection;

typedef struct _CdsAreaPointManager                CdsAreaPointManager;

typedef struct _CDDSGeoPoint                    CDDSGeoPoint;

typedef struct _CdsAreaLine                        CdsAreaLine;

typedef struct _CdsAreaLineManager                 CdsAreaLineManager;

typedef struct _managers                           managers;
typedef struct _xycoord                            xycoord;
typedef struct _xycoordwmgrs                       xycoordwmgrs;
typedef struct _actionwmgrs                        actionwmgrs;

//CdsAreaWindow* window;
//CdsAreaDrawingArea* drawarea;

//gboolean shutdown_in_progress;

//GPtrArray* allpoints;
//static gint n_allpoints;

#ifdef CDDS_DEBUG
//GPtrArray* allactions;
#endif


void cds_area_test_actions ( CdsAreaDrawingArea* area );
void cds_area_test_points ( CdsAreaDrawingArea* area );

