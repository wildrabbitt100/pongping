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
 
 
 


#include "pongping_headers/pongping_get_ready_and_level_finished_bitmaps_headers.h"






ALLEGRO_BITMAP *get_get_ready_bitmap(BITMAP_LIST *list)
{
   ALLEGRO_BITMAP *bmp = al_create_bitmap(IN_GAME_INFO_TEXT_BOX_WIDTH, IN_GAME_INFO_TEXT_BOX_HEIGHT);
   if(bmp == NULL)
   {
      return NULL;
   }



   al_set_target_bitmap(bmp);

   al_clear_to_color(PONGPING_WHITE);
   
   al_clear_to_color(IN_GAME_INFO_TEXT_BOX_BORDER_COLOR);
   
   al_draw_filled_rectangle(IN_GAME_INFO_TEXT_BOX_BORDER_SIZE, IN_GAME_INFO_TEXT_BOX_BORDER_SIZE,
                            al_get_bitmap_width(bmp) - IN_GAME_INFO_TEXT_BOX_BORDER_SIZE,
							al_get_bitmap_height(bmp) - IN_GAME_INFO_TEXT_BOX_BORDER_SIZE, IN_GAME_INFO_TEXT_BOX_MAIN_COLOR);



   al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
   
   
   al_draw_bitmap(list->bitmaps[GET_READY], (al_get_bitmap_width(bmp) - al_get_bitmap_width(list->bitmaps[GET_READY])) / 2,
                                            GET_READY_BITMAP_GET_READY_Y, 0);
   
   /*
   set_target_bitmap_to_backbuffer();
   al_clear_to_color(WHITE);
   
   al_draw_bitmap(bmp, (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(bmp)) / 2.0, (SCREEN_HEIGHT - al_get_bitmap_height(bmp) )/2.0, 0);
   al_flip_display();
   wait_for_key_down();
   */
   
   return bmp;
}


ALLEGRO_BITMAP *get_level_finished_bitmap(BITMAP_LIST *list)
{
   ALLEGRO_BITMAP *bmp = al_create_bitmap(IN_GAME_INFO_TEXT_BOX_WIDTH, IN_GAME_INFO_TEXT_BOX_HEIGHT);

   if(bmp != NULL)
   {
      al_set_target_bitmap(bmp);

      al_clear_to_color(IN_GAME_INFO_TEXT_BOX_BORDER_COLOR);
   
      al_draw_filled_rectangle(IN_GAME_INFO_TEXT_BOX_BORDER_SIZE, IN_GAME_INFO_TEXT_BOX_BORDER_SIZE,
                            al_get_bitmap_width(bmp) - IN_GAME_INFO_TEXT_BOX_BORDER_SIZE,
							al_get_bitmap_height(bmp) - IN_GAME_INFO_TEXT_BOX_BORDER_SIZE, IN_GAME_INFO_TEXT_BOX_MAIN_COLOR);


      al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
      al_draw_bitmap(list->bitmaps[LEVEL_COMPLETE], (al_get_bitmap_width(bmp) - al_get_bitmap_width(list->bitmaps[LEVEL_COMPLETE])) / 2,
                                            GET_READY_BITMAP_LEVEL_COMPLETE_Y, 0);
   
   }
   
   return bmp;
}
