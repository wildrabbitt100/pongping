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
 
 
#ifndef BAT_H
#define BAT_H






#define BAT_TICKS_PER_UPDATE 4
#define BALL_TICKS_PER_UPDATE 10




#define BAT_MATERIAL_PER_PIXEL_MASS 0.5

//class Circle;


enum { PONGPING_BOTTOM_LEFT_BAT_VERTEX, PONGPING_TOP_LEFT_BAT_VERTEX, PONGPING_TOP_RIGHT_BAT_VERTEX, PONGPING_BOTTOM_RIGHT_BAT_VERTEX };


class Bat {

   

   float x;
   
   int sections;
   
   
   float time_of_last_update;
   
   
   int counter;
   
   
   bool moving;
   
  
   ALLEGRO_BITMAP *bat_left_end, *bat_middle_section, *bat_right_end;
   
   
   bool init_okay;
   
   
   
   double bat_mass;
   
public :


   Point bat_vertices[4];

   float movement_inc;

   double cx, cy;
   
   double area;
   
   int direction;
   
   Bat();
   
   int get_x(void) { return x; }
   
   void Draw();
   
   
   void Update(bool left, bool right) 
   {
      if(left == true && right == true)
	  {
	     direction = BAT_NOT_MOVING;
	     return;	 
	  }
      else if(left == true)
      {
	     if(x >= movement_inc + BALL_ARENA_LEFT_EDGE_X)
		 {
	        direction = BAT_MOVING_LEFT;
   	        x -= movement_inc;
			set_bat_vertices();
		    return;
	     }
		 else if(x > BALL_ARENA_LEFT_EDGE_X)
		 {
		    x = BALL_ARENA_LEFT_EDGE_X;
			set_bat_vertices();
	     }
	  }
	  else if(right == true)
      {
	     if(x <= BALL_ARENA_RIGHT_EDGE_X - movement_inc - get_bat_width())
		 {
	        direction = BAT_MOVING_RIGHT;
    	    x += movement_inc;
			set_bat_vertices();
		    return;
	     }
		 else if(x < BALL_ARENA_RIGHT_EDGE_X - get_bat_width())
		 {
		    x = BALL_ARENA_RIGHT_EDGE_X - movement_inc - get_bat_width();
			set_bat_vertices();
	     }
	  }
	  direction = BAT_NOT_MOVING;
   }
   
   bool init_okay_function() { return init_okay; }
   
   int bat_direction() { return direction; }
   
   float get_bat_movement_inc() { return movement_inc; }
   
   void set_bat_vertices()
   {
      /* bottom left */
	  
	  bat_vertices[PONGPING_BOTTOM_LEFT_BAT_VERTEX].x = x;
	  bat_vertices[PONGPING_BOTTOM_LEFT_BAT_VERTEX].y = BAT_Y_POSITION + 10.0;
      
	  /* top left */
	  
	  bat_vertices[PONGPING_TOP_LEFT_BAT_VERTEX].x = x;
	  bat_vertices[PONGPING_TOP_LEFT_BAT_VERTEX].y = BAT_Y_POSITION;
	  
	  /* top right */
	  
	  bat_vertices[PONGPING_TOP_RIGHT_BAT_VERTEX].x = x + 26.0 + (float) sections * 14.0;
	  bat_vertices[PONGPING_TOP_RIGHT_BAT_VERTEX].y = BAT_Y_POSITION;
	  
	  /* bottom right */
	  
	  bat_vertices[PONGPING_BOTTOM_RIGHT_BAT_VERTEX].x = x + 26.0 + (float) sections * 14.0;
	  bat_vertices[PONGPING_BOTTOM_RIGHT_BAT_VERTEX].y = BAT_Y_POSITION + 10.0;
	  
	  
	  cx = bat_vertices[PONGPING_BOTTOM_LEFT_BAT_VERTEX].x + (bat_vertices[PONGPING_TOP_RIGHT_BAT_VERTEX].x - bat_vertices[PONGPING_TOP_LEFT_BAT_VERTEX].x) / 2.0f;
	  cy = bat_vertices[PONGPING_BOTTOM_LEFT_BAT_VERTEX].y - (bat_vertices[PONGPING_TOP_LEFT_BAT_VERTEX].y - bat_vertices[PONGPING_BOTTOM_LEFT_BAT_VERTEX].y) / 2.0f;
	  
	  
	  area = (bat_vertices[PONGPING_TOP_RIGHT_BAT_VERTEX].x - bat_vertices[PONGPING_TOP_LEFT_BAT_VERTEX].x) * ( bat_vertices[PONGPING_BOTTOM_LEFT_BAT_VERTEX].y
	  - bat_vertices[PONGPING_TOP_LEFT_BAT_VERTEX].y); 
	  
	  
	  bat_mass = area * BAT_MATERIAL_PER_PIXEL_MASS; 
	  
	 
	  //eek();
   
   }
   
   int get_bat_width(void) { return 26.0 + (float) sections * 14.0; }
   
   /* Functions for getting the midpoints - needed because when checking if there's a collision between the ball and a vertex,
      the x and y of a point which is solid has to be passed - so as to know which side of the line is solid. */
   
   int get_bat_midpoint_x() { return bat_vertices[1].x + (bat_vertices[2].x - bat_vertices[1].x) / 2.0f; }
   int get_bat_midpoint_y() { return (float) BAT_Y_POSITION + 5.0; }
   
   double get_mass() { return bat_mass; }
   
   //int check_bat_hits_circle( Circle *circle, PotentialMove& pm);
   int circle_near(Circle *circle);
};



#endif
