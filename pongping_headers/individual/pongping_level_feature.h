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
 
 
#ifndef LEVEL_FEATURE_H
#define LEVEL_FEATURE_H



enum { PONGPING_FEATURE_C_DRAW, PONGPING_FEATURE_G_DRAW };



class LevelFeature {


   protected:
      LevelFeature() {}
      
   public:
   
      int feature_type;
   
      int time_to_delete;
   
      int draw_priority;
  
   
      virtual void draw() const = 0;
      virtual void update_by(double pfot) = 0;
      
      virtual int check_for_collision(Circle* circle, PotentialMove **pm, int hit_this_polygon_last_time, int side_hit, int vertex_hit, double ftr) = 0;
      
      virtual int circle_near(Circle *circle)
      {
         return 1;
      }
      
      
      virtual int is_done() { return 1; }
      
      virtual int has_collectable() { return 0; }
      
      virtual int get_collectable_type() { return -1; }
      virtual float get_x_for_collectable() {return 0; }
      virtual float get_y_for_collectable() { return 0; }
      virtual float get_type_for_collectable() { return 0; }
      virtual int check_feature_hits_circle(Circle* circle, PotentialMove& pm) 
      {
         return 0;
      }
      
	  
	  //remove
      virtual int circle_left_or_right_of_line(Circle *circle, int v1, int v2) { return 0; }
      
      virtual int set_feature_type(int type) { feature_type = type;} 
      
      virtual int emit() { return 0; }
      
      virtual void set_convex_or_concave_flag() { ; }
};



#endif
