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
 
 
 
#include "pongping_headers/pongping_main_headers.h"







int use_log;



int main()
{
   
   int state = PONGPING_GUI_STATE; /* Variable for whether to run a go at the game or to run the Graphical User Interface. */ 
   
   int menu_result = -1;  /* variable to store the different possible outcomes of the menu. initialise it to -1 because if it's PLAY_GAME it'll cause
                             a run of the game */
  
   int level_result = 0;   /* Variable for storing the conlusion of level */
  
   
   /* menu_option_selected : for passing into run_main_menu() - this function is called every time there's button pressed. When a button is pressed
      the menu option it's for needs to be stored so after the action take for the button has taken place, the button which was pressed is still
      the one highlighted when run_main_menu() is called again. */
   
   //int menu_option_selected = PONGPING_PLAY_GAME;
   
   int control_options_menu_result;
   
   /* Set the mode of the GUI to menu mode. The other modes are 'enter name mode' and 'end credits' mode. */
   
   int gui_mode = PONGPING_GUI_MENUS_MODE;
   
   int gui_result;
   
   set_pixel_formats();
   
   /* initialise_log - see pongping_main.h */
   
   if( initialise_log() != 0 )
   {
      return 0;
   }
   
   PongPingEssentials ppe = PongPingEssentials();
   if(ppe.init_okay_flag == PONGPING_FLAG_DOWN) 
   {
      LOG("Couldn't initialise PongPingEssentials class.\n");
      return 1;
   }
   
   LOG("Checking bitmap in ponping_main() .\n");
   
   
   LOG("Constructed PongPingEssentials singleton successfully.\n\n");
   
  
   
   if(check_resources_exist() != PONGPING_ALL_RESOURCES_PRESENT)
   {
      LOG("Resources missing. Game won't run properly. Try reinstalling.\n");
	  return 1;
   }
   
  
   
   FallingCollectable::letters_bitmap = ppe.menu_list->bitmaps[EXTRA_LIFE_LETTERS];
   FallingCollectable::bat_size_up_circle = al_load_bitmap("game_data/bat_size_up_circle.png");
   quickmask1(FallingCollectable::bat_size_up_circle);
   FallingCollectable::ball_size_up_circle = al_load_bitmap("game_data/ball_size_up_circle.png");
   
   if(FallingCollectable::letters_bitmap == NULL || FallingCollectable::bat_size_up_circle == NULL || FallingCollectable::ball_size_up_circle == NULL)
   {
      LOG("couldn't load one of the collectable circles bitmaps.\n");
	  return 0;
   }   
   
   //set_target_bitmap_to_backbuffer();
 
   al_clear_to_color(al_map_rgb(0,0,0));
 
   al_flip_display();
 
 
 
 
 
   LOG("Creating OneGame singleton.\n\n");
   
   OneGame single_game = OneGame(ppe);
   
   if(ppe.use_log)
   {
      logfile << "Done creating OneGame singleton.\n\n";
   }
   
   OneGame::get_ready = get_get_ready_bitmap(ppe.menu_list);
   
   OneGame::level_finished_bitmap = get_level_finished_bitmap(ppe.menu_list);
   
   if(OneGame::get_ready == NULL || OneGame::level_finished_bitmap == NULL)
   {
      LOG("Couldn't get get_ready_bitmap or level_finished_bitmap.\n\n"); 
      return 1;
   }
  
  
   if(single_game.init_went_okay() == false)
   {
      LOG("problem initialising OneGame .\n");
   }
   
   LOG("Checking bitmap in ponping_main() 2.\n");
  
   
   
   
   al_start_timer(ppe.timer);
  
   while(state != PONGPING_QUIT_GAME_AND_END_PROGRAM_STATE)
   {
      if( state == PONGPING_GUI_STATE )
      {	
		 LOG(" state is PONGPING_GUI_STATE\n\n");
		 	
         state = ppe.run_gui( gui_mode );
         
        
         al_flush_event_queue(ppe.queue);
         
      }    
      
      if( state == PONGPING_PLAY_STATE )
      {     
          /* This call uninstalls input devices which won't be used during the game, except the keyboard which
			 is always used if there is one to use. If the control type set for the game is the keyboard or joypad but 
			 the mouse was being used for the menus, then it won't show over the game screen when the game starts. */
            
	      ppe.unregister_input_devices();
			
          single_game.GetSetForNewGame( ppe );
			
          do
          {
             level_result = single_game.run_level( ppe );           
              
             switch(level_result)
             {
                  case PONGPING_LEVEL_RESULT_DISPLAY_CLOSED: 
                  case PONGPING_LEVEL_RESULT_INITIALISATION_PROBLEM:
                     state = PONGPING_QUIT_GAME_AND_END_PROGRAM_STATE;
                     break;
                  case PONGPING_LEVEL_RESULT_GO_TO_NEXT_LEVEL:
					 if( single_game.increment_level() == PONGPING_LAST_LEVEL_PLAYED_WAS_FINAL_LEVEL )
					 {
					    state = PONGPING_GAME_COMPLETE_STATE;
					 }
					 else
					 {
					    single_game.GetSetForNewLevel();
                     } 
					 break;
                  case PONGPING_LEVEL_RESULT_SKIP_TO_NEXT_LEVEL:
                     single_game.increment_level();
                     break;
                  case PONGPING_LEVEL_RESULT_GAME_OVER: /* no lives left - the words 'game over' should have been shown */
				     LOG("This Game has Finished.\n\n");
                     state = PONGPING_GUI_STATE;
                     break;
              }
			  
          }
          while(state == PONGPING_PLAY_STATE);
            
            
      }
		  
   }
   
   return 0;
}



