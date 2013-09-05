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


#include <fstream>



/*******************************************************************************************************************************/
/******************************** Defines for when developing. *****************************************************************/
/*******************************************************************************************************************************/


#define PONGPING_MAX_LEVELS 50

#define PONGPING_RELEASE_BUILD

#ifdef PONGPING_RELEASE_BUILD


   /* This when defined, means that the game will start at the level specified in the configuration file. */

   #define ENABLE_LEVEL_TO_START_AT_VARIABLE
   
   
   /* This makes a button available to press to skip to the next level. */
   
   #define SKIP_LEVEL 
   
   /* This makes sure that the angle of the balls are always between 0 and 2 * pi, and stops the program if any aren't. */
   
   #define CHECK_BALL_ANGLE 
   
   /* This when defines means that there's a global font which can be used from anywhere to display debugging information
      on the screen. */
   
   #define PONGPING_ENABLE_DEBUG_FONT


   /* This is used to catch any unexpected situations. */

   #define PONGPING_CATCH_BUGS

   #define DONT_SHOW_TRADEMARK
   
   
   
   
   
#endif



/*******************************************************************************************************************************/
/******************************** Defines for which platform to compile for. ***************************************************/
/*******************************************************************************************************************************/



#define PONGPING_WINDOWS_VERSION



/*******************************************************************************************************************************/
/********************************  ***************************************************/
/*******************************************************************************************************************************/


/* So as to solve the problem of choosing which of the words 'color' and 'colour' (the american or english spelling) to use. 
 * ie the code uses the french spelling. */

#define COULEUR ALLEGRO_COLOR




#define set_target_bitmap_to_backbuffer() al_set_target_backbuffer(al_get_current_display())




#define draw_respecting_alpha_value() (al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA))



/*******************************************************************************************************************************/
/******************************** Defines for some colours the game uses. ******************************************************/
/*******************************************************************************************************************************/



#define PONGPING_BLACK                          al_map_rgba( 0, 0, 0, 255)

#define PONGPING_GUI_BACKGROUND_COLOR            al_map_rgb( 25,  71,  11)

#define MENU_FONT_TEXT_ORIGINAL_COLOR           al_map_rgb( 42, 142, 254)

#define MENU_FONT_COLOR_FOR_WHEN_SELECTED       al_map_rgb(255, 255, 255)

#define MENU_FONT_COLOR_FOR_WHEN_CHOSEN         al_map_rgb( 20,  90,   162)

#define BLACK                                   al_map_rgb(  0,   0,   0)

#define PONGPING_WHITE                          al_map_rgb(255, 255, 255)

#define ELLIPSE_EDGE_COLOR                      al_map_rgb(10, 79, 114)

#define MASK_COLOR                              al_map_rgb(176, 20, 4)

#define PONGPING_MASK_COULEUR                   al_map_rgba(176, 20, 4, 255)

#define RED                                     al_map_rgb(240, 0, 0)

#define PONGPING_MASK_COLOR                     al_map_rgb(176, 20, 4)

#define PONGPING_TEXT_OUTLINE_COULEUR           al_map_rgb(11, 24, 87)

#define PONGPING_VIOLET                         al_map_rgb(194, 129, 255 )

#define PONGPING_MENU_ELLIPSE_COLOUR            al_map_rgba(13, 37, 7, 100)

#define PONGPING_END_CREDITS_ORANGE             al_map_rgba(255, 140, 5, 255)

#define PONGPING_CREDITS_TEXT_COLOUR            al_map_rgba(255, 255, 255, 255)


/* PONGPING_CREDITS_TEXT_COLOUR_ASPECT : the colour for text such as "Programming", "Graphics", etc on the credits screen. */

#define PONGPING_CREDITS_TEXT_COLOUR_ASPECT     al_map_rgba( 22, 96, 10, 255)

/* PONGPING_CREDITS_TEXT_COLOUR_PERSONS_NAME : the colour for any text that is a persons name on the credits screen. */

#define PONGPING_CREDITS_TEXT_COLOUR_PERSONS_NAME     al_map_rgba( 103, 250, 96, 255)

#define GAME_SCREEN_BORDER_COLOUR al_map_rgb(14, 48, 10)

