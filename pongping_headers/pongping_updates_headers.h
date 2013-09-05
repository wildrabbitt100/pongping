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
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <vector>
#include <algorithm>
#include <cmath>

#include "tiny_xml/tinyxml.h"

#include "individual/pongping.h"




#include "individual/pongping_potential_move.h"
#include "individual/pongping_miscellaneous.h"
#include "individual/pongping_gui.h"
//#include "individual/pongping_font_manager.h"
#include "individual/pongping_essentials.h"
#include "individual/pongping_circles_base.h" 
#include "individual/pongping_collectables.h" 
#include "individual/pongping_level_feature.h" 
#include "individual/pongping_polygon.h"
#include "individual/pongping_bat.h"
#include "individual/pongping_one_game.h"
#include "individual/pongping_circle_circle_collisions.h"

