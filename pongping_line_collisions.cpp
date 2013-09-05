/*
 *
 *
 *            __________     __________     ____        ___     ____________         __________     __      ___        ___    ____________
 *           /   __    /\   / ______  /\   /    \      /   /\  /   ________/\       /   __    /\   / /\    /   \      /  /\  /   ________/\
 *          /   /_/   / /  / /\____/ / /  /      \    /   / / /   /\_______\/      /   /_/   / /  / / /   /     \    /  / / /   /\_______\/
 *         /   ______/ /  / / /   / / /  /   /\   \  /   / / /   /_/______        /   ______/ /  / / /   /   /\  \  /  / / /   /_/______
 *        /   /\_____\/  / /_/___/ / /  /   / /\   \/   / / /________    /\      /   /\_____\/  / / /   /   / /\  \/  / / /________    /\
 *       /___/ /        /_________/ /  /___/ /  \______/ /  \_______/___/ /     /___/ /        /_/ /   /___/ /  \____/ /  \_______/___/ /
 *       \___\/         \_________\/   \___\/    \_____\/           \___\/      \___\/         \_\/    \___\/    \___\/           \___\/
 *
 *
 *      See readme.txt for copyright information.
 */


#include "pongping_headers/pongping_line_collisions_headers.h"
 



#define PONGPING_ACTUAL_FRAME 0
#define PONGPING_MOVING_FRAME 1




using namespace std;

							
//static int circle_and_rotating_line_collision(Circle *c, const double cr_x, const double cr_y, const double pai, Point v1, Point v2,  PotentialMove *move);
int circle_and_rotating_line_collision(Circle *c, const double ftr, const double cr_x, const double cr_y, const double pai, Point v1, Point v2, PotentialMove *move);



int side_of_line(const double m, const double c, const double cx, const double cy, int gradient_infinite, const double line_X);

enum { PONGPING_LEFT_SIDE_OF_LINE, PONGPING_RIGHT_SIDE_OF_LINE, PONGPING_BELOW_LINE, PONGPING_ABOVE_LINE };



int stationary_line_collision(Circle *circle, double angle, float ftr, Point v1, Point v2, float solid_x, float solid_y, PotentialMove *move, int circle_inside);





#define PONGPING_INFINITE_GRADIENT 1



int circle_and_line_collision(const unsigned char line_move, const Point v1, const Point v2,  
                              const double cx, const double cy, const double polygon_increment_angle, const double dx, const double dy, Circle *c, PotentialMove* move, int circle_inside, double ftr)
{


   double x2, y2; // for where circle travels to
   double oa; /* other angle */
   double df; /* distance in other frame of reference */
     
   double ld; /* distance the line moves */
   
   Point vv1, vv2;
   
   PotentialMove *dummy_pm = new PotentialMove();
   
   double y_on_line_for_sx;
   
   double cx2, cy2;
   
   if( line_move == PONGPING_TRANSLATION )
   {
      
	  //logfile << "******************************************************\n";
	  
      x2 = c->cc_x + c->distance_increment * ftr * cos( c->circle_angle ) + dx;
      y2 = c->cc_y + c->distance_increment * ftr * sin( c->circle_angle ) + dy;
   
      /* Work out other angle. */
      
      /* check for 0, 0 call */
	  
      oa = principal_value_2(atan2( y2 - c->cc_y  , x2 - c->cc_x ));
         
      
   
      df = sqrt( (c->cc_x - x2) * (c->cc_x - x2) + (c->cc_y - y2) * (c->cc_y - y2) );
      //int stationary_line_collision(Circle *circle, double angle, float ftr, Point v1, Point v2, float solid_x, float solid_y, PotentialMove *move, int circle_inside);
      
	  if( stationary_line_collision( c, oa, ftr, v1, v2, cx, cy, dummy_pm, circle_inside) )
      {
         
         
      
         ld = sqrt( dx * dx + dy * dy ) * dummy_pm->pfot;
      
	     double lma = principal_value_2(atan2( dy, dx ));
      
         vv1.x = v1.x + ld * cos( lma );
         vv1.y = v1.y + ld * sin( lma );
         
         vv2.x = v2.x + ld * cos( lma );
         vv2.y = v2.y + ld * sin( lma );
      
         cx2 = cx + ld * cos( lma );
		 cy2 = cy + ld * sin( lma );
         
         if(stationary_line_collision( c, c->circle_angle, ftr, vv1, vv2, cx2, cy2, move, circle_inside))
         {
		    delete dummy_pm;
            return 1;
         }
      }
	  
   }
   
   delete dummy_pm;
   
   if(line_move == PONGPING_ROTATION)
   {  
      return circle_and_rotating_line_collision(c, ftr, cx, cy, polygon_increment_angle, v1, v2, move);
   }
   
   return 0;
   
   
}






