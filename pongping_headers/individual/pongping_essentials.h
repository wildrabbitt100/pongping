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
 









/* class which contains all the data for each game, like which level to start at. */

class PongPingEssentials {
 
   
public:
   
   ALLEGRO_EVENT event;
   ALLEGRO_EVENT_QUEUE *queue;
   ALLEGRO_TIMER *timer;
   ALLEGRO_DISPLAY *display;
   
   BITMAP_LIST *graphics_for_game;
   
   BITMAP_LIST *menu_list;
   
   ALLEGRO_BITMAP *mol[NUMBER_OF_MAIN_MENU_OPTIONS]; /* Menu option letters. */
   
   ALLEGRO_BITMAP *col[NUM_CONTROL_MENU_OPTIONS]; /* Control options letters */
   
   PongPingFontManager font_manager;

 
   
   int window_width;
   int window_height;
   
   int window_width_fullscreen;
   int window_height_fullscreen;
   
   int window_pixel_format;
   
   
   int need_transform;
   
   
   int number_of_joysticks;

   static int pixel_formats[NUM_DESIRED_PIXEL_FORMATS];
   
   int mouse_installed_flag;
   int keyboard_installed_flag;
   int joystick_or_joypad_driver_installed_flag;
   
   
   int joystick_or_joypad_driver_available_flag;
   int mouse_driver_available_flag;
   int keyboard_driver_available_flag;
   
   
   int screen_res_width, screen_res_height;
   
   int use_log;
   
   int run_fullscreen_flag;
   
   int found_fullscreen_mode_flag;
   
   int start_level;
   
   int current_control_type;
   
   int number_of_levels;
   
   PongPingGUIMenu *next_menu;
   PongPingGUIBitmap *next_gui_bitmap;
   
   int init_okay_flag;
   
   
   PongPingGUI *gui;
   
   PongPingEssentials() : mouse_installed_flag(PONGPING_FLAG_DOWN), 
                          keyboard_installed_flag(PONGPING_FLAG_DOWN),
                          joystick_or_joypad_driver_installed_flag(PONGPING_FLAG_DOWN),
                          
                          joystick_or_joypad_driver_available_flag(PONGPING_FLAG_DOWN), 
                          mouse_driver_available_flag(PONGPING_FLAG_DOWN),
                          keyboard_driver_available_flag(PONGPING_FLAG_DOWN),
                          
                          use_log(0), run_fullscreen_flag(PONGPING_FLAG_DOWN),
                          
                          start_level(1),
                          number_of_joysticks(0),
                          found_fullscreen_mode_flag(PONGPING_FLAG_DOWN),
                          init_okay_flag(PONGPING_FLAG_DOWN)
   {  
     
      srand(time(0));
      
      initialise_allegro();
      
      #ifdef PONGPING_WINDOWS_VERSION
      screen_res_width = GetSystemMetrics(SM_CXSCREEN);
      screen_res_height = GetSystemMetrics(SM_CYSCREEN);
      #endif
      
      
      LOG("Screen resolution width  = ");
      LOGINT(screen_res_width);
      LOG("\n Screen resolution height = ");
      LOGINT(screen_res_height);
      LOG("\n");
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      queue = al_create_event_queue();
      if(queue == NULL)
      {
        LOG("couldn't create event_queue.\n"); 
        return;
      }
      
      check_for_input_drivers();
      
      
      install_all_input_devices();
      
      if( process_configuration_file() != 0)
      {
         return;
      }
      
      

      number_of_levels = get_number_of_levels_from_xml();
      if(number_of_levels == -1)
      {
         LOG("Couldn't read total_number_of_levels from xml file.\n");
         return;
      }
      
      
      timer = al_create_timer(1.0 / PONGPING_TIMER_TICKS_PER_SECOND);
      if(timer == NULL)
      {
         LOG("couldn't create timer.\n");
         return;
      }
      
      
      
      /***********************************************************************/
      /******************* Display. ******************************************/
      /***********************************************************************/
      
      if(get_display_details() != 0)
      {
         LOG("Problem with graphics modes.\n");
         return;
      }
      
      al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_REQUIRE);
      
      if( run_fullscreen_flag == PONGPING_FLAG_UP )
      {
         al_set_new_display_flags( ALLEGRO_FULLSCREEN_WINDOW );
         display = al_create_display(window_width, window_height);
         if(display == NULL)
         {
            LOG("couldn't create display.\n");
            return;
         }       
      }
      else
      {
         al_set_new_display_flags( ALLEGRO_WINDOWED );
         //logfile << "In windowed mode : window width = " << window_width << "window height = " << window_height << "\n";
         display = al_create_display(window_width, window_height);
         if(display == NULL)
         {
            LOG("Couldn't create display.\n");
            return;
         }
      }
      
      //set_needs_transform_flag();
      
