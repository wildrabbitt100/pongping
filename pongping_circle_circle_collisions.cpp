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



#include "pongping_headers/pongping_circle_circle_collisions_headers.h"


//#define CTCC_DEBUG

#define USE_ADJUSTMENT_VECTOR

#define ADJUSTMENT_VECTOR_REPORT



#define CHECK_CONSERVATION_OF_MOMENTUM

/* enumeration for return values of new_angle_for_circle() */

enum { REFLECT, DONT_REFLECT };




static int new_angle_for_circle(const double x, double const y, double const ox, double const oy, const double angle_before, double *angle_after);

/* check_two_circles_collide :

WHAT IT DOES :

Given two circles which are moving at constant velocity (ie in a straight line), this function checks to see if the collide
and if so where.

ARGUMENTS :

   still : this is a pointer to a circle. It's called still because in the moving frame, this circle doesn't move.
   
   moving : another pointer to a circle. It's called moving because it moves in the moving frame.
   
   move : this is where to store the x and y's of where the circle's are if they hit.

   ftr : the fraction of time remaining

*/
int check_two_circles_collide( Circle *still, Circle *moving, PotentialMove *move, double ftr)
{
   /* The equation for the line that the circle which is moving travels is y = mcm * x + mcc where
   
      mcm stands for Moving Circle Multiplier
	  mcc stands for Moving Circle Constant
   */

   double mcm, mcc;

   /* dbcc : distance between circle centres */
   
   double dbcc = hypotenuse( moving->cc_y, still->cc_y, moving->cc_x, still->cc_x );
   
   double scd =  still->distance_increment * ftr;            /* Still Circle Distance : the distance the still circle moves (in the actual frame). */
   double mcd = moving->distance_increment * ftr;            /* Moving Circle Distance : the distance the moving circle moves (in the actual frame). */
   
   /* mcm is a fraction, the numerator is the vertical distance travelled in the moving frame and the denominator is the
      horizontal distance travelled. 
	  
	  mcmn stands for Moving Circle Multiplier Numerator. 
	  
	  mcmd stands for Moving Circle Multiplier Denominator.
      
	  
   */
   
   double mcmn = mcd * sin( moving->circle_angle ) - scd * sin( still->circle_angle );
   double mcmd = mcd * cos( moving->circle_angle ) - scd * cos( still->circle_angle );
   
   int mcm_infinite = 0;
   
   double xd, yd;    /* x discriminant, y discriminant */

   double mfd;       /* Moving Frame Distance : the distance the circle which is moving in the moving frame moves (in the moving frame). */
   double mca;
   
   double moving_cacx, moving_cacy;
   
   /* abc stands for Angle Between Centres. The angle is the angle of the line from the centre of the still circle to the centre of the
      moving circle. */
   
   double scc_to_mcc_angle = principal_value_2( atan2(moving->cc_y - still->cc_y, moving->cc_x - still->cc_x) );
   
   double dfhmf; /* distance for hit in moving frame */ 
   double dfhaf; /* distance for hit in actual frame */
   
   double angleA = principal_value_2(scc_to_mcc_angle - PONGPING_PI_BY_2);
   double angleB = principal_value_2(scc_to_mcc_angle + PONGPING_PI_BY_2);
   
   double L2m; 
   
   double L2c;
   
   //double ax = moving->cc_x + (moving->mass  / (still->mass + moving->mass)) * dbcc * cos( scc_to_mcc_angle );
   //double ay = moving->cc_y + (moving->mass  / (still->mass + moving->mass)) * dbcc * sin( scc_to_mcc_angle );;
   double ax = still->cc_x + 0.5 * dbcc * cos( scc_to_mcc_angle );
   double ay = still->cc_y + 0.5 * dbcc * sin( scc_to_mcc_angle );
   double bx;
   double by;
   double cx;
   double cy;
   
   /* Variables for working out */
   
   double mcfa, mcfd; /* moving circle final angle, moving circle final distance */
   
   double ava;        /* adjustment vector angle */
 
   double scfa, scfd; /* still circle final angle, still circle final distance */ 
   
   double mc_new_angle;
   double sc_new_angle;
   double d1;
   double rlc;
   
   double abc2;
    
   int scACTION, mcACTION;
   
   PONGPING_VECTOR system_vector;
   
   PONGPING_VECTOR still_circle_vector;
   PONGPING_VECTOR moving_circle_vector;
   
   still_circle_vector.a = still->circle_angle;
   still_circle_vector.d = still->distance_increment;
   
   moving_circle_vector.a = moving->circle_angle;
   moving_circle_vector.d = moving->distance_increment;
   
   //scale_vector( &moving_circle_vector, moving->mass );
   //scale_vector( &still_circle_vector, still->mass );
   
   
   add_vectors_( still_circle_vector, moving_circle_vector, &system_vector);
   
   
   #ifdef CHECK_CONSERVATION_OF_MOMENTUM
   
      double momentum_before = still->distance_increment + moving->distance_increment;
	  double momentum_after = 0.0f;
   
   #endif
   
   
   #ifdef CTCC_DEBUG
      logfile << "**********************************************************************************\n";
   #endif
   
   
   /* Check if distances between circles mean they can't collide. */
   
   if( scd + mcd < dbcc  - moving->radius    - still->radius )      
   {
      #ifdef CTCC_DEBUG
	  /***/ //logfile << "circles too far apart. ftr was " << ftr << "\n";
	  #endif
      return 0;
   }
   
   
   
   if(mcmd == 0.0)
   {
      #ifdef CTCC_DEBUG
	  /***/ logfile << "mcm is infinite\n";
	  #endif
      mcm_infinite = 1;
   }
   else
   {
      
	  /* Work out the gradient of the line which the circle which is moving in the moving frame moves along. */
   
      mcm = mcmn / mcmd;
      
	  /* Work out the distance the circle which is moving in the moving frame travels. */
	  
      mfd = sqrt(mcmn * mcmn + mcmd * mcmd);
	  
	  /* Work out the angle the same circle travels at. */
	  
	  mca = principal_value_2( atan2( mcmn, mcmd ) );
	  
      /* Check if angle moving circle is moving on means they won't collide. If the angle is greater than (abc + pi/2) and less than (abc - pi/2),
	  then the moving circle is going away from the still circle. */
	  
	  if( angleA > PONGPING_PI )
	  {
	     if( mca > angleA || mca < angleB )
		 {
		    return 0;
		 }
	  }
	  else
	  {
	     if( mca > angleA && mca < angleB )
		 {
		    return 0;
		 }
	  
	  }
      	  
      
   }
 
    
   if(mcm_infinite)
   {
      
      moving_cacx = moving->cc_x;
	  moving_cacy = still->cc_y + (mcmn < 0.0 ? 1.0 : -1.0) * 
	                sqrt( (moving->radius + still->radius) * (moving->radius + still->radius) - ( moving_cacx - still->cc_x ) * ( moving_cacx - still->cc_x) ); 
   }
   else
   {
      /* Use the known point on the line and the gradient (mcm) to work out the constant mcc. */
   
      mcc = moving->cc_y - mcm * moving->cc_x;   
   
      if(get_discriminants_( &xd, &yd, still->cc_x, still->cc_y, mcm, mcc, still->radius + moving->radius) == PONGPING_SOLUTIONS_IMAGINARY)
      {
	     #ifdef CTCC_DEBUG
		 /***/ logfile << "check_two_circles_hit() : no hit : line doesn't touch circle.\nRETURNING.\n\n";
		 #endif
	     return 0;
	  }
   
      moving_cacx = ( still->cc_x + mcm * still->cc_y - (mcm * mcc) + (((mca > PONGPING_PI_BY_2 && mca < PONGPING_3_PI_BY_2) ? 1.0 : -1.0) * sqrt(xd)) ) / ( 1.0 + mcm * mcm);
	 
	  moving_cacy = mcm * moving_cacx + mcc;
   }

   dfhmf = sqrt( (moving->cc_x - moving_cacx) * (moving->cc_x - moving_cacx) + (moving->cc_y - moving_cacy) * (moving->cc_y - moving_cacy) );
   
   if( dfhmf > mfd )
   {
      /* Circle doesn't travel far enough to hit the other circle. */
	  #ifdef CTCC_DEBUG
	  /***/ logfile << "distance between circles : " << (sqrt( (still->cc_x  - moving->cc_x) * (still->cc_x  - moving->cc_x) + (still->cc_y - moving->cc_y) * (still->/***/ cc_y - moving->cc_y) )  - moving->radius    - still->radius) << "\n";
	  /***/ logfile << "distance between centres : " << (sqrt( (still->cc_x  - moving->cc_x) * (still->cc_x  - moving->cc_x) + (still->cc_y - moving->cc_y) * (still->/***/ cc_y - moving->cc_y) ) ) << "\n";
	  /***/ logfile << "moving->radius = " << moving->radius << " still->radius = " << still->radius << "\n";
	  /***/ logfile << " still x " << still->cc_x << " still y " << still->cc_y << " angle " << still->circle_angle << "\n";
	  /***/ logfile << " moving x " << moving->cc_x << " moving y " << moving->cc_y << " angle " << moving->circle_angle << "\n";
	  /***/ logfile << "mfd = sqrt(mcmn * mcmn + mcmd * mcmd) = " << (sqrt(mcmn * mcmn + mcmd * mcmd)) << "\n";
	  /***/ logfile << "mca = " << mca << "\n";
	  /***/ logfile << "mcd = " << mcd << "scd = " << scd << "\n";
	  /***/ logfile << "cos( moving->circle_angle ) = " << cos( moving->circle_angle ) << " cos( still->circle_angle ) " << cos( still->circle_angle ) << "\n";
	  /***/ logfile << "mcmd = " << mcd << " * " << cos( moving->circle_angle ) << " - " << scd << " * " << cos( still->circle_angle ) << " = " << mcmd << "\n";
	  /***/ logfile << "mcmn = " << mcmn << " mcmd = " << mcmd << "\n";
	  /***/ logfile << " mfd = " << mfd << "\n";
      /***/ logfile << " circle doesn't travel far enough.\n" << dfhmf << " > " << mfd << "\n";
	  /***/ logfile << " mcm = " << mcm << " mcc = " << mcc << "\n";
	  /***/ logfile << " moving_cacx = " << moving_cacx << " moving_cacy = " << moving_cacy << "\n";
	  /***/ logfile << "**********************************************************************************************************\n";
      #endif
      return 0;
   }
   
   /* If control gets here the circles collide. */
   
   double dfh_af_mc;
   
   dfh_af_mc =  ( dfhmf / mfd ) * mcd;

   double dfh_af_sc = ( dfhmf / mfd ) * scd;
  
   double mc_new_x = moving->cc_x + dfh_af_mc * cos( moving->circle_angle );
   double mc_new_y = moving->cc_y + dfh_af_mc * sin( moving->circle_angle );
   
   double ss_new_x = still->cc_x + dfh_af_sc * cos( still->circle_angle );
   double ss_new_y = still->cc_y + dfh_af_sc * sin( still->circle_angle );
   
   
   
   double rla = principal_value_2( atan2( ss_new_y - mc_new_y, ss_new_x - mc_new_x ) + PONGPING_PI_BY_2 );
   
   /* angle between centres 2 : set it to the angle from still circle to moving circle */ 
   
   abc2 = principal_value_2(atan2( mc_new_y - ss_new_y, mc_new_x - ss_new_x ));
   
   d1 = sqrt( (mc_new_y - ss_new_y) * (mc_new_y - ss_new_y) + (mc_new_x - ss_new_x) * (mc_new_x - ss_new_x) );
    
   bx = ss_new_x + 0.5 * d1 * cos(abc2); 
   by = ss_new_y + 0.5 * d1 * sin(abc2);
   
   rlc = by - tan( rla ) * bx;
   
   
   
   get_intersection_coordinates( &cx, &cy, -1.0 / tan( rla ), ay - (-1.0 / tan( rla ) ) * ax, tan( rla ), rlc );
   
   double new_vv_m = sqrt( (cx - ax) * (cx - ax) + (cy - ay) * (cy - ay) );
   
   scACTION = new_angle_for_circle( ss_new_x, ss_new_y, mc_new_x, mc_new_y, still->circle_angle, &sc_new_angle);
   
   mcACTION = new_angle_for_circle( mc_new_x, mc_new_y, ss_new_x, ss_new_y, moving->circle_angle, &mc_new_angle);
   
   
   
   
   //void add_vectors(double theta_v1, double mag_v1, double theta_v2, double mag_v2, double *theta_out_v, double *mag_out_v);
   
   if(scACTION == REFLECT && mcACTION == REFLECT )
   {
      ava = principal_value_2( atan2( cy - ay, cx - ax ) );
      add_vectors( mc_new_angle, moving->distance_increment, ava, new_vv_m, &mcfa, &mcfd);
      add_vectors( sc_new_angle, still->distance_increment, ava, new_vv_m, &scfa, &scfd);
      move->set_members( 2, dfh_af_mc / moving->distance_increment, scfa, scfd, mcfa, mcfd );
	  
	  #ifdef CHECK_CONSERVATION_OF_MOMENTUM
	  
	  momentum_after = scfd + mcfd;
	  
	  if(fabs(momentum_before - momentum_after) > 0.05)
	  {
	     cout << "momentum not conserved 1.\n";
	  }
	  #endif
	  
   }
   else if( scACTION == DONT_REFLECT && mcACTION == REFLECT )
   {
      /* Work out the new vector for the circle which is MOVING in the moving frame of reference. */
      
	  ava = principal_value_2( atan2( cy - ay, cx - ax ) );
	  add_vectors( mc_new_angle, moving->distance_increment, ava, new_vv_m, &mcfa, &mcfd);
	  
	  /* Work out the new vector for the circle which is STILL in the moving frame of reference. */
	  
	  //still_circle_vector.a = still->circle_angle;
      //still_circle_vector.d = moving->distance_increment;
	  
	  moving_circle_vector.a = mcfa;
      moving_circle_vector.d = mcfd;
	  //scale_vector( &moving_circle_vector, moving->mass );
	  
	  subtract_vectors_( system_vector, moving_circle_vector, &still_circle_vector);
	  
	  //scale_vector(&still_circle_vector, 1.0f / still->mass );
	  
	  scfa = still_circle_vector.a;
	  scfd = still_circle_vector.d;
	  
	  move->set_members( 2, dfh_af_mc / moving->distance_increment, scfa, scfd, mcfa, mcfd );
	  //cout << "here 44444.\n";
	  //eek();
	  #ifdef CHECK_CONSERVATION_OF_MOMENTUM
	  momentum_after = scfd + mcfd;
	  if(fabs(momentum_before - momentum_after) > 0.05)
	  {
	     cout << "momentum not conserved 2.\n";
		 
	  }
	  #endif
   }
   else if( scACTION == REFLECT && mcACTION == DONT_REFLECT )
   {
      /* Work out the new vector for the circle which is STILL in the moving frame of reference. */
	  
	  ava = principal_value_2( atan2( cy - ay, cx - ax ) );
      add_vectors( sc_new_angle, still->distance_increment, ava, new_vv_m, &scfa, &scfd);
	  
	  /* Work out the new vector for the circle which is MOVING in the moving frame of reference. */
	  
	  still_circle_vector.a = scfa;
	  still_circle_vector.d = scfd;
	  
	  subtract_vectors_( system_vector, still_circle_vector, &moving_circle_vector);
	  
	  //scale_vector(&moving_circle_vector, 1.0f / moving->mass );
	  
	  mcfa = moving_circle_vector.a;
	  mcfd = moving_circle_vector.d;
	  
	  move->set_members( 2, dfh_af_mc / moving->distance_increment, scfa, scfd, mcfa, mcfd );
      //cout << "here 5555 : scfd = " << scfd << "\n";
	  //eek();
	  #ifdef CHECK_CONSERVATION_OF_MOMENTUM
	  momentum_after = scfd + mcfd;
	  if(fabs(momentum_before - momentum_after) > 0.05)
	  {
	     cout << "momentum not conserved 3.\n";
		
	  }
	  #endif
   }
   else
   {
      cout << "here 66666.\n";
	  eek();
   }
   
   #ifdef ADJUSTMENT_VECTOR_REPORT
   /***/logfile << "ADJUSTMENT VECTOR REPORT :\n\n";
   /***/logfile << "STILL CIRCLE  : angle before hit : " << still->circle_angle << "\n\n";
   /***/logfile << "MOVING CIRCLE : angle before hit : " << moving->circle_angle << "\n\n";
   /***/logfile << "STILL CIRCLE  : coordinates before move : ( " << still->cc_x << " , " << still->cc_y << " )\n\n";
   /***/logfile << "MOVING CIRCLE : coordinates before move : ( " << moving->cc_x << " , " << moving->cc_y << " )\n\n";
   /***/logfile << "STILL CIRCLE  : coordinates at hit time : ( " << ss_new_x << " , " << ss_new_y << " )\n\n";
   /***/logfile << "MOVING CIRCLE : coordinates at hit time : ( " << mc_new_x << " , " << mc_new_y << " )\n\n";
   /***/logfile << "POINT A : ( " << ax << " , " << ay << " ) \n\n";
   /***/logfile << "POINT B : ( " << bx << " , " << by << " ) \n\n";
   /***/logfile << "POINT C : ( " << cx << " , " << cy << " ) \n\n";
   /***/logfile << "rebound line angle = " << rla << "\n";
   /***/logfile << "ADJUSTMENT VECTOR ANGLE " << ava << "\n\n";
   /***/logfile << "new_vv_m = " << new_vv_m << "\n";
   /***/logfile << "STILL CIRCLE  : intermediate angle : " << sc_new_angle << "\n\n";
   /***/logfile << "MOVING CIRCLE : intermediate angle : " << mc_new_angle << "\n\n";
   /***/logfile << "STILL CIRCLE  : angle after hit : " << scfa << "\n\n";
   /***/logfile << "MOVING CIRCLE  : angle after hit : " << mcfa << "\n\n";
   /***/logfile << "STILL CIRCLE  : distance before hit : " << still->distance_increment << "\n\n";
   /***/logfile << "STILL CIRCLE  : distance after hit : " << scfd << "\n\n";
   /***/logfile << "MOVING CIRCLE  : distance before hit : " << moving->distance_increment  << "\n\n";
   /***/logfile << "MOVING CIRCLE  : distance after hit : " << mcfd << "\n\n";
   /***/logfile << "sum of velocities before : " << (still->distance_increment + moving->distance_increment) << "\n";
   /***/logfile << "sum of velocities after  : " << (mcfd + scfd) << "\n";
   /***/logfile << "END OF REPORT.\n\n";
   #endif
   
   if(fabs(momentum_before - momentum_after) > 0.05)
   {
	  cout << "Quitting because momentum not conserved.\n";
	  logfile << "Quitting because momentum not conserved.\n";
	  fleek();
   }
  //eek();
  
   logfile << "GOT A CIRCLE TO CIRCLE HIT\n";
   return PONGPING_HIT; 
}





