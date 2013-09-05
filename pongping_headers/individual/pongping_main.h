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





using namespace std;





std::ofstream logfile;








void set_pixel_formats()
{
   PongPingEssentials::pixel_formats[0] = ALLEGRO_PIXEL_FORMAT_RGBA_8888; 
   PongPingEssentials::pixel_formats[1] = ALLEGRO_PIXEL_FORMAT_ARGB_8888; 
   PongPingEssentials::pixel_formats[2] = ALLEGRO_PIXEL_FORMAT_ABGR_8888;
   PongPingEssentials::pixel_formats[3] = ALLEGRO_PIXEL_FORMAT_XBGR_8888;
   PongPingEssentials::pixel_formats[4] = ALLEGRO_PIXEL_FORMAT_RGBX_8888;
   PongPingEssentials::pixel_formats[5] = ALLEGRO_PIXEL_FORMAT_XRGB_8888;
}




int initialise_log(void)
{
   logfile.open("pongping_log.txt", ios_base::out); 
   if(logfile == NULL)
   {
      return 1;
   }
   
   PPLOG(PROGRAM_START);
   
   return 0;
}



static ALLEGRO_BITMAP **get_extra_menu_bitmaps( BITMAP_LIST *list, COULEUR color );


int PongPingEssentials::pixel_formats[NUM_DESIRED_PIXEL_FORMATS];


