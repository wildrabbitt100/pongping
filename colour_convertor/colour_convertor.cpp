#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


#include <iostream>

using namespace std;





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
      logfile << "loaded spiral_coloured.png";
   }
   
   COULEUR c;
   
   int x, y;
   
   unsigned char r, g, b;
   
   float new_r, new_g, new_b;
   
   float diff_r, diff_g, diff_b;
   
   al_set_target_bitmap(load);
   
   for(y = 0; y < al_get_bitmap_height(load); ++y)
   {
      for(x = 0; x < al_get_bitmap_width(load); ++x)
	  {
	     //logfile << "here.\n";
	  
	     c = al_get_pixel(load, x, y);
		 
		 al_unmap_rgb(c, &r, &g, &b);
		 
		 if(r != 25 || g != 71 || b != 11)
		 {
		    diff_r = 25 - (float) r;
		    diff_g = 71 - (float) g;
		    diff_b = 11 - (float) b;
			
			new_r = (float) r * 0.4;
			new_g = (float) g * 0.4;
			new_b = (float) b * 0.4;
			
			c = al_map_rgb(new_r, new_g, new_b);
			
			
			
			al_put_pixel(x, y, c);
			
		 }
	  
	  }
   }



   al_save_bitmap("spiral_conveted.png", load);
   

   int k;
   
   cin >> k;
}