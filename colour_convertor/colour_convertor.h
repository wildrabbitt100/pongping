#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>










int main()
{
   ALLEGRO_BITMAP *load;
   
   al_init();
   al_init_image_addon();
   al_init_primitives_addon();
   
   load = al_load_bitmap("spiral_coloured.png");
   
   if(load == NULL)
   {
      logfile << "couldn't load spiral_coloured.png .\n";
	  return 1;
   }
   else
   {
      logfile << "loaded spiral_coloured.png");
   }
   







}