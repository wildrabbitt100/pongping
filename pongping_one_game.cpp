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

 
#include "pongping_headers/pongping_one_game_headers.h"
 
 
 
 

ALLEGRO_BITMAP *OneGame::get_ready;
ALLEGRO_BITMAP *OneGame::level_finished_bitmap;
ALLEGRO_BITMAP *FallingCollectable::letters_bitmap;
ALLEGRO_BITMAP *FallingCollectable::bat_size_up_circle;
ALLEGRO_BITMAP *FallingCollectable::ball_size_up_circle;



vector <ALLEGRO_BITMAP *> CatchmentArea::bitmaps;
int CatchmentArea::bitmaps_loaded;

int Ball::ball_bitmap_sizes[NUMBER_OF_BALL_SIZES];



#define GAME_OVER_FLAG 1
#define LIFE_LOST_TRANSITION 2
#define GOT_FREEZE_FRAME 4
#define SHOW_LEVEL_INFO 8
#define GOT_FREEZEFRAME 16
#define LEVEL_FINISHED_TRANSITION 32


static void erase_and_clear_potential_move_vector( vector <PotentialMove *>& vec );
void erase_circle(vector<Circle *>& circles, int circle_number);


int all_collisions_catchment_areas( vector<PotentialMove *>& moves);


/* Why does this return a double ? */
double get_polar_coordinates(polar_coordinates *pc, double cx, double x, double cy, double y);

double get_polar_coordinates(polar_coordinates *pc, double cx, double x, double cy, double y)
{


   pc->radius = sqrt( (cx - x) * (cx - x) + (cy - y) * (cy - y) ); 
   
   if( cx == x )
   {
      if( y > cy )
      {
         pc->angle = PONGPING_PI/ 2.0;
      }
      else
      {
         pc->angle = 3.0 * PONGPING_PI/ 2.0;
      }
   }
   else
   {
      pc->angle = atan( (y - cy) / (x - cx) );
      if( x - cx < 0.0 )
      {
         pc->angle += ALLEGRO_PI;
      }
   }
   
}


int find_feature_number_from_id(vector <LevelFeature *> &features_for_level, int id)
{
    vector<LevelFeature*>::iterator it = features_for_level.begin();
   
    int fn = 0;
   
      do
	  {
	     if( features_for_level[ fn ]->feature_type == PONGPING_POLYGON_FEATURE )
		 {
		    if( ((PongPingPolygon *) features_for_level[fn])->polygon_id == id)
			{
			   return fn;
			}
		 }
		 
		 ++fn;
	  
	  } while(1);
	  
}

OneGame::OneGame(PongPingEssentials& ppe) 
   : init_okay(false), increments_done_so_far(0), end_when_time_reached(false), extra_life_record(0)
{
   circles_inside_polygon = 0;
   
   number_of_balls = 1;
   
   queue = ppe.queue;
   timer = ppe.timer;
   
   
   TheBat = Bat();
   if(TheBat.init_okay_function() == false)
   {
      printf("problem initialising Bat.\n");
      return;
   }
   
   
   
   bitmap_list = ppe.menu_list;
  
  
   ball_colour = al_map_rgb(255, 255, 255);
  
   /*
   int i;
   for( i = 0; i < 4; ++i)
   {
      logfile << "point x = " << TheBat.bat_vertices[i].x << "  point y = " << TheBat.bat_vertices[i].y << "\n\n";
   }
   
   */
   
   //level_finished_func[0] = time_up;
   
   /********************************************************/
   /******* Make the borders of the game screen. ***********/
   /********************************************************/
   
   borders[PONGPING_TOP_EDGE]    = al_create_bitmap(1024, 20);
   borders[PONGPING_RIGHT_EDGE]  = al_create_bitmap( 10, 768);
   borders[PONGPING_BOTTOM_EDGE] = al_create_bitmap(1024, 10);
   borders[PONGPING_LEFT_EDGE]   = al_create_bitmap( 10,768);
   
   int i;
   
   for(i = 0; i < PONGPING_NUMBER_OF_BORDERS; ++i)
   {
      al_set_target_bitmap(borders[i]);
	  al_clear_to_color(PONGPING_BLACK);
   
   }
   
   background = al_create_bitmap(PONGPING_CANVAS_WIDTH - PONGPING_BORDER_SIDE_EDGE_WIDTH * 2, PONGPING_CANVAS_HEIGHT - (PONGPING_TOP_EDGE_WIDTH + PONGPING_BOTTOM_EDGE_WIDTH));
   
   
   Ball::ball_bitmap_sizes[0] = 8;
   Ball::ball_bitmap_sizes[1] = 12;
   Ball::ball_bitmap_sizes[2] = 16;
   Ball::ball_bitmap_sizes[3] = 20;
   
   number_of_levels = ppe.get_number_of_levels();
   
   init_okay = true;
   
   
}





