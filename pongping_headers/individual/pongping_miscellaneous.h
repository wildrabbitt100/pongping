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
 
 
#ifndef BITMAP_LIST_H
#define BITMAP_LIST_H






typedef struct
{
   ALLEGRO_BITMAP **bitmaps;

   int n;       /* number of bitmaps */



} BITMAP_LIST;





BITMAP_LIST *get_bitmaps(const char *fn, const char *dir);
void destroy_bitmap_list(BITMAP_LIST *bl);

BITMAP_LIST * get_bitmap_list_from_zip(const char *fn, const char *dir, const char *zip_filename);
void swap_color_a( ALLEGRO_LOCKED_REGION *locked, int w, int h, COULEUR old_color,  COULEUR new_color);
void quickmask1(ALLEGRO_BITMAP *bmp);



int get_int_from_config_file(ALLEGRO_CONFIG* f, const char *section, const char *name, int *value);


int wait_for_key_down(void);

void save_line(float m, float c);






COULEUR get_shade(const unsigned char r1, const unsigned char g1, const unsigned char b1, const unsigned char r2, const unsigned char g2, const unsigned char b2, const double ratio);




bool compare_distance( PotentialMove *m1, PotentialMove *m2 );






void check_bitmap(ALLEGRO_BITMAP *b);





#endif

