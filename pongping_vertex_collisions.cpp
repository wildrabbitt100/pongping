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

 

 
#include "pongping_headers/pongping_vertex_collisions_headers.h"




#define PONGPING_ACTUAL_FRAME 0
#define PONGPING_MOVING_FRAME 1


//#define LOG_CVC


int cvc_linear_internal( Circle *circle, double a, double d, Point v, const double ocx, const double ocy, const double om, int use_centre_of_mass, PotentialMove *pm, const int frame, const double dx, const double dy, const double object_mass_displacement_x, const double object_mass_displacement_y );

static int circle_and_rotating_vertex_collision(Circle *c, const double cr_x, const double cr_y, const double pai, Point v, PotentialMove *move, double ftr);



/* cvc 

NAME   

The letters stand for circle vertex collision. The functions checks for collisions between circles and vertices of polygons. It only deals with circles
moving in a straight line with constant velocity.

ARGUMENTS

vertex_move : a flag which should be one of PONGPING_STILL (when the vertex isn't moving), PONGPING_TRANSLATION (when a vertex is moving in a straight line) or
              PONGPING_ROTATION
			  
cx : the x coordinate of the centre of rotation (used when vertex_move is PONGPING_ROTATION)

cy : the y coordinate of the centre of rotation (used when vertex_move is PONGPING_ROTATION)


ocx :

ocy :



*/
int cvc(const unsigned char vertex_move_type, const double cx, const double cy, const int use_centre_of_mass, 
        const double ocx, const double ocy, const double om, const Point v, const double angle, const double dx, const double dy, Circle *c, PotentialMove *move,
	    const double ftr)
{

   double x2, y2, oa;
   PotentialMove *dummy_pm = new PotentialMove();
   Point v2;
   
   
   double vd; /* vertex distance : the distance the vertex moves if it hits the circle. */
   
   double a2; // angle between position of vertex where it is and where it will be if the circle hits it
   
   
   
   double od; /* other distance - ie distance in other (moving) frame */
   
   /* x2 and y2 and where the circle moves to in the frame of reference of the vertex. */
   
   
   if( vertex_move_type == PONGPING_TRANSLATION )
   {
      
    	  
	  /* Work out od. */
	  
	  //logfile << "in cvc - checking vertex TRANSLATING in moving frame \n";
	  
      od = sqrt( ( c->distance_increment * ftr * cos( c->circle_angle ) - dx * ftr) * ( c->distance_increment * ftr * cos( c->circle_angle ) - dx * ftr ) 
	           + ( c->distance_increment * ftr * sin( c->circle_angle ) - dy * ftr) * ( c->distance_increment * ftr * sin( c->circle_angle)) - dy * ftr );
     
      oa = principal_value_2(atan2(  c->distance_increment * ftr * sin( c->circle_angle ) - dy, c->distance_increment * ftr * cos( c->circle_angle ) - dx * ftr));
      
	  #ifdef LOG_CVC
	     logfile << "**********************************************************************\n\n";
	     logfile << "calling cvc_linear_internal()\n\n";
	     logfile << " circle distance increment = " << c->distance_increment << "\n";
		 logfile << "              circle angle = " << c->circle_angle       << "\n";
	     logfile << "                       ftr = " << ftr << "\n\n";
	     logfile << "                        od = " << od << "\n";
		 logfile << "                        oa = " << oa << "\n";
		 logfile << "**********************************************************************\n\n";
	  #endif
     
      if( cvc_linear_internal(c, oa, od, v, ocx, ocy, om, use_centre_of_mass, dummy_pm, PONGPING_MOVING_FRAME, dx, dy, 0, 0) )
      {
       
         
		 //logfile << "*********************************hits in MOVING FRAME.*****************************************\n\n";
         //logfile << "dummy_pm->pfot = " << dummy_pm->pfot << "\n";
		 //#endif
         vd = sqrt(dx * dx + dy * dy) * dummy_pm->pfot;
         
         
         if(dx == 0 && dy == 0)
         {
            v2.x = v.x;
            v2.y = v.y;
         }
         else
         {
            a2 = principal_value_2( atan2( dy, dx ) );
           
            v2.x = v.x + vd * cos(a2);
            v2.y = v.y + vd * sin(a2);
         }
         
		 #ifdef LOG_CVC
		 logfile << "\n  HITS IN MOVING FRAME.\n";
		 
		 logfile << "v2.x = " << v2.x << "\n";
	     logfile << "v2.y = " << v2.y << "\n";
           
		 #endif
		 
		 
		 //logfile << "***************************************\nCHECKING REAL FRAME.\n";
		 
		 //int check_collision_against_vertex( Circle *circle, const double a, const double d, Point v, const double ocx, const double ocy, const double om, int //use_centre_of_mass, PotentialMove *pm, const int frame, const double dx, const double dy );
		 
         if( cvc_linear_internal(c, c->circle_angle, c->distance_increment * ftr, v2, ocx, ocy, om, use_centre_of_mass, move, PONGPING_ACTUAL_FRAME, dx, dy, v2.x - v.x, v2.y - v.y) )
         {
		    
			//cout << "got collision in real frame.\n";
			//eek();
			
			
			
			return 1;
         }
		 else 
		 {
		    logfile << "No hit with second call.\n";
		    logfile << "No hit with second call. Printing out info : \n\n";
			logfile.flush();
			fleek();
		 }
      }
	  else
	  {
	     //logfile << "no hit in MOVING FRAME\n\n";
	     #ifdef LOG_CVC
		 
		 logfile << "BACK IN cvc()\n\n";
		 
	     logfile << "cvc() : no hit .\n";
			
	     logfile << "circle x = " << c->cc_x << "\n";
		 logfile << "circle y = " << c->cc_y << "\n";
		 
		 logfile << "v.x = " << v.x << "\n";
		 logfile << "v.y = " << v.y << "\n";
            
	     //logfile << "distance necessary for hit in moving frame : " << dummy_pm->distance << "\n";
			
	     logfile << "oa = " << oa << " od = " << od << "\ndx = " << dx << " dy = " << dy << "\n";
			
	     logfile << "circle angle = " << c->circle_angle << "\n";
		 logfile << "circle remaining distance = " << c->remaining_distance << "\n";
	     #endif
	  }
     
   }
   else if(vertex_move_type == PONGPING_ROTATION)
   {
      //circle_and_rotating_vertex_collision(Circle *c, const double cr_x, const double cr_y, const double pai, Point v, PotentialMove& move);
   
      return circle_and_rotating_vertex_collision( c, cx, cy, angle, v, move, ftr);
   
   }
   
   
   return 0;

}





