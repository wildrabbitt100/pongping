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
 
 
#ifndef COLLECTABLES_HEADER
#define COLLECTABLES_HEADER







#define EXTRA_LIFE_LETTER_MASS_PER_PIXEL 1.5

class FallingCollectable : Circle
{
   int type_of_collectable;
   
   
   int init_okay;
   
   
   
   
public:

   static ALLEGRO_BITMAP *letters_bitmap;
   static ALLEGRO_BITMAP *bat_size_up_circle;
   static ALLEGRO_BITMAP *ball_size_up_circle;
   
   int polygon_id_it_came_from;
   
   
   
   FallingCollectable() { }

   
   /* FallingCollectable constructor :
   
      x, y : where it starts
      collectable_type_ : one of 
                	PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE
 					PONGPING_BAT_SIZE_UP_CIRCLE_TYPE
					PONGPING_BALL_SPEED_CHANGER_CIRCLE_TYPE
   */
   FallingCollectable(float x, float y, int collectable_type_, int sub_type, int polygon_source_type, int polygon_id) 
     
	 : init_okay(0), polygon_id_it_came_from(polygon_id)
   
   {
      set_double(CIRCLE_X, x);
      set_double(CIRCLE_Y, y);
      set_double(DISTANCE_INCREMENT, 5.0);
	  set_double(CIRCLE_ANGLE, PONGPING_PI_BY_2 + 0.1);
   
      if(collectable_type_ == PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE)
	  {
	     mass = PONGPING_PI * EXTRA_LIFE_LETTER_RADIUS * EXTRA_LIFE_LETTER_RADIUS * EXTRA_LIFE_LETTER_MASS_PER_PIXEL;
		 
	     radius = EXTRA_LIFE_LETTER_RADIUS;
	     //set_bitmap_null();
		 //set_bitmap_used(0);
		 circle = letters_bitmap;
	  }
	  else if(collectable_type_ == PONGPING_BAT_SIZE_UP_CIRCLE_TYPE)
	  {
	     radius = PONGPING_BAT_SIZE_UP_CIRCLE_RADIUS;
		 circle = bat_size_up_circle;
	  }
	  else if(collectable_type_ == PONGPING_BALL_SIZE_UP_CIRCLE_TYPE)
	  {
	     radius = PONGPING_BALL_SIZE_UP_CIRCLE_RADIUS;
		 circle = ball_size_up_circle;
	  }
	  
      doing_speed_transition = false;
    
	  set_last_feature_hit();
	  
	  
	  set_circle_type( collectable_type_ );
	  
	  circle_sub_type = sub_type;
	  
      
	  
	  reset_remaining_distance();
	  
	  
	  
	  if(polygon_source_type & PONGPING_HIT_X_TIMES_POLYGON)
	  {
	     inside_polygon = 1;
	  }
	  else
	  {
	     inside_polygon = 0;
	  }
	  
	  phase = PONGPING_CIRCLE_MOVING_PHASE;
	  
	  dying_updates = 0;
	  
	  circle_frame = 0;
	  
	  /*
	  if(load_circle_bitmap("game_data/extra_life_letters.png") != 0)
	  {
	     init_okay = 0;
	     return;
	  }
	  */
	  
	  /* Set the circle_hit member in the base class to -1 just in case it gets used before it's hit a circle. */
	  
	  circle_hit = -1;
	  
	  init_okay = 1; 
	  
   }
   
   int type_of_circle(void) {return type_of_collectable; }
  
   void set_angle(double angle)
   {
      set_double(CIRCLE_ANGLE, angle);
   } 
   
   int circle_is_collectable( void ) { return 1; }
   
};





#endif