/* stationary_line_collision : 

WHAT IT DOES

This function sees if a circle which is travelling in a straight line will hit a line through two vertices between the vertices.


ARGUMENTS

circle : a pointer to the circle in question

circle_angle : the angle of a velocity vector for the circle which might be the actual velocity vector for the circle or the velocity vector 


ftr : fraction of time remaining. Each timer tick 100% of the distance_increment for each circle has to be used but everything is
      updated step by step so at the time this function is called the circle might already have moved 30% of it's distance_increment in
	  which case ftr would be 0.7

	  
v1, v2 : the first and second vertex which the line between makes the side of the polygon, bat or whatever.


solid_x : the x coordinate of a point which when combined with any two consecutive vertices of the polygon, form a triangle the area of
          which is all inside the polygon. 

solid_y : the y coordinate of the aforementioned point 


move : a pointer to a PotentialMove class which is used to put all the data to implement the move if when all potential moves are looked
       at, this is the one to apply.

circle_inside : a flag to indicate if the circle is inside the polygon or not. 

				
check for angle of ball being vertical when line is vertical 

add code to return 0 when polygon line is vertical but circle is too close to hit it.

*/

//#define SLC_LOG

int stationary_line_collision(Circle *circle, double circle_angle, float ftr, Point v1, Point v2, float solid_x, float solid_y, PotentialMove *move, int circle_inside )
{
   
   /* Given that the polygon line has equation y = multiplier * x + constant, then mpL and cpL stant for
      Multiplier of Polygon Line and Constant of Polygon Line */
   
   double mpL; /* multiplier for polygon line. */
   double cpL; /* constant for polygon line */
   
   
   /*
      The side of the polygon makes a line : the point on the circle's edge which is closest to the line is where the circle will make contact with
      the side if it does hit. The circle is travelling with the direction of a arrow pointing away from the origin and the arrow's angle is that passed
      by the angle argument. This angle can be used to work out the gradient of the line the point on the circle travels along.
      
      constant_of_line_of_point_on_ball     is for storing the y-intercept of this line
      gradient_of_line_of_point_on_ball     is for storing the gradient.
   */
   
   double constant_of_line_of_point_on_ball;
   double gradient_of_line_of_point_on_ball;
   
   double new_angle;
   
   double distance;
   
   double cpx1, cpy1;
   double cpxL, cpyL;
   
   double fcpx1, fcpy1;
   
   double Ax, Ay;
   
   #ifdef SLC_LOG
	   logfile << "Beginning of stationary_line_collision().\n circle_inside = " << circle_inside << "\n";
   #endif
   
   if( fabs(v2.x - v1.x) < 0.01 ) /* line is vertical */
   {
      #ifdef SLC_LOG
	     logfile << "line VERTICAL.\n";
	  #endif
      Ax = solid_x < v1.x ? (circle_inside ? circle->radius : - circle->radius) : (circle_inside ? - circle->radius : circle->radius );
      Ay = 0.0;
         
	  cpx1 = circle->cc_x + Ax;
      cpy1 = circle->cc_y;
        
	  /* The four cases where the circle is in the right position are :
		 
		    1) The solid side is to the left of the line AND the circle is outside the polygon AND the point on the circle which could hit the line is
			   to the right of the line.
			   
			2) The solid side is to the left of the line AND the circle is inside the polygon AND the point on the circle which could hit the line is
			   to the left of the line.
			
			3)
			
			4)
	  */
      
         if( (solid_x < v1.x && (( !circle_inside && cpx1 > v1.x) || ( circle_inside && cpx1 < v1.x)) ) /* case 1 or 2 */
		     || 
			 (solid_x > v1.x && ((!circle_inside && cpx1 < v1.x) || (circle_inside && cpx1 > v1.x)))    /* case 3 or 4 */
		   )
         {
            
               /* Does the ball cross the line ? */
               
			   fcpx1 = cpx1 + cos(circle_angle) * circle->distance_increment * ftr;
               fcpy1 = cpy1 + sin(circle_angle) * circle->distance_increment * ftr;
               
			   //logfile << "angle good.\n";
			   
               //logfile << "future_x_of_closest_point_on_ball_edge "<< future_x_of_closest_point_on_ball_edge << "\n\n";
               
               if( (solid_x < v1.x && ((!circle_inside && fcpx1 < v1.x) ||
                  (circle_inside && fcpx1 > v1.x))) || (solid_x > v1.x && ((!circle_inside && fcpx1 > v1.x) ||
                    (circle_inside && fcpx1 < v1.x)) ) )
               {
                  //logfile << "ball crosses line.\n\n";
               
               
                  gradient_of_line_of_point_on_ball = tan(circle_angle);
                  constant_of_line_of_point_on_ball = fcpy1 - gradient_of_line_of_point_on_ball * fcpx1;
                     
                  cpxL = v1.x; 
                  cpyL = gradient_of_line_of_point_on_ball * v1.x + constant_of_line_of_point_on_ball;
               
                  if
				  ( 
                  
				  /* if( Solid side is to the left of line AND contact point is between the y coordinates of the vertices. ) */ 
     
 				  (solid_x < v1.x && cpyL >= v1.y && cpyL <= v2.y) || 
				  
				  /* or solid is to the right AND contact point is....... */
				  
				  (solid_x > v1.x && cpyL >= v2.y && cpyL <= v1.y)
				  
				  )
                  {
                     
				     /* The circle centre has x coordinate (cpxL - Ax) when it hits the line.
                         *                    y coordinate cpyL 				
						 */
				  
				     distance = sqrt( ((cpxL - Ax) - circle->cc_x) * ((cpxL - Ax) - circle->cc_x) + (cpyL - circle->cc_y) * (cpyL - circle->cc_y) );
				     
					 #ifdef PONGPING_CATCH_BUGS
				     /***/if( (distance / circle->distance_increment) > 1.0 )
				     /***/{
				     /***/   logfile << "stationary_line_collision() : line was VERTICAL, and circle calculated to hit line but distance required" <<
				     /***/	          " is longer than distance_increment.\n\n";
				     /***/	 logfile << "ftr was " << ftr << "\n";	  
				     /***/	 eek();
				     /***/}
				     #endif
					 
				  
                     if(circle_angle < PONGPING_PI)
                     {
                        new_angle = PONGPING_PI - circle_angle;
                     }
                     else
                     {
                        new_angle = 3.0 * PONGPING_PI - circle_angle;
					 }
					
					 move->set_members( 1, distance / circle->distance_increment, principal_value_2(new_angle), -1.0, 0.0, 0.0);
					 
					 return 1;
                  }
               }
         }
    
         return 0;
   }
   else
   {
	  /* HORIZONTAL CASE : If v2.y == v1.y (the line is horizontal), then mpL will be 0. */
      
      mpL = (v2.y - v1.y) / (v2.x - v1.x);
     
	  /* HORIZONTAL CASE : cpL will be v1.y . */
	  
      cpL = v1.y - mpL * v1.x;
   
      double aL;     /* angle of line polygon makes */
      
	  double am;     /* angle multiplier (gets set to either -1 or 1). The point on the circle which might hit the line if the circle hits the line,
                        is the point on the circle where the line which is perpendicular to the polygon side line and which goes through the
                        centre of the circle, .....intersects the circle. This line intersects the circle in two places. If the angle of the polygon
                        side line is A then the angle to the point is either (A + pi / 2) or (A - pi / 2) depending on whether the circle is above or below
                        the line respectively. So this variable is either -1.0 or 1.0 and is used to multiply with (pi / 2) when working out the point on
						the circle. */
	  
	  
	  double perp_m = - 1.0 / mpL;
	  
	  double perp_c = circle->cc_y - perp_m * circle->cc_x;
	  
	  double ix, iy;
	  
	  get_intersection_coordinates(&ix, &iy, mpL, cpL, perp_m, perp_c);
	  
	  if( sqrt( (circle->cc_x - ix) * (circle->cc_x - ix) + (circle->cc_y - iy) * (circle->cc_y - iy) ) < circle->radius ) 
	  {
	     return 0;
	  }
	  
	  
	  
	  /* HORIZONTAL CASE : aL will be 0. */
	  
      aL = atan( mpL );
   
      /* Does this work with vertical angles ? */
   
	  if( mpL * circle->cc_x + cpL < circle->cc_y ) /* Circle is below the line the side of the polygon makes. */
	  {
		  if( solid_x < mpL * solid_x + cpL )  
		  {
			 if(  circle_inside ) return 0;
		  }
		  else
		  { 
			 if( !circle_inside ) return 0;
		  }
			
		  am = -1.0;
	  }
	  else                                         /* Circle is above the line the side of the polygon makes. */                                    
	  {
		 if( solid_x < mpL * solid_x + cpL ) 
		 {
		    if( !circle_inside ) return 0;
		 }
		 else
		 {
			if(  circle_inside ) return 0;
		 }
			
		 am = 1.0;
	  }
	 
    
	
      cpx1 = circle->cc_x + circle->radius * cos( aL + am * PONGPING_PI_BY_2 );
      cpy1 = circle->cc_y + circle->radius * sin( aL + am * PONGPING_PI_BY_2 );
         
      fcpx1 = cpx1 + cos( circle_angle ) * ftr * circle->distance_increment;
      fcpy1 = cpy1 + sin( circle_angle ) * ftr * circle->distance_increment;
      
	  if( angle_is_vertical( circle_angle ) )
	  {
	     /* tan( circle_angle ) is undefined so need to do something else. */
	     
		 if( (am < 0.0 /* if circle is below the line */ && fcpy1 < mpL * circle->cc_x + cpL)    /* AND circle ends up above the line. */
		     || (am > 0.0 /* if circle is above the line */ && fcpy1 > mpL * circle->cc_x + cpL)) /* AND circle ends up below the line */
		 {
		    /* If so the circle crosses the line. */
		 
		    /* The line the contact point on the circle is travelling on is y = circle->cc_x */
		 
		    cpxL = circle->cc_x;
			
			/* Got the x value for the point on the line - so use it to get the y value. */
			
			cpyL = mpL * cpxL + cpL;
		    
			/* Is the contact point between the vertices ? */
			
			if( (v2.x > v1.x && cpxL >= v1.x && cpxL <= v2.x) || (v2.x < v1.x && cpxL <= v1.x && cpxL >= v2.x) )
			{
			   distance = sqrt( ( (cpxL + circle->radius * cos( aL - am * PONGPING_PI_BY_2 ) ) - circle->cc_x ) * 
						        ( (cpxL + circle->radius * cos( aL - am * PONGPING_PI_BY_2 ) ) - circle->cc_x ) 
						   +    ( (cpyL + circle->radius * sin( aL - am * PONGPING_PI_BY_2 ) ) - circle->cc_y ) * 
						        ( (cpyL + circle->radius * sin( aL - am * PONGPING_PI_BY_2 ) ) - circle->cc_y ) );
			   #ifdef PONGPING_CATCH_BUGS
			   /***/if( (distance > circle->distance_increment) )
			   /***/{
			   /***/   logfile << "circle angle is " << circle_angle << "\n";
			   /***/   logfile << "circle x = " << circle->cc_x << "circle y = " << circle->cc_y << "\n\n";
			   /***/   logfile << "v1.y = " << v1.y << "v1.x = " << v1.x << "\n\n";
			   /***/   logfile << "v2.y = " << v2.y << "v2.x = " << v2.x << "\n\n";
			   /***/   logfile << "stationary_line_collision() : line was NOT HORIZONTAL OR VERTICAL, and circle calculated to hit line but distance required" <<
			   /***/	          " is longer than distance_increment (CIRCLE ANGLE was vertical).\n\n";
			   /***/   logfile << "ftr was " << ftr << "\n";	  
			   /***/   logfile << "distance was " << distance << " increment " << circle->distance_increment << "\n";
			   /***/   logfile << "mpL = " << mpL << "cpL = " << cpL << "\n";
			   /***/   logfile << "cpx1 = " << cpx1 << "cpy1 = " << cpy1 << "\n";
			   /***/   logfile << "cpxL = " << cpxL << "cpyL = " << cpyL << "\n";
			   /***/   fleek();
			   /***/}
			   #endif
			   
			   move->set_members( 1, distance / circle->distance_increment, principal_value_2(atan(mpL) * 2.0 - circle_angle), -1.0, 0.0, -1.0);	
			}
		 }

 			  
		 
		 
		 
		 
		 //logfile << "stationary_line_collision : circle angle vertical with non right angled line.\n";
		 //fleek();
	  }
	  else
	  {
	     /* gradient of line of point on ball is tan(circle_angle)  */
	     
	     constant_of_line_of_point_on_ball = cpy1 - tan( circle_angle ) * cpx1;
		 
	     if( (am < 0.0 && fcpy1 < fcpx1 * mpL + cpL) || (am > 0.0 && fcpy1 > fcpx1 * mpL + cpL ) ) /* If the circle crosses the line. */
	     {
		 
		    /* Control getting here means the circle crosses. */
		 
		    /* There's two lines - the polygon line which is                               y = mpL * x + cpL
			                 and - the line the contact point is travelling on which is    y = tan( circle_angle ) * x + constant_of_line_of_point_on_ball
			
			   The intersection coordinates are needed.
			*/
		
		    cpxL =  (/* c2 */ constant_of_line_of_point_on_ball - /* c1 */ cpL) / ( /* m1 */ mpL - /* m2 */ tan( circle_angle ) );
		    cpyL = mpL * cpxL + cpL;
		    
			/* Is the contact point between the vertices ? */
		    
			if( (v2.x > v1.x && cpxL >= v1.x && cpxL <= v2.x) || (v2.x < v1.x && cpxL <= v1.x && cpxL >= v2.x) )
			{
			   /* The circle centre has x coordinate  cpxL + circle->radius * cos( ap - am * PONGPING_PI_BY_2 ) when it hits the line.
                *                       y coordinate  cpyL + circle->radius * sin( ap - am * PONGPING_PI_BY_2 )						
				*/
				
			   distance = sqrt( ( (cpxL + circle->radius * cos( aL - am * PONGPING_PI_BY_2 ) ) - circle->cc_x ) * 
						        ( (cpxL + circle->radius * cos( aL - am * PONGPING_PI_BY_2 ) ) - circle->cc_x ) 
						   +    ( (cpyL + circle->radius * sin( aL - am * PONGPING_PI_BY_2 ) ) - circle->cc_y ) * 
						        ( (cpyL + circle->radius * sin( aL - am * PONGPING_PI_BY_2 ) ) - circle->cc_y ) );
			   
			   
			   #ifdef PONGPING_CATCH_BUGS
			   /***/if( (distance > circle->distance_increment) )
			   /***/{
			   /***/   logfile << "circle angle is " << circle_angle << "\n";
			   /***/   logfile << "circle x = " << circle->cc_x << "circle y = " << circle->cc_y << "\n\n";
			   /***/   logfile << "v1.y = " << v1.y << "v1.x = " << v1.x << "\n\n";
			   /***/   logfile << "v2.y = " << v2.y << "v2.x = " << v2.x << "\n\n";
			   /***/   logfile << "stationary_line_collision() : line was NOT HORIZONTAL OR VERTICAL, and circle calculated to hit line but distance required" <<
			   /***/	          " is longer than distance_increment.\n\n";
			   /***/   logfile << "ftr was " << ftr << "\n";	  
			   /***/   logfile << "distance was " << distance << " increment " << circle->distance_increment << "\n";
			   /***/   logfile << "mpL = " << mpL << "cpL = " << cpL << "\n";
			   /***/   logfile << "cpx1 = " << cpx1 << "cpy1 = " << cpy1 << "\n";
			   /***/   logfile << "cpxL = " << cpxL << "cpyL = " << cpyL << "\n";
			   /***/   fleek();
			   /***/}
			   #endif
			   
			   //logfile << "for line collision, circle_angle before hit was " << circle_angle << "\n";
			   
			   move->set_members( 1, distance / circle->distance_increment, principal_value_2(atan(mpL) * 2.0 - circle_angle), -1.0, 0.0, -1.0);	
               			   
               return 1;
			}
			
		 }
	  }
   }  
		   
   //logfile << "no hit : returning 0.\n";

   return 0;

}


 






