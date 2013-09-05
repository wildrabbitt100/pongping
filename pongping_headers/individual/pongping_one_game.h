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
 
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>





 
#define PONGPING_NUMBER_OF_BORDERS 4

enum { PONGPING_TOP_EDGE, PONGPING_RIGHT_EDGE, PONGPING_BOTTOM_EDGE, PONGPING_LEFT_EDGE };





using namespace std;















class BallSpeedData {
   
   
   
   public:
      int seconds;
      float increment;
      BallSpeedData() {}
      BallSpeedData(int s, float i)
	     : seconds(s), increment(i)
	  {
	  }
	  
	  

};






ifstream& operator>>(ifstream& is, BallSpeedData& bsd);





class OneGame {

   int lives;
   
   int number_of_levels;
   
   
   int current_level;
   
   
   float initial_ball_x;
   float initial_ball_y;
   
   
   float initial_ball_angle;
   float initial_ball_increment;
   
   char task[100];
   
   bool init_okay;
   
   vector<BallSpeedData> bsd;
   
   int32_t circles_moved[2];
   
   bool end_when_time_reached;
   
   int time_to_reach;
   
   BITMAP_LIST *bitmap_list;
   
   ALLEGRO_BITMAP *borders[PONGPING_NUMBER_OF_BORDERS];
   
   int extra_life_record;
   
   vector<ALLEGRO_SAMPLE *> samples;
   
   int number_of_balls;
   
public :

   int circles_inside_polygon;


   COULEUR border_colour;

   COULEUR background_colour;
   
   COULEUR old_ball_colour;
   
   COULEUR ball_colour;
   
   int increments_done_so_far;
   Bat TheBat;
   
   vector<Circle*> circle_vector;
 
   OneGame(PongPingEssentials&);
   
   ALLEGRO_EVENT_QUEUE *queue;
   ALLEGRO_TIMER *timer;
  
   
   
   
   static ALLEGRO_BITMAP *get_ready;
   
   static ALLEGRO_BITMAP *level_finished_bitmap;
   
   ALLEGRO_BITMAP *background;
   
   //int font_heights[NUM_FONTS];
   
   int run_level( PongPingEssentials& ppe);

   vector<LevelFeature *> features_for_level;
   
   
   /* collectable_quota : integer for storing how many collectables the level file says there should be for
      the level - needs to be not more than the number of polygons because, each polygon
	  can only have one collectable associated with it. */
   
   int collectable_quota;
   int collectable_type_quotas[NUMBER_OF_COLLECTABLE_TYPES];
   
   
   void GetSetForNewGame( PongPingEssentials& settings);
   
   void GetSetForNewLevel() { // need to free feature memory. and circles?
   
      
	  if(features_for_level.size() > 0)
	  {
	     int i;
		 for(i = 0; i < features_for_level.size(); ++i)
		 {
		    delete features_for_level[i];
		 }
	  
	  }
   
      features_for_level.clear();
   
      
   
      collectable_quota = 0;
	  
	  memset( collectable_type_quotas, 0, sizeof(int) * NUMBER_OF_COLLECTABLE_TYPES );
	  
	  
	  
   };
   
   void remove_non_ball_circles( void );
   
   
   int increment_level() { 
      
	  ++current_level; 
	  
	  LOG("incremented level, it is now ");
	  LOGINT(current_level);
	  LOG("\n total levels = ");
	  LOGINT(number_of_levels);
	  LOG("\n");
	  
	  /* First level is always taken to be 1 (current_level is 1 for first level), so when current_level is total_number_of_levels + 1
	  there's no level for this value so the return value indicates there's no more levels. */
	  
	  return (current_level == number_of_levels + 1) ? PONGPING_LAST_LEVEL_PLAYED_WAS_FINAL_LEVEL : PONGPING_NEXT_LEVEL_EXISTS;
   }
   
   int read_level();
   
   int arrange_collectables();
   
   bool init_went_okay() { return init_okay; }
   
   void draw_level();
   
   int check_data_type(string string_from_file, string expected_string)
   {
      return string_from_file == expected_string;
   }
   
   void decrement_lives() { --lives; }
   int get_lives() { return lives; }
   
   void clear_features_from_vector() { features_for_level.clear(); }
   
   ALLEGRO_BITMAP *get_level_info_bitmap_ptr();
   
   
   
   int level_finished();
   
   
   int get_number_of_levels();
   
   int update_objects();
   
   #ifdef LEVEL_TO_SIMULATE_GAME_ENDING
   
      void set_short_game_end_time()
	  {
	      end_when_time_reached = true;
		  time_to_reach = 4.0;
	  }
   #endif
   
   ~OneGame();

   
};



