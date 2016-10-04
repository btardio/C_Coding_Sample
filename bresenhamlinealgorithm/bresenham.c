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

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>





gint 
determinecaseoctant ( gint x1, gint y1, gint x2, gint y2 )
{
  
  gint rval = 0;
  
  // octant check
  if ( x1 > x2 || y1 > y2 )
  {
    printf("can not plot this line, line points in wrong direction\n");
    
    
    if ( x1 > x2 && y1 > y2 )
    {
      
      rval += 1;
      
    }
    
    else if ( x1 > x2 && y1 < y2 )
    {
      
      rval += 2;
      
    }
    
    else if ( x1 < x2 && y1 > y2 )
    {
      
      rval += 3;
      
    }
    
  }
  
  return rval;
}

gint 
determinecaseslope ( gint x1, gint y1, gint x2, gint y2 )
{
  
  
  
}

void
lineinoctant ( gint switchcaseoctant, gint* x1, gint* y1, gint* x2, gint* y2 )
{
  gint a;
  
  switch ( switchcaseoctant )
  {
    case 1:
      //
      // -----------x1,y1
      //          .. |
      //      ....   |
      //    ..       |
      //             |
      //
      // ( x1 > x2 && y1 > y2 )
      a = *x1;
      *x1 = *x2;
      *x2 = a;
      
      a = *y1;
      *y1 = *y2;
      *y2 = a;
      
      break;
      
    case 2:
      
      //
      // 
      //  ..        |
      //    ....    |
      //        ..  |
      // ----------x1,y1
      // ( x1 > x2 && y1 < y2 )
      
      
      a = *x1;
      *x1 = *x2;
      *x2 = a;      
      
      break;
      
    case 3:
      // x1,y1---------------
      //  | ..
      //  |   ....
      //  |       ..
      //
      // ( x1 < x2 && y1 > y2 )
      
      a = *y1;
      *y1 = *y2;
      *y2 = a;      
      
      break;
  }
  
}


void
lineoutoctant ( gint switchcaseoctant, gint* x1, gint* y1, gint* x2, gint* y2 )
{
  gint a;
  
  switch ( switchcaseoctant )
  {
    case 1:
      //
      // -----------x1,y1
      //          .. |
      //      ....   |
      //    ..       |
      //             |
      //
      // ( x1 > x2 && y1 > y2 )
      a = *x1;
      *x1 = *x2;
      *x2 = a;
      
      a = *y1;
      *y1 = *y2;
      *y2 = a;
      
      break;
      
    case 2:
      
      //
      // 
      //  ..        |
      //    ....    |
      //        ..  |
      // ----------x1,y1
      // ( x1 > x2 && y1 < y2 )
      
      
      a = *x1;
      *x1 = *x2;
      *x2 = a;      
      
      break;
      
    case 3:
      // x1,y1---------------
      //  | ..
      //  |   ....
      //  |       ..
      //
      // ( x1 < x2 && y1 > y2 )
      
      a = *y1;
      *y1 = *y2;
      *y2 = a;      
      
      break;
  }
  
  
}


gboolean
lineinslope ( gint* x1, gint* y1, gint* x2, gint* y2 )
{
  gint a;

  if ( *x2 - *x1 <= *y2 - *y1 )
  {
   
    a = *x1;
    *x1 = *y1;
    *y1 = a;
    
    a = *x2;
    *x2 = *y2;
    *y2 = a;
    
    printf("switchcaseslope = TRUE\n");
    return TRUE;
    
  }
  printf("switchcaseslope = FALSE\n");
  return FALSE;
}


void
lineoutslope ( gboolean switchcaseslope, gint* x1, gint* y1, gint* x2, gint* y2 )
{
  gint a;

  if ( switchcaseslope )
  {
   
    a = *x1;
    *x1 = *y1;
    *y1 = a;
    
    a = *x2;
    *x2 = *y2;
    *y2 = a;
        
  }

}



struct point {
  gint x;
  gint y;
};


struct point
lineoutslopel ( gint switchcaseslope, gint x, gint y )
{
  struct point p;
  
  gint a;
     
  a = x;
  x = y;
  y = a;
    
  //printf("plot(%d, %d)\n", x, y);
  
  p.x = x;
  p.y = y;
  
  return p;
  
}


void
plotlinefloat ( gint x, gint x1, gint y1, gint x2, gint y2 )
{
  
  gdouble rise = y2 - y1;
  gdouble run = x2 - x1;
  
  gdouble slope = rise / run;
  
  //printf( "plotlinefloat: %d, %f\n", x, slope * x + y1 );
  
}