/* circle_and_rotating_line_collision() : checks if a circle moving in a straight line, collides with a
 * straight line which is rotating around a fixed point
 *
 * WHAT IT DOES
 * 
 * checks if a circle moving in a straight line, collides with a
 * straight line which is rotating around a fixed point
 *
 * ARGUMENTS
 *
 *  c      : pointer to the circle (represented by a Circle class
 *
 *  cr_x   : x coordinate of centre of rotation of the polygon 
 *
 *  cr_t   : y coordinate of centre of rotation of the polygon 
 *
 *     pai : polygon angle increment : angle in radians for how much the polygon rotates per update
 *     
 *  v1, v2 : the two vertices
 *  
 *    move : a pointer to the PotentialMove class where the data is kept to implement the move if it's turns out to be the next 
 *    one
 */
int circle_and_rotating_line_collision(Circle *c, const double ftr, const double cr_x, const double cr_y, const double pai, Point v1, Point v2, PotentialMove *move)
{
   Point v1_s, v2_s; /* v1 shifted and v2 shifted by radius of circle in direction perpendicular to side of polygon away from centre of rotation */
   double rda;       /* reverse displacement angle */
   
   double new_x, new_y;
   
   double distance_for_hit;
   
   double new_circle_angle;
   
   double m_for_polygon_side_line;
   double c_for_polygon_side_shifted_line;
   
   int polygon_side_has_infinite_gradient = 0;
  
   
   /* Work out the gradient of the line between the vertices. */
   
   m_for_polygon_side_line = (v2.y - v1.y) / (v2.x - v1.x);
	  
   
   //logfile << "v1.x = " << v1.x << "v1.y = " << v1.y << "v2.x = " << v2.x << "v2.y = " << v2.y << "\n"; 
   
   
   double da = ((v2.x == v1.x) ? ((v2.y > v1.y) ? PONGPING_PI_BY_2 : PONGPING_3_PI_BY_2) : principal_value_2(atan2( (v2.y - v1.y) , (v2.x - v1.x) ))) - PONGPING_PI_BY_2;
   
   
   
   if( v2.x == v1.x )
   {
      polygon_side_has_infinite_gradient = 1;
   }   

   
   rda = da + PONGPING_PI;
   principal_value(&rda);
   
  
   v1_s.x = v1.x + c->radius * cos(da);
   v1_s.y = v1.y + c->radius * sin(da);
   v2_s.x = v2.x + c->radius * cos(da);
   v2_s.y = v2.y + c->radius * sin(da);
  
   if(polygon_side_has_infinite_gradient)
   {
      ;
   }
   else
   {
      c_for_polygon_side_shifted_line = v2_s.y - m_for_polygon_side_line * v2_s.x;
   }
  
   const double k = c->distance_increment / pai;                 // for storing the constant value : (distance circle travels) / (polygon increment angle)
   const double xL = v1_s.x < v2_s.x ? v1_s.x - cr_x : v2_s.x - cr_x; 
   const double yL = v1_s.x < v2_s.x ? v1_s.y - cr_y : v2_s.y - cr_y;
   const double xR = v1_s.x < v2_s.x ? v2_s.x - cr_x : v1_s.x - cr_x;
   const double yR = v1_s.x < v2_s.x ? v2_s.y - cr_y : v1_s.y - cr_y;
   const double sL = sqrt( (xR - xL) * (xR - xL) + (yR - yL) * (yR - yL) ); // length of polygon side
   
   if(sL == 0.0) 
   {
      logfile << "circle_and_rotating_line_collision() : side length 0.\n";
      eek();
   }
   
   const double xt =  ((xL * (xR - xL)) + (yL * (yR - yL))) / sL;  // xt : x translation for the spiral
   const double yt = (yL * xR - xL * yR) / sL;                     // yt : y translation for the spiral
   const double cos_psa = (xR - xL) / sL;      // cosine of the angle side of polygon makes 
   const double sin_psa = (yR - yL) / sL;      // sine of the angle side of polygon makes
   double pc[7]; // polynomial coeffecients
   double z[ 12 ]; // array for real and imaginary parts of solutions to polynomial
   int i;
   gsl_poly_complex_workspace *w = gsl_poly_complex_workspace_alloc (7);
   
   double r, pA;
   
   double a = c->cc_x - cr_x;
   double b = c->cc_y - cr_y;
   double psa = (xR == xL) ? ((yR > yL) ? PONGPING_PI_BY_2 : PONGPING_3_PI_BY_2 ) : principal_value_2(atan2( yR - yL, xR - xL ));
   
   //logfile << "sL = "<< sqrt( (xR - xL) * (xR - xL) + (yR - yL) * (yR - yL) ) << "\n";
   
   //logfile << "(xR - xL) = " << (xR - xL) << " (yR - yL) = " << (yR - yL) << "\n";
   
   //logfile << "xL = " << xL << "yL = " << yL << "xR = " << xR << "yR = " << yR << "\n";
   
   //logfile << "cos_psa = " << cos_psa << "sin_psa = " << sin_psa << "\n";
   
   
   
   pc[0] = b * cos_psa - a * sin_psa - yt;
   pc[1] = k * sin(c->circle_angle) * cos_psa - a * cos_psa - k * cos(c->circle_angle) * sin_psa - b * sin_psa;
   pc[2] = - b * cos_psa / 2.0f + a * sin_psa / 2.0f - k * sin(c->circle_angle) * sin_psa  - k * cos(c->circle_angle) * cos_psa;
   pc[3] = k * sin(c->circle_angle) * cos_psa / 2.0f + a * cos_psa / 6.0 + b * sin_psa / 6.0f + k * cos(c->circle_angle) * sin_psa / 2.0f;
   pc[4] = b * cos_psa / 24.0f - a * sin_psa / 24.0f + k * cos(c->circle_angle) * cos_psa / 6.0f + k * sin(c->circle_angle) * sin_psa / 6.0f;
   pc[5] = k * sin(c->circle_angle) * cos_psa / 24.0f - k * cos(c->circle_angle) * sin_psa / 24.0f - a * cos_psa / 120.0f - b * sin_psa / 120.0f;
   pc[6] = - k * cos(c->circle_angle) * cos_psa / 120.0f - k * sin(c->circle_angle) * sin_psa / 120.0f;
   
   //logfile << pc[0] << pc[1] << pc[2] << pc[3] << pc[4] << pc[5] << pc[6] << "\n";
   
   gsl_poly_complex_solve(pc, 7, w, z);

   for(i = 0; i < 6; ++i)
   {    
      if( z[ 2 * i + 1] == 0.0)
      {
         r = sqrt( ( b + k * z[2*i] * sin( c->circle_angle )) * ( b + k * z[2*i] * sin( c->circle_angle )) + 
		           ( a + k * z[2*i] * cos( c->circle_angle )) * ( a + k * z[2*i] * cos( c->circle_angle )) );
    
         pA = atan2( b + k * z[2*i] * sin( c->circle_angle ), a + k * z[2*i] * cos( c->circle_angle ));

         if( (( pai <= 0.0 && z[2*i] <= 0.0 && z[2*i] >= (pai * ftr)) || (pai >= 0.0 && z[2*i] >= 0.0 && z[2*i] <= (pai * ftr))) && (r * cos( pA - z[2*i] - psa) - xt >= 0) && (r * cos( pA - z[2*i] - psa ) - xt <= sL) )        
         {
            
            double hit_x, hit_xr;
            double hit_y, hit_yr;
            
            //logfile << "degrees required to hit " << (z[2*i] * 180.0f / PONGPING_PI) << "\n"; 
            
            rotate_point( psa, r * cos( pA - z[2*i] - psa), r * sin( pA - z[2*i] - psa), &hit_x, &hit_y);
            rotate_point( z[2*i], hit_x, hit_y, &hit_xr, &hit_yr);
            
            new_x = hit_xr + cr_x;
            new_y = hit_yr + cr_y;
            
			
			
			
            double xLr, yLr, xRr, yRr; /* for storing where the coordinates of the ends of the line where it
                                          it is when the circle hits it */
            
            /* rotate points (xL, yL) and (xR, yR) to where they are when circle hits it so the angle of it
               can be worked out. This is needed to determine the angle the circle bounces off at */
			   
			
            
            rotate_point(z[2*i], xL, yL, &xLr, &yLr);
            rotate_point(z[2*i], xR, yR, &xRr, &yRr);
            
			double m_for_line_where_circle_hits;
			
			/********************************************************************************/
			/**** Work out where the actual line is where it is when the circle hits it. ****/
			/********************************************************************************/
			
			rda += z[2*i];
			
			principal_value(&rda);
			
			double xR_actual = xRr + c->radius * cos( rda );
			double yR_actual = yRr + c->radius * sin( rda );
			
			if(xRr == xLr) /* line where it is when circle hits is vertical */
			{
			   if(polygon_side_has_infinite_gradient) /* line where it was is also vertical so it hasn't moved. */
			   {
			      if( side_of_line(0, 0, c->cc_x, c->cc_y, PONGPING_INFINITE_GRADIENT, v1_s.x) !=
				      side_of_line( 0, 0, hit_xr, hit_yr, PONGPING_INFINITE_GRADIENT, xRr) )
                  {
				     return 0;
				  }
			   }
			   else
			   {
			      /* If the side of the line where it is when it started, the circle was on where it started is not the same
				     as the side of the line it's on where the line is when the circle hits it. */
			   
			      if( side_of_line( m_for_polygon_side_line, c_for_polygon_side_shifted_line, c->cc_x, c->cc_y, 0, 0) !=
				      side_of_line( 0, 0, hit_xr, hit_yr, PONGPING_INFINITE_GRADIENT, xRr) )
				  {
				     return 0;
				  }
			   }
			}
			else if( xR == xL )
			{
			   logfile << "circle_and_rotating_line_collision() : problem 6.\n";
			   eek();
			}
			else
			{
			   m_for_line_where_circle_hits = ( yRr - yLr ) / ( xRr - xLr ); 
			
			   double c_for_line_where_circle_hits = yR_actual - xR_actual * m_for_line_where_circle_hits;
			
			   if( side_of_line( m_for_polygon_side_line, c_for_polygon_side_shifted_line, c->cc_x, c->cc_y, 0, 0) !=
			       side_of_line( m_for_line_where_circle_hits, c_for_line_where_circle_hits, hit_xr, hit_yr, 0, 0) )
			   {
			      
				  gsl_poly_complex_workspace_free (w);
			      return 0;
			   }
			}
			
			/**************************************************/
			/***** Work out angle circle comes off at. ********/
			/**************************************************/
			
            /* angle of side where it is when circle hits it is atan2( yRr - yLr, xRr - xLr ) */
            
			
			
            new_circle_angle = principal_value_2(2.0f * (xRr == xLr ? PONGPING_PI : atan2( yRr - yLr, xRr - xLr )) - c->circle_angle);;
         
		    distance_for_hit = sqrt( (new_x - c->cc_x) * (new_x - c->cc_x) + ( new_y - c->cc_y) * (new_y - c->cc_y) );
		    
			
			//int set_members(int nc, double _pfot, double _a1, double _new_di_1, double _a2, double _new_di_2 )
			
			move->set_members(1, distance_for_hit / c->distance_increment, new_circle_angle, -1.0, 0.0, -1.0);
		 
		 
		 
		 
            /* free memory */
			
			gsl_poly_complex_workspace_free (w);
            
			return 1;
         }
      }
      
   }
   
   
   /* free memory */
   
   gsl_poly_complex_workspace_free (w);
   
   return 0;
}





























