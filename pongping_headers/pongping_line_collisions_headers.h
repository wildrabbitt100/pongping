#include <allegro5/allegro.h>

#include <allegro5/allegro_primitives.h>

#include <iostream>
#include <cmath>

#include <gsl/gsl_poly.h>


/***********************************************************************************************************/
/**************** Headers which don't need any other headers to have already been included. ****************/
/***********************************************************************************************************/

#include "individual/pongping.h"



/***********************************************************************************************************/
/**************** End of Headers which don't need any other headers to have already been included. *********/
/***********************************************************************************************************/


#include "individual/pongping_potential_move.h"
#include "individual/pongping_miscellaneous.h"                          
#include "individual/pongping_circles_base.h"
#include "individual/pongping_all_collisions.h"
#include "individual/pongping_line_collisions.h"
