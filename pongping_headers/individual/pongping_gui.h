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
 

/* PONGPING_NUMBER_OF_SCREENS : this is used by the constructor of the PongPingGUI class so as to know how many GUI screens
   to assign memory for. */ 

#define PONGPING_NUMBER_OF_GUI_SCREENS 5

/*****************************************************************************************************************************/

//enum { PONGPING_MENU_NUMBER_OF_OPTIONS, PONGPING_MENU_CENTRE_X , PONGPING_MENU_Y, PONGPING_MENU_SPACING };
#define NUMBER_OF_MAIN_MENU_OPTIONS 4




/* Values for gui_mode in main() */

enum
{
   PONGPING_GUI_MENUS_MODE,
   PONGPING_GUI_ENTER_NAME_MODE,
   PONGPING_GUI_END_CREDITS_MODE
};


enum
{
   PONGPING_PLAY_GAME_OPTION,
   PONGPING_CONTROL_OPTIONS_OPTION,
   PONGPING_INSTRUCTIONS_OPTION,
   PONGPING_DISPLAY_OPTIONS_OPTION,
   PONGPING_HIGH_SCORES_OPTION,
   PONGPING_QUIT_GAME_OPTION
};



/* For menu message. Sometimes a message needs to be shown on the main menu : */

enum { NO_MESSAGE, NO_JOYSTICK_DRIVER_MESSAGE, NO_JOYSTICKS_PLUGGED_IN_MESSAGE };

enum { PONGPING_MAIN_MENU_GUI, PONGPING_CONTROL_OPTIONS_GUI, PONGPING_DISPLAY_OPTIONS_GUI, PONGPING_INSTRUCTIONS_GUI, PONGPING_HIGH_SCORES_GUI };


enum { PONGPING_BUTTON_UNSELECTED, PONGPING_BUTTON_SELECTED, PONGPING_BUTTON_SELECTED_AND_CHOSEN };





#define PONGPING_NUMBER_OF_MENUS_IN_GUI 3

enum { PONGPING_FONT_OUTLINE, PONGPING_FONT_INSIDE };


enum 
{ 
   PONGPING_MENU_BUTTON_TYPE_GUI_MODE_SWITCH,
   PONGPING_MENU_BUTTON_TYPE_GUI_SETTING_SWITCH,
   PONGPING_MENU_BUTTON_TYPE_GUI_FINISH_SWITCH
};
       
       
       



#define SPACE_BITMAP_WIDTH 20

enum { PONGPING_MENU_CENTRED };

enum { PONGPING_MENU_NUMBER_OF_BITMAPS };

#define PONGPING_BLOCK_INPUT_TIME 20


#define CONVERT_STRING_TO_BITMAP_DEBUG



class PongPingFontManager {
	
	
	public:
	
	ALLEGRO_BITMAP *font_bitmap;
	
	
    int font_height;
    
    
    int letter_x[27];	
	

	int digits[10];
	
	ALLEGRO_BITMAP *space_bitmap;
	
	int x;
	
	
	
	int init_okay_flag;
	
	unsigned char *c1r, *c2r, other_couleur, this_pixel_couleur_r;
	
	 
	
	PongPingFontManager() {}
	
	PongPingFontManager( ALLEGRO_BITMAP *_font_bitmap ) : init_okay_flag(PONGPING_FLAG_DOWN)
	{
		
	   
	   int i;
	   
	   int next_letter_number = 0;
	   
	   int current_couleur_r;
	    
	   if(_font_bitmap == NULL)
	   {
		  LOG("ALLEGRO_BITMAP pointer argument was NULL.\n");
		  return;   
	   }  
	    
	   
	   font_bitmap = _font_bitmap;
	   
	   
	   font_height = al_get_bitmap_height(font_bitmap);
	    
	   ALLEGRO_LOCKED_REGION *lock = al_lock_bitmap(font_bitmap, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE);
	   
	   if(lock == NULL)
	   {
		  LOG("Couldn't lock the font bitmap.\n");   
		  return;
	   }
	   
	   //
	   
	 
	   
	   /* get the red component of the pixel at 0,0 on the font bitmap (let it be 10)*/
	   
	   current_couleur_r = (int) *((unsigned char *) lock->data + lock->pitch + 3);
	  
	   x = 0;
	   
	   while( next_letter_number <= 26 ) 
	   {
		  
	      letter_x[next_letter_number++] = x;
		  
	      this_pixel_couleur_r = *((unsigned char *) lock->data + lock->pitch + x * 4 + 3);	
	      
	      while ( this_pixel_couleur_r == current_couleur_r )
	      {
			 ++x;   
			 this_pixel_couleur_r = *((unsigned char *) lock->data + lock->pitch + x * 4 + 3);	 
		
	      }
	 
	     
	      current_couleur_r = this_pixel_couleur_r;
	  
       }
	   
	   
	   
	   swap_color_a(lock, al_get_bitmap_width(font_bitmap), al_get_bitmap_height(font_bitmap), al_map_rgba(25, 114, 10, 255), al_map_rgba(176, 20, 4, 255));
	   
	   al_unlock_bitmap(font_bitmap);
	   
	   
	   
	   quickmask1(font_bitmap);
	   
	   /* Create the bitmap for a space character. */
	   
	   space_bitmap = al_create_bitmap( SPACE_BITMAP_WIDTH, font_height);
	   al_set_target_bitmap(space_bitmap);
	   al_clear_to_color(PONGPING_MASK_COULEUR);
	   quickmask1(space_bitmap);
	   
	   //check_bitmap(font_bitmap);
	   
	   init_okay_flag = PONGPING_FLAG_UP;
   
    };
	

	
	