#define PONGPING_FONT_OUTLINE_COULEUR al_map_rgba(11, 24, 87, 255)


/*******************************************************************************************************************************/
/******************************** Things to do with the bat. *******************************************************************/
/*******************************************************************************************************************************/


enum {BAT_NOT_MOVING, BAT_MOVING_LEFT, BAT_MOVING_RIGHT };

#define BAT_Y_POSITION 720



/*******************************************************************************************************************************/
/********************************  ***************************************************/
/*******************************************************************************************************************************/






#define SIZE_40_FONT 0
#define SIZE_50_FONT 1
#define IN_GAME_INFO_FONT 2

#define FONT_FOR_IN_GAME_INFO_SIZE 22
#define NUMBER_OF_FONTS 3


class Rect {

   int x1, y1, x2, y2;
   
   
   public:
      
	  Rect() {}
	  
	  Rect(int _x1, int _y1, int _x2, int _y2)
	     : x1(_x1), y1(_y1), x2(_x2), y2(_y2)
	  {}
   
   
      int x_y_inside_rect(int x, int y) const
	  {
	     return x >= x1 && x < x2 && y >= y1 && y <= y2;
	  }

};



struct Point {
   
   double x, y;
   
};


/*******************************************************************************************************************************/
/******************************** Things to do with the different kinds of circles. ********************************************/
/*******************************************************************************************************************************/

#define EXTRA_LIFE_LETTER_RADIUS 12.0
#define PONGPING_BAT_SIZE_UP_CIRCLE_RADIUS 7.0
#define PONGPING_BALL_SIZE_UP_CIRCLE_RADIUS 10.0
#define NUMBER_OF_CIRCLE_TYPES 5
#define NUMBER_OF_COLLECTABLE_TYPES 4
#define NUMBER_OF_LETTER_SUBTYPES 9
#define NUMBER_OF_BAT_SIZE_UP_SUBTYPES 1
#define NUMBER_OF_BALL_SPEED_CHANGER_SUBTYPES 2

enum { PONGPING_BALL_CIRCLE_TYPE, PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE, PONGPING_BAT_SIZE_UP_CIRCLE_TYPE, PONGPING_BALL_SIZE_UP_CIRCLE_TYPE, PONGPING_BALL_SPEED_DOWN_CIRCLE_TYPE };


/* sub-types for PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE */

enum { LETTER_FIRST_E, LETTER_X, LETTER_T, LETTER_R, LETTER_A, LETTER_L, LETTER_I, LETTER_F, LETTER_SECOND_E}; 


/* Values for the different phases a non-ball type circle can have. */

enum { PONGPING_CIRCLE_MOVING_PHASE, PONGPING_CIRCLE_COLLECTED_PHASE, PONGPING_CIRCLE_DELETE_PHASE };


/* Collision types. */

enum 
{ 
   
PONGPING_BAT_COLLISION = 1, 
PONGPING_CIRCLE_TO_CIRCLE_COLLISION, 
PONGPING_ROTATING_SIDE_POLYGON_COLLISION, 
PONGPING_ROTATING_VERTEX_COLLISION, 
PONGPING_TRANSLATED_VERTEX_COLLISION, 
PONGPING_CATCHMENT_AREA_COLLISION 

};


/*******************************************************************************************************************************/
/******************************** Things to do with the timing of things. ******************************************************/
/*******************************************************************************************************************************/

#define PONGPING_TIMER_TICKS_PER_SECOND 60.0f


#define TICKS_FOR_TEXT_BOX_AT_START_OF_LEVEL 200
#define TICKS_TO_WAIT_IN_LEVEL_FINISHED_TRANSITION 120
#define TICKS_FOR_HOW_LONG_TO_SHOW_MESSAGE_ON_MAIN_MENU 120
#define TICKS_TO_SHOW_HIGHLIGHTED_BUTTON_FOR 5
#define TICKS_FOR_GAME_OVER_WORDS_FADE_IN 100
#define TICKS_FOR_TO_SHOW_GAME_OVER_WORDS_FOR 150
#define TICKS_TO_SHOW_GAME_OVER 60 * 3
#define TICKS_TO_WAIT_BETWEEN_LIVES 120



/*******************************************************************************************************************************/
/******************************** Unorganised. *********************************************************************************/
/*******************************************************************************************************************************/