/* new_angle :


WHAT IT DOES :

   This function is used when two circles collide. It looks at the angle one circle has come from and by comparing it with
   the rebound line angle, works out what the angle is that the circle bounces off at. This doesn't take into account any
   exchange of momentum. 

ARGUMENTS :




*/
static int new_angle_for_circle(const double x, double const y, double const ox, double const oy, const double angle_before, double *angle_after)
{
   /* abc : angle between centres - the angle from the centre of the circle the new angle is being worked out for to
            the centre of the other circle. */

   const double abc = atan2( oy - y, ox - x );

   
   /* Calculate the angles 90 degrees (or pi / 2 radians) less than abc and 90 degrees (or pi / 2 radians) more than angle abc.
   
      Must call principal_value_2() on the calculated angles because the code is designed to work on angles in the range
	  
	  0 to 2 * pi radians. */
   
   
   /* LA - lesser angle - abc - 90 degrees. */
  
   const double LA = principal_value_2( abc - PONGPING_PI_BY_2 );
   
   /* GA - lesser angle - abc + 90 degrees. */
   
   const double GA = principal_value_2( abc + PONGPING_PI_BY_2 );

   if( LA < PONGPING_PI && LA > 0.0)
   {
      if( (angle_before >= 0.0 && angle_before <= LA) || (angle_before >= GA && angle_before <= PONGPING_2_PI) )
      {
         /* Return angle unchanged. */
		 
		 return DONT_REFLECT;
      }
      else
      {
	     *angle_after = principal_value_2( 2.0 * LA - angle_before );
         return REFLECT;
      }	  
   }
   else
   {
      if( angle_before >= GA && angle_before <= LA )
	  {
	     return DONT_REFLECT;
	  }
      else
	  {
	     *angle_after = principal_value_2( 2.0 * LA - angle_before );
	     return REFLECT;
	  }
   }
}
