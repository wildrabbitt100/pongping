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







typedef struct 
{
   double radius;
   double angle;


} polar_coordinates;







class PongPingPolygon : LevelFeature {

   
   
   float radius_to_vertex;
   
   int direction_of_spin;
   int type;
   static vector<int> ids_used;
   
public:
   Point *vertices;
   int polygon_id;
   double angle;
   int destroyed;
   double increment_angle;
   double cx, cy; /* centre x, centre y */
   
   /* attributes : variable for storing infomation about the polygon which is used to determine how to draw it, what to do when
      a circle collides with it etc... */
   
   int attributes;
   
   vector<COULEUR> polygon_colours;          // colours for the polygon
   
   vector<polar_coordinates> unrotated_points;
   
   int number_of_sides;
   
   bool contains_collectable;
   int collectable_type;
   int collectable_subtype;
   
   double furthest_vertex;
   
   PongPingPolygon() { }

   PongPingPolygon(int cx, int cy, int number_of_sides, float r, float ic, int direction_of_spin, float angle, int type);

   void draw() const;

   void draw_debug(int type) const;
   
   void update_by(double pfot);
   
   void backdate();
   
   //int circle_near(Circle *circle);
   
   int check_for_collision(Circle* circle, PotentialMove **pm, int hit_this_polygon_last_time, int side_hit, int vertex_hit, double ftr);
   
   void set_for_test(float angle_);
   void print_out_vertex_data();
   
   int active_layers;
   
   int is_done() { return destroyed; }
   
   int has_collectable() 
   { 
      if( contains_collectable == true ) 
	  {
	     //logfile << "returning 1 for has collectable.\n\n";
	     return 1;
      }
  	  else 
	  {
	     //logfile << "returning 0 for has collectable.\n\n";
	     return 0; 
	  }
   }
   
   float get_x_for_collectable() { return (float) cx - (float) EXTRA_LIFE_LETTER_RADIUS; }
   float get_y_for_collectable() { return (float) cy - (float) EXTRA_LIFE_LETTER_RADIUS; }
   float get_type_for_collectable() { return collectable_type; }
  
 
   void decrement_active_layers()
   {
      if( active_layers > 0 )
	  {
	     if(polygon_id == 9)
		 {
	        //logfile << "decrmenting active layers for polygon with id : " << polygon_id << "\n";
		    //wait_for_key_down();
	     }
	     --active_layers;
	  }
	  
	  if(attributes & PONGPING_VANISHING_POLYGON && active_layers == 0)
	  {
		 
         if(polygon_id == 9)
		 {
	        //logfile << "setting time to delete flag : " << polygon_id << "\n";
		    //wait_for_key_down();
			//eek();
	     }
		 
		 time_to_delete = 1;
		 
	  }
   
   }
   
   int emit() 
   { 
      //logfile << "in emit - active_layers = " << active_layers << "\n\n";
   
      if( active_layers == 0 ) 
	  { 
	     
	  
	     //logfile << "setting contains_collectable to false.\n\n"; 
	     contains_collectable = false;
	     return 1;
      }
	  return 0; 
  }
  
  #ifdef PONGPING_LABEL_POLYGONS
  void label(int fn)
  {
     al_draw_textf(debug_font, al_map_rgb(255, 255, 255), cx, cy, 0, "%c%c %c%c", ((char) polygon_id / 10) + '0', ((char) polygon_id % 10) + '0',
	                                                                              ((char) fn / 10) + '0', ((char)  fn % 10) + '0'); 
  
  }
  #endif
  
  
  void set_convex_or_concave_flag();
  
  int circle_near(Circle *circle);
  
  
  void clear_ids() { ids_used.clear(); }
  
  
  void update_irregular_polygon( )
  {
     int vn;
     double a;
   
     
     for(vn = 0; vn < number_of_sides; ++vn)
     {
	     //if(polygon_id == 9) logfile << "a = " << a * 180.0f / PONGPING_PI<< "\n";
		 
		 
		 
		 
		 
	     a = unrotated_points[vn].angle;
	  
	     
		 vertices[vn].x = cx + unrotated_points[vn].radius * cos(a);
         vertices[vn].y = cy + unrotated_points[vn].radius * sin(a);
        
         		 
	 }
  
  }
  
  
  int get_polygon_id() { return polygon_id; }
  
};