#define PONGPING_JOYSTICKS_VERTICAL_AXIS 1

#define PONGPING_NOT_A_NORMAL_VALUE -1
extern int use_log;

#define LOG(x) (use_log ? ((void) (logfile << __FILE__ << ":" <<  __LINE__ << ":" << __PRETTY_FUNCTION__ << ": " << (x)) ) : (void) 0)

#define LOGINT(x) (use_log ? ((void) (logfile << x) ) : (void) (0))

#define fleek() { logfile.flush(); system("PAUSE"); exit(1); }
#define eek() { system("PAUSE"); exit(1); }



#define LIVES_AT_START_OF_GAME 3

#define  LEFT_KEY 1
#define RIGHT_KEY 2
#define    UP_KEY 4
#define  DOWN_KEY 8

#define PONGPING_CANVAS_WIDTH 1024
#define PONGPING_CANVAS_HEIGHT 768

#define PONGPING_CANVAS_MIDDLE_X (PONGPING_CANVAS_WIDTH / 2)




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




enum { PONGPING_USING_KEYBOARD, PONGPING_USING_JOYPAD_OR_JOYSTICK, PONGPING_USING_MOUSE };


#define MENU_MESSAGE_Y 600
#define MENU_CONTROL_TYPE_MESSAGE_Y 650



#define MULTICOLOURED_POLYGONS 


#define NUMBER_OF_DATA_TYPES 8

enum { LEVEL_TASK, BALL_POSITION, INITIAL_ANGLE, INITIAL_DISTANCE_INCREMENT, BALL_SPEEDS, REGULAR_POLYGON, END_OF_LEVEL_FILE, SURVIVE_UNTIL };




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


enum { PONGPING_NEXT_LEVEL_EXISTS, PONGPING_LAST_LEVEL_PLAYED_WAS_FINAL_LEVEL };




#define PONGPING_LEVELS_FILENAME ("game_data/levels_file.xml")






using namespace std;

extern std::ofstream logfile;





enum { PONGPING_FLAG_DOWN, PONGPING_FLAG_UP };



/*******************************************************************************************************************************/
/******************************** Angles in radians. ***************************************************************************/
/*******************************************************************************************************************************/



#define        PONGPING_PI      3.14159265358979323846
#define      PONGPING_2_PI      3.14159265358979323846 * 2.0
#define   PONGPING_PI_BY_2      3.14159265358979323846 / 2.0
#define PONGPING_3_PI_BY_2      ((3.0 * 3.14159265358979323846) / 2.0)


/*******************************************************************************************************************************/
/********************************  ***************************************************************************/
/*******************************************************************************************************************************/



enum { 

TITLE,
PONG_PLAY,
PONG_CONTROL_OPTIONS,
PONG_INSTRUCTIONS,
PONG_QUIT,
SET_TO_USE,
KEYBOARD,
JOYSTICK_SLASH_JOYPAD,
NO,
PLUGGED_IN,
INSTALLED,
DRIVER,
LEVEL,
NUMBERS,
GET_READY,
LEVEL_COMPLETE,
GAME_OVER_BITMAP,
EXTRA_LIFE_LETTERS,
SPIRAL_CONVERTED,
CONGRATULATIONS,
CONGRATULATIONS_OVERLAY,
CONGRATULATIONS_BACKGROUND,
PONGPING_FONT


};


/*******************************************************************************************************************************/
/********************************  ***************************************************************************/
/*******************************************************************************************************************************/


enum { PONGPING_VOID_FEATURE, PONGPING_POLYGON_FEATURE, PONGPING_CATCHMENT_AREA };


/*******************************************************************************************************************************/
/********************************  ***************************************************************************/
/*******************************************************************************************************************************/



/*******************************************************************************************************************************/
/********************************  ***************************************************************************/
/*******************************************************************************************************************************/

#define NUMBER_OF_LOG_STRINGS 7


enum { 

PROGRAM_START,
GETTING_DISPLAY_DETAILS,
FINISHED_GETTING_DISPLAY_DETAILS,
MAIN_LOOP_START, 
READ_LEVEL_READING_LEVEL, 
READ_LEVEL_LOADED_XML_FILE, 
READ_LEVEL_FOUND_LEVEL, 
READ_LEVEL_FINISHED_READING_LEVEL 

};  



   
/* PPLOG - prints a string to logfile. */
   
