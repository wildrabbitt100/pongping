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
 
 
 
#include "pongping_headers/pongping_credits_headers.h"


#define MINIMUM_TIME_TO_SHOW_CREDITS 8   /* how many seconds */

#define ORANGE_FADE_IN_TIME 3.0f

#define WHERE_TO_PUT_CONGRATULATIONS_Y 50

#define NUMBER_OF_TEXT_LINES 8

#define FADE_OUT_TIME 180

#define BLACK_SCREEN_TIME 120

/* run_credits :

WHAT IT DOES :

Runs the screen which gets shown if the game is complete (all the levels have been finished).

ARGUMENTS :

ppe : the PongPingEssentials class which is needed for the BITMAP_LIST and ALLEGRO_EVENT it contains

one_game : the OneGame class which is needed to get access to the fonts.

*/
int run_credits( PongPingEssentials& ppe, OneGame& one_game )
{
   
   int redraw = 1;
   
   int result = -1;
   
   int start_time = al_get_timer_count( ppe.timer );
   
   int fading_to_black = 0;
   
   int fade_start_time;
   
   #ifdef USE_LOG_FILE
      logfile << "In run_credits().\n";
   #endif
   
   const char *text[NUMBER_OF_TEXT_LINES] = { " for completing Pong Ping", " Programming and Graphics ", "William Labbett ", 
                                        " Physics Tutelage ", "Dominic Hopkins", "Aesthetics", "Laure Labbett", "THE END" };
   
   //eek();
   
   al_flush_event_queue(ppe.queue);
   
   set_target_bitmap_to_backbuffer();
   
   do
   {
   
      al_wait_for_event( ppe.queue, &ppe.event );
      
	  switch( ppe.event.type )
	  {
         
		 case ALLEGRO_EVENT_TIMER:
		    redraw = 1;
			
			if(fading_to_black == 1 && al_get_timer_count(ppe.timer) > fade_start_time + FADE_OUT_TIME + BLACK_SCREEN_TIME)
			{
			   //logfile << "set flag.\n";
			   //fleek();
			   fading_to_black = 2; /* This will mean this do { } while loop will end. */
			}
			
			
            break;
		 case ALLEGRO_EVENT_KEY_DOWN:
		    if( ppe.event.keyboard.keycode == ALLEGRO_KEY_ESCAPE )
			{
			   result = PONGPING_GUI_RESULT_VALUE_END_PROGRAM;
			}
			
		    if(result == -1 && al_get_timer_count(ppe.timer) > start_time + (MINIMUM_TIME_TO_SHOW_CREDITS * 60.0))
			{
		       result = PONGPING_GUI_RESULT_VALUE_FINISHED_OKAY;
			   fading_to_black = 1;
			   fade_start_time = al_get_timer_count(ppe.timer);
			}
			
			break;
		 case ALLEGRO_EVENT_DISPLAY_CLOSE:
		    logfile << "got a display close event.\n";
		    result = PONGPING_GUI_RESULT_VALUE_END_PROGRAM;
			break;
	  }
	  
	  if( redraw && al_event_queue_is_empty(ppe.queue) )
	  {
   
         al_draw_bitmap(ppe.menu_list->bitmaps[CONGRATULATIONS_BACKGROUND], 0, 0, 0);
   
  
         al_draw_bitmap(ppe.menu_list->bitmaps[CONGRATULATIONS], (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(ppe.menu_list->bitmaps[CONGRATULATIONS])) / 2.0, WHERE_TO_PUT_CONGRATULATIONS_Y, 0);
   
         if( al_get_timer_count( ppe.timer ) < start_time + ORANGE_FADE_IN_TIME * 60.0 )
		 {
	        al_draw_filled_rectangle(0, 0, PONGPING_CANVAS_WIDTH, PONGPING_CANVAS_HEIGHT, al_map_rgba(255, 140, 5, 255.0f * (1.0 - (al_get_timer_count( ppe.timer ) - start_time) / (ORANGE_FADE_IN_TIME * 60.0)) ));
         }
		 else
		 {
		    /* for completing Pong Ping */
		    //al_draw_text( one_game.pongping_fonts[SIZE_50_FONT].font, PONGPING_CREDITS_TEXT_COLOUR, PONGPING_CANVAS_MIDDLE_X, 200, ALLEGRO_ALIGN_CENTRE, text[0] );
		    
			
			/* Programming and Graphics */
			//al_draw_text( one_game.pongping_fonts[SIZE_50_FONT].font, PONGPING_CREDITS_TEXT_COLOUR_ASPECT, PONGPING_CANVAS_MIDDLE_X, 280, ALLEGRO_ALIGN_CENTRE, text[1] );
		    
			/* name */
			
			//al_draw_text( one_game.pongping_fonts[SIZE_50_FONT].font, PONGPING_CREDITS_TEXT_COLOUR_PERSONS_NAME, PONGPING_CANVAS_MIDDLE_X, 325, ALLEGRO_ALIGN_CENTRE, text[2] );
		 
		    /* Physics Tutelage */
			//al_draw_text( one_game.pongping_fonts[SIZE_50_FONT].font, PONGPING_CREDITS_TEXT_COLOUR_ASPECT, PONGPING_CANVAS_MIDDLE_X, 405, ALLEGRO_ALIGN_CENTRE, text[3] );
		    
			/* name */
			
			//al_draw_text( one_game.pongping_fonts[SIZE_50_FONT].font, PONGPING_CREDITS_TEXT_COLOUR_PERSONS_NAME, PONGPING_CANVAS_MIDDLE_X, 450, ALLEGRO_ALIGN_CENTRE, text[4] );
		 
			
			/* Aesthetics */
			//al_draw_text( one_game.pongping_fonts[SIZE_50_FONT].font, PONGPING_CREDITS_TEXT_COLOUR_ASPECT, PONGPING_CANVAS_MIDDLE_X, 530, ALLEGRO_ALIGN_CENTRE, text[5] );
		    
            /* name */
			
			//al_draw_text( one_game.pongping_fonts[SIZE_50_FONT].font, PONGPING_CREDITS_TEXT_COLOUR_PERSONS_NAME, PONGPING_CANVAS_MIDDLE_X, 575, ALLEGRO_ALIGN_CENTRE, text[6] );
		 	
			/* THE END */
			
			//al_draw_text( one_game.pongping_fonts[SIZE_50_FONT].font, PONGPING_CREDITS_TEXT_COLOUR, PONGPING_CANVAS_MIDDLE_X, 650, ALLEGRO_ALIGN_CENTRE, text[7] );
		 	
			
		 
		 
		 
		 }
		 
		 if(fading_to_black)
		 {
		    al_draw_filled_rectangle(0, 0, PONGPING_CANVAS_WIDTH, PONGPING_CANVAS_HEIGHT, al_map_rgba_f(0, 0, 0, (al_get_timer_count( ppe.timer ) - fade_start_time) / ((float) FADE_OUT_TIME)));
		 }
         //al_draw_text( ppe.);
   
         redraw = 0;
          
		 if(fading_to_black != 2)
		 {
            al_flip_display();
	     }
      }
   
   
   } while( fading_to_black != 2 || result == -1 );


   return result;
}