void
checkplotlinefloat ( gboolean switchcaseslope, gint in_x, gint in_y, gint in_x1, gint in_y1, gint in_x2, gint in_y2 )
{
  
  gdouble x1 = in_x1;
  gdouble y1 = in_y1;
  gdouble x2 = in_x2;
  gdouble y2 = in_y2;
  
  gdouble x = in_x;
  gdouble y = in_y;
  
  gdouble outx, outy;
  
  gdouble rise = y2 - y1;
  gdouble run = x2 - x1;
  
  gdouble slope = rise / run;
  gdouble oppositeslope = run / rise;
  
  // y = mx + b     =     b = y - mx
  gdouble b = y1 - slope * x1;
  
  
  //if ( x > y )
  if ( !switchcaseslope )
  {
    outx = x;
    //outy = slope * outx + b;
    
    outy = slope * ( outx - x1 ) + y1;
           
    // y - y1 = m ( x - x1 )
    
    if ( outy - y > 1.0 || outy - y < -1.0 )
    {
      //printf("### ERROR checkplotlinefloat ### equation: ( %f * ( %f - %d ) ) + %d \n", slope, outx, x1, y1);
      printf("### ERROR checkplotlinefloat ### ");
    }
    
    printf("point: %f, %f actual: %f, %f\n", x, y, outx, outy );
  }

  else
  {
    // y = mx + b    y - b = mx    ( y - b ) / m = x
    outy = y;
    //outx = ( y - b ) / slope;
    
    outx = oppositeslope * ( outy - y1 ) + x1;

    if ( outx - x > 1 || outx - x < -1 )
    {
      printf("### eERROR checkplotlinefloat ### equation: ( %f * ( %f - %d ) ) + %d \n", slope, outx, x1, y1);
    }
    
    printf("point: %f, %f actual: %f, %f\n", x, y, outx, outy );
    
  }

}


GPtrArray* plotpoints;


void
ptrpoints_freefunc ( gpointer data ) { g_free ( data ); }


void
linecalc ( gboolean switchcaseslope, gint x1, gint y1, gint x2, gint y2 )
{
  struct point* p;
  struct point slopep;
  
  plotpoints = g_ptr_array_new_with_free_func ( ptrpoints_freefunc );
  
  //printf( "(%d, %d) (%d, %d)\n", x1, y1, x2, y2 );

  // octant check
  if ( x1 > x2 || y1 > y2 )
  {
    //printf("can not plot this line, line points in wrong direction\n");
    //return;
  }

  // slope check
  if ( x2 - x1 <= y2 - y1 )
  {
    //printf("horizontal projection is not as long as vertical projection\n");
    //return;
  }

  gdouble deltax = x2 - x1;
  gdouble deltay = y2 - y1;

  //printf("deltax: %f\n", deltax);
  //printf("deltay: %f\n", deltay);

  gdouble error = -1.0;

  // rise is always negative, run is always positive, or wont pass checks
  gdouble deltaerror = ( deltay / deltax );

  //printf("deltaerror: %f\n", deltaerror);

  int y = y1;

  for ( int x = x1-1; x < x2; x++ )
  {
    
    p = g_malloc ( sizeof ( struct point ) );
    
    
    if ( switchcaseslope )
    {
      
      
      
      //plotlinefloat ( x+1, x1, y1, x2, y2 );
      slopep = lineoutslopel ( switchcaseslope, x+1, y );
      //checkplotlinefloat ( x+1, y, x1, y1, x2, y2 );
      
      p->x = slopep.x;
      p->y = slopep.y;
      g_ptr_array_add ( plotpoints, p );
      
    }
    else
    {
      
      
      //plotlinefloat ( x+1, x1, y1, x2, y2 );
      //printf("plot(%d, %d)\n", x+1, y);
      //checkplotlinefloat ( x+1, y, x1, y1, x2, y2 );

      p->x = x+1;
      p->y = y;
      g_ptr_array_add ( plotpoints, p );
      
    }
      

    error = error + deltaerror;
    if ( error >= 0.0 )
    {
      y = y + 1.0;
      error = error - 1.0; // why not error = -1
    }
  }


  if ( switchcaseslope )
  {
    plotlinefloat ( x2, x1, y1, x2, y2 );
    lineoutslopel ( switchcaseslope, x2, y2 );
    //checkplotlinefloat ( x2, y, x1, y1, x2, y2 );
  }
  else
  {
    plotlinefloat ( x2, x1, y1, x2, y2 );
    //printf("plot(%d, %d)\n", x2, y2);
    //checkplotlinefloat ( x2, y, x1, y1, x2, y2 );
  }



}


