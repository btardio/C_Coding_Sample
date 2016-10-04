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

#include "cdds.h"
#include "cdds_application.h"

//#define BRESEN

#ifdef BRESEN
#include "bresenhamlinealgorithm/bresenham.h"
#endif

/*
 * Source:
 * https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
 */



int main(int argc, char **argv) {  

#ifndef BRESEN
  
  g_setenv ("GSETTINGS_SCHEMA_DIR", ".", FALSE);  
    
  return g_application_run (G_APPLICATION (cds_area_app_new ()), argc, argv);
  
#endif
  
  
#ifdef BRESEN
  
  bresen(argc, argv);
  
  return 0;
  
#endif
}