      al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);
      
      al_set_window_title(display, "Pong Ping");
      
      /***********************************************************************/
      /************* Load bitmaps. *******************************************/
      /***********************************************************************/
      
      LOG("\nLoading bitmaps from game_menu_list.txt \n");
     
      
      menu_list = get_bitmaps("game_menu_list.txt", "game_data");
      if(menu_list == 0)
      {
         LOG("problem initialising. (couldn't load bitmaps from game_data/game_menu_list.txt)\n");
         return;
      }
      else
      {
		 LOG("Done loading bitmaps from game_menu_list.txt.\n\n");
	  }
      
      
      
      
      
      
      LOG("Creating the Font Manager.\n");
    
      
      
      cout << "making font manager.\n";
      
      font_manager = PongPingFontManager(menu_list->bitmaps[PONGPING_FONT]);
      
      if(font_manager.init_okay_flag == PONGPING_FLAG_DOWN)
      {
         LOG("Aborting in PongPingEssentials constructor because of problem in PongPingFontManger constructor.\n");
         return;
      }
      
      cout << "made font manager.\n";
      
      /* Make the GUI... */
      
      gui = new PongPingGUI(PONGPING_NUMBER_OF_GUI_SCREENS); 
      
      gui->screens[PONGPING_MAIN_MENU_GUI]->set_clear_couleur(PONGPING_GUI_BACKGROUND_COLOR);
      
      next_gui_bitmap = new PongPingGUIBitmap( menu_list->bitmaps[TITLE], (PONGPING_CANVAS_WIDTH - al_get_bitmap_width(menu_list->bitmaps[TITLE])) / 2, TITLE_GRAPHIC_Y);
      
      if(next_gui_bitmap->init_okay_flag == PONGPING_FLAG_DOWN)
      {
         LOG("Couldn't create PongPingGUIBitmap of title graphic.\n");  
         return;
      }
      
      gui->screens[PONGPING_MAIN_MENU_GUI]->add_gui_bitmap(next_gui_bitmap);
      
      
      cout << "making menu.\n";
      
      next_menu = new PongPingGUIMenu(PONGPING_MAIN_MENU_GUI, 300, 0, font_manager);
      
      cout << "made menu.\n";
      
      
      gui->screens[PONGPING_MAIN_MENU_GUI]->add_gui_menu(next_menu);
      
     
      
      al_register_event_source( queue, al_get_timer_event_source( timer ) );
      
      al_register_event_source( queue, al_get_display_event_source(al_get_current_display()));
      
      
      /* make the gui's */
      
      //new_gui = new PongPingGUI();
      
      //font_manager.make_gui( new_gui, main_menu_gui );
     
      /*************************************************************************/
      /*********** Set the flag indicating that initialisation went okay. ******/
      /*************************************************************************/
      
      init_okay_flag = PONGPING_FLAG_UP;
   }
   
  
   
   int get_number_of_levels_from_xml(void)
   {
      int total_number_of_levels = 0;
      
      TiXmlDocument level_file;
   
      if( level_file.LoadFile( PONGPING_LEVELS_FILENAME, TIXML_DEFAULT_ENCODING ) == false)
      {
         
         LOG("PongPingEssentials::get_number_of_levels : couldn't load xml file.\n");
      
         return -1;
      }
   
      TiXmlHandle hDoc(&level_file);
      TiXmlElement *pLevelDefinitionElement = hDoc.FirstChild("level_definition_for_willam_labbetts_awesome_new_game" ).Element();
      if(pLevelDefinitionElement == NULL)
      {
         LOG("reading xml file - couldn't find a level_definition_for_willam_labbetts_awesome_new_game element.\n");
         return -1;
      }
      //logfile << "got level definition element.\n";
      TiXmlHandle hLevelDefinition = TiXmlHandle(pLevelDefinitionElement); 
   
   
      TiXmlElement *pNumberOfLevelsElement = hLevelDefinition.FirstChild("number_of_levels").Element();
   
      if(pNumberOfLevelsElement == NULL)
      {
         LOG("get_number_of_levels() : couldn't get number of levels from xml file.\n");
         return -1; 
      }
      else
      {
         if( pNumberOfLevelsElement->QueryIntAttribute( "n", &total_number_of_levels) != TIXML_SUCCESS )
         {
            LOG("couldn't read n attribute from number_of_levels element.\n");
            return -1;

         }     
         else
         {
            return total_number_of_levels;
         }   
   
      }
   }
   
   
   
   
  

   int get_level_to_start_at()
   {
     
     return start_level;
   }
   
 
   
   
   /*
   void set_needs_transform_flag()
   {
      if(running_fullscreen() && (window_width_fullscreen != 1024 || window_height_fullscreen != 768))
      {
         need_transform = 1;
      }
      else if( ! running_fullscreen() && (window_width != 1024 || window_height != 768))
      {
         need_transform = 1;
      }
      else
      {
         need_transform = 0;
      }
   }
   
   void set_up_transform(void)
   {
   
      const int DESIRED_W = 1024;
      const int DESIRED_H = 768;
      int w = al_get_display_width(display);
      int h = al_get_display_height(display);
      ALLEGRO_TRANSFORM t;
      al_identity_transform(&t);
      al_scale_transform(&t, (float)w/(float)DESIRED_W, (float)h/(float)DESIRED_H);
      al_use_transform(&t);
   }
   
   */
   
   int get_number_of_levels() { return number_of_levels; }
   
   void initialise_allegro(void);
   int process_configuration_file(void);
   void check_for_input_drivers(void);
   int get_display_details(void);
   
   int switch_to_fullscreen(void) { };
   int switch_to_windowed(void) { };
   
   int install_all_input_devices(void);
   void unregister_input_devices(void);
   
   int run_gui(int gui_mode);
   
   
};

