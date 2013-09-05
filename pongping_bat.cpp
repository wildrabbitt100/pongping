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
 

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "pongping_headers/pongping_bat_headers.h"






using namespace std;

/* Constructor for class Bat */


Bat::Bat()
   : movement_inc(6.0)
{
   al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
   
   /* Load the bitmaps which are used for the bat. There are three bitmaps. */
   
         bat_left_end = al_load_bitmap("game_data/bat_left_end.png");
        bat_right_end = al_load_bitmap("game_data/bat_right_end.png");
   bat_middle_section = al_load_bitmap("game_data/bat_middle_section.png");
   
   if(bat_left_end == 0 || bat_right_end == 0 || bat_middle_section == 0)
   {
	  logfile << "couldn't load a bat bitmap.\n";
	  init_okay = false;
	  return;
   }
   
  
   sections = 3;
   
   
   
   x = (PONGPING_CANVAS_WIDTH / 2) - (get_bat_width() / 2);
   
   /* set_bat_vertices can now be called as x is set. */
   
   set_bat_vertices();
  
   //int i;
   //for(i = 0; i < 4; ++i)
   //{
   //   logfile << "bat v " << i << " = ( " << bat_vertices[i].x << "," << bat_vertices[i].y << " )\n";
   //}
   //eek();
   counter = BAT_TICKS_PER_UPDATE;
   
   
   direction = BAT_NOT_MOVING;
   init_okay = true;
}

 
 


void Bat::Draw(void)
{
   int i;
   
   for(i = 0 ; i < sections; ++i)
   {
      al_draw_bitmap(bat_middle_section, 13 + x + i * 14, BAT_Y_POSITION, 0);
   }
   
   al_draw_bitmap( bat_left_end, x, BAT_Y_POSITION, 0);
   al_draw_bitmap( bat_right_end, 13 + x + sections * 14, BAT_Y_POSITION, 0);
   
  
   
}




int Bat::circle_near(Circle *circle)
{
   return circle->cc_x >= (bat_vertices[PONGPING_BOTTOM_LEFT_BAT_VERTEX].x - movement_inc - circle->radius - circle->distance_increment) && 
          circle->cc_x <= (bat_vertices[PONGPING_TOP_RIGHT_BAT_VERTEX].x + movement_inc + circle->radius + circle->distance_increment) && 
		  circle->cc_y <= (bat_vertices[PONGPING_BOTTOM_LEFT_BAT_VERTEX].y + circle->radius + circle->distance_increment) && 
		  circle->cc_y >= (bat_vertices[PONGPING_TOP_RIGHT_BAT_VERTEX].y - circle->radius - circle->distance_increment);
}
