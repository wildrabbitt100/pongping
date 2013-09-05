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








#define NUMBER_OF_CATCHMENT_AREA_BITMAPS 1

class CatchmentArea : LevelFeature {

   
      static vector <ALLEGRO_BITMAP *> bitmaps; 
   
	  static int bitmaps_loaded;
   
      
   public: 
      int init_okay;
      int x, y;

      int w, h;
	  
	  
	  ALLEGRO_BITMAP *bitmap;  
	  
	  CatchmentArea( int xx, int yy, int ww, int hh, int bitmap_number ) : x(xx), y(yy), w(ww), h(hh), init_okay(0)
	  {
	     if(bitmaps_loaded == 0)
		 {
		    if( load_catchment_area_bitmaps() != 0 )
			{
			   init_okay = 0;
			   return;
			}
			else
			{
			   bitmaps_loaded = 1;
			}
	     }
	  
	     feature_type = PONGPING_CATCHMENT_AREA;
		 
		 bitmap = bitmaps[bitmap_number];
		 
		 draw_priority = PONGPING_FEATURE_C_DRAW;
		 
		 init_okay = 1;
		 
	  }

      
	  int check_for_collision(Circle* circle, PotentialMove **pm, int hit_this_polygon_last_time, int side_hit, int vertex_hit, double ftr)
	  {
	     if( (circle->cc_x - circle->radius) > x && (circle->cc_x + circle->radius) < x + w && circle->cc_y - circle->radius > y && circle->cc_y + circle->radius < y + h )
		 {
		    
		    (*pm)->hit_type = PONGPING_CATCHMENT_AREA_COLLISION;
   		    return 1;
		 }
		 else
		 {
		    return 0;
		 }
	 
	  }
	  
	  int load_catchment_area_bitmaps() {
         
		 int i;
		 
         char name[100] = "game_data/catchment_area_"; 

		 for(i = 0; i < NUMBER_OF_CATCHMENT_AREA_BITMAPS; ++i)
		 {
		    char suffix[20];
            ALLEGRO_BITMAP *b = NULL;
            name[25] = '\0';
			
			suffix[0] = ((i % 1000) / 100) + '0';
			suffix[1] = ((i % 100) / 10) + '0';
			suffix[2] = ((i % 10)) + '0';
			suffix[3] = '.';
			suffix[4] = 'p';
			suffix[5] = 'n';
			suffix[6] = 'g';
			suffix[7] = '\0';

			strcat( name, suffix );
			
			//printf("name is <%s>.\n", name);
			
			b = al_load_bitmap( name );
			
			if(b == NULL) return 1;
			
			bitmaps.push_back(b);
			
         }

         return 0;
      }
	  
	  void draw() const
	  {
	     al_draw_bitmap( bitmap, x, y, 0 );
	  }
	  
	  void update_by( double pfot )
	  {
	  }


};
