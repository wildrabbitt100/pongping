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

 
 
#include "pongping_headers/pongping_pixel_pusher_headers.h"









void make_bitmap_a_gradient(ALLEGRO_BITMAP *bitmap, int x1, int y1, int x2, int y2, int gradient_type)
{
    ALLEGRO_LOCKED_REGION *lock = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE );

	int x, y;

	unsigned char r, g, b, *ptr, *ptr2;
	
	unsigned char Lr, Lg, Lb;
	
	double new_r, new_g, new_b;
	
	double gradient_size;
	
	int start_x, start_y;
	
	if(gradient_type == PONGPING_HORIZONTAL_GRADIENT_LIGHT_EDGE_RIGHT )
	{
	   gradient_size = x2 - x1;
	   
	   
	   for(x = x1; x < x2; ++x)
	   {
	      ptr = (unsigned char *) lock->data + y1 * lock->pitch + x * 4;
	   
	      r = *(ptr + 3);
		  g = *(ptr + 2);
		  b = *(ptr + 1);
		  
		  Lr = (float) r + (255.0f - (float) r) * 0.8f;
		  Lg = (float) g + (255.0f - (float) g) * 0.8f;
		  Lb = (float) b + (255.0f - (float) b) * 0.8f;
		  
		  
		  new_r =  (Lr - r ) * ((double) (x - x1) / gradient_size) + (double) r;
		  new_g =  (Lg - g ) * ((double) (x - x1) / gradient_size) + (double) g;
		  new_b =  (Lb - b ) * ((double) (x - x1) / gradient_size) + (double) b;
		  
		  
	      for(y = y1; y < y2; ++y)
		  {
		     ptr2 = (unsigned char *) lock->data + y * lock->pitch + x * 4;
		  
	         *(ptr2 + 3) = (unsigned char) new_r;
	         *(ptr2 + 2) = (unsigned char) new_g;
			 *(ptr2 + 1) = (unsigned char) new_b;
	      }
	   }
	  
	}
	else if(gradient_type == PONGPING_HORIZONTAL_GRADIENT_LIGHT_EDGE_LEFT )
	{
	   gradient_size = x2 - x1;
	   
	   
	   for(x = x1; x < x2; ++x)
	   {
	      ptr = (unsigned char *) lock->data + y1 * lock->pitch + x * 4;
	   
	      r = *(ptr + 3);
		  g = *(ptr + 2);
		  b = *(ptr + 1);
		  
		  new_r = (225.0 - r ) * (1.0 - ((double) (x - x1) / gradient_size)) + (double) r;
		  new_g = (225.0 - g ) * (1.0 - ((double) (x - x1) / gradient_size)) + (double) g;
		  new_b = (225.0 - b ) * (1.0 - ((double) (x - x1) / gradient_size)) + (double) b;
		 
		  
	      for(y = y1; y < y2; ++y)
		  {
		     ptr2 = (unsigned char *) lock->data + y * lock->pitch + x * 4;
		  
	         *(ptr2 + 3) = (unsigned char) new_r;
	         *(ptr2 + 2) = (unsigned char) new_g;
			 *(ptr2 + 1) = (unsigned char) new_b;
	      }
	   }
	  
	}
	else if( gradient_type == PONGPING_VERTICAL_GRADIENT_LIGHT_EDGE_BOTTOM )
	{
	   gradient_size = y2 - y1;
	   
	   for( y = y1; y < y2; ++y )
	   {
	      ptr = (unsigned char *) lock->data + y * lock->pitch + x1 * 4;   
	      
		  r = *(ptr + 3);
		  g = *(ptr + 2);
		  b = *(ptr + 1);
		  
		  /* Get the colour for this column. */
		  
		  new_r =  (225.0 - r ) * ((double) (y - y1) / gradient_size) + (double) r;
		  new_g =  (225.0 - g ) * ((double) (y - y1) / gradient_size) + (double) g;
		  new_b =  (225.0 - b ) * ((double) (y - y1) / gradient_size) + (double) b;
	   
	      for( x = x1; x < x2; ++x)
		  {
		     ptr2 = (unsigned char *) lock->data + y * lock->pitch + x * 4;
		     
			 *(ptr2 + 3) = (unsigned char) new_r;
	         *(ptr2 + 2) = (unsigned char) new_g;
			 *(ptr2 + 1) = (unsigned char) new_b;
		  }
	   
	   }
	   
	   
	
	
	}
	else if( gradient_type == PONGPING_VERTICAL_GRADIENT_LIGHT_EDGE_TOP )
	{
	   gradient_size = y2 - y1;
	   
	   for( y = y1; y < y2; ++y )
	   {
	      ptr = (unsigned char *) lock->data + y * lock->pitch + x1 * 4;   
	      
		  r = *(ptr + 3);
		  g = *(ptr + 2);
		  b = *(ptr + 1);
		  
		  /* Get the colour for this column. */
		  
		  new_r =  (225.0 - r ) * (1.0 - (double) (y - y1) / gradient_size) + (double) r;
		  new_g =  (225.0 - g ) * (1.0 - (double) (y - y1) / gradient_size) + (double) g;
		  new_b =  (225.0 - b ) * (1.0 - (double) (y - y1) / gradient_size) + (double) b;
	   
	      for( x = x1; x < x2; ++x)
		  {
		     ptr2 = (unsigned char *) lock->data + y * lock->pitch + x * 4;
		     
			 *(ptr2 + 3) = (unsigned char) new_r;
	         *(ptr2 + 2) = (unsigned char) new_g;
			 *(ptr2 + 1) = (unsigned char) new_b;
		  }
	   
	   }
	}




   al_unlock_bitmap(bitmap);
   
}





