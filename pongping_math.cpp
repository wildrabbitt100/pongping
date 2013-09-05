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

 
 
 
 
 
 
 #include "pongping_headers/pongping_math_headers.h"







double principal_value_2(double angle)
{
   if(angle < 0.0f)
   {
      while( angle < 0.0f)
      {
         angle += PONGPING_PI * 2.0f;
      }
   }
   else if(angle > PONGPING_PI * 2.0f)
   {
      while(angle > PONGPING_PI * 2.0f)
      {
         angle -= PONGPING_PI * 2.0f;
      }
   }
   
   return angle;
}







int power_of_ten(int power)
{
   int result = 1;
   
   while(power > 0)
   {
      result *= 10;
      --power;
   }

   return result;
}




void get_intersection_coordinates(double *x, double *y, const double m1, const double c1, const double m2, const double c2)
{

   if( m2 == m1 )
   {
      logfile << "get_intersection coordinates :- gradients are the same.\n";
	  eek();
   }

   *x = (c1 - c2) / (m2 - m1);
   *y = m1 * (*x) + c1;

}





int point_in_triangle( const double px, const double py, const double x1, const double y1, const double x2, const double y2, const double x3, const double y3)
{

   double v0_ic = x3 - x1;
   double v0_jc = y3 - y1;
   double v1_ic = x2 - x1;
   double v1_jc = y2 - y1;
   double v2_ic = px - x1;
   double v2_jc = py - y1;
   
   double dot00 = dot_product(v0_ic, v0_jc, v0_ic, v0_jc);
   double dot01 = dot_product(v0_ic, v0_jc, v1_ic, v1_jc);
   double dot02 = dot_product(v0_ic, v0_jc, v2_ic, v2_jc);
   double dot11 = dot_product(v1_ic, v1_jc, v1_ic, v1_jc);
   double dot12 = dot_product(v1_ic, v1_jc, v2_ic, v2_jc);



   double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
   double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
   double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

   return (u >= 0) && (v >= 0) && (u + v < 1.0);

}



double dot_product(const double ic1, const double jc1, const double ic2, const double jc2)
{
   return ic1 * ic2 + jc1 * jc2;
}





void get_discriminants(double *x, double *y, const double a, const double b, const double m, const double c, const double r)
{
   /* 2abmmm + 2cbmm - mmbb + mmrr - mmcc - 2cmmma - mmmmaa + mmmmrr */

   *x = 2.0*m*m*m*a*b + 2.0*b*c*m*m - b*b*m*m + r*r*m*m - m*m*c*c - 2.0*c*m*m*m*a - a*a*m*m*m*m  + r*r*m*m*m*m;

   
   /* -2mca + 2mab - bb - cc + 2bc + rr - mmaa + mmrr */
   
   *y = -2.0*m*c*a + 2.0*m*a*b - b*b - c*c + 2.0*b*c + r*r - m*m*a*a + r*r*m*m;
   
}



int get_discriminants_(double *x, double *y, const double a, const double b, const double m, const double c, const double r)
{
   /* 2abmmm + 2cbmm - mmbb + mmrr - mmcc - 2cmmma - mmmmaa + mmmmrr */

   *y = 2.0*m*m*m*a*b + 2.0*b*c*m*m - b*b*m*m + r*r*m*m - m*m*c*c - 2.0*c*m*m*m*a - a*a*m*m*m*m  + r*r*m*m*m*m;

   
   /* -2mca + 2mab - bb - cc + 2bc + rr - mmaa + mmrr */
   
   *x = -2.0*m*c*a + 2.0*m*a*b - b*b - c*c + 2.0*b*c + r*r - m*m*a*a + r*r*m*m;
   
   if(*x < 0.0 || *y < 0.0)
   {
      return PONGPING_SOLUTIONS_IMAGINARY;
   }
   
   return PONGPING_SOLUTIONS_REAL;
}




double hypotenuse( const double y1, const double y2, const double x1, const double x2 )
{
   return sqrt( (y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1) );
}





