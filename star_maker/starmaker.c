







#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>


int main(int argc, char **argv)
{
   
   FILE *f = fopen("xml.txt", "w");
   
   float x, y;

   if( argc != 6 )
   {
      printf("need six arguments. Can't continue.\n");
	  
	  getch();
	  
	  return 1;
	  
   }
   
   printf(" number of points = %f . Outer radius = %f Inner radius = %f Centre x %f Centre y %f ", 
   strtod(argv[1], 0), strtod(argv[2], 0), strtod(argv[3], 0), strtod(argv[4], 0), strtod(argv[5], 0) ); 
   
   al_init();
   al_init_primitives_addon();
   
   ALLEGRO_DISPLAY *d = al_create_display(1024, 768);
   
   
   
   al_clear_to_color(al_map_rgb(0, 0, 0));
   
   int i;
   
   double cx = strtod ( argv[4], 0 );
   double cy = strtod ( argv[5], 0 );
   
   double points = strtod( argv[1], 0 );
   
   double outer_radius = strtod(argv[2], 0);
   double inner_radius = strtod(argv[3], 0);
   
   double angle;
   
   
   
   
   for( i = 0; i < points; ++i )
   {
      angle = (double) i * 2.0 * 3.1415926 / (double) points;
      
	  x = cx + outer_radius * cos( angle );
      y = cy + outer_radius * sin( angle );
	  
	  al_draw_pixel(x, y, al_map_rgb(255, 255, 255));
	  
	  fprintf(f, "      <vertex x=\"%3d\" y=\"%3d\" />\n", (int) x, (int) y);
	  
	  angle = (double) i * 2.0 * 3.1415926 / (double) points + 2.0 * 3.1415926 / ((double) points * 2.0);
      
	  x = cx + inner_radius * cos( angle );
      y = cy + inner_radius * sin( angle );
	  
	  al_draw_pixel(x, y, al_map_rgb(255, 255, 255));
	  
	  fprintf(f, "      <vertex x=\"%3d\" y=\"%3d\" />\n", (int) x, (int) y);
   
   }
   
   al_flip_display();
   
   getch();
   
   fclose(f);
}