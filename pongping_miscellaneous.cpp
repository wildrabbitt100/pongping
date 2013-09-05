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
 
 

 
#include "pongping_headers/pongping_miscellaneous_headers.h"

using namespace std;


/* get_bitmaps() : */

BITMAP_LIST *get_bitmaps(const char *fn, const char *dir)
{
   BITMAP_LIST *list = NULL;
   FILE *file_for_png_names = NULL;
   ALLEGRO_BITMAP *test = NULL;
   
   int i;

   int n; /* number of bitmaps in the list */

   char dir_and_filename[100] = "";

   char dir_and_bitmap_filename[150] = "";
   char bitmap_filename[100] = "";

   int length;

   int convert_mask;

   int string_index;

   unsigned char r, g, b;

 
   /* make sure bitmaps get loaded to VRAM. */

   al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);

   list = (BITMAP_LIST *) malloc( sizeof( BITMAP_LIST ) );

   if(list == NULL)
   {
      printf("get_bitmaps() : couldn't get memory for BITMAP_LIST.\n");
      return NULL;
   }
   
   if(strlen(dir) >= 50)
   {
      printf("bitmap_list.c : get_bitmaps() : directory string too long.\n");
      return NULL; 
   }



   

   if( strlen(dir) + strlen(fn) > 98 )
   {
      printf(" get_bitmaps() : dir_and_filename too long.\n");
      return NULL;
   }
   
   /* dir_and_filename is [100] need to add dir + / + fn + '\0' so (dir + fn) without the '\0' must be 98 max */

   sprintf(dir_and_filename, "%s/%s", dir, fn);

   file_for_png_names = fopen(dir_and_filename, "r");
   
   if(file_for_png_names == NULL)
   {
       printf(" get_bitmaps() : couldn't open file (%s).\n", dir_and_filename);
       return NULL;
   }

   n = 0;

   while( fgets(bitmap_filename, 100, file_for_png_names) != NULL)
   {
      ++n;
   }
   
   if(n == 0)
   {
       printf(" get_bitmaps() : no bitmap names in list file (%s).\n", dir_and_filename);
       printf("%s \n", bitmap_filename);
       return NULL;
   }
   
   list->bitmaps = (ALLEGRO_BITMAP **) malloc(sizeof(ALLEGRO_BITMAP *) * n);
   
   if(list->bitmaps == NULL) 
   {
       printf(" get_bitmaps() : couldn't get memory for ALLEGRO_BITMAP pointers.\n");
       return NULL;
   }
   
   list->n = n;


   rewind(file_for_png_names);

   for(i = 0; i < n; ++i)
   {
      
      if(fgets(bitmap_filename, 99, file_for_png_names) == NULL)
      {
         printf("create_bitmap_list() : bitmap name missing from file (%s).\n", fn);
         return NULL;
      }




      length = strlen(bitmap_filename);


      convert_mask = 0;

      if(bitmap_filename[length - 2] == '\r')
      {
         bitmap_filename[length - 2] = '\0';

         if(bitmap_filename[length - 3] == 'm')
         {
            convert_mask = 1;

            string_index = 0;

            while(bitmap_filename[string_index] != ' ')
            {
               ++string_index;
            }

            bitmap_filename[string_index] = '\0';

         }
      }
      else if( bitmap_filename[length - 1] == '\n' || bitmap_filename[length - 1] == EOF)
      {

         bitmap_filename[length - 1] = '\0';

         if(bitmap_filename[length - 2] == 'm')
         {
            convert_mask = 1;
         }

         string_index = 0;

         while(bitmap_filename[string_index] != ' ' && bitmap_filename[string_index] != '\0')
         {
            ++string_index;
         }

         bitmap_filename[string_index] = '\0';
        
      }
      else
      {
          if( bitmap_filename[length - 1] == 'm' )
          {
              convert_mask = 1;
          }

          string_index = 0;

         while(bitmap_filename[string_index] != ' ' && bitmap_filename[string_index] != '\0')
         {
            ++string_index;
         }

         bitmap_filename[string_index] = '\0';

      }



      if(strlen(bitmap_filename) > 100)
      {
         printf("create_bitmap_list() : bitmap filename too long in (%s).\n", fn);
         return NULL;
      }

      strcat(bitmap_filename, ".png");


      if(snprintf(dir_and_bitmap_filename,  149, "%s/%s",  dir, bitmap_filename) == -1)
      {
         printf("create_bitmap_list() : couldn't make filename.\n");
         return NULL;
      }
      
      assert(i < n);
      
      assert(dir_and_bitmap_filename);
      
      
      if( (list->bitmaps[i] = al_load_bitmap(dir_and_bitmap_filename)) == NULL)
      {
         logfile << "create_bitmap_list() : couldn't load png " << dir_and_bitmap_filename << " from " << fn << "\n"; 
         return NULL;
      }
      
      


      if(convert_mask)
      {
         quickmask1(list->bitmaps[i]);
      }

   }

   fclose(file_for_png_names);
   
   return list;
}






void destroy_bitmap_list(BITMAP_LIST *bL)
{
   int i;


   for(i = 0; i < bL->n; ++i)
   {
      al_destroy_bitmap(bL->bitmaps[i]);
   }
   
   free(bL->bitmaps);

   free(bL);

}