	ALLEGRO_BITMAP *convert_string_to_bitmap(const char *word, int which_half)
	{
	   #ifdef CONVERT_STRING_TO_BITMAP_DEBUG
	      cout << " in convert_string_to_bitmap().\n";
	   #endif
	  
	   int sL; /* string length */
	   
	   ALLEGRO_BITMAP *bmp;
	   
	   if(word == NULL)
	   {
		  return NULL;   
	   }
	   
	   #ifdef CONVERT_STRING_TO_BITMAP_DEBUG
	      cout << "word argument not NULL.\n";
	   #endif
	   
	   int i;
	   
	   
	   int bw = 0;
	   
	   int bx = 0;
	   
	   int ascii_val;
	   
	   int letter_width;
	   
	   ALLEGRO_LOCKED_REGION *lock;
	   
	   sL = strlen(word); /* Get the length of the string minus the NULL character at the end. */
	   
	   
	   
	   
	   
	   al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	   
	   
	   /* Calculate the width the bitmap needs to be. */
	   
	   for(i = 0; i < sL; ++i)
	   {
	      ascii_val = (int) word[i];
	      
	      if(ascii_val >= 97 && ascii_val <= 122) /* If it's a lower case character. */
	      {
		     ascii_val -= 32;  
		  }
	      
	      if(ascii_val >= 65 && ascii_val <= 90)
	      {
			 ascii_val -= 65; 
			 
			 bw += letter_x[ascii_val + 1] - letter_x[ascii_val]; 
		  }
		  else if(ascii_val == 32) /* It's the space character. */
		  {
		     bw += SPACE_BITMAP_WIDTH;  
			 
			 continue;
			  
		  }
	      else
	      {
			  #ifdef CONVERT_STRING_TO_BITMAP_DEBUG
			     LOG("String has unexpected character in it."); 
			     cout << "String has unexpected character in it.\n";
			     cout << "It was " << ((char) ascii_val) << "\n"; 
			  #endif
			  return NULL;
		  }
		  
		  
	   }
	   
	   
	   bmp = al_create_bitmap(bw, font_height);
	   
	   
	   
	   if(bmp == NULL)
	   {
		  #ifdef CONVERT_STRING_TO_BITMAP_DEBUG
		     cout << "couldn't create the bitmap.\n";
		  
		  #endif
		  LOG("Couldn't create bitmap.\n");
		  
	   }
	   
	   al_set_target_bitmap(bmp);
	   
	   al_clear_to_color(PONGPING_MASK_COULEUR);
	   
	   draw_respecting_alpha_value();
	   
	   for(i = 0; i < sL; ++i)
	   {
	      ascii_val = (int) word[i];
	      
	      if(ascii_val >= 97 && ascii_val <= 122) /* If it's a lower case character. */
	      {
		     ascii_val -= 32;  
		  }
	      
	      if(ascii_val >= 65 && ascii_val <= 90)
	      {
			 ascii_val -= 65; 
			  
		  }
		  else if(ascii_val == 32) /* It's the space character. */
	      {
			 
			  al_draw_bitmap(space_bitmap, bx, 0, 0);
			  bx += letter_width;
			  continue;
			  
		  }
	      else
	      {
			  LOG("String has unexpected character in it.");
			  #ifdef CONVERT_STRING_TO_BITMAP_DEBUG
			     cout << "String has unexpected character in it.\n";
			  #endif 
			  return NULL;
		  }
		  
		  
		  letter_width = letter_x[ascii_val + 1] - letter_x[ascii_val];
		 
		  al_draw_bitmap_region( font_bitmap, letter_x[ascii_val], 0, letter_width , font_height, bx, 0, 0 );
		  
		  bx += letter_width;
		   
	   }
	   
	   /* The bitmap is currently the complete font - ie outline plus the inside. The following strips the bitmap
	    * down to either just the outline or just the inside of the letters. */
	   
	   lock = al_lock_bitmap(bmp, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE);
	   if(lock == NULL)
	   {
		  al_destroy_bitmap(bmp);
		  #ifdef CONVERT_STRING_TO_BITMAP_DEBUG
		     cout << "couldn't lock the bitmap.\n";
		  #endif
		  return NULL; 
	   }
	   else
	   {
		   COULEUR c = (which_half == PONGPING_FONT_OUTLINE ? PONGPING_WHITE : PONGPING_FONT_OUTLINE_COULEUR );
		   
		   swap_color_a(lock, bw, font_height, c, PONGPING_MASK_COULEUR);
		   
		   al_unlock_bitmap(bmp);
	   }
	   
	   quickmask1(bmp);
	   
	   if(bmp == NULL)
	   {
	      #ifdef CONVERT_STRING_TO_BITMAP_DEBUG
	         cout << "about to return bitmap but bitmap NULL.\n";
	      #endif
	   
       }
       
	   return bmp;
	}
	
	
	
};