int OneGame::run_level( PongPingEssentials& ppe )
{
   
   int result;
   bool left_key_down, right_key_down;                /* Variables for storing whether or not left and right keys are down or up. */
   ALLEGRO_EVENT event;                         /* for storing an event fetched from the queue */
   bool quit = false;
   bool need_redraw = true;
   bool level_info_shown = false;
   int line_num = 0;
   int time;
   int new_time; /* needed for when fading out text at beginning. */
   float opacity;
   int feature_number;
   
   ifstream level_file;
   
   ALLEGRO_BITMAP *level_info_bitmap_ptr;// = get_level_info_bitmap_ptr();
   
   ALLEGRO_BITMAP *ff = NULL; /* freeze frame */
   
   ALLEGRO_BITMAP *game_over_clone = NULL;
   
   int level_state;
   
   int feature_hit;
   
   int space_key = 0;
   
   
   int at_least_one_collision;
   int pm_number_to_use;
   int pm_number;
   double shortest;
   
   int step_through = 0;
   
   int features_updated = 0;
   
   ALLEGRO_KEYBOARD_STATE kbd;
   
   ALLEGRO_BITMAP *black_rectangle;
   int get_black_rectangle = 0;
   
   
   //PotentialMove pm, circle_to_circle_move;
   
   
   
   
   
   vector <PotentialMove *> circle_1_feature_moves;
   vector <PotentialMove *> circle_2_feature_moves;
   
   
   
   int circle_number;
   int cn;
   
   int cn1, cn2;
   
   int check_features_hit_circles = 0;
   
   int draws = 0;
   int updates = 0;
   
   int which_side;
   
   int collision_type__;
   
   int circle_to_check_a_circle_hits;
   
   int one_or_more_collisions; //variable to store the number of collisions that are on the list for a circle
   
   int circle_updated;
   
   double last_distance_used;
   
   
   
   double c1_cn_to_use, c2_cn_to_use; /* circle 1 collision number */
   
   int Lfn; // level feature number
   
   
   int circles_inside = 0;
   
   int do_bat_size_transition = 0;
  
   
   PongPingPolygon *play_area = new PongPingPolygon();

   play_area->clear_ids();

   play_area = new PongPingPolygon( BALL_ARENA_LEFT_EDGE_X + (BALL_ARENA_RIGHT_EDGE_X - BALL_ARENA_LEFT_EDGE_X) / 2.0,
                              BALL_ARENA_TOP_EDGE_Y + (BALL_ARENA_BOTTOM_EDGE_Y - BALL_ARENA_TOP_EDGE_Y) / 2.0,
                              4, 0, 0, 0, 0, PLAY_AREA | CIRCLES_INSIDE_ONLY | IRREGULAR);
   
   
   
   polar_coordinates play_area_point;

   get_polar_coordinates( &play_area_point,  BALL_ARENA_LEFT_EDGE_X + (BALL_ARENA_RIGHT_EDGE_X - BALL_ARENA_LEFT_EDGE_X) / 2.0, BALL_ARENA_LEFT_EDGE_X, 
                                                   BALL_ARENA_TOP_EDGE_Y + (BALL_ARENA_BOTTOM_EDGE_Y - BALL_ARENA_TOP_EDGE_Y) / 2.0, BALL_ARENA_TOP_EDGE_Y );
  
   play_area->unrotated_points.push_back(play_area_point);
   
   get_polar_coordinates( &play_area_point, BALL_ARENA_LEFT_EDGE_X + (BALL_ARENA_RIGHT_EDGE_X - BALL_ARENA_LEFT_EDGE_X) / 2.0, BALL_ARENA_RIGHT_EDGE_X, 
                                                   BALL_ARENA_TOP_EDGE_Y + (BALL_ARENA_BOTTOM_EDGE_Y - BALL_ARENA_TOP_EDGE_Y) / 2.0, BALL_ARENA_TOP_EDGE_Y );
  
   play_area->unrotated_points.push_back(play_area_point);
   
   get_polar_coordinates( &play_area_point, BALL_ARENA_LEFT_EDGE_X + (BALL_ARENA_RIGHT_EDGE_X - BALL_ARENA_LEFT_EDGE_X) / 2.0, BALL_ARENA_RIGHT_EDGE_X, 
                                                   BALL_ARENA_TOP_EDGE_Y + (BALL_ARENA_BOTTOM_EDGE_Y - BALL_ARENA_TOP_EDGE_Y) / 2.0, BALL_ARENA_BOTTOM_EDGE_Y );
  
   play_area->unrotated_points.push_back(play_area_point);
   
   get_polar_coordinates( &play_area_point, BALL_ARENA_LEFT_EDGE_X + (BALL_ARENA_RIGHT_EDGE_X - BALL_ARENA_LEFT_EDGE_X) / 2.0, BALL_ARENA_LEFT_EDGE_X,
                                                   BALL_ARENA_TOP_EDGE_Y + (BALL_ARENA_BOTTOM_EDGE_Y - BALL_ARENA_TOP_EDGE_Y) / 2.0, BALL_ARENA_BOTTOM_EDGE_Y );
   
   play_area->unrotated_points.push_back(play_area_point);
   
   features_for_level.push_back( (LevelFeature *) play_area);
   
   //PongPingPolygon::PongPingPolygon(int cx, int cy, int sides, float r, float ic, int direction_of_spin, float angle_, int _attributes)
   
   PongPingPolygon *testpolygon = new PongPingPolygon( 400, 400, 5, 100, 0.0, 1, 0.0, ONE_COLOURED_POLYGON | REGULAR);
   
   testpolygon->polygon_colours.push_back( al_map_rgb(0, 200, 50) );
   
   features_for_level.push_back( (LevelFeature *) testpolygon);
   
   borders[PONGPING_TOP_EDGE]    = al_create_bitmap(1024, 20);
   borders[PONGPING_RIGHT_EDGE]  = al_create_bitmap( PONGPING_BORDER_SIDE_EDGE_WIDTH, 768);
   borders[PONGPING_BOTTOM_EDGE] = al_create_bitmap(1024, 30);
   borders[PONGPING_LEFT_EDGE]   = al_create_bitmap( PONGPING_BORDER_SIDE_EDGE_WIDTH,768);
   
   
   
   
   
   
   //al_reserve_samples(100);
   
  

   /* Store the ball_colour before read_level() because read_level changes ball_colour to the new one for this level. */
   old_ball_colour = ball_colour;
   
   logfile << "reading level.\n";
   
   
   /*
   if(read_level() != 0 )
   {
      #ifdef PONGPING_DEBUG_BUILD
         logfile << "problem reading level.\n";
	  #endif
      return PONGPING_INITIALISATION_PROBLEM;
   }
   */
   
   #ifdef LEVEL_TO_SIMULATE_GAME_ENDING
      set_short_game_end_time();
   #endif
   
   
   
   
   int test_circle_number = 0;
   
   
   for( ; test_circle_number < 20; ++ test_circle_number )
   {
      FallingCollectable *fc = new FallingCollectable( pongping_random_number( 100, 900), pongping_random_number( 40, 700), PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE,
                                                       test_circle_number % 6, 200, 2	  );
   
      circle_vector.push_back((Circle *) fc);
	  
  	  //logfile << "circle " << test_circle_number << " , angle is " << circle_vector[test_circle_number]->circle_angle << " \n";
   }
   
   
   //al_clear_to_color(background_colour);
   //al_flip_display();
   //eek();
   
   /***************************************************************************************************************************************/
   /***************************************************************************************************************************************/
   
   al_set_target_bitmap(borders[PONGPING_TOP_EDGE]);
   al_clear_to_color(border_colour);
   make_bitmap_a_gradient(borders[PONGPING_TOP_EDGE], 0, PONGPING_TOP_EDGE_WIDTH * 0.25, PONGPING_CANVAS_WIDTH, PONGPING_TOP_EDGE_WIDTH, PONGPING_VERTICAL_GRADIENT_LIGHT_EDGE_BOTTOM);
   
   al_draw_filled_triangle(0, 0, 0, PONGPING_TOP_EDGE_WIDTH, PONGPING_BORDER_SIDE_EDGE_WIDTH - 1, PONGPING_TOP_EDGE_WIDTH, PONGPING_MASK_COLOR );
   al_draw_filled_triangle(1024, 0, 1024, PONGPING_TOP_EDGE_WIDTH, 1024 - PONGPING_BORDER_SIDE_EDGE_WIDTH + 1, PONGPING_TOP_EDGE_WIDTH, PONGPING_MASK_COLOR ); 
   
   quickmask1(borders[PONGPING_TOP_EDGE]);
   
   /***************************************************************************************************************************************/
   /***************************************************************************************************************************************/
   
   al_set_target_bitmap(borders[PONGPING_BOTTOM_EDGE]);
   al_clear_to_color(border_colour);
   
   make_bitmap_a_gradient(borders[PONGPING_BOTTOM_EDGE], 0, 0, PONGPING_CANVAS_WIDTH, PONGPING_BOTTOM_EDGE_WIDTH * 0.75, PONGPING_VERTICAL_GRADIENT_LIGHT_EDGE_TOP );
   
   al_draw_filled_triangle(0, 0, PONGPING_BORDER_SIDE_EDGE_WIDTH - 1, 0, 0, PONGPING_BOTTOM_EDGE_WIDTH + 1, PONGPING_MASK_COLOR );
  
   al_draw_filled_triangle(PONGPING_CANVAS_WIDTH - PONGPING_BORDER_SIDE_EDGE_WIDTH + 1, 0, PONGPING_CANVAS_WIDTH, 0, PONGPING_CANVAS_WIDTH, PONGPING_BOTTOM_EDGE_WIDTH + 1, PONGPING_MASK_COLOR);
   
   quickmask1(borders[PONGPING_BOTTOM_EDGE]);
   
   /***************************************************************************************************************************************/
   
   al_set_target_bitmap(borders[PONGPING_RIGHT_EDGE]);
   al_clear_to_color(border_colour);
   make_bitmap_a_gradient(borders[PONGPING_RIGHT_EDGE], 0, 0, PONGPING_BORDER_SIDE_EDGE_WIDTH * 0.75, PONGPING_CANVAS_HEIGHT, PONGPING_HORIZONTAL_GRADIENT_LIGHT_EDGE_LEFT);
   
   al_draw_filled_triangle(0, 0, PONGPING_BORDER_SIDE_EDGE_WIDTH, 0, 0, PONGPING_TOP_EDGE_WIDTH + 1, PONGPING_MASK_COLOR );
   
   al_draw_filled_triangle(0, PONGPING_CANVAS_HEIGHT, PONGPING_BORDER_SIDE_EDGE_WIDTH, PONGPING_CANVAS_HEIGHT + 1, 0, PONGPING_CANVAS_HEIGHT - PONGPING_BOTTOM_EDGE_WIDTH - 2, PONGPING_MASK_COLOR);
   
   quickmask1(borders[PONGPING_RIGHT_EDGE]);
  
   /***************************************************************************************************************************************/
   /***************************************************************************************************************************************/
   /* LEFT EDGE */
   
   al_set_target_bitmap(borders[PONGPING_LEFT_EDGE]);
   al_clear_to_color(border_colour);
   make_bitmap_a_gradient(borders[PONGPING_LEFT_EDGE], PONGPING_BORDER_SIDE_EDGE_WIDTH * 0.25, 0, PONGPING_BORDER_SIDE_EDGE_WIDTH, PONGPING_CANVAS_HEIGHT, PONGPING_HORIZONTAL_GRADIENT_LIGHT_EDGE_RIGHT);
   al_draw_filled_triangle(0, 0, PONGPING_BORDER_SIDE_EDGE_WIDTH, 0, PONGPING_BORDER_SIDE_EDGE_WIDTH, PONGPING_TOP_EDGE_WIDTH + 1, PONGPING_MASK_COLOR );
   al_draw_filled_triangle(0, PONGPING_CANVAS_HEIGHT + 1, PONGPING_BORDER_SIDE_EDGE_WIDTH, PONGPING_CANVAS_HEIGHT, PONGPING_BORDER_SIDE_EDGE_WIDTH, PONGPING_CANVAS_HEIGHT - PONGPING_BOTTOM_EDGE_WIDTH - 2, PONGPING_MASK_COLOR);
   
   quickmask1(borders[PONGPING_LEFT_EDGE]);
   
   /***************************************************************************************************************************************/
   
   
   /*
   if( arrange_collectables() != 0 )
   {
      #ifdef PONGPING_DEBUG_BUILD
         logfile << "problem reading level.\n";
	  #endif
      return PONGPING_INITIALISATION_PROBLEM;
   }
   */
   //wait_for_key_down();
   
    
   
   
   //logfile << "pushing circle.\n\n";
   
   //eek();
   
   //logfile << "read level.\n";
   
   level_info_bitmap_ptr = get_level_info_bitmap_ptr();
   
   //logfile << "got level info pointer.\n\n";
   
   /*
   for(feature_number = 0; feature_number < features_for_level.size(); ++feature_number)
   { 
      //logfile << "updated polygon " << feature_number << "\n\n";
      features_for_level[feature_number]->update();
      
      features_for_level[feature_number]->set_convex_or_concave_flag();
   }
   */
   
   
   
   
   //logfile << "read level.\n";
   
   //logfile << "ball x : " << initial_ball_x << " ball y : " << initial_ball_y << endl;
   
   
   /* Make sure the only circles in the vector are balls. There might be other types left over from a previous
      level which should carry over. */
   
   #ifdef LEVEL_TO_SIMULATE_GAME_ENDING
      remove_non_ball_circles();
      circle_vector[0]->set_double(DISTANCE_INCREMENT, 5.0);
      circle_vector[0]->set_double(CIRCLE_X, 100);
      circle_vector[0]->set_double(CIRCLE_Y, 100);
      circle_vector[0]->set_double(CIRCLE_ANGLE, 0.3);
   #endif
   
   //logfile << "circle vector size is " << circle_vector.size() << "\n";
   
   
   
   
   // call goes here.
   
   
   
   //logfile << "cn was " << cn << "circle vector size is " << circle_vector.size() << "\n";
   
 
   /* start the timer from 0 */
   
   al_stop_timer(timer);
   al_set_timer_count(timer, 0);
   
   
   al_flush_event_queue(queue);
   
   
  
   
   
   
   
   
  
   left_key_down = right_key_down = false;
   
   al_flush_event_queue(queue);
   
   
   
   
   al_start_timer(timer);
   
   /* Need to get the timer count so to know when to stop drawing text box saying what the level is and task is for the level. */
   
   time = al_get_timer_count(timer);
   
   level_state = SHOW_LEVEL_INFO;
   
   al_get_keyboard_state(&kbd);
   if(al_key_down(&kbd, ALLEGRO_KEY_LEFT))
   {
      left_key_down = true;
   }
   if(al_key_down(&kbd, ALLEGRO_KEY_RIGHT))
   {
      right_key_down = true;
   }
   
   
   
   
   
   //((PongPingPolygon*)features_for_level[0])->print_out_vertex_data();
    
   
   
   
   
   //logfile << "going into game loop.\n";
   updates = 0;
   while( quit == false )
   {
       printf(" ");
       
       
       
       if( !(level_state & SHOW_LEVEL_INFO) && !(level_state & LIFE_LOST_TRANSITION) && space_key == 0 )
       {
        
       al_wait_for_event(queue, &event);
   
       
   
       switch(event.type)
       {
          case ALLEGRO_EVENT_KEY_CHAR:
             break;
          case ALLEGRO_EVENT_DISPLAY_CLOSE:
             
			 
			 result = PONGPING_LEVEL_RESULT_DISPLAY_CLOSED;
             quit = true;
              
             break;
          case ALLEGRO_EVENT_TIMER:
             
             //logfile << "dealing with timer tick.\n\n\n";
             
             if( level_state & (GAME_OVER_FLAG | LEVEL_FINISHED_TRANSITION))
             {
                need_redraw = true;
                break;
             }
             
             
			 //logfile << "calling update objects.\n";
	  
             update_objects();
            
             need_redraw = true;
             
			 //logfile << "redraw is on.************************************************************************************\n";
			 
             ++updates;
             
             break;
             
          case ALLEGRO_EVENT_KEY_DOWN:
             if(event.keyboard.keycode == ALLEGRO_KEY_LEFT) 
             {
                left_key_down = true;
             }
             else if( event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
             { 
                right_key_down = true;
             }
             else if( event.keyboard.keycode == ALLEGRO_KEY_SPACE)
             {
                space_key = 1;
               
             
             }
             else if(event.keyboard.keycode == ALLEGRO_KEY_Q)
             {
                step_through = 1;
             
             }
             #ifdef SKIP_LEVEL
             else if( event.keyboard.keycode == ALLEGRO_KEY_S)
             {
                
                al_destroy_bitmap(ff);
                al_destroy_bitmap(level_info_bitmap_ptr);
                
                return PONGPING_LEVEL_RESULT_GO_TO_NEXT_LEVEL;
                
             }
             #endif
             else if( event.keyboard.keycode == ALLEGRO_KEY_F1)
             {
                int fullscreen_set = al_get_display_flags(al_get_current_display()) & ALLEGRO_FULLSCREEN_WINDOW ? 1 : 0;
                al_toggle_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN_WINDOW, ~fullscreen_set & 1); 
             }
             else if( event.keyboard.keycode == ALLEGRO_KEY_M)
             {
                check_features_hit_circles = 1;
             }
             else if( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
             {
                
                result = PONGPING_LEVEL_RESULT_DISPLAY_CLOSED;
                quit = true;
             }
                
             break;
          case ALLEGRO_EVENT_KEY_UP:
             if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
             {
                left_key_down = false;
             }
             else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
             {
                right_key_down = false;
             }
             else if( event.keyboard.keycode == ALLEGRO_KEY_SPACE)
             {
                space_key = 0;
             
             }
             break;
          case ALLEGRO_EVENT_JOYSTICK_AXIS:
             //logfile << "got joystick event.\n";
             if(ppe.current_control_type == PONGPING_USING_JOYPAD_OR_JOYSTICK && event.joystick.axis == 0 )
             {
                if(event.joystick.pos < 0.0)
                {
                   left_key_down = true;
                }
                else 
                {
                   left_key_down = false;
                }
                
                if(event.joystick.pos > 0.0)
                {
                   right_key_down = true;   
                }
                else right_key_down = false;
             
             }
          default:
             printf("some other event\n");
          
       
       }
	   
	   
	   
       }
       else
       {
          al_wait_for_event(queue, &event);
   
          if(event.type == ALLEGRO_EVENT_TIMER)
          {
              need_redraw = true;     
          }
       }
       
       
       if((  (level_state & GAME_OVER_FLAG) || (level_state & LEVEL_FINISHED_TRANSITION) ) && 
           !(level_state & GOT_FREEZEFRAME))
       {
          
          ff = al_create_bitmap(PONGPING_CANVAS_WIDTH, PONGPING_CANVAS_HEIGHT);
          if(!ff)
          {
             return PONGPING_LEVEL_RESULT_INITIALISATION_PROBLEM;
          }
          ff = al_clone_bitmap(al_get_backbuffer(al_get_current_display()));
          
          
          /* Set the GOT_FREEZEFRAME flag so this if statement gets ignored next time around. */
          
          level_state |= GOT_FREEZEFRAME;
          
       }
      
       //logfile << "here 3.\n";
       
       #ifdef CHECK_BALL_ANGLE
          if(circle_vector[0]->circle_angle < 0.0f || circle_vector[0]->circle_angle > PONGPING_PI* 2.0f)
          {
             printf(" angle out of range (%f)\n\n", circle_vector[0]->circle_angle);
             system("PAUSE");
             exit(0);
          }
       #endif
       
 
  
       //logfile << "finished updating\n\n\n\n\n\n";
       
       if(step_through)
       {
          logfile << "waiting to draw.\n";
          al_stop_timer(timer);
          if( wait_for_key_down() == 2 )
          {
             result = PONGPING_LEVEL_RESULT_DISPLAY_CLOSED;
             quit = true;
          
          }
          al_flush_event_queue(queue);
          al_start_timer(timer);
		  
		  
       }
       
       
       if(get_black_rectangle)
       {
          black_rectangle = al_create_bitmap(PONGPING_CANVAS_WIDTH, PONGPING_CANVAS_HEIGHT);
          if(black_rectangle == NULL)
          {
             result = PONGPING_MEMORY_PROBLEM;
             quit = true;
          }
          else
          {
             al_set_target_bitmap(black_rectangle);
             al_clear_to_color(BLACK);
          }
          get_black_rectangle = 0;
       }
      
       //logfile << "point 4444.\n";
      
	  
	   //logfile << "need_redraw = " << need_redraw << "\n";
	   //logfile << "al_event_queue_is_empty(queue) " << al_event_queue_is_empty(queue) << "\n";
       
       if((need_redraw && al_event_queue_is_empty(queue)) || circle_vector[0]->gone_off_bottom()) 
       {
         
          
       
          /* Draw Stuff. */
          
          if( level_state & SHOW_LEVEL_INFO)
          {
            
          
             new_time = al_get_timer_count(timer);
             if( new_time < time + TICKS_TO_WAIT_WHILE_SHOWING_LEVEL_INFORMATION)
             {
			    //logfile << "drawing.\n";
			 
                set_target_bitmap_to_backbuffer();
             
                al_clear_to_color(PONGPING_GUI_BACKGROUND_COLOR);
                
                al_draw_bitmap( level_info_bitmap_ptr, (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(level_info_bitmap_ptr)) / 2, 
                                                       (PONGPING_CANVAS_HEIGHT - al_get_bitmap_height(level_info_bitmap_ptr)) / 2 , 0);
             
                al_flip_display();
             
             
               
                //logfile << "point 4450.\n";
             
             }
             else
             {
                al_destroy_bitmap(level_info_bitmap_ptr);
                level_info_bitmap_ptr = NULL;
                
                level_state = level_state & ~SHOW_LEVEL_INFO;
            
                level_state |= LIFE_LOST_TRANSITION;
                
				
				
                time = al_get_timer_count(timer);
                
                //logfile << "switched off show level info flag.\n\n";
				//ek();
             }
          }
          else if( level_state & LEVEL_FINISHED_TRANSITION )
          {
            
          
             if( al_get_timer_count(timer) > time + TICKS_TO_WAIT_IN_LEVEL_FINISHED_TRANSITION )
             {
                
                al_destroy_bitmap(ff);
                al_destroy_bitmap(level_info_bitmap_ptr);
                
                
                
                return PONGPING_LEVEL_RESULT_GO_TO_NEXT_LEVEL;
             }
             else
             {
                al_draw_bitmap(ff, 0, 0, 0);
                
                al_draw_bitmap( bitmap_list->bitmaps[LEVEL_COMPLETE], (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(bitmap_list->bitmaps[LEVEL_COMPLETE])) / 2, 
                                                       (PONGPING_CANVAS_HEIGHT - al_get_bitmap_height(bitmap_list->bitmaps[LEVEL_COMPLETE])) / 2, 0);  
                al_flip_display();
             }
          }
          else
          {
             //logfile << "drawing to backbuffer.\n\n";
          
             set_target_bitmap_to_backbuffer();
             
             //logfile << "returned.\n";
             
             
          
                
                if(!(level_state & GAME_OVER_FLAG))
                {
                   //logfile << "drawing level.\n\n";
                   
                   draw_level();
                   
                   //logfile << "returned.\n";
                }
                
                
                if( level_state & LIFE_LOST_TRANSITION )
                {  
                   
                   new_time = al_get_timer_count(timer);
                   if(new_time < time + TICKS_TO_WAIT_BETWEEN_LIVES)
                   {
                      al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
                      //logfile << "point 4440.\n\n";
                      al_draw_filled_rectangle(BALL_ARENA_LEFT_EDGE_X, BALL_ARENA_TOP_EDGE_Y, BALL_ARENA_RIGHT_EDGE_X, BALL_ARENA_BOTTOM_EDGE_Y, al_map_rgba(0, 0, 0, 128));
                   
                      //logfile << "point 4450.\n\n";
                      al_draw_bitmap(bitmap_list->bitmaps[GET_READY], 
                                    (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(bitmap_list->bitmaps[GET_READY])) / 2, 
                                    (PONGPING_CANVAS_HEIGHT - al_get_bitmap_height(bitmap_list->bitmaps[GET_READY])) / 2, 0); 
                                    
                      //logfile << "point 4500.\n";
                   }
                   else
                   {
                      //logfile << "point 4501.\n";
                      
                      /* switch off LIFE_LOST_TRANSITION flag. */
                      level_state = level_state & ~LIFE_LOST_TRANSITION;
                   
				   
				      /* This makes it so the program stops after every draw until a key is pressed. It's here so that the program starts stopping
					     after the */
                      //step_through = 1;
                
                      al_flush_event_queue(queue);
                      al_get_keyboard_state(&kbd);
                      
                      left_key_down = al_key_down(&kbd, ALLEGRO_KEY_LEFT) ? true : false;
                      
                      right_key_down = al_key_down(&kbd, ALLEGRO_KEY_RIGHT) ? true : false;
                      
                      //step_through =1 ;
					  //logfile << "done.\n";
					  //eek();
                      
                   }
                }
                else if( level_state & GAME_OVER_FLAG )
                {
                   int game_over_tint_opacity; // when the 'game over' words get drawn, a black version is drawn over the 

                   if( ((float) al_get_timer_count(timer) - (float) time) < TICKS_FOR_GAME_OVER_WORDS_FADE_IN )
                   {
                      game_over_tint_opacity = 255.0f - 255.0f * ((float) al_get_timer_count(timer) - (float) time) / (float) TICKS_FOR_GAME_OVER_WORDS_FADE_IN;
                      //logfile << game_over_tint_opacity << "\n\n";
                   }
                   else
                   {
                      game_over_tint_opacity = 0;
                   }
                   
                   al_draw_bitmap(bitmap_list->bitmaps[GAME_OVER_BITMAP], (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(bitmap_list->bitmaps[GAME_OVER_BITMAP]) ) / 2,
                                                                   (PONGPING_CANVAS_HEIGHT - al_get_bitmap_height(bitmap_list->bitmaps[GAME_OVER_BITMAP]))/ 2, 0);
                   //al_set_target_bitmap(bitmap_list->bitmaps[GAME_OVER_BITMAP]);
                   al_draw_tinted_bitmap(game_over_clone, al_map_rgba(0, 0, 0, game_over_tint_opacity), (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(bitmap_list->bitmaps[GAME_OVER_BITMAP]) ) / 2, 
                                                                                    (PONGPING_CANVAS_HEIGHT - al_get_bitmap_height(bitmap_list->bitmaps[GAME_OVER_BITMAP]))/ 2, 0);
                   
                   //set_target_bitmap_to_backbuffer();
                   
                   if( al_get_timer_count(timer) - time >= TICKS_FOR_TO_SHOW_GAME_OVER_WORDS_FOR) quit = true;
                   
                }
                
				/*
                int o = 0; 
				
				for(; o < circle_vector.size(); ++o) 
				{
				    logfile << "circle " << o << " angle : circle_vector[o]->circle_angle " << "\n";
				}
                */
                //logfile << " Drawn *******************************************************************************************.\n";
                
				//logfile << " Drawn *******************************************************************************************.\n";
				
                al_flip_display();
                
                //logfile << "flipped display.\n";
          } 
          
		  /*
          for(cn1 = 0; cn1 < circle_vector.size(); ++cn1)
          {
			    if(circle_vector[cn1]->circle_inside_bat(TheBat))
				{
				   logfile << "circle is inside bat. Stopping.\n\n";
				   logfile.flush();
				   logfile << "a circle is inside bat.\n";
				   eek();
				}
		  
		  }
		  */
          need_redraw = false;
          
          
          
          if(!(level_state & LEVEL_FINISHED_TRANSITION) && level_finished() )
          {
             level_state |= LEVEL_FINISHED_TRANSITION;
             time = al_get_timer_count(timer);    
          }
          
          
          
          
       }
      
    }
    
    
    clear_features_from_vector();
    
    circle_vector.clear();
    
    if(game_over_clone)
    {
       al_destroy_bitmap(game_over_clone);
    }
    
    return result;
}






void OneGame::GetSetForNewGame( PongPingEssentials& ppe)
{
   lives = LIVES_AT_START_OF_GAME;
   current_level = ppe.get_level_to_start_at();
   logfile << "current_level = " << current_level << "\n";
   extra_life_record = 0;
   
   
   //circle_vector.push_back( (Circle *) new Ball());
}




OneGame::~OneGame() { }







#define GAP_BETWEEN_EXTRA_LIFE_CIRCLES 10


static bool compare_draw_priority( LevelFeature *f1, LevelFeature *f2 )
{
   return f1->draw_priority < f2->draw_priority;

}


void OneGame::draw_level()
{
   int feature_number;
   int circle_number;
   
   unsigned char r, g, b;
   
   al_unmap_rgb(border_colour, &r, &g, &b);
   
   //logfile << "draw level point 1.\n\n";
   
   //al_draw_bitmap(background, BALL_ARENA_LEFT_EDGE_X, BALL_ARENA_TOP_EDGE_Y, 0);
   
   al_clear_to_color(background_colour);
   TheBat.Draw();
   
   //logfile << "bat x = " << TheBat.get_x() << "bat y = " << BAT_Y_POSITION << "\n\n";
   
   //logfile << "draw level point 2.\n\n";
   
   
   
   //logfile << "draw level point 3.\n\n";
   
   
   

   /* Strip on left */
   
   al_draw_bitmap(borders[PONGPING_LEFT_EDGE], 0, 0, 0); 
   
   /* Strip on right. */
   
   al_draw_bitmap(borders[PONGPING_RIGHT_EDGE], BALL_ARENA_RIGHT_EDGE_X, 0, 0);
   
   /* Strip across bottom. */
   
   al_draw_bitmap(borders[PONGPING_BOTTOM_EDGE], 0, BALL_ARENA_BOTTOM_EDGE_Y, 0);
   
   /* Strip across top */
   
   al_draw_bitmap(borders[PONGPING_TOP_EDGE], 0, 0, 0);
   
   /* draw some white lines on edge of border */
   
   //al_draw_line( BALL_ARENA_LEFT_EDGE_X - 1, BALL_ARENA_TOP_EDGE_Y - 1, BALL_ARENA_RIGHT_EDGE_X - 1, BALL_ARENA_TOP_EDGE_Y - 1, WHITE, 1);
   
   //al_draw_line( BALL_ARENA_LEFT_EDGE_X - 1, BALL_ARENA_BOTTOM_EDGE_Y, BALL_ARENA_RIGHT_EDGE_X - 1, BALL_ARENA_BOTTOM_EDGE_Y, WHITE, 1);
   
   //al_draw_line( BALL_ARENA_LEFT_EDGE_X - 1, BALL_ARENA_TOP_EDGE_Y - 1, BALL_ARENA_LEFT_EDGE_X - 1, BALL_ARENA_BOTTOM_EDGE_Y, WHITE, 1);
   
   //logfile << "draw level point 9.\n\n";
   
   
   sort(features_for_level.begin(), features_for_level.end(), compare_draw_priority);
   
   for(feature_number = 0; feature_number < features_for_level.size(); ++feature_number)
   {
      //logfile << "drawing feature " << feature_number << "\n\n";
   
      
	  features_for_level[feature_number]->draw();
	 
	  
	  #ifdef PONGPING_LABEL_POLYGONS
	  if(features_for_level[feature_number]->feature_type == PONGPING_POLYGON_FEATURE)
	  {
	     ((PongPingPolygon *) features_for_level[feature_number])->label(feature_number);
	  }
	  #endif
   }
   
   
   //logfile << "drawing circles.\n\n";
   
   for(circle_number = 0; circle_number < circle_vector.size(); ++circle_number)
   {
      //logfile << "drawing circle " << circle_number << "\n\n";
      
	  if( TheBat.circle_near(circle_vector[circle_number]) )
	  {
	     //logfile << "circle x = " << circle_vector[circle_number]->cc_x << "\n";
	     //logfile << "circle y = " << circle_vector[circle_number]->cc_y << "\n";
		 //logfile << "circle angle = " << circle_vector[circle_number]->circle_angle << "\n";
	  }
	  
      circle_vector[circle_number]->draw();
   }
   
   //logfile << "drawn******************************************************************\n\n";
  // logfile << "draw level point 10.\n\n";
   
   
   //al_draw_textf( pongping_fonts[IN_GAME_INFO_FONT].font, WHITE, 950, BALL_ARENA_BOTTOM_EDGE_Y + (PONGPING_CANVAS_HEIGHT - BALL_ARENA_BOTTOM_EDGE_Y - pongping_fonts[IN_GAME_INFO_FONT].height) / 2, ALLEGRO_ALIGN_LEFT, "TIME %d", (int) floor(al_get_timer_count(timer) / 60.0f));  
  
   //al_draw_textf( pongping_fonts[IN_GAME_INFO_FONT].font, WHITE, 20, BALL_ARENA_BOTTOM_EDGE_Y + (PONGPING_CANVAS_HEIGHT - BALL_ARENA_BOTTOM_EDGE_Y - pongping_fonts[IN_GAME_INFO_FONT].height) / 2, ALLEGRO_ALIGN_LEFT, "BATS %d", lives - 1);    
  
   /* Reuse circle_number rather than declare another variable. */
  
  // logfile << "draw level point 100.\n\n";
  
   for(circle_number = 0; circle_number < 9; ++circle_number)
   {
      al_draw_filled_circle(  150 + circle_number * (2.0 * 13.0 + GAP_BETWEEN_EXTRA_LIFE_CIRCLES), BALL_ARENA_BOTTOM_EDGE_Y + (PONGPING_CANVAS_HEIGHT - BALL_ARENA_BOTTOM_EDGE_Y) / 2.0f, 13.0, BLACK);
      
	  al_draw_circle(150 + circle_number * (2.0 * 13.0 + GAP_BETWEEN_EXTRA_LIFE_CIRCLES), BALL_ARENA_BOTTOM_EDGE_Y + (PONGPING_CANVAS_HEIGHT - BALL_ARENA_BOTTOM_EDGE_Y) / 2.0f, 13.0, al_map_rgb( (float) r * 0.5, (float) g * 0.5, (float) b * 0.5), 1);
      al_draw_circle(150 + circle_number * (2.0 * 13.0 + GAP_BETWEEN_EXTRA_LIFE_CIRCLES), BALL_ARENA_BOTTOM_EDGE_Y + (PONGPING_CANVAS_HEIGHT - BALL_ARENA_BOTTOM_EDGE_Y) / 2.0f, 14.0, al_map_rgb( r, g, b), 1);
	  
      if(extra_life_record & (1 << circle_number))
      {
         al_draw_bitmap_region( bitmap_list->bitmaps[EXTRA_LIFE_LETTERS], 24 * circle_number, 0, 24, 24, 150 + circle_number * (2.0 * 13.0 + GAP_BETWEEN_EXTRA_LIFE_CIRCLES) - 12.0,
                         BALL_ARENA_BOTTOM_EDGE_Y + (PONGPING_CANVAS_HEIGHT - BALL_ARENA_BOTTOM_EDGE_Y) / 2.0f - 12.0,  0);
      }
   }
  
  
   
   
}







ALLEGRO_BITMAP *OneGame::get_level_info_bitmap_ptr()
{
   ALLEGRO_BITMAP *bmp_p;
   int text_width;
   int first_non_zero_digit_found;
   int digit;
   int next_space_for_digit;
   int digit_value;
   int level_digits[3];
   int number_of_digits;
   int ln_bitmap_width;
   
   char line_of_text[40];
   
   int y_for_line;
   
   char level_id[30];
   const char *level_id_prefix = "LEVEL ";
   
   
   bmp_p = al_create_bitmap( ceil((float) LEVEL_INFORMATION_BITMAP_HEIGHT * 1.618), LEVEL_INFORMATION_BITMAP_HEIGHT);
   if(bmp_p == NULL)
   {
      return bmp_p;
   }

   al_set_target_bitmap(bmp_p);
   
   al_draw_filled_rectangle(0, 0, al_get_bitmap_width(bmp_p), al_get_bitmap_height(bmp_p), IN_GAME_INFO_TEXT_BOX_BORDER_COLOR);
   
   al_draw_filled_rectangle(IN_GAME_INFO_TEXT_BOX_BORDER_SIZE, IN_GAME_INFO_TEXT_BOX_BORDER_SIZE,
                            al_get_bitmap_width(bmp_p) - IN_GAME_INFO_TEXT_BOX_BORDER_SIZE, 
                            al_get_bitmap_height(bmp_p) - IN_GAME_INFO_TEXT_BOX_BORDER_SIZE,
                            IN_GAME_INFO_TEXT_BOX_MAIN_COLOR);
   
  
  
  return bmp_p;
  
}







int OneGame::level_finished()
{
   int i;
   int finished = 0;
   
   //logfile << "checking if level is finished.\n";
   
   if(end_when_time_reached == true)
   {
      return al_get_timer_count(timer) > (time_to_reach * PONGPING_TIMER_TICKS_PER_SECOND);
   }
   
   for(i = 0; i < features_for_level.size(); ++i )
   {
      if( features_for_level[i]->is_done() == 0)
      {
         //logfile << "returning 0 for level_finished.\n";
       
         return 0;
      }
   }
   
   //logfile << "returning 1 for level_finished().\n";
   
   /* all features are done */
   
   return 1;
}


















int OneGame::arrange_collectables(void)
{
   vector <int> polygon_features;
   vector <int> polygons_assigned_a_collectable;
   vector <int> possible_polygons;
   int i, j;
   int polygon_attributes;
   
   int letter_frequencies[NUMBER_OF_LETTER_SUBTYPES];
   
   int letter_number;
   int polygon_random_number;
   
   memset(letter_frequencies, 0, sizeof(int) * NUMBER_OF_LETTER_SUBTYPES);
   
   //logfile << "feature vector size is " << features_for_level.size() << "\n";
   
   /* Count the number of features which are polygons. */
   
   for(i = 0; i < features_for_level.size(); ++i)
   {
      if(features_for_level[i]->feature_type == PONGPING_POLYGON_FEATURE)
      {
         polygon_attributes = ((PongPingPolygon *) features_for_level[i])->attributes;
      
         if(polygon_attributes & PONGPING_VANISHING_POLYGON || polygon_attributes & PONGPING_HIT_X_TIMES_POLYGON)
         {
            polygon_features.push_back(i); // so polygon_features() : contains the index in features_for_level() which are polygons.
            polygons_assigned_a_collectable.push_back(0);
         }
      }
   
   }
   
   /* make sure there's enough polygons to put all the collectables in. If not give error and return 1. */
   
   if( polygon_features.size() < collectable_quota )
   {
       logfile << "not enough polygons to put all the collectables in. collectable quota is " << collectable_quota << " number of polygons is " << polygon_features.size() << "\n";
       return 1;
   }
   
   for(i = 0; i < collectable_type_quotas[PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE - 1]; ++i)
   {
      possible_polygons.clear();
   
      /* Pick a letter. */
      letter_number = pongping_random_number( 0, NUMBER_OF_LETTER_SUBTYPES - 1 );
      
      for(j = 0; j < polygon_features.size(); ++j)
      {
         if(polygons_assigned_a_collectable[j] == 0) // the jth polygon in polygon_features[] could possibly be used to put the collectable in
         {
            //logfile << "feature " << polygon_features[j] << " is possible.\n";
            possible_polygons.push_back( polygon_features[j]);  
         }
      }
      
      //logfile << "number of possible polygons is " << possible_polygons.size() << "\n";
      
      polygon_random_number = pongping_random_number(0, possible_polygons.size() - 1);
      //logfile << "polygon_random_number = " << polygon_random_number << "\n";
      
      
      int feature_number = possible_polygons[polygon_random_number];
      
      //out << "setting feature " << feature_number << "to have collectable.\n";
      
      ((PongPingPolygon *) features_for_level[feature_number])->collectable_type = PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE;
      ((PongPingPolygon *) features_for_level[feature_number])->collectable_subtype = letter_number;
      ((PongPingPolygon *) features_for_level[feature_number])->contains_collectable = true;
      
      //logfile << "set things.\n";
      
      for(j = 0; j < polygon_features.size(); ++j)
      {
         if( polygon_features[j] == feature_number)
         {
            polygons_assigned_a_collectable[j] = 1; 
         }
      }
      
      
      //logfile << "polygon_features[" << polygon_features[
      
   }
   
   polygon_features.clear();
   polygons_assigned_a_collectable.clear();
   
   for(i = 0; i < features_for_level.size(); ++i)
   {
      if(features_for_level[i]->feature_type == PONGPING_POLYGON_FEATURE)
      {
	     if( ((PongPingPolygon *) features_for_level[i])->contains_collectable == false )
		 {
	   
            polygon_attributes = ((PongPingPolygon *) features_for_level[i])->attributes;
      
            if(polygon_attributes & PONGPING_VANISHING_POLYGON || polygon_attributes & PONGPING_HIT_X_TIMES_POLYGON)
            {
               polygon_features.push_back(i);                 // so polygon_features() : contains the index in features_for_level() which are polygons.
               polygons_assigned_a_collectable.push_back(0);
            }
	     }
      }
   
   }
   
   logfile << "quota for bat size up collectables :- " << collectable_type_quotas[PONGPING_BAT_SIZE_UP_CIRCLE_TYPE - 1] << "\n";
   
   for(i = 0; i < collectable_type_quotas[PONGPING_BAT_SIZE_UP_CIRCLE_TYPE - 1]; ++i)
   {
      possible_polygons.clear();
   
      /* Pick a letter. */
      letter_number = pongping_random_number( 0, NUMBER_OF_LETTER_SUBTYPES - 1 );
      
      for(j = 0; j < polygon_features.size(); ++j)
      {
         if(polygons_assigned_a_collectable[j] == 0) // the jth polygon in polygon_features[] could possibly be used to put the collectable in
         {
            logfile << "feature " << polygon_features[j] << " is possible.\n";
            possible_polygons.push_back( polygon_features[j] );  
         }
      }
      
	  logfile << "\n\n";
	  
      //logfile << "number of possible polygons is " << possible_polygons.size() << "\n";
      
      polygon_random_number = pongping_random_number(0, possible_polygons.size() - 1);
      //logfile << "polygon_random_number = " << polygon_random_number << "\n";
      
      
      int feature_number = possible_polygons[polygon_random_number];
      
      //logfile << "setting feature " << feature_number << "to have bat size up collectable.\n";
      
      ((PongPingPolygon *) features_for_level[feature_number])->collectable_type = PONGPING_BAT_SIZE_UP_CIRCLE_TYPE;
      ((PongPingPolygon *) features_for_level[feature_number])->contains_collectable = true;
      
      //logfile << "set things.\n";
      
      for(j = 0; j < polygon_features.size(); ++j)
      {
         if( polygon_features[j] == feature_number)
         {
            polygons_assigned_a_collectable[j] = 1; 
         }
      }
      
      
      //logfile << "polygon_features[" << polygon_features[
      
   }

   /* Ball size up collectables. */
   
   polygon_features.clear();
   polygons_assigned_a_collectable.clear();
   
   for(i = 0; i < features_for_level.size(); ++i)
   {
      if(features_for_level[i]->feature_type == PONGPING_POLYGON_FEATURE)
      {
	     if( ((PongPingPolygon *) features_for_level[i])->contains_collectable == false )
		 {
	   
            polygon_attributes = ((PongPingPolygon *) features_for_level[i])->attributes;
      
            if(polygon_attributes & PONGPING_VANISHING_POLYGON || polygon_attributes & PONGPING_HIT_X_TIMES_POLYGON)
            {
               polygon_features.push_back(i);                 // so polygon_features() : contains the index in features_for_level() which are polygons.
               polygons_assigned_a_collectable.push_back(0);
            }
	     }
      }
   
   }
   
   logfile << "quota for ball size up collectables :- " << collectable_type_quotas[PONGPING_BAT_SIZE_UP_CIRCLE_TYPE - 1] << "\n";
   
   for(i = 0; i < collectable_type_quotas[PONGPING_BAT_SIZE_UP_CIRCLE_TYPE - 1]; ++i)
   {
      possible_polygons.clear();
   
      /* Pick a letter. */
      letter_number = pongping_random_number( 0, NUMBER_OF_LETTER_SUBTYPES - 1 );
      
      for(j = 0; j < polygon_features.size(); ++j)
      {
         if(polygons_assigned_a_collectable[j] == 0) // the jth polygon in polygon_features[] could possibly be used to put the collectable in
         {
            logfile << "feature " << polygon_features[j] << " is possible.\n";
            possible_polygons.push_back( polygon_features[j] );  
         }
      }
      
	  logfile << "\n\n";
	  
      //logfile << "number of possible polygons is " << possible_polygons.size() << "\n";
      
      polygon_random_number = pongping_random_number(0, possible_polygons.size() - 1);
      //logfile << "polygon_random_number = " << polygon_random_number << "\n";
      
      
      int feature_number = possible_polygons[polygon_random_number];
      
      logfile << "setting feature " << feature_number << "to have ball size up collectable.\n";
      
      ((PongPingPolygon *) features_for_level[feature_number])->collectable_type = PONGPING_BALL_SIZE_UP_CIRCLE_TYPE;
      ((PongPingPolygon *) features_for_level[feature_number])->contains_collectable = true;
      
      //logfile << "set things.\n";
      
      for(j = 0; j < polygon_features.size(); ++j)
      {
         if( polygon_features[j] == feature_number)
         {
            polygons_assigned_a_collectable[j] = 1; 
         }
      }
      
      
      //logfile << "polygon_features[" << polygon_features[
      
   }
   
   
   //eek();
   
   return 0;
}



int OneGame::get_number_of_levels(void)
{
   int total_number_of_levels = 0;
   TiXmlDocument level_file;
   
   if( level_file.LoadFile( PONGPING_LEVELS_FILENAME, TIXML_DEFAULT_ENCODING ) == false)
   {
      logfile << "OneGame::get_number_of_levels : couldn't load xml file.\n";
      return -1;
   }
   
   TiXmlHandle hDoc(&level_file);
   TiXmlElement *pLevelDefinitionElement = hDoc.FirstChild("level_definition_for_willam_labbetts_awesome_new_game" ).Element();
   if(pLevelDefinitionElement == NULL)
   {
      logfile << "OneGame::get_number_of_levels() : reading xml file - couldn't find a level_definition_for_willam_labbetts_awesome_new_game element.\n";
      return -1;
   }
   logfile << "got level definition element.\n";
   TiXmlHandle hLevelDefinition = TiXmlHandle(pLevelDefinitionElement); 
   
   
   TiXmlElement *pNumberOfLevelsElement = hLevelDefinition.FirstChild("number_of_levels").Element();
   
   if(pNumberOfLevelsElement == NULL)
   {
      logfile << "get_number_of_levels() : couldn't get number of levels from xml file.\n";
      return -1; 
   }
   else
   {
      if( pNumberOfLevelsElement->QueryIntAttribute( "n", &total_number_of_levels) != TIXML_SUCCESS )
      {
         logfile << "couldn't read attribute from number_of_levels element.\n";
         return -1;

      }     
      else
      {
         return total_number_of_levels;
      }   
   
   }


}





int all_collisions_catchment_areas( vector<PotentialMove *>& moves)
{
   int i = 0;
   
   for( ; i < moves.size(); ++i )
   {
      if(moves[i]->hit_type != PONGPING_CATCHMENT_AREA_COLLISION)
      {
         return 0;
      }
   }
  
   return 1;
}





static void erase_and_clear_potential_move_vector( vector <PotentialMove *>& vec )
{
   vector< PotentialMove* >::iterator it = vec.begin();

   int i;
   
   for(i = 0; i < vec.size(); ++i)
   {
      vec.erase(it);
      ++it;      
   }
}





/* This bit removes any circles from the vector with all the circles in except the ones which are balls. */

void OneGame::remove_non_ball_circles( void )
{

   /* First see if there are any non ball type circles in the vector. */
   
   int cn; // circle number
   
   int  have_non_ball_circles = 0;
   
   vector<Circle*>::iterator circle_it;
   
   for(cn = 0; cn < circle_vector.size(); ++cn)
   {
      if(circle_vector[cn]->circle_type != PONGPING_BALL_CIRCLE_TYPE)
      {
	     have_non_ball_circles = 1;
		 break;
	  }
   }

   
   while(have_non_ball_circles)
   {
      circle_it = circle_vector.begin();
	  
      for(cn = 0; cn < circle_vector.size(); ++cn)
      {
         //logfile << "cn = " << cn << "\n";
      
         if(circle_vector[cn]->circle_type != PONGPING_BALL_CIRCLE_TYPE)
         {
	        /* erase circle. */
	  
	        logfile << "erased circle. cn = \n" << cn << "\n";
	  
            circle_vector.erase(circle_it);
			break;
         }
         else
         {
            logfile << "setting ball variables.\n\n";
        
         
         /* Set the position, angle and speed of the ball. */
         
            circle_vector[cn]->set_double(DISTANCE_INCREMENT, initial_ball_increment);
            circle_vector[cn]->set_double(CIRCLE_X, initial_ball_x);
            circle_vector[cn]->set_double(CIRCLE_Y, initial_ball_y);
            circle_vector[cn]->set_double(CIRCLE_ANGLE, initial_ball_angle);
         
		    circle_vector[cn]->set_circle_colour(ball_colour);
	
            ((Ball *) circle_vector[cn])->change_ball_colour(old_ball_colour, ball_colour);
 	       
         //logfile << "set angle to " << initial_ball_angle << "\n";
            
			if(cn == circle_vector.size() - 1)
			{
			   /* This is the last circle in the vector and while this run of the for loop has been going the 'break' command hasn't been
			      enlogfileered so all the circles before this (if there were any) were balls so it's time to set
				  
				  non_ball_circles_left to 1 to stop the while loop. */
			
			   have_non_ball_circles = 0;
			   break;
			}
         }
      
         ++circle_it;
      }
   }







}










