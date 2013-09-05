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
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#ifdef USE_LOG_FILE
   #include <fstream>
#endif


//#ifdef USE_LOG_FILE
   #define fleek() { logfile.flush(); system("PAUSE"); exit(1); }
//#else
   #define eek() { system("PAUSE"); exit(1); }
//#endif

#define PONGPING_TIMER_TICKS_PER_SECOND 60.0f


#define LIVES_AT_START_OF_GAME 3

#define  LEFT_KEY 1
#define RIGHT_KEY 2
#define    UP_KEY 4
#define  DOWN_KEY 8

#define PONGPING_CANVAS_WIDTH 1024
#define PONGPING_CANVAS_HEIGHT 768

#define PONGPING_CANVAS_MIDDLE_X (PONGPING_CANVAS_WIDTH / 2)

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768


#define PONGPING_BORDER_SIDE_EDGE_WIDTH 10
#define PONGPING_TOP_EDGE_WIDTH 20
#define PONGPING_BOTTOM_EDGE_WIDTH 30
#define BALL_ARENA_LEFT_EDGE_X       10
#define BALL_ARENA_RIGHT_EDGE_X    1014
#define BALL_ARENA_BOTTOM_EDGE_Y    738
#define BALL_ARENA_TOP_EDGE_Y        20


#define IN_GAME_INFO_TEXT_BOX_WIDTH   530.0 
#define IN_GAME_INFO_TEXT_BOX_HEIGHT  530.0 / 1.618



#define NUM_DESIRED_PIXEL_FORMATS 6


#define AMOUNT_TO_ADD_TO_BALL_DISTANCE_INCREMENT_PER_UPDATE 0.0083


#define BAT_Y_POSITION 720


#define TICKS_FOR_TEXT_BOX_AT_START_OF_LEVEL 200
#define TICKS_TO_WAIT_IN_LEVEL_FINISHED_TRANSITION 120

#define TICKS_FOR_HOW_LONG_TO_SHOW_MESSAGE_ON_MAIN_MENU 120
#define TICKS_TO_SHOW_HIGHLIGHTED_BUTTON_FOR 5

#define TICKS_FOR_GAME_OVER_WORDS_FADE_IN 100
#define TICKS_FOR_TO_SHOW_GAME_OVER_WORDS_FOR 150
#define TICKS_TO_SHOW_GAME_OVER 60 * 3
#define TEXT_SIZE 40

#define TASK_BITMAP_Y 300



#define NUM_CONTROL_MENU_OPTIONS 2

#define TITLE_GRAPHIC_Y 50
#define MAIN_MENU_OPTIONS_BITMAP_FIRST_Y 330



/* These are for knowing the indexes of the bitmaps in the main bitmap list, for the main menu and the control option
   menu. These are needed because the bitmaps are cloned and modified for when displaying an option highlighted. */

#define FIRST_MAIN_MENU_OPTION_BITMAP_INDEX 1
#define FIRST_CONTROL_MENU_OPTION_BITMAP_INDEX 6

#define CONTROL_OPTIONS_FIRST_OPTION_Y 300



/* Width of 1 digit of numbers font. */
#define NUMBER_GLYPH_WIDTH 29

#define NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES 3







#define set_target_bitmap_to_backbuffer() al_set_target_backbuffer(al_get_current_display())

struct Point {
   double x, y;
};



enum { PONGPING_USING_KEYBOARD, PONGPING_USING_JOYPAD_OR_JOYSTICK, PONGPING_USING_MOUSE };


#define MENU_MESSAGE_Y 600
#define MENU_CONTROL_TYPE_MESSAGE_Y 650







#define MULTICOLOURED_POLYGONS 


#define NUMBER_OF_DATA_TYPES 8

enum { LEVEL_TASK, BALL_POSITION, INITIAL_ANGLE, INITIAL_DISTANCE_INCREMENT, BALL_SPEEDS, REGULAR_POLYGON, END_OF_LEVEL_FILE, SURVIVE_UNTIL };


#define TICKS_TO_WAIT_BETWEEN_LIVES 120


  




#define TICKS_TO_WAIT_WHILE_SHOWING_LEVEL_INFORMATION 80

/* defines for the bit where the task for the level and the level number are shown before the level starts */

#define LEVEL_INFORMATION_SCREEN_COLOUR al_map_rgb(20, 20, 140)

#define LEVEL_INFORMATION_BITMAP_HEIGHT 240

#define MAX_LENGTH_FOR_TASK_STRING_LINE (((float) LEVEL_INFORMATION_BITMAP_HEIGHT * 1.618) - 120)

#define LEVEL_INFORMATION_LEVEL_STRING_COLOUR al_map_rgb(127, 29, 163)







#define IN_GAME_INFO_TEXT_BOX_BORDER_SIZE 10
#define IN_GAME_INFO_TEXT_BOX_BORDER_COLOR   al_map_rgb(9, 53, 14)
#define IN_GAME_INFO_TEXT_BOX_MAIN_COLOR     al_map_rgb(12, 94, 22)

#define GET_READY_BITMAP_GET_READY_Y 30

#define GET_READY_BITMAP_LEVEL_COMPLETE_Y 30

#define LEVEL_INFORMATION_LEVEL_STRING_Y (IN_GAME_INFO_TEXT_BOX_BORDER_SIZE + 10)













#define PONGPING_HIT 1





typedef struct PONGPING_ALLEGRO_FONT
{

   ALLEGRO_FONT *font;

   int height;

   

} PONGPING_ALLEGRO_FONT;

#define SIZE_40_FONT 0
#define SIZE_50_FONT 1
#define IN_GAME_INFO_FONT 2

#define FONT_FOR_IN_GAME_INFO_SIZE 22
#define NUMBER_OF_FONTS 3


/*********************************************************************************************************/
/*********************************************************************************************************/
/*********************************************************************************************************/
/*********************************************************************************************************/


enum { PONGPING_NEXT_LEVEL_EXISTS, PONGPING_LAST_LEVEL_PLAYED_WAS_FINAL_LEVEL };




#define PONGPING_INIT_OKAY_BIT 1

#define POWER_OF_TWO(x) (1 << x)

#define PONGPING_LEVELS_FILENAME ("game_data/levels_file.xml")


#ifdef PONGPING_ENABLE_DEBUG_FONT

extern ALLEGRO_FONT *debug_font;

#endif


#ifdef USE_LOG_FILE

   using namespace std;

   extern std::ofstream logfile;

#endif



enum { PONGPING_FLAG_DOWN, PONGPING_FLAG_UP };



