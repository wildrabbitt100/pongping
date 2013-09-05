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
 
 

#include "pongping_headers/pongping_control_options_menu_headers.h"




using namespace std;




int run_control_options_menu( PongPingEssentials& ppe)
{
   vector <Rect> rectangles(2); 
   ALLEGRO_KEYBOARD_STATE ks;
   ALLEGRO_MOUSE_STATE ms;
   
   int control_type_selected = ppe.current_control_type;
   
   ALLEGRO_BITMAP *option_1, *option_2;
   ALLEGRO_LOCKED_REGION *lock;
   bool show_highlighted_button = false;
   bool up_key, down_key;
   int return_val = -1;
   int i;
   bool need_redraw = false;
   al_flush_event_queue(ppe.queue);
   bool enter_key = false;
   
   int time;
   
   int option; /* variable for looping through options when drawing bitmaps. */
   
   up_key = false;
   down_key = false;
   
   
  
   
   while(return_val == -1 || show_highlighted_button == true)
   {
      al_get_keyboard_state(&ks);
	  al_get_mouse_state(&ms);
   
      if(!al_event_queue_is_empty(ppe.queue) && show_highlighted_button == false)
	  {
	     while(al_get_next_event(ppe.queue, &ppe.event))
	     {
		    switch(ppe.event.type)
			{
			   case ALLEGRO_EVENT_DISPLAY_CLOSE:
		          return PONGPING_QUIT_GAME_AND_END_PROGRAM;
			   case ALLEGRO_EVENT_TIMER:
			      need_redraw = true;
				  break;
			   case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			     
				  break;
			   case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			      ;
			      break;
			   case ALLEGRO_EVENT_KEY_DOWN:
			      if(ppe.event.keyboard.keycode == ALLEGRO_KEY_UP)
				  {
				     up_key = true;
				  }
				  else if(ppe.event.keyboard.keycode == ALLEGRO_KEY_DOWN)
				  {
				     down_key = true;
				  }
				  else if(ppe.event.keyboard.keycode == ALLEGRO_KEY_ENTER)
	              {
	                 enter_key = true;
	              }
				  break;
				     
			   case ALLEGRO_EVENT_KEY_UP:
			      if(ppe.event.keyboard.keycode == ALLEGRO_KEY_UP)
				  {
				     up_key = false;
				  }
				  else if(ppe.event.keyboard.keycode == ALLEGRO_KEY_DOWN)
				  {
				     down_key = false;
				  }
				  else if(ppe.event.keyboard.keycode == ALLEGRO_KEY_ENTER)
	              {
				     if(enter_key == true)
					 {
	                    return_val = PONGPING_MENU_FINISHED_OKAY;
		                
						ppe.current_control_type = control_type_selected;
						show_highlighted_button = true;
						
						/* Get the time so as to... */
						
						time = al_get_timer_count(ppe.timer);
						
						al_flush_event_queue(ppe.queue);
						need_redraw = true;
						
					 }
	              }
			      else if(ppe.event.keyboard.keycode == ALLEGRO_KEY_F1)
			      {
			         int fullscreen_set = al_get_display_flags(al_get_current_display()) & ALLEGRO_FULLSCREEN_WINDOW ? 1: 0;
				     al_toggle_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN_WINDOW, ~fullscreen_set & 1); 
			      }
			      break;
			   default:
			      break;
			      //logfile << "control_options_menu : some other event.\n";
			}
		 }
	  }
	  else if(show_highlighted_button == true)
	  {
	     need_redraw = true;
	  }
   
    
	    
	 
	  
	  if(up_key == true && control_type_selected == PONGPING_USING_JOYPAD_OR_JOYSTICK)
	  {
	     control_type_selected = PONGPING_USING_KEYBOARD;
	  }
	  else if(down_key == true && control_type_selected == PONGPING_USING_KEYBOARD)
	  {
	     control_type_selected = PONGPING_USING_JOYPAD_OR_JOYSTICK;
	  }
	  
	  
	  
	  

	  
	  
      if(show_highlighted_button == true && al_get_timer_count(ppe.timer) > time + TICKS_TO_SHOW_HIGHLIGHTED_BUTTON_FOR )
      {
         show_highlighted_button = false;

      }	  
   
     
	  
	  if(need_redraw == true && al_event_queue_is_empty(ppe.queue))
	  {
	     al_clear_to_color(OUT_OF_GAME_BACKGROUND_COLOR);
		 
		 for(option = 0; option < NUM_CONTROL_MENU_OPTIONS; ++option)
		 {
		  
		    al_draw_bitmap( ppe.menu_list->bitmaps[FIRST_CONTROL_MENU_OPTION_BITMAP_INDEX + option], (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(ppe.menu_list->bitmaps[FIRST_CONTROL_MENU_OPTION_BITMAP_INDEX + option]) ) / 2.0f, CONTROL_OPTIONS_FIRST_OPTION_Y + 50 * option, 0);
		  
		    if(control_type_selected == option)
			{
			   if(show_highlighted_button == true)
               {
                  al_draw_tinted_bitmap(ppe.col[option], al_map_rgba(0, 0, 0, 255), (1024 -  (al_get_bitmap_width(ppe.col[option]))) / 2.0f, 
				  CONTROL_OPTIONS_FIRST_OPTION_Y + 50 * option, 0);
               }
			}
			else
			{
			   al_draw_tinted_bitmap(ppe.col[option], al_map_rgba(1, 50, 101, 128), (1024 -  (al_get_bitmap_width(ppe.col[option]))) / 2.0f, 
			   CONTROL_OPTIONS_FIRST_OPTION_Y + 50 * option, 0);
			}
		 
		 }
		 
		 
		 
	     al_flip_display();
		 need_redraw = false;
		 
      }
	  
   }
   
   

   
   
   return return_val;
   
}