void quickmask1(ALLEGRO_BITMAP *bmp)
{
   ALLEGRO_LOCKED_REGION *locked;
   register unsigned char *ptr;
   int x, y;

   const int width = al_get_bitmap_width(bmp);
   const int height = al_get_bitmap_height(bmp);

   locked = al_lock_bitmap(bmp, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE);


   for(y = 0; y < height; ++y)
   {
      for(x = 0; x < width; ++x)
      {
         ptr = (unsigned char *) locked->data + y * locked->pitch + x * 4;

         if(*(ptr + 3) == (unsigned char) 176 &&     //al_map_rgba(176, 20, 4, 255)
            *(ptr + 2) == (unsigned char) 20 &&
            *(ptr + 1) == (unsigned char) 4)
         {
            *ptr = 0;
         }
      }
   }

   al_unlock_bitmap(bmp);
}




void swap_color_a( ALLEGRO_LOCKED_REGION *locked, int w, int h, COULEUR old_color,  COULEUR new_color)
{
   unsigned char old_color_r, old_color_g, old_color_b, old_color_a;

   unsigned char new_color_r, new_color_g, new_color_b, new_color_a;


   unsigned char r, b, g, a;

   int x, y;



   al_unmap_rgba( old_color, &old_color_r, &old_color_g, &old_color_b, &old_color_a);
   al_unmap_rgba( new_color, &new_color_r, &new_color_g, &new_color_b, &new_color_a);


   for(y = 0; y < h; ++y)
   {
      for(x = 0; x < w; ++x)
      {
         r = (unsigned char) *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 3);
         g = (unsigned char) *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 2);
         b = (unsigned char) *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 1);
         a = (unsigned char) *((unsigned char *) locked->data + y * locked->pitch + x * 4);

        if( r == old_color_r && g == old_color_g && b == old_color_b && a == old_color_a)
        {
            *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 3) = new_color_r;
            *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 2) = new_color_g;
            *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 1) = new_color_b;
            *((unsigned char *) locked->data + y * locked->pitch + x * 4) = new_color_a;
        }
      }
   }

}














int get_int_from_config_file(ALLEGRO_CONFIG*f, const char *section, const char *name, int *value)
{
   const char* string;
   
   
   string = al_get_config_value(f, section, name);
   
   if(string == NULL)
   {
      return 1;     
   }
   
   *value = atoi(string);
   
   return 0;
   
   


}








int wait_for_key_down(void)
{

   ALLEGRO_EVENT_QUEUE *q = al_create_event_queue();
   ALLEGRO_EVENT ev;
   
   al_register_event_source(q, al_get_keyboard_event_source());
   
   //printf(" need a key down.\n");
   
   do
   {
      //logfile << "waiting for key down.\n";
      al_wait_for_event(q, &ev);
      
   } while( ev.type != ALLEGRO_EVENT_KEY_DOWN );

   //printf("got key down.\n");
   
   al_destroy_event_queue(q);
   
   if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
   {
      return 2;
   }
   else
   {
      return 1;
   }
}




















COULEUR get_shade(const unsigned char r1, const unsigned char g1, const unsigned char b1, const unsigned char r2, const unsigned char g2, const unsigned char b2, const double ratio)
{
   unsigned char new_r = r1 + (unsigned char) (( (float) r2 - (float) r1 ) * ratio);
   unsigned char new_g = g1 + (unsigned char) (( (float) g2 - (float) g1 ) * ratio);
   unsigned char new_b = b1 + (unsigned char) (( (float) b2 - (float) b1 ) * ratio);


   return al_map_rgb(new_r, new_g, new_b);
}



/*
void frame_side(ALLEGRO_BITMAP *bitmap, int direction_triangle_points, int xtL, int ytL, int xbR, int ybR, COULEUR tint )
{
   ALLEGRO_LOCKED_REGION *lock = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE);
   
   int x, y;

 
   int w = al_get_bitmap_width(bitmap);
   int h = al_get_bitmap_height(bitmap);
  
   unsigned char r, g, b, a;
   
   int indent;
   
   if(direction_triangle_points == PONGPING_POINTING_DOWN)
   {
   
      for(y = ytL; y < ybR; ++y)
      {
	     indent = 
		 
         for(x = 0; x < w; ++x)
	     {
            r = (unsigned char) *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 3);
            g = (unsigned char) *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 2);
            b = (unsigned char) *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 1);
            a = (unsigned char) *((unsigned char *) locked->data + y * locked->pitch + x * 4);


            *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 3) = new_color_r;
            *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 2) = new_color_g;
            *((unsigned char *) locked->data + y * locked->pitch + x * 4 + 1) = new_color_b;
            *((unsigned char *) locked->data + y * locked->pitch + x * 4) = new_color_a;
         }        
	  }
   }

}
*/






/* change this to better name */
bool compare_distance( PotentialMove *m1, PotentialMove *m2 )
{
    return m1->pfot < m2->pfot;
}






void check_bitmap(ALLEGRO_BITMAP *b)
{
	
   set_target_bitmap_to_backbuffer();
	
   
   draw_respecting_alpha_value();
   
   al_clear_to_color(PONGPING_BLACK);
   
   al_draw_bitmap(b, 0, 0, 0);
   
   al_flip_display();
	
   al_save_bitmap("check.png", b);
   wait_for_key_down();
	
	
   eek();
	
	
	
	
}