/*

cvc_linear_internal : 

The cvc stands for circle vertex collision which means it deals with circles colliding with vertices. The word linear is in the name because the function
deals with circles when they are moving in a straight line. The internal emphasises that this function is only used and accessible in this file. 

ARGUMENTS

a : angle circle is travelling at which might be the actual angle in reality or the angle in the moving frame. That's the reason this argument is
    needed - the angle the circle is travelling at is stored in the Circle class but it's not always the angle that is used.
	
d : the distance the circle is due to move - this is supposed to have already been multiplied by the fraction (proper fraction) of time remaining.

v : is the vertex


omx - object mass x
omy - object mass y

om : 

use_centre_of_mass :

pm :

frame :

dx :

dy :

object_mass_displacement_x :

object_mass_displacement_y :

*/

//#define CVC_LI_DEBUG


int cvc_linear_internal( Circle *circle, double a, double d, Point v, const double omx, const double omy, const double om, int use_centre_of_mass, PotentialMove* pm, const int frame, const double dx, const double dy, const double object_mass_displacement_x, const double object_mass_displacement_y )
{
   double cctvd = sqrt( (circle->cc_x - v.x) * (circle->cc_x - v.x) + (circle->cc_y - v.y) * (circle->cc_y - v.y)    ); // circle centre to vertex distance
   
   double m; // gradient of line through vertex with the same angle as the circle is travelling.
   double c; // constant of line which the circle is travelling along (y-intercept).
   
   double discriminant_for_x;   /* This is used in working out the x coordinate of the point where the line touches the circle. */
   double discriminant_for_y;   /* This one for the y coordinate of the point where the line touches the circle. */
   
   double mfyd; /* multiplier for y discrimant */
   
   double tx, ty; /* x and y values of where line touches circle */
   
   double dnfh; /* Distance necessary for hit. */
   
   long double xLEFT, xRIGHT;
   
   //cout << "in check collision against vertex.\n\n";
   
   double ctva = principal_value_2( atan2( v.y - circle->cc_y, v.x - circle->cc_x) );
   
   
   
   
   //cout << "ctva = " << (ctva * 57.3) << "\n";
   
   //double Qx, Qy;  /* This is point P which is the centre of mass of the circle / bat */
   
   // distance between mass centres
   //double dbmc = sqrt( (circle->cc_x - omx) * (circle->cc_x - omx) + (circle->cc_y - omy) * (circle->cc_y - omy) ); 
   
   //double angle_to_bat_centre = atan2( omy - circle->cc_y, omx - circle->cc_x );
   
   //#ifdef VC_DEBUG
   //
   //logfile << "angle to bat centre = " << angle_to_bat_centre << "\n";
   
   //logfile << "dbmc (distance between mass centres) = " << dbmc << "\n";
   //logfile << "omy = " << omy << "omx = " << omx << "\n\n";
   
   //#endif
   
   //double mass_sum = om + circle->mass;
   
   //double cmx1 = circle->cc_x + ((om / mass_sum) * dbmc) * cos( angle_to_bat_centre );
   //double cmy1 = circle->cc_y + ((om / mass_sum) * dbmc) * sin( angle_to_bat_centre );
   
   double cmx2, cmy2;
   
   double dbmc2;
   double avc;
   
   
   double rlg; /* rebound line gradient */
   
   
   double c2;
   
   double ava;
   double avm;
   double new_angle = 0;
   double new_magnitude = 0;
   
   double angle_to_bat_centre_2;
   
   
   
   #ifdef CVC_LI_DEBUG
      logfile << "********************************************************\n";
      logfile << "**** ENTERED FUNCTION ******* cvc_linear_internal() ****\n";
      logfile << "********************************************************\n\n";
      //logfile << "om = " << om << " circle mass = " << circle->mass << "\n";
      //logfile << "cc_x = " << circle->cc_x << "   cc_y =  " << circle->cc_y << " cmx = " << cmx << "   cmy = " << cmy << "\n"; 
   #endif
   
   // circle centre to bat centre
   //double cc_to_bc = sqrt( circle->cc_x -  ) ;
   
   if( cctvd < circle->radius )
   {
      #ifdef CVC_LI_DEBUG
	     logfile << "Distance from circle centre to vertex is less than the radius of the circle (vertex is inside circle). Needs work.\n\n";         
	     fleek();
	  #endif
   }
   else
   {
      /* Vertex is outside the circle - see if the angle is appropriate for where the vertex is. */
  
      if( !angle_appropriate( circle->circle_angle, principal_value_2( ctva - PONGPING_PI_BY_2), principal_value_2(ctva + PONGPING_PI_BY_2 )) )
      {
	     //logfile << "returning 0 because angle no good.\n";
	     //cout << "circle angle is " << circle->circle_angle << "\n\n";
	     //cout << "lower angle = " << (principal_value_2(ctva - PONGPING_PI_BY_2) * 57.3) << "\n";
	     //cout << "upper angle = " << (principal_value_2(ctva + PONGPING_PI_BY_2) * 57.3) << "\n";
         return 0;
      }
   }	  
   
   
   
  
   
   //cout << "angle of " << a << "okay.\n\n";
   
   /* Work out the line (y = m * x + c) which has the gradient equal to the angle the circle is travelling in and which
      goes through the vertex. */
   
   /* Must use a here , not circle->circle_angle */
   
   if( a == PONGPING_PI_BY_2 || a == PONGPING_3_PI_BY_2 )
   {
      logfile << "check_collision_against_vertex() : angle is 90 or 270 \n";
	  fleek();
   }
   
   m = tan(a);
   
   //logfile << "m = " << m << "\n";
   
   c = v.y - m * v.x;


   /* Does the line touch the circle ? */
  
  
   #ifdef CVC_LI_DEBUG
   /***/ logfile << "calling get_discriminants(..., ..., circle x = " << circle->cc_x << " , circle y = " << circle->cc_y << " , m = " << m << ", c = ";
   /***/ logfile << c << ", radius = " << circle->radius << ");\n\n";
   #endif
   
   if( get_discriminants_( &discriminant_for_x, &discriminant_for_y, circle->cc_x, circle->cc_y, m, c, circle->radius) == PONGPING_SOLUTIONS_IMAGINARY )
   {
      #ifdef CVC_LI_DEBUG
      /***/ logfile << " circle doesn't hit vertex because solutions to line and circle intersection not real.\n";
      #endif
      return 0;
   }
   
   
   #ifdef CVC_LI_DEBUG
       logfile << "called get discriminants.\n";
   #endif
   
   //logfile << "intersection real.\n";
   
   
   /* Work out whether to add or subtract sqrt(take_root_of_x) and my * sqrt(take_root_of_y) */
   
   
   if( cctvd < circle->radius )
   {
      logfile << " vertex inside circle. What's going on ? " << "\n";
	  fleek();
   
      /*
      #ifdef VC_DEBUG
      logfile << "inside circle.\n";
      #endif
	    
      if( a > 0.0 && a < PONGPING_PI_BY_2 )
	  {
	     tx = (circle->cc_x + m * circle->cc_y - m * c - sqrt(take_root_of_x) ) /  (1.0 + m * m);
		 ty = (c + circle->cc_x * m + circle->cc_y * m * m - sqrt(take_root_of_y)) / (1.0 + m * m);
	  }
	  else if( a > PONGPING_PI_BY_2 && a < PONGPING_PI )
	  {
	     tx = (circle->cc_x + m * circle->cc_y - m * c + sqrt(take_root_of_x) ) /  (1.0 + m * m);
		 ty = (c + circle->cc_x * m + circle->cc_y * m * m - sqrt(take_root_of_y)) / (1.0 + m * m);
	  
	  }
	  else if( a > PONGPING_PI && a < PONGPING_3_PI_BY_2 )
	  {
	     tx = (circle->cc_x + m * circle->cc_y - m * c + sqrt(take_root_of_x) ) /  (1.0 + m * m);
		 ty = (c + circle->cc_x * m + circle->cc_y * m * m + sqrt(take_root_of_y)) / (1.0 + m * m);
	  }
	  else if( a > PONGPING_3_PI_BY_2 && a < PONGPING_2_PI )
	  {
	     tx = (circle->cc_x + m * circle->cc_y - m * c - sqrt(take_root_of_x) ) /  (1.0 + m * m);
		 ty = (c + circle->cc_x * m + circle->cc_y * m * m + sqrt(take_root_of_y)) / (1.0 + m * m);
	  }
      else
	  {
	      cout << "angle not dealt with.\n";
		  eek();
	  }
   
      */
   }
   else
   {
   
      //logfile << "vertex not inside circle.\n";
   
      xLEFT = (circle->cc_x + m * circle->cc_y - m * c - sqrt(discriminant_for_x) ) /  (1.0 + m * m);
   
      
      xRIGHT = (circle->cc_x + m * circle->cc_y - m * c + sqrt(discriminant_for_x) ) /  (1.0 + m * m);
  
      
      
   
      if( fabs(v.x - xLEFT) < fabs(v.x - xRIGHT) ) // xLEFT is closer to the vertex so use xLEFT
      {
	     #ifdef VC_DEBUG
	     logfile << "using xLEFT.\n";
	     #endif
         tx = xLEFT;
	     
         mfyd = m < 0.0 ? 1.0 : -1.0; 
		 
		 /* What if m == 0 ? */
      }
      else // xRIGHT is closer to vertex so use xRIGHT
      {
	     #ifdef VC_DEBUG
	     logfile << "using xRIGHT.\n";
	     #endif
         tx = xRIGHT;
   
         mfyd = m < 0.0 ? -1.0 : 1.0;
	     
		 /* What if m == 0 ? */
		 
      }
   }
   
   ty = (c + circle->cc_x * m + circle->cc_y * m * m + mfyd * sqrt(discriminant_for_y)) / (1.0 + m * m);
  
   
   
   // work out distance from this point to the vertex
   
   // angle to point
   
   
   #ifdef VC_DEBUG
   
   logfile << "tx = " << tx << "\n";
   
   logfile << "tx - circle->cc_x = " << tx - circle->cc_x << "\n";
   
   logfile << "ty = " << ty << "\n";
   
   logfile << "ty - circle->cc_y = " << ty - circle->cc_y << "\n";
   
   #endif
   
   double atp = principal_value_2( atan2( ty - circle->cc_y, tx - circle->cc_x ) );
   
   //cout << "atp = " << atp << "\n";
   
   
  
   #ifdef VC_DEBUG
   logfile << "v.y = " << v.y << "\n";
   logfile << "v.x = " << v.x << "\n";
   
   logfile << "(v.y - ty) = " << (v.y - ty) << "\n";
   logfile << "(v.x - tx) = " << (v.x - tx) << "\n";
   #endif
   
   dnfh = sqrt( (v.y - ty) * (v.y - ty) + (v.x - tx) * (v.x - tx) );
   
   if( dnfh > d )
   {
      //logfile << "circle doesn't move far enough. Necessary distance was " << dnfh << " remaining distance : " << d << "\n\n";
	  //cout << "tx = " << tx << "ty = " << ty << "\n\n";
      return 0;
   }
   
   /* Circle does hit vertex if control gets here. */
   
   #ifdef VC_DEBUG
   logfile << "************************************** HIT ********************************************************\n\n";
   //circle_centre_to_bat_centre = circle->cc_x - ;
   #endif
   
   rlg = tan( atp + PONGPING_PI_BY_2 );
   
   //double rlc;
   
   // adjustment vector gradient
   //double avg = - 1.0 / rlg;
   
   //logfile << " rlg = " << rlg << " avg = " << avg << "\n";  
   
   //double cmx1 = circle->cc_x + ((om / mass_sum) * dbmc) * cos( angle_to_bat_centre );
   //double cmy1 = circle->cc_y + ((om / mass_sum) * dbmc) * sin( angle_to_bat_centre );
   
   /* If the collision is in the actual frame then more than just whether or not the circle hits the vertex needs to be worked out. */
   /* Also need to work out the adjustment vector due the mass of the system acting on the circle. */
   
   
   //if(frame == PONGPING_ACTUAL_FRAME)
   //{
   //   angle_to_bat_centre_2 = atan2( (omy + object_mass_displacement_y) - new_circle_y, (omx + object_mass_displacement_x) - new_circle_x);
   //   
   //   dbmc2 = sqrt( (new_circle_x - (omx + object_mass_displacement_x) ) * (new_circle_x - (omx + object_mass_displacement_x)) + (new_circle_y - (omy + //object_mass_displacement_y)) * (new_circle_y - (omy + object_mass_displacement_y)) ); 
   //
   //   cmx2 = new_circle_x + ((om / mass_sum) * dbmc2) * cos( angle_to_bat_centre_2 );
   //
   //   cmy2 = new_circle_y + ((om / mass_sum) * dbmc2) * sin( angle_to_bat_centre_2 );
   //	  
	  /* Get the constant for the line which goes through the centre of mass of the circle and object where they are when they hit
	     and has the gradient of the rebound line. */
	  
   //	  c2 = cmy2 - rlg * cmx2;
	  
   //	  avc = cmy1 - cmx1 * avg;
	  
	  /* So there are two lines :  y = (rlg) * x + c2  and y = (avg) * x + (avc) */
	  /* Need to find the point of intersection of these two lines. */
	  
   //	  get_intersection_coordinates( &Qx, &Qy, rlg, c2, avg, avc ); 
	  /* Work out the angle and magnitude of the adjustment vector. */  
   //	  ava = principal_value_2(atan2( Qy - cmy1, Qx - cmx1 ));
//	  avm = sqrt( (Qy - cmy1) * (Qy - cmy1) + (Qx - cmx1) * (Qx - cmx1) );
	  
//	  add_vectors( principal_value_2( 2.0f * (atp + PONGPING_PI_BY_2) - circle->circle_angle ), circle->distance_increment, ava, avm, &new_angle, &new_magnitude); 
      
	  
	  
	  
//   }
   
   //pm->set_potential_move_values( circle->cc_x, circle->cc_y, new_circle_x, new_circle_y, , 1, , PONGPING_BAT_COLLISION, dfh / d);  
   
   //int set_members(int nc, double _pfot, double _a1, double _new_di_1, double _a2, double _new_di_2 )
   
   pm->set_members(1, dnfh / circle->distance_increment, principal_value_2(2.0 * rlg - a), -1.0, 0, 0);
   
   return 1;
}

  

  
static int circle_and_rotating_vertex_collision(Circle *c, const double cr_x, const double cr_y, const double pai, Point v, PotentialMove *move, double ftr)
{
   const double k = c->distance_increment / pai;                 // for storing the constant value : (distance circle travels) / (polygon increment angle)
   
   double cosa = cos( c->circle_angle );
   double sina = sin( c->circle_angle );
   
   double xv = v.x - cr_x;
   double yv = v.y - cr_y;
   double a = c->cc_x - cr_x;
   double b = c->cc_y - cr_y;
   
   double pc[7]; // polynomial coeffecients
   double z[ 12 ]; // array for real and imaginary parts of solutions to polynomial
   int i;
   
   double c2 = xv * xv + yv * yv - c->radius * c->radius;
   
   double dnfh;
   
   //cout << "2";
   
   gsl_poly_complex_workspace *w = gsl_poly_complex_workspace_alloc (7);
   
   pc[0] = - (2.0f * a * xv) - (2.0f * b * yv) + a * a + b * b + c2;
   pc[1] = - (2.0f * yv * k * sina) + (2.0f * yv * a) - (2.0f * xv * k * cosa) - (2.0f * xv * b) + 2.0 * a * k * cosa + 2.0 * b * k * sina;  
   pc[2] = yv * b + 2.0f * yv * k * cosa + xv * a - 2.0f * xv * k * sina + k * k;
   pc[3] = yv * k * sina - yv * a / 3.0f + xv * k * cosa + xv * b / 3.0f;
   pc[4] = - yv * b / 12.0f - yv * k * cosa / 3.0f - xv * a / 12.0f + xv * k * sina / 3.0f;
   pc[5] =  - yv * k * sina / 12.0f + yv * a / 60.0f - xv * k * cosa / 12.0f - xv * b / 60.0f;
   pc[6] = yv * k * cosa / 60.0f - xv * k * sina / 60.0f;
   
   gsl_poly_complex_solve(pc, 7, w, z);

   
   for(i = 0; i < 6; ++ i)
   {
	  if( z[ 2 * i + 1] == 0.0 && fabs(z[2*i]) <= fabs(pai * ftr) )
      {
         
		  double new_circle_x = a + k * z[2*i] * cosa;
		  double new_circle_y = b + k * z[2*i] * sina;
		  
		  double rebound_line_angle;
		  
		  
		  double v_rotated_x, v_rotated_y;
		  rotate_point(z[2*i], xv, yv, &v_rotated_x, &v_rotated_y);
		  
		  if( v_rotated_x == new_circle_x )
		  {
		     if( v_rotated_y > new_circle_y )
			 {
			    rebound_line_angle = PONGPING_PI_BY_2;
			 }
			 else
			 {
			    rebound_line_angle = PONGPING_3_PI_BY_2;
			 }
		  }
		  else
		  {
		     rebound_line_angle = atan2( v_rotated_y - new_circle_y, v_rotated_x - new_circle_x);
		  }
		  
		  double adjustment_vector_direction;
		  
		  if(v_rotated_x == 0)
		  {
		     adjustment_vector_direction = v_rotated_y > 0.0 ? PONGPING_PI_BY_2 : PONGPING_3_PI_BY_2;
		  }
		  else
		  {
		     adjustment_vector_direction = atan2( v_rotated_y, v_rotated_x );
		  }
		  
		  if( pai > 0.0 ) /* rotating clockwise */
		  {
		     adjustment_vector_direction += PONGPING_PI_BY_2;			 
		  }
		  else 
		  {
		     adjustment_vector_direction -= PONGPING_PI_BY_2;			 
		  }
		  
          principal_value(&adjustment_vector_direction);
		  
		  double new_angle, new_velocity;
		  
		  
		  
		  double r = sqrt( v_rotated_x * v_rotated_x + v_rotated_y * v_rotated_y );
		  
		  rebound_line_angle += PONGPING_PI_BY_2;
		  
		  principal_value(&rebound_line_angle);
		  
		  add_vectors( principal_value_2(2.0 * rebound_line_angle - c->circle_angle), c->distance_increment, adjustment_vector_direction, r * fabs(pai), &new_angle, &new_velocity);
		  
		  
		  
		  //cout << "rebound line angle = " << rebound_line_angle << "\n";
		  //cout << "circle_angle = " << c->circle_angle << "\n";
		  //move->angle = principal_value_2(new_angle);
		  //move.angle = principal_value_2( 2.0 * rebound_line_angle - c->circle_angle );
		  
		  /*
		  if(new_velocity != c->distance_increment)
		  {
		      move->change_distance_increment = 1;
		      move->new_distance_increment = new_velocity;
		  }
		  */
		  principal_value(&new_angle);
		  
		  new_circle_x += cr_x;
		  new_circle_y += cr_y;
		  
		  //move.hit_type =
		  
		  //cout << "got a vertex hit.\n";
		  //eek();
		  
		  dnfh = sqrt( (new_circle_x - c->cc_x) * (new_circle_x - c->cc_x) + (new_circle_y - c->cc_y) * (new_circle_y - c->cc_y));
		  
		  move->set_members( 1, dnfh / c->distance_increment, new_angle, -1.0, 0.0, -1.0);
		  
		  gsl_poly_complex_workspace_free (w);
		  
		  return 1;
      }
   }
   
   gsl_poly_complex_workspace_free (w);
   return 0;
}