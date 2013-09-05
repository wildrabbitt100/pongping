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


#ifndef CIRCLE_BASE_H_
#define CIRCLE_BASE_H_

#include <iostream>


using namespace std;

enum { DISTANCE_INCREMENT, CIRCLE_X, CIRCLE_Y, CIRCLE_ANGLE };


class Bat;
//class Ball;
class PongPingPolygon;


#define PONGPING_DYING_CIRCLE_BITMAP_WIDTH   140
#define PONGPING_DYING_CIRCLE_BITMAP_HEIGHT  140

#define PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE 30

#define PONGPING_DYING_CIRCLE_EXPLOSION_FINAL_WIDTH 30

/* Theses defines are for the colors of the explosions which are a mix of yellow and red. */


#define RED_R      231
#define RED_G      19
#define RED_B      24
#define YELLOW_R   255
#define YELLOW_G   253
#define YELLOW_B   115



class Circle {

   

   ALLEGRO_BITMAP *dying_circle;
   
   
   protected :
   
   Circle() {}
   
   
   public:
   
      ALLEGRO_BITMAP *circle;   
   
      COULEUR circle_colour;
   
      int circle_type;
      int circle_sub_type;
      int hit_pending;
      
	  float increment_to_get_to;
      
	  int id_of_last_polygon_hit;
	  int side_hit;
	  int vertex_hit;
	  
	  
	  int hit_type;
	  int circle_hit;
	  int vertex_or_side_hit;
	  
      int inside_polygon;
      
	  double mass;
      
	  int phase;
      
	  unsigned char circle_frame;
      double opacity_of_circle_explosion;
      double explosion_radius_1;
      double explosion_radius_2;
	  
	  
	  
	  
      int load_circle_bitmap(const char *string_for_bitmap)
      {
         circle = al_load_bitmap(string_for_bitmap);
         if(circle == NULL)
         {
            return 1;
         }
         else
         {
            LOG("loaded circle bitmap.\n\n");
            al_convert_mask_to_alpha( circle, al_map_rgb(176, 36, 4));
            return 0;
         }
      }
      
      
   
      double circle_angle, cc_x, cc_y, remaining_distance, distance_increment, radius;

      bool doing_speed_transition;
      
      void set_double(int variable_number, double value)
      {
        
         switch (variable_number)
         {
            case DISTANCE_INCREMENT:
               distance_increment = value;
               break;
            case CIRCLE_X:
               cc_x = value;
               break;
            case CIRCLE_Y:
               cc_y = value;
               break;
            case CIRCLE_ANGLE:
               circle_angle = value;
               break;
        }
    
     }
     
     void set_last_feature_hit() { id_of_last_polygon_hit = -1; }
      
      
     
      
      int check_collision_with_bat(Bat& bat, PotentialMove **pm, double ftr);
      
	  
	  /**** This can go. ****/
      void reset_remaining_distance()
      {
         remaining_distance = distance_increment;
      }
      /**** ****/
      
      int gone_off_bottom() { return (cc_y - radius) > BALL_ARENA_BOTTOM_EDGE_Y; }
      
      
      virtual void draw(void) 
	  {
	     if(phase == PONGPING_CIRCLE_MOVING_PHASE)
	     switch(circle_type)
		 {
		    
			case PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE:
			   al_draw_bitmap_region( circle, circle_sub_type * 24, 0, 24, 24, cc_x - radius, cc_y - radius, 0);
		       break;
			case PONGPING_BAT_SIZE_UP_CIRCLE_TYPE:
			   al_draw_bitmap( circle, cc_x - radius, cc_y - radius, 0);
			   break;
			case PONGPING_BALL_SIZE_UP_CIRCLE_TYPE:
			   al_draw_bitmap( circle, cc_x - radius, cc_y - radius, 0);
			   break;
			   
		 }
		 else if(phase == PONGPING_CIRCLE_COLLECTED_PHASE)
		 {
		    COULEUR explosion_color;
			
			if( dying_updates < PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE - 10 )
			{
			    explosion_color = get_shade(RED_R, RED_G, RED_B, YELLOW_R, YELLOW_G, YELLOW_B, (double) dying_updates / (double) (PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE - 10));
			}
			else
			{
			    explosion_color = get_shade(YELLOW_R, YELLOW_G, YELLOW_B, 255, 255, 255, ((double) dying_updates - 20.0f)/ (double) 10);
			
			}
		 
		    al_set_target_bitmap(dying_circle);
		    al_clear_to_color(al_map_rgb(176, 20, 4));
		    al_draw_filled_circle( PONGPING_DYING_CIRCLE_BITMAP_WIDTH / 2.0f, PONGPING_DYING_CIRCLE_BITMAP_HEIGHT / 2.0f, explosion_radius_1, al_map_rgba(0, 0, 0, 255) );
		    al_draw_filled_circle( PONGPING_DYING_CIRCLE_BITMAP_WIDTH / 2.0f, PONGPING_DYING_CIRCLE_BITMAP_HEIGHT / 2.0f, explosion_radius_1, explosion_color ); 
	        al_draw_filled_circle( PONGPING_DYING_CIRCLE_BITMAP_WIDTH / 2.0f, PONGPING_DYING_CIRCLE_BITMAP_HEIGHT / 2.0f, explosion_radius_2, al_map_rgba(176, 20, 4, 255) );
		    
			quickmask1(dying_circle);
			
			set_target_bitmap_to_backbuffer();
			
			
			
			al_draw_bitmap( dying_circle, cc_x - (PONGPING_DYING_CIRCLE_BITMAP_WIDTH / 2), cc_y - (PONGPING_DYING_CIRCLE_BITMAP_HEIGHT / 2 ), 0);
			   
		 }
	  }

      virtual int circle_is_collectable( void ) { return 0; }
      
      void start_speed_transition(float increment)
      {
         doing_speed_transition = true;
         increment_to_get_to = distance_increment + increment;
     
 
      }
      
      void update_increment()
      {
         distance_increment += AMOUNT_TO_ADD_TO_BALL_DISTANCE_INCREMENT_PER_UPDATE;
      
        
      
         if(distance_increment >= increment_to_get_to)
         {
            distance_increment = increment_to_get_to;
         
            doing_speed_transition = false;
         
         }      
      } 


      void set_bitmap(ALLEGRO_BITMAP *c) { circle = c; }

	  ALLEGRO_BITMAP *get_dying_circle_bitmap_ptr() { return dying_circle; }
	  
      void set_circle_type(int t) { circle_type = t; }
      int type_of_circle(void) { return circle_type; }
      
      
      int check_circles_hit( Circle *circle_could_hit, PotentialMove *pm, double ftr);
      
      int check_circle_is_clear_of_polygon( PongPingPolygon *polygon);
      
	  
	  
	  void begin_collected_phase()
	  {
	     phase = PONGPING_CIRCLE_COLLECTED_PHASE;
		 dying_updates = 0;
		 dying_circle = al_create_bitmap( PONGPING_DYING_CIRCLE_BITMAP_WIDTH, PONGPING_DYING_CIRCLE_BITMAP_HEIGHT );
	     double exponent = 1.75;
		 opacity_of_circle_explosion = ((double) pow(dying_updates, exponent) / pow(60.0f, exponent)) * 255.0f;
		 /* outer circle radius is a function of dying_updates */
		 
		 explosion_radius_1 = radius + ((double) pow(dying_updates, exponent) / pow(PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE, exponent)) * (PONGPING_DYING_CIRCLE_BITMAP_WIDTH - radius) / 2.0f;
		 
		 
		 
		 
		 exponent = 2;
		 
		 explosion_radius_2 = explosion_radius_1 - (double) PONGPING_DYING_CIRCLE_EXPLOSION_FINAL_WIDTH * ((double) dying_updates / (double) PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE );
		 
		
		 
		 if(explosion_radius_2 < 0.0)
		 {
		   explosion_radius_2 = 1.0;
		 }
		 

	  }
	  
	  int dying_updates;
	  
	  void update_dying_circle()
	  {
	     
	     
		 
		 
		 double exponent = 1.75;
		 
		 opacity_of_circle_explosion = ((double) pow(dying_updates, exponent) / pow(PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE, exponent)) * 255.0f;
		 /* outer circle radius is a function of dying_updates */
		 
		 explosion_radius_1 = radius + ((double) pow(dying_updates, exponent) / pow(PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE, exponent)) * (PONGPING_DYING_CIRCLE_BITMAP_WIDTH - radius) / 2.0f;
		 
		 //logfile << "r = " << r << "\n";
		 
		 
		 exponent = 1.5;
		 
		 //explosion_radius_2 = radius / 2.0f + ((double) pow(dying_updates, exponent) / pow(30.0f, exponent)) * (PONGPING_DYING_CIRCLE_BITMAP_WIDTH - radius - 20.0f) / 2.0f;
		 explosion_radius_2 = explosion_radius_1 - (double) PONGPING_DYING_CIRCLE_EXPLOSION_FINAL_WIDTH * ((double) dying_updates / (double)PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE );
		 if(explosion_radius_2 < 0.0)
		 {
		   explosion_radius_2 = 1.0;
		 }
		 
		 //logfile << "explosion radius = " << explosion_radius_1 << "\n";
		 //logfile << "r1 =  " << explosion_radius_2 << "\n";
		 
		 ++dying_updates;
		 if(dying_updates == PONGPING_NUMBER_OF_UPDATES_FOR_DYING_CIRCLE)
		 {
		    al_destroy_bitmap( dying_circle );
		    phase = PONGPING_CIRCLE_DELETE_PHASE;
			//logfile << "changed phase to delete phase\n";
			//eek();
		 }
	  }
	  
	  
	  void set_circle_colour(COULEUR c)
	  {
	     circle_colour = c;
	  }
	  
	  
	  /*
	  circle_vector[cn]->cc_x = circle_1_feature_moves[0]->x;
                             circle_vector[cn]->cc_y = circle_1_feature_moves[0]->y;
                             circle_vector[cn]->circle_angle = circle_1_feature_moves[0]->angle; 
                       
                             //logfile << "******************new angle is " << circle_1_feature_moves[pm_number_to_use].angle << "\n";
                       
                             circle_vector[cn]->remaining_distance -= circle_1_feature_moves[0]->distance;
                       
					         //logfile << "rd after = " << circle_vector[cn]->remaining_distance << "\n";
					   
                             circle_vector[cn]->id_of_last_polygon_hit = circle_1_feature_moves[0]->f;
                             circle_vector[cn]->side_hit = circle_1_feature_moves[0]->polygon_side;
							 circle_vector[cn]->vertex_hit = circle_1_feature_moves[0]->vertex_hit;
							 //logfile << "setting vertex hit to " <<  circle_1_feature_moves[pm_number_to_use].vertex_hit << "\n";
                             circle_vector[cn]->hit_type = circle_1_feature_moves[0]->hit_type;
	  */
	  
