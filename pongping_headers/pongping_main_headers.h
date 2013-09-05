#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


#include "tiny_xml/tinyxml.h"

#include "individual/pongping.h"



/***********************************************************************************************************/
/**************** End of Headers which don't need any other headers to have already been included. *********/
/***********************************************************************************************************/


#include "individual/pongping_potential_move.h"
#include "individual/pongping_miscellaneous.h"
#include "individual/pongping_gui.h"
#include "individual/pongping_essentials.h" 
#include "individual/pongping_circles_base.h" 
#include "individual/pongping_collectables.h"
#include "individual/pongping_level_feature.h" 
#include "individual/pongping_bat.h" 
#include "individual/pongping_one_game.h" 
#include "individual/pongping_info_bitmaps.h" 
#include "individual/pongping_control_options_menu.h" 
#include "individual/pongping_credits.h"
#include "individual/pongping_resource_check.h"
#include "individual/pongping_main.h"




#ifdef PONGPING_WINDOWS_VERSION

#include <Windows.h>

#endif
