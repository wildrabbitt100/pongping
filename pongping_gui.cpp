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
 
 

 #include "pongping_headers/pongping_gui_headers.h"
 

 
/* 




    returns PONGPING_PLAY_GAME, PONGPING_QUIT_GAME_AND_END_PROGRAM or PONGPING_MENUS_ERROR 
*/ 

int PongPingEssentials::run_gui(int mode)
{
   /* time_when_gui_started : needed because some of gui screens have fade in functions which must know when to end. */
	
	
   int time_when_gui_started;
   
   al_stop_timer(timer);
   al_start_timer(timer);
   
   time_when_gui_started = al_get_timer_count(timer);
   
   /* redraw : flag to indicate when enough time has passed for another redraw to be done. */
   
   int redraw = 1;
   
   /* rv : stands for return value : this is tell the caller to either that the user wants to have a go at the game or close the program / game */
   
   int return_value = PONGPING_NOT_A_NORMAL_VALUE;
   
   
   
   cout << "in run_gui.\n";
   
   /* Set the mode of the GUI : in other words which GUI screen to display. */
   
   gui->set_mode(mode);
  
   int up, down;
   
   int menu_id;
   
   int ignore_input = 0;
   
   int time_of_input;
   
   up = down = 0;
  
   while( return_value == PONGPING_NOT_A_NORMAL_VALUE )
   {
      al_wait_for_event(queue, &event);

      switch( event.type )
      {
		 case ALLEGRO_EVENT_TIMER:
		 
		    menu_id = gui->screens[mode]->menu_index;
		    
		    if(ignore_input == 0)
			{
			   if(menu_id != PONGPING_NOT_A_NORMAL_VALUE) /* If it's equal to something else, then there is a menu so the input applies to the menu. */
		       {
			      ((PongPingGUIMenu *) gui->screens[mode]->features[menu_id])->update(down, up);
			   }	
			}
			else if( al_get_timer_count(timer) > time_of_input + PONGPING_BLOCK_INPUT_TIME)
			{
			   ignore_input = 0;
			}
		    
		    
		    redraw = 1;
		    break;
		 case ALLEGRO_EVENT_DISPLAY_CLOSE:
		    return_value = PONGPING_QUIT_GAME_AND_END_PROGRAM_STATE;
		    break;
		 case ALLEGRO_EVENT_KEY_DOWN:
		    
		    switch(event.keyboard.keycode)
		    {
			   case ALLEGRO_KEY_DOWN:
		          down = 1;
		          ignore_input = 1;
		          time_of_input = al_get_timer_count(timer);
		          break;
		       case ALLEGRO_KEY_UP:
		          up = 1;
		          ignore_input = 1;
		          time_of_input = al_get_timer_count(timer);
		          break;	
			   case ALLEGRO_KEY_ESCAPE:
			      return_value = PONGPING_QUIT_GAME_AND_END_PROGRAM_STATE;
			      break;
			}
			break;
		 case ALLEGRO_EVENT_JOYSTICK_AXIS:
		 
		    if( event.joystick.axis == PONGPING_JOYSTICKS_VERTICAL_AXIS )
		    {
				if(event.joystick.pos < 0.0 )
				{
				   down = 0;
				   up = 1;
				}
				else if(event.joystick.pos > 0.0 )
				{
				   up = 0;
				   down = 1;	
				}
				cout << "got a vertcal axis movement.\n";
			}
		    
		    break;
		 case ALLEGRO_EVENT_KEY_UP:
		    switch(event.keyboard.keycode)
		    {
			   case ALLEGRO_KEY_DOWN:
			      down = 0;
			      break;
			   case ALLEGRO_KEY_UP:
			      up = 0;
			      break;
	     	}
	     	break;
		 default:
		    break;
	
      }

      if(redraw && al_is_event_queue_empty(queue))
      {
		 set_target_bitmap_to_backbuffer();
		 
         cout << "calling draw_gui_screen.\n";
         
         /* Draw the screen : the mode variable specifies which GUI screen to draw. */
         
		 gui->screens[mode]->draw_gui_screen();
		 
		 cout << "called it.\n";
	      
	     al_flip_display();
	      
	     redraw = 0;
	  }
	   
   } /* End of while(1) */
  
   
   
   return return_value;
}









PONGPING_MENU_BUTTON *get_menu_button(int menu_id, int button_number)
{
   //char *main_menu_words[6]    = { "PLAY GAME", "CONTROL OPTIONS", "DISPLAY OPTIONS", "INSTRUCTIONS", "HIGH SCORES", "QUIT" };
   //char *control_menu_words[3] = { "KEYBOARD", "JOYPAD OR JOYSTICK", "MOUSE" };
   //char *display_menu_words[2] = { "WINDOWED", "FULLSCREEN" }; 
   
   
   PONGPING_MENU_BUTTON main_menu_buttons[6] = {
	   
   {       "PLAY GAME", PONGPING_MENU_BUTTON_TYPE_GUI_FINISH_SWITCH, NULL },
   
   { "CONTROL OPTIONS",   PONGPING_MENU_BUTTON_TYPE_GUI_MODE_SWITCH, NULL },
   
   { "DISPLAY OPTIONS",   PONGPING_MENU_BUTTON_TYPE_GUI_MODE_SWITCH, NULL },
   
   {    "INSTRUCTIONS",   PONGPING_MENU_BUTTON_TYPE_GUI_MODE_SWITCH, NULL },
   
   {     "HIGH SCORES",   PONGPING_MENU_BUTTON_TYPE_GUI_MODE_SWITCH, NULL },
   
   {            "QUIT", PONGPING_MENU_BUTTON_TYPE_GUI_FINISH_SWITCH, NULL }
   
   };
   
   //char *word_arrays[3] = { main_menu_words, control_menu_words, display_menu_words }; 
   
   PONGPING_MENU_BUTTON *button = new PONGPING_MENU_BUTTON;
   
   button->button_word[0] = '\0';
   
   if( menu_id == 0 )
   {
      strcpy( button->button_word, (char *) main_menu_buttons[button_number].button_word ); 
   
      button->menu_button_type = main_menu_buttons[button_number].menu_button_type; 
   
      button->action_function = main_menu_buttons[button_number].action_function;
   }
   
   
   return button;
	
}



int get_number_of_buttons_for_this_menu(int menu_id)
{
   int number_of_buttons[PONGPING_NUMBER_OF_MENUS_IN_GUI] = { 6, 3, 2 };
   
   return number_of_buttons[menu_id];
	
	
}