	  double apply_move(PotentialMove *move, int circle_number)
	  { 
	  
	     //logfile << "apply_move() : circle coordinates : " << cc_x << " , " << cc_y << " reciprocal = " << move->pfot << "\n";
		 
		 
		 if( move->pfot != 1.0 )
		 {
		    //logfile << "applying move which isn't full time unit.\n";
		 }
		 
	     cc_x += move->pfot * distance_increment * cos( circle_angle );
		 cc_y += move->pfot * distance_increment * sin( circle_angle );
		 
		 if(move->pfot > 1.0)
		 {
		    LOG("\n\napply_move() : pfot greater than 1 . It is ");
		    LOGINT(move->pfot);
		    LOG("\n\n");
		    //eek();
		 }
		 
		 
		 //logfile << "circle coordinates (AFTER CHANGES) : " << cc_x << " , " << cc_y << "\n";
		 
		/// logfile << " angle before " << circle_angle << "\n";
		 
         circle_angle = circle_number == 1 ? move->a1 : move->a2;
         
		/// logfile << " angle after " << circle_angle << "\n";
		 
		 
		 if( circle_number == 1 && move->hit_type == PONGPING_CIRCLE_TO_CIRCLE_COLLISION )
		 {
		    //logfile << "setting circle 1 (" << move->circle_id_1 << " ) as having hit circle " << move->circle_id_2 << "\n";
		    circle_hit = move->circle_id_2;
		 }
		 else if( circle_number == 2 && move->hit_type == PONGPING_CIRCLE_TO_CIRCLE_COLLISION )
		 {
		    //logfile << "setting circle 2 (" << move->circle_id_2 << " ) as having hit circle " << move->circle_id_1 << "\n";
		    circle_hit = move->circle_id_1;
		 }
		 
		 if( circle_number == 1 && move->new_di_1 != -1.0 )
		 {
		    
		 
		    distance_increment = move->new_di_1;
			
			//logfile << "changed distance_increment.\n";
			
			//eek();
			
		 }
		 
		 if( circle_number == 2 && move->new_di_2 != -1.0 )
		 {
		    distance_increment = move->new_di_2;
		 }
		          
         id_of_last_polygon_hit = move->feature_id;
         vertex_or_side_hit = move->polygon_vertex_or_side_hit;
         hit_type = move->hit_type;
		 
	  }
	  
	  /*
	  void normal_update()
	  {
	     cc_x += remaining_distance * cos( circle_angle );
         cc_y += remaining_distance * sin( circle_angle );					   
         remaining_distance = 0.0;
	  }
	  
	  */
	  void move_circle_by(double reciprocal)
	  {
	  
	     //logfile << " (move_circle_by) circle coordinates : " << cc_x << " , " << cc_y << " reciprocal = " << reciprocal << "\n";
	  
	     if(reciprocal > 1.0)
		 {
		    //logfile << "\n\n move_circle_by() : reciprocal > 1. It is " << reciprocal << "\n\n";
			eek();
		 }
	  
	  
	     cc_x += distance_increment * reciprocal * cos( circle_angle );
		 cc_y += distance_increment * reciprocal * sin( circle_angle );
		 
		 
		 //logfile << " (move_circle_by) circle coordinates (AFTER CHANGES) : " << cc_x << " , " << cc_y << "\n";
	  }
	  
	  
	  
	  int circle_inside_bat(Bat& b);
	  
	  
};






#endif