typedef struct 
{
   char button_word[20];
	
   int menu_button_type;
    
   int (*action_function)(int button_number);
	
	
} PONGPING_MENU_BUTTON;

PONGPING_MENU_BUTTON *get_menu_button(int menu_id, int button_number);

int get_number_of_buttons_for_this_menu(int menu_id);


class PongPingGUIFeature {
	
	
	public:
	   
	   PongPingGUIFeature() {}
	   
	
	   virtual int draw(void) = 0;
	 
};









class PongPingGUIBitmap : PongPingGUIFeature {
	
   ALLEGRO_BITMAP *bitmap;
      
   int x, y;
   
   
   
   public:
   
      int init_okay_flag;
      
      PongPingGUIBitmap() { };
      
      PongPingGUIBitmap( ALLEGRO_BITMAP *b, int _x, int _y ) : init_okay_flag(PONGPING_FLAG_DOWN)
      { 
		 bitmap = b;
		 
		 x = _x;
		 y = _y;
		 
		 init_okay_flag = PONGPING_FLAG_UP;
		 
	  };

   int draw() 
   {
      al_draw_bitmap(bitmap, x, y, 0);
      return 0;
   }


};




class PongPingGUIMenu : PongPingGUIFeature {
	
	PongPingGUIMenu(const PongPingGUIMenu&);
	
	public:
	
	int n;
	
	PONGPING_MENU_BUTTON **menu_buttons;
	
	ALLEGRO_BITMAP **outline_bitmaps;
	
	ALLEGRO_BITMAP **character_body_bitmaps;
	
	int *button_states;
	
	int init_okay_flag;
	
	int bitmap_height;
	
	int x_centre, top_y;
	
	int y_spacing;
	
	int option_selected;
	int option_chosen;
	
	int menu_id;
	
	
	COULEUR tint;
	
	PongPingGUIMenu( int _menu_id, int _top_y, int _option_selected, PongPingFontManager& font_manager ) 
	{
	   outline_bitmaps = NULL;
	   
	   character_body_bitmaps = NULL;
	   
	   menu_id = _menu_id;
	   
	   option_selected = _option_selected;
	   
	   option_chosen = PONGPING_NOT_A_NORMAL_VALUE;
	   
	   top_y = _top_y;
	   
	   n = get_number_of_buttons_for_this_menu(_menu_id);
	   
	   outline_bitmaps = new ALLEGRO_BITMAP *[n];
	   character_body_bitmaps = new ALLEGRO_BITMAP *[n];
	   menu_buttons = new PONGPING_MENU_BUTTON *[n];
	   
	   int i;
	   
	   for(i = 0 ; i < n; ++i)
	   {
		  menu_buttons[i] = get_menu_button( menu_id, i);
		  
		  
		  cout << "making bitmap with word - >>" << menu_buttons[i]->button_word << "<< \n\n";
		  
	      outline_bitmaps[i] = font_manager.convert_string_to_bitmap( menu_buttons[i]->button_word, PONGPING_FONT_OUTLINE );    
		  
		  if(outline_bitmaps[i] == NULL)
		  {
			 cout << "PongPingGUIMenu : in constructor : failed to create bitmap (outline_bitmaps[]).\n";
			 eek();
		  }
		  
		  character_body_bitmaps[i] = font_manager.convert_string_to_bitmap( menu_buttons[i]->button_word, PONGPING_FONT_INSIDE );       

	   }
       
       
       	   
	   init_okay_flag = PONGPING_FLAG_UP;
	   
	}
	
	
	
	
	
	
	int draw() 
	{
	   
	   int i;
	   
	   int xdraw, ydraw;
	   
	   draw_respecting_alpha_value();
	  
	   cout << "in PongPingGUIMenu::draw().\n";
	   
	   for(i = 0; i < n; ++i)
	   {
		  cout << "checking if bitmap is null.\n";
		  
		  if(outline_bitmaps[i] == NULL)
		  {
			 cout << "a bitmap is NULL. i = " << i << "\n\n";  
		  }
		  
		  xdraw = x_centre - al_get_bitmap_width(outline_bitmaps[i]) / 2;
		  
		  ydraw = top_y + (i - 1) * (bitmap_height + y_spacing);
		  
		  
		  //al_save_bitmap("check.png", character_body_bitmaps[i]);
		  
		  //check_bitmap(character_body_bitmaps[5]);
	      
		  
		  
		  
		  al_draw_bitmap(outline_bitmaps[i], xdraw, ydraw, 0);
		  
		  //tint
		  
		  tint = ((i == option_chosen) ? MENU_FONT_COLOR_FOR_WHEN_CHOSEN : ((i == option_selected) ? MENU_FONT_COLOR_FOR_WHEN_SELECTED : MENU_FONT_TEXT_ORIGINAL_COLOR));
		  
		  if(character_body_bitmaps[i] == NULL)
		  {
			 cout << "a bitmap is NULL.\n";  
		  }
		  
		  al_draw_tinted_bitmap(character_body_bitmaps[i], tint, xdraw, ydraw, 0);  
		 
		  
	   }	
	   
	   
	   
	   return 0;
	}
	