void
line ( gint x1, gint y1, gint x2, gint y2 )
{
  
  gint ox1 = x1;
  gint oy1 = y1;
  gint ox2 = x2;
  gint oy2 = y2;
  
  gint switchcaseoctant;
  gboolean switchcaseslope = FALSE;
  
  struct point* p;
  
  printf( "(%d, %d) (%d, %d)\n", x1, y1, x2, y2 );
  
  switchcaseoctant = determinecaseoctant ( x1, y1, x2, y2 );
  
  lineinoctant ( switchcaseoctant, &x1, &y1, &x2, &y2 );
  
  //switchcaseslope = determinecaseslope ( x1, y1, x2, y2 );
  
  switchcaseslope = lineinslope ( &x1, &y1, &x2, &y2 );
  
  linecalc( switchcaseslope, x1, y1, x2, y2 );
  
  lineoutslope ( switchcaseslope, &x1, &y1, &x2, &y2 );
  
  lineoutoctant ( switchcaseoctant, &x1, &y1, &x2, &y2 );
  


  for ( int i = 0; i < plotpoints->len; i++ )
  {
    p = g_ptr_array_index ( plotpoints, i );
    printf( "point[%d]:", i );
    checkplotlinefloat ( switchcaseslope, p->x, p->y, x1, y1, x2, y2 );
  }
  
  if ( ( p->x - x2 > 1 || p->x - x2 < -1 ) || ( p->y - y2 > 1 || p->y - y2 < -1 ) )
  {
    
    printf( "end points don't meet." );
    
  }
  
  g_ptr_array_free ( plotpoints, TRUE );
  
}


void
plotaline ( gint in_x1, gint in_y1, gint in_x2, gint in_y2 )
{
  
  
  
  gdouble x;
  gdouble y;
  gdouble x1 = in_x1;
  gdouble y1 = in_y1;
  gdouble x2 = in_x2;
  gdouble y2 = in_y2;
  
  gdouble rise = y2 - y1;
  gdouble run = x2 - x1;
  
  gdouble slope = rise / run;
  gdouble oppositeslope = run / rise;


  for ( gdouble i = 0; i < 10; i++ )
  {
  
    x = i;
    
    y = slope * ( x - ((gdouble) x1 ) ) + ((gdouble) y1 );
    
    printf ( "%f, %f\n", x, y );
    
  }
  
  
}


int bresen(int argc, char **argv) {  

  gint x1;
  gint y1;
  gint x2;
  gint y2;

  
  
  if ( argc == 5 )
  {

    x1 = strtol ( argv[1], NULL, 10 );
    y1 = strtol ( argv[2], NULL, 10 );
    x2 = strtol ( argv[3], NULL, 10 );
    y2 = strtol ( argv[4], NULL, 10 );
    
    line ( x1, y1, x2, y2 );
    
  }
    
  
  /*
  
  for ( int a = 0; a < 5; a ++ )
  {
    for ( int b = 0; b < 5; b ++ )
    {
      for ( int c = 0; c < 5; c ++ )
      {
	for ( int d = 0; d < 5; d ++ )
	{
	  x1 = a; y1 = b; x2 = c; y2 = d;
	  
	  printf( "(%d, %d) (%d, %d)\n", x1, y1, x2, y2 );
	  
	  switchcaseoctant = determinecaseoctant ( x1, y1, x2, y2 );
	  
	  lineinoctant ( switchcaseoctant, &x1, &y1, &x2, &y2 );
	  
	  switchcaseslope = lineinslope ( &x1, &y1, &x2, &y2 );
	  
	  line( switchcaseslope, x1, y1, x2, y2 );
	  
	  lineoutslope ( switchcaseslope, &x1, &y1, &x2, &y2 );
	  
	  lineoutoctant ( switchcaseoctant, &x1, &y1, &x2, &y2 );	  
	  
	  
	  
	  for ( int i = 0; i < plotpoints->len; i++ )
	  {
	    p = g_ptr_array_index ( plotpoints, i );
	    printf( "point[%d]:", i );
	    checkplotlinefloat ( switchcaseslope, p->x, p->y, x1, y1, x2, y2 );
	  }
	  g_ptr_array_free ( plotpoints, TRUE );
	  
	}
	
      }
      
    }
    
  }
  
  */
  
  
  //printf ("line with run greater than rise in top right: 0 0 5 3\n");
  //line ( 0, 0, 5, 3 ); plotaline ( 0, 0, 5, 3 );
  
  //printf ("line with rise greater than run in top right: 0 0 3 5\n");
  //line ( 0, 0, 3, 7 ); plotaline ( 0, 0, 3, 7 );
  
  
  //printf ("line with run greater than rise in top left: 5 3 0 0\n");
  //line ( 0, 0, 5, 3 ); plotaline ( 0, 0, 5, 3 );

  
  
  
  return 0;
}