/* rotate_point() : take's a point given by coordinates (in_x, in_y) and rotates it by extra_angle around (0,0) and
   stores the result in the double variables pointed to by out_x and out_y

   
*/
void rotate_point(const double extra_angle, const double in_x, const double in_y, double *out_x, double *out_y)
{
   const double radius = sqrt( in_x * in_x + in_y * in_y );
   
   if(in_x == 0.0 && in_y == 0.0) 
   {
      *out_x = in_x;
      *out_y = in_y;
      return;
   }      
   
   double angle = (in_x == 0) ? ( in_y > 0.0 ? PONGPING_PI_BY_2 : PONGPING_3_PI_BY_2) : atan2( in_y , in_x );
   
   *out_x = radius * cos( angle + extra_angle);
   *out_y = radius * sin( angle + extra_angle);
}




void scale_vector(PONGPING_VECTOR *v, const double scalar)
{
   v->d *= scalar;
}



void add_vectors_( PONGPING_VECTOR v1, PONGPING_VECTOR v2, PONGPING_VECTOR *newv )
{
   double x;
   double y;
   
   double x2, y2;
   
   x = v1.d * cos( v1.a );
   y = v1.d * sin( v1.a );
   
   x2 = x + v2.d * cos( v2.a );
   y2 = y + v2.d * sin( v2.a );
  
   newv->a = principal_value_2( atan2( y2, x2 ) );
   newv->d = sqrt(x2 * x2 + y2 * y2);
}



/* subtract_vectors()

WHAT IT DOES :

  subtracts vector v2 from v1 and stores resultant vector in vector pointed to by newv

  
*/
void subtract_vectors_( PONGPING_VECTOR v1, PONGPING_VECTOR v2, PONGPING_VECTOR *newv )
{
   double x;
   double y;
   
   double x2, y2;
   
   x = v1.d * cos( v1.a );
   y = v1.d * sin( v1.a );
   
   x2 = x + v2.d * cos( principal_value_2( v2.a - PONGPING_PI) );
   y2 = y + v2.d * sin( principal_value_2( v2.a - PONGPING_PI) );
  
   newv->a = principal_value_2( atan2( y2, x2 ) );
   newv->d = sqrt(x2 * x2 + y2 * y2);
}



float radians_to_degrees( float angle )
{

   return angle * 360.f / (PONGPING_PI* 2.0f);
}


int pongping_random_number(int l, int u)
{
   return ( l + rand() % (u - l + 1) );
}




/* angle_appropriate : 

					   
*/

int angle_appropriate(const double angle, const double lower_angle, const double higher_angle)
{
   if( angle == lower_angle || angle == higher_angle ) return 0;
   else
   {
      if( lower_angle > higher_angle )
	  {
	     return angle > lower_angle || angle < higher_angle;
	  }
      else
      return (angle > lower_angle && angle < higher_angle);
   }
}



void add_vectors(double theta_v1, double mag_v1, double theta_v2, double mag_v2, double *theta_out_v, double *mag_out_v)
{
   double x;
   double y;
   
   double x2, y2;
   
   x = mag_v1 * cos(theta_v1);
   y = mag_v1 * sin(theta_v1);
   
   
   x2 = x + mag_v2 * cos(theta_v2);
   y2 = y + mag_v2 * sin(theta_v2);
   
   
   if(x2 == 0)
   {
      *theta_out_v = y2 > 0 ? PONGPING_PI_BY_2 : PONGPING_3_PI_BY_2; 
   }
   else
   {
      *theta_out_v = principal_value_2( atan2( y2, x2 ) );
   }
   
   principal_value_2(*theta_out_v);
   
   *mag_out_v = sqrt(x2 * x2 + y2 * y2);

}



int angle_is_vertical(double angle)
{
   return fabs( angle - PONGPING_PI_BY_2 ) < 0.01 || fabs( angle - PONGPING_3_PI_BY_2) < 0.01;
}



void principal_value(double *angle)
{
   if(*angle < 0.0f)
   {
      while( *angle < 0.0f)
      {
         *angle += PONGPING_PI* 2.0f;
      }
   }
   else if(*angle > PONGPING_PI* 2.0f)
   {
      while(*angle > PONGPING_PI* 2.0f)
      {
         *angle -= PONGPING_PI* 2.0f;
      }
   }
}
