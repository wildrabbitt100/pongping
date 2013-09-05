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
 
 
 


#include "pongping_headers/pongping_circle_headers.h"





/* check_collision_with_bat() return 0 if no collision or 1 if there is */


int Circle::check_collision_with_bat( Bat& bat, PotentialMove **pm, double ftr)
{ 
   int current_ticks;
   int n;
   float future_x_at_bat_y;
   int vn;
   int result;
   double dx; /* Need to get this from the Bat Class - how far the bat moves horizontally. */
   double cmx, cmy;
   double circle_to_bat_angle = principal_value_2(atan2( bat.cy - cc_y, bat.cx - cc_x ));
   
   
   
   PotentialMove *m;
   
   vector <PotentialMove *> moves(0);
   
   int move_number = 0;
   
   
   
   switch( bat.direction ) 
   {
      case BAT_NOT_MOVING:
         dx = 0.0f;
		 break;
	  case BAT_MOVING_LEFT:
	     //logfile << "setting \n";
	     dx = - bat.movement_inc * ftr;
	     break;
	  case BAT_MOVING_RIGHT:
	     dx = bat.movement_inc * ftr;
	     break;
		 
   }   
     
   /* check for collision against edges of bat */
	  
	 
   for(vn = 0; vn < 4; ++vn)
   {
	  if( circle_and_line_collision( PONGPING_TRANSLATION, bat.bat_vertices[vn], bat.bat_vertices[vn + 1], bat.get_bat_midpoint_x(), 
	                                 bat.get_bat_midpoint_y(), 0, dx, 0.0, this, *pm, 0, ftr) )
      {
	     /* Do I need to distinguish between side collisions and vertex collisions ? */
	  
		 (*pm)->set_hit_type_and_side(PONGPING_BAT_COLLISION, vn);
		 
		 return 1;
	  }
   }
   
   
   
   
	  
	  
   /* check for collision against vertices of bat */
   
   m = new PotentialMove();
   
   moves.push_back(m);
   
   for(vn = 0; vn < 4; ++vn)
   {
       logfile << "checking vertex " << vn << "\n\n";
       /*
	   int (const unsigned char vertex_move,
                                 	   const double cx, 
									   const double cy, 
									   const int use_centre_of_mass, 
									   const double cmx, 
									   const double cmy, 
									   const Point v, 
									   const double angle, 
									   const int dx, 
									   const int dy, 
									   Circle *c, 
									   PotentialMove& move);
       */
	   
	   
	   
	   
	   
       if(cvc( PONGPING_TRANSLATION, 0.0, 0.0, PONGPING_USE_CENTRE_OF_MASS, bat.get_bat_midpoint_x(), bat.get_bat_midpoint_y(), bat.get_mass(),  bat.bat_vertices[vn], 0.0, dx, 0.0, this, moves[move_number], ftr))
	   {
	      moves[move_number]->set_hit_type_and_side(PONGPING_BAT_COLLISION, vn);
	   
	      ++move_number;
	      
		  //logfile << "move_number = " << move_number << "\n";
		  
		  //pm.hit_type = PONGPING_BAT_COLLISION;
	      
		  m = new PotentialMove();
   
          moves.push_back(m);
		  
	      //return 1;
	   }
      
   }
   
   vector< PotentialMove* >::iterator it = moves.end();
   
   
   
   if(move_number > 0)
   {  
      --it;
      sort(moves.begin(), it, compare_distance);
	  
	  (*pm) = moves[0]; 
	 
	  //logfile << "got a vertex hit.\n";
	  //eek();
	  
	  return 1;
   }
   
   

   return 0;
}




int Circle::check_circle_is_clear_of_polygon( PongPingPolygon *polygon)
{

   Point v1, v2;

   double d;
   
   double a;
   
   int vn;
   
   if( polygon->attributes & IRREGULAR )
   {
      if( polygon->attributes & PONGPING_HIT_X_TIMES_POLYGON )
	  {
	     for(vn = 0; vn < polygon->number_of_sides; ++vn)
	     {
	        a = polygon->unrotated_points[vn].angle + polygon->angle;
	     
		    v1.x = polygon->cx + polygon->unrotated_points[vn].radius * cos(a);
	        v1.y = polygon->cy + polygon->unrotated_points[vn].radius * sin(a);
		 
			a = polygon->unrotated_points[(vn + 1) % polygon->number_of_sides].angle + polygon->angle;
		 
		    v2.x = polygon->cx + polygon->unrotated_points[(vn + 1) % polygon->number_of_sides].radius * cos(a);
	        v2.y = polygon->cy + polygon->unrotated_points[(vn + 1) % polygon->number_of_sides].radius * sin(a);
		 
            if( point_in_triangle(cc_x, cc_y, v1.x , v1.y, v2.x , v2.y, polygon->cx , polygon->cy ))   
	        {
	           /* centre of circle is in this triangle so the circle is in the polygon. */
	           logfile << "in this one.\n";
	           return 0;
	        }
	        else
	        {
	           //logfile << "not in this one.\n";
	        }
			
			d = sqrt( (v1.x - cc_x) * (v1.x - cc_x) + (v1.y - cc_y) * (v1.y - cc_y));
	        if( d <= radius )
	        {
			   logfile << "near vertex.\n";
	           return 0;
	        }
	    }
	  }
   }
   
   

   return 1;
}





int Circle::circle_inside_bat(Bat& b)
{
	     if( cc_x > b.bat_vertices[1].x - (radius - 0.2) && cc_x < b.bat_vertices[2].x + (radius - 0.2))
		 {
		    if( cc_y > b.bat_vertices[1].y - (radius - 0.2) && cc_y < b.bat_vertices[0].y + (radius - 0.2))
			{
			   return 1;
			}
		 }
		 
		 return 0;
}

