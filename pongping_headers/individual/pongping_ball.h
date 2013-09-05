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



#include <iostream>



#ifndef BALL_H 
#define BALL_H






#define TICKS_FOR_SLOWEST_SPIN 10

#define NUMBER_OF_BALL_SIZES 4


using namespace std;

//class Circle;

class Ball : Circle {

   
   int counter;
   
   bool init_okay;
   
   
   
   
   
   
   
public :

   int ball_size;
   
   static int ball_bitmap_sizes[NUMBER_OF_BALL_SIZES];
 
   Ball() 
   {
	  if( load_circle_bitmap("game_data/ball.png") != 0 )
      {
         init_okay = false;
         return;
      }
      
	  radius = 4.0;
      
      
      doing_speed_transition = false;
   

	  /* Must set the variable circle_type to PONGPING_BALL_CIRCLE_TYPE in the base class Circle so that
	     it can be identified as such when a new level is started. Identifying it
		 as such means the variables retrieved from the level file can be set for the ball. */
	  
	  set_circle_type(PONGPING_BALL_CIRCLE_TYPE);
	  
	  circle_frame = 0;
	  
	  phase = PONGPING_CIRCLE_MOVING_PHASE;
	  
	  ball_size = 0;
	  
	  init_okay = true;
   }
   
   Ball(int xx, int yy, float angle, float ddistance_increment)
   {
   
   
      if( load_circle_bitmap("game_data/ball.png") != 0 )
      {
         init_okay = false;
         return;
      }
      
	  
	  
	  set_double(DISTANCE_INCREMENT, ddistance_increment);
	  set_double(CIRCLE_X, xx);
	  set_double(CIRCLE_Y, yy);
	  set_double(CIRCLE_ANGLE, angle);
	 
	  //last_feature_hit = -1;
	  set_last_feature_hit();
	  radius = 4.0;
      mass = 4.0 * 4.0;
	  
      
      
      doing_speed_transition = false;
    
      
	  
	  // can this function be changed so as just to use an assignment ?
	  set_circle_type(PONGPING_BALL_CIRCLE_TYPE);
	 
	  
	  phase = PONGPING_CIRCLE_MOVING_PHASE;
	  
	  ball_size = 0;
	  
	  init_okay = true;
   }
  
   
   bool init_okay_function() { return init_okay; }
   
   
  
   void deal_with_bat_collision(Bat& );
   
   
   void draw(void)
   {
      
	   al_draw_bitmap_region( circle, 0, 0, ball_bitmap_sizes[ball_size], ball_bitmap_sizes[ball_size], floor(cc_x - radius + 0.5f), floor(cc_y - radius + 0.5f), 0); 
	
   
   }
  
   int change_ball_colour(COULEUR old_colour, COULEUR new_colour)
   {
      ALLEGRO_LOCKED_REGION *lock = al_lock_bitmap(circle, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE );
   
      
	  if(lock == NULL)
	  {
		 LOG("couldn't lock circle bitmap to change colour.\n");
	     return 1;
	  }
	  
   
      swap_color_a(lock, al_get_bitmap_width(circle), al_get_bitmap_height(circle), old_colour, new_colour);
   
      al_unlock_bitmap(circle);
	  
	  return 0;
   }
   //~Ball

};



#endif /* BALL_H */
