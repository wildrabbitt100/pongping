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

#include "pong_allegro5_headers.h"

#include "ball.h"




class PushToTopFeature : LevelFeature {

  
   static ALLEGRO_BITMAP *track_section;



   public:
      
	  LevelFeature() {}
	  LevelFeature(int length, int wwidth_of_bar, int xx_pos) : length_in_sections(length), width_of_bar(wwidth_of_bar), x_pos(xx_pos)
      {
      
	     init_okay = 1;
      }	  
   
      int init_okay;
   
      int length_in_sections;
      
	  int width_of_bar;
	  
	  int x_pos;
	  
	  float y_pos_of_bar;
	  
	  int at_top;

      int check_for_collision(Ball& ball, int i);
      void draw();


};