#define PPLOG(x) ( logfile << log_strings[x])
   
/* PPLOGINT - prints an integer to logfile. */
   
#define PPLOGINT(x) (logfile << x)
#define PPLOG2NEWLINES() (logfile << "\n\n\n")
   
   
extern const char *log_strings[]; // defined in pongping_log_strings.h

/*******************************************************************************************************************************/
/********************************  ***************************************************************************/
/*******************************************************************************************************************************/


typedef struct 
{
   double a, d;
  
} PONGPING_VECTOR;




enum { PONGPING_SOLUTIONS_IMAGINARY, PONGPING_SOLUTIONS_REAL };

enum { ANGLE_WITHIN, ANGLE_WITHOUT };

void get_discriminants(double *x, double *y, const double a, const double b, const double m, const double c, const double r);

int get_discriminants_(double *x, double *y, const double a, const double b, const double m, const double c, const double r);

double hypotenuse( const double y1, const double y2, const double x1, const double x2 );

double dot_product( double a, double b, double c, double d );

void add_vectors(double theta_v1, double mag_v1, double theta_v2, double mag_v2, double *theta_out_v, double *mag_out_v);

double principal_value_2(double angle);

int power_of_ten(int power);

void get_intersection_coordinates(double *x, double *y, const double m1, const double c1, const double m2, const double c2);

int point_in_triangle( const double px, const double py, const double x1, const double y1, const double x2, const double y2, const double x3, const double y3);

void rotate_point(const double extra_angle, const double in_x, const double in_y, double *out_x, double *out_y);

int angle_is_vertical(double angle);

void principal_value(double *angle);

int pongping_random_number(int l, int u);

int angle_appropriate(const double angle, const double lower_angle, const double higher_angle);

float radians_to_degrees( float angle );

void add_vectors_( PONGPING_VECTOR v1, PONGPING_VECTOR v2, PONGPING_VECTOR *newv );

void subtract_vectors_( PONGPING_VECTOR v1, PONGPING_VECTOR v2, PONGPING_VECTOR *newv );

void scale_vector(PONGPING_VECTOR *v, const double scalar);



/*******************************************************************************************************************************/
/********************************  ***************************************************************************/
/*******************************************************************************************************************************/


enum { 
 
PONGPING_GUI_RESULT_VALUE_ERROR,
 
PONGPING_GUI_RESULT_VALUE_END_PROGRAM,
 
PONGPING_GUI_RESULT_VALUE_FINISHED_OKAY
 
 };

/*******************************************************************************************************************************/
/********************************  ***************************************************************************/
/*******************************************************************************************************************************/


#define REGULAR                               1
#define IRREGULAR                             2
#define ALL_CONVEX                            4
#define CONCAVE                               8
#define ONE_COLOURED_POLYGON                  16
#define SHADED_POLYGON                        32
#define PONGPING_VANISHING_POLYGON            64
#define PONGPING_HIT_X_TIMES_POLYGON          128
#define CIRCLES_INSIDE_ONLY                   256         /* for use with the play area polygon */
#define PLAY_AREA                             1024


/*******************************************************************************************************************************/
/******************************** Enumeration for the variable level_result in pongping_main.cpp ***************************************************************************/
/*******************************************************************************************************************************/


enum { PONGPING_GUI_STATE, PONGPING_PLAY_STATE, PONGPING_GAME_COMPLETE_STATE, PONGPING_QUIT_GAME_AND_END_PROGRAM_STATE };

enum {

PONGPING_LEVEL_RESULT_DISPLAY_CLOSED,
				    
PONGPING_LEVEL_RESULT_CLEAR_UP_AND_QUIT_STATE,
					
PONGPING_LEVEL_RESULT_INITIALISATION_PROBLEM,
				              
PONGPING_LEVEL_RESULT_GO_TO_NEXT_LEVEL,
					 
PONGPING_LEVEL_RESULT_SKIP_TO_NEXT_LEVEL,
                     
PONGPING_LEVEL_RESULT_GAME_OVER,

PONGPING_LEVEL_RESULT_GAME_COMPLETE,

PONGPING_MEMORY_PROBLEM

};