	void update(int down, int up)
	{
	   if( down && up )
	   {
		  return;    
	   }
	   
	   if(up && option_selected > 0) 
	   {
		   --option_selected;
		   return;
	   }
	   
	   if(down && option_selected < (n - 1)) 
	   {
		   ++option_selected;
		   return;
	   }
	   
    }
	
	~PongPingGUIMenu()
	{
	   int i;
	   
	   for(i = 0; i < n; ++i)
	   {
	      al_destroy_bitmap( outline_bitmaps[i] );   
	   }
	   
	   
	   delete [] outline_bitmaps;
	   
	   
	   cout << "called Menu dtor.\n";
		
		
	}
	
};




class PongPingGUIScreen {
	
	COULEUR clear_couleur;
	
	public:
	
	vector <PongPingGUIFeature *> features;
	
	int index_of_last_feature;
	
	int menu_index;
	
	PongPingGUIScreen() : menu_index(PONGPING_NOT_A_NORMAL_VALUE) 
	{ 
		/* Could have used PONGPING_NOT_A_NORMAL_VALUE here but the initialisation to -1 is 
		 * important because the first increment of index_of_last_feature sets it to 0 so it's better to see it here. 
		 * Subsequent increments occur each time a new feature gets added. */
		index_of_last_feature = -1; 
	}
	
	void add_gui_bitmap(PongPingGUIBitmap *gui_bitmap)
	{
	   
	   features.push_back( (PongPingGUIFeature *) gui_bitmap );
	   
	   ++index_of_last_feature;
    }
    
    void add_gui_menu(PongPingGUIMenu *menu)
    {
	   features.push_back( (PongPingGUIFeature *) menu );
	   
	   ++index_of_last_feature;
	   
	   menu_index = index_of_last_feature;
    }
    
    
    void draw_gui_screen(void)
    {
       
       int i = index_of_last_feature;
       
       //al_clear_to_color(clear_couleur);
       
       while(i >= 0)
       {
		  
		  if(features[i] != NULL)
		  {
		     features[i--]->draw();
		  }

	   }
    
	
    }
    
    void set_clear_couleur(COULEUR c)
	{
	   clear_couleur = c;	
	}
	
};



class PongPingGUI {
	
	int init_okay_flag;
	
	// Disable copy constructor and assignment operator. 
	PongPingGUI(const PongPingGUI&);
	PongPingGUI& operator=(const PongPingGUI&);
	
	
	
	
	public:
	
	PongPingGUIScreen **screens;
	
	
	int mode;
	
	
	
	
	explicit PongPingGUI(int n) : init_okay_flag(PONGPING_FLAG_DOWN), screens(new PongPingGUIScreen*[n]), 
	                              mode(PONGPING_MAIN_MENU_GUI)
	{  
	   int i;
	   
	   /* Got to create the Screen class(es). */
	   
	   for(i = 0; i < n; ++i)
	   {
	      screens[i] = new PongPingGUIScreen();
       }
		
	   init_okay_flag = PONGPING_FLAG_UP;
	}
	

	
	
	~PongPingGUI()
	{
	   delete [] screens; // does the destructor get called for the PongPingGUIScreen's ?	
	}
	
	int set_mode(int m)
	{
	   mode = m;
	}
	
	
};