int side_of_line(const double m, const double c, const double cx, const double cy, int gradient_infinite, const double line_X)
{

   // y = m*x + c
   
    
   if(gradient_infinite)
   {
      if( cx > line_X )
	  {
	     return PONGPING_RIGHT_SIDE_OF_LINE;
	  }
	  else if(cx < line_X )
	  {
	     return PONGPING_LEFT_SIDE_OF_LINE;
	  }
	  else
	  {
	     logfile << "side_of_line : point on line.\n";
		 eek();
	  }
   }
	
   if( m > 0.0 ) 
   {  
   
      if( cy < m * cx + c )
	  {
	     //logfile << "right\n";
		 //logfile << cy << " < " << m << " * " << cx << " + " << c << " = " << (m * cx + c) << "\n\n";
         return PONGPING_RIGHT_SIDE_OF_LINE;
	  }
	  else if( cy > m * cx + c )
	  {
	     //logfile << cy << " > " << m << " * " << cx << " + " << c << " = " << (m * cx + c) << "\n\n";
	  
	     //logfile << "left.\n";
	  
	     return PONGPING_LEFT_SIDE_OF_LINE;
	  }
	  else
	  {
	     logfile << "side_of_line : point on line.\n";
		 eek();
	  }
   
   }
   else if( m < 0.0 )
   {
      if( cy < m * cx + c )
	  {
         return PONGPING_LEFT_SIDE_OF_LINE;
	  }
	  else if( cy > m * cx + c )
	  {
	     return PONGPING_RIGHT_SIDE_OF_LINE;
	  }
	  else
	  {
	     logfile << "side_of_line : point on line.\n";
		 eek();
	  }
   }
   else if( m == 0.0 )
   {
      if( cy > c )
	  {
	     return PONGPING_BELOW_LINE;
	  }
	  else if( cy < c )
	  {
	     return PONGPING_ABOVE_LINE;
	  }
	  else
	  {
	     logfile << "side_of_line : point on line.\n";
		 eek();
	  }
   }
 
}