void outline(ALLEGRO_BITMAP *bitmap, COULEUR couleur_to_outline, COULEUR couleur_to_replace, COULEUR replacement_couleur)
{
   ALLEGRO_LOCKED_REGION *lock1 = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READONLY);
   
   int in_width  = al_get_bitmap_width(bitmap);
   int in_height = al_get_bitmap_height(bitmap);
   
   unsigned char r_outline, g_outline, b_outline;
   unsigned char r_replace, g_replace, b_replace;
   unsigned char r_replacement, g_replacement, b_replacement;

   unsigned char r_mask, g_mask, b_mask;
   
   ALLEGRO_BITMAP *overlay = al_create_bitmap( in_width, in_height );
   
   ALLEGRO_LOCKED_REGION *lock2 = al_lock_bitmap(overlay, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_WRITEONLY);
   
   unsigned char *ptr_in, *ptr_in_2, *ptr_overlay;

   
   int x, y;
   
   al_unmap_rgb( couleur_to_outline, &r_outline, &g_outline, &b_outline);
   al_unmap_rgb( couleur_to_replace, &r_replace, &g_replace, &b_replace);
   al_unmap_rgb( replacement_couleur, &r_replacement, &g_replacement, &b_replacement);
   al_unmap_rgb( PONGPING_MASK_COLOR, &r_mask, &g_mask, &b_mask);
   
   for( x = 0; x < in_width; ++x )
   {
      for( y = 0; y < in_height; ++y )
	  {
	     ptr_overlay = (unsigned char *) lock2->data + y * lock2->pitch + x * 4;  
		 
		 *(ptr_overlay + 3) = r_mask;
		 *(ptr_overlay + 2) = g_mask;
		 *(ptr_overlay + 1) = b_mask;
		 *(ptr_overlay) = 255;
	  }
   }
   
   
   for( x = 0; x < in_width; ++x )
   {
      for( y = 0; y < in_height; ++y )
	  {
         ptr_in = (unsigned char *) lock1->data + y * lock1->pitch + x * 4;  
	  
	     /* If this pixel is the colour to outline. */
	  
	     if( *(ptr_in + 3) == r_outline && *(ptr_in + 2) == g_outline && *(ptr_in + 1) == b_outline )
		 {
		    if( y + 1 < in_height )
			{
		       ptr_in_2 = (unsigned char *) lock1->data + (y + 1) * lock1->pitch + x * 4;  
			   
			   if( *(ptr_in_2 + 3) == r_replace && *(ptr_in_2 + 2) == g_replace && *(ptr_in_2 + 1) == b_replace )
			   {
			      ptr_overlay = (unsigned char *) lock2->data + (y + 1)* lock2->pitch + x * 4;  
			   
			      *(ptr_overlay + 3) = r_replacement; 
			      *(ptr_overlay + 2) = g_replacement;
				  *(ptr_overlay + 1) = b_replacement;
				  *ptr_overlay       = 255; 
			   }
			   
			
			
			}
		 }
	  }
   }
   
   for( x = 0; x < in_width; ++x )
   {
      for( y = in_height - 1; y >= 0; --y )
	  {
         ptr_in = (unsigned char *) lock1->data + y * lock1->pitch + x * 4;  
	  
	     /* If this pixel is the colour to outline. */
	  
	     if( *(ptr_in + 3) == r_outline && *(ptr_in + 2) == g_outline && *(ptr_in + 1) == b_outline )
		 {
		    if( y - 1 >= 0 )
			{
		       ptr_in_2 = (unsigned char *) lock1->data + (y - 1) * lock1->pitch + x * 4;  
			   
			   if( *(ptr_in_2 + 3) == r_replace && *(ptr_in_2 + 2) == g_replace && *(ptr_in_2 + 1) == b_replace )
			   {
			      ptr_overlay = (unsigned char *) lock2->data + (y - 1) * lock2->pitch + x * 4;  
			   
			      *(ptr_overlay + 3) = r_replacement; 
			      *(ptr_overlay + 2) = g_replacement;
				  *(ptr_overlay + 1) = b_replacement;
				  *ptr_overlay       = 255; 
			   }
			   
			
			
			}
		 }
	  }
   }
   
   
   for( x = 0; x < in_width; ++x )
   {
      for( y = in_height - 1; y >= 0; --y )
	  {
         ptr_in = (unsigned char *) lock1->data + y * lock1->pitch + x * 4;  
	  
	     /* If this pixel is the colour to outline. */
	  
	     if( *(ptr_in + 3) == r_outline && *(ptr_in + 2) == g_outline && *(ptr_in + 1) == b_outline )
		 {
		    if( y - 1 >= 0 )
			{
		       ptr_in_2 = (unsigned char *) lock1->data + (y - 1) * lock1->pitch + x * 4;  
			   
			   if( *(ptr_in_2 + 3) == r_replace && *(ptr_in_2 + 2) == g_replace && *(ptr_in_2 + 1) == b_replace )
			   {
			      ptr_overlay = (unsigned char *) lock2->data + (y - 1) * lock2->pitch + x * 4;  
			   
			      *(ptr_overlay + 3) = r_replacement; 
			      *(ptr_overlay + 2) = g_replacement;
				  *(ptr_overlay + 1) = b_replacement;
				  *ptr_overlay       = 255; 
			   }
			   
			
			
			}
		 }
	  }
   }
   
   for( y = 0; y < in_height; ++y  )
   {
      for( x = 0; x < in_width; ++x  )
	  {
         ptr_in = (unsigned char *) lock1->data + y * lock1->pitch + x * 4;  
	  
	     /* If this pixel is the colour to outline. */
	  
	     if( *(ptr_in + 3) == r_outline && *(ptr_in + 2) == g_outline && *(ptr_in + 1) == b_outline )
		 {
		    if( x + 1 >= 0 )
			{
		       ptr_in_2 = (unsigned char *) lock1->data + y * lock1->pitch + (x + 1) * 4;  
			   
			   if( *(ptr_in_2 + 3) == r_replace && *(ptr_in_2 + 2) == g_replace && *(ptr_in_2 + 1) == b_replace )
			   {
			      ptr_overlay = (unsigned char *) lock2->data + y * lock2->pitch + (x + 1) * 4;  
			   
			      *(ptr_overlay + 3) = r_replacement; 
			      *(ptr_overlay + 2) = g_replacement;
				  *(ptr_overlay + 1) = b_replacement;
				  *ptr_overlay       = 255; 
			   }
			   
			
			
			}
		 }
	  }
   }
   
   for( y = 0; y < in_height; ++y  )
   {
      for( x = in_width - 1; x >= 0; --x  )
	  {
         ptr_in = (unsigned char *) lock1->data + y * lock1->pitch + x * 4;  
	  
	     /* If this pixel is the colour to outline. */
	  
	     if( *(ptr_in + 3) == r_outline && *(ptr_in + 2) == g_outline && *(ptr_in + 1) == b_outline )
		 {
		    if( x - 1 >= 0 )
			{
		       ptr_in_2 = (unsigned char *) lock1->data + y * lock1->pitch + (x - 1) * 4;  
			   
			   if( *(ptr_in_2 + 3) == r_replace && *(ptr_in_2 + 2) == g_replace && *(ptr_in_2 + 1) == b_replace )
			   {
			      ptr_overlay = (unsigned char *) lock2->data + y * lock2->pitch + (x - 1) * 4;  
			   
			      *(ptr_overlay + 3) = r_replacement; 
			      *(ptr_overlay + 2) = g_replacement;
				  *(ptr_overlay + 1) = b_replacement;
				  *ptr_overlay       = 255; 
			   }
			   
			
			
			}
		 }
	  }
   }

   al_unlock_bitmap(bitmap);
   al_unlock_bitmap(overlay);
   
   quickmask1(overlay);
   
   al_set_target_bitmap(bitmap);
   
   al_draw_bitmap(overlay, 0, 0, 0);
   

}










