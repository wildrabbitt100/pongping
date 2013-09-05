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
 
 
 
 #include "pongping_headers/pongping_essentials_headers.h"
 
 
 
 
 void PongPingEssentials::initialise_allegro(void)
 {
    al_init();
	
	if( al_init_primitives_addon() == false)
	{
	   logfile << "Couldn't initialise primitives addon.\n";
	   return;
	}
	  
    if( al_init_image_addon() == false )
	{
	   logfile << "Couldn't initialise image addon.\n";
	   return;
	}
	  
 
    if(al_install_audio() == false)
    {
       logfile << "Couldn't init audio.\n\n";
	   return;
    }
   
    if(al_init_acodec_addon() == false)
	{
	   logfile << "Couldn't initialise audio codec add on.\n";
	   return;
	}
 
 }
 
 
 
 
 void PongPingEssentials::check_for_input_drivers(void)
 {
    if( al_install_mouse() == true )
	{
	   mouse_driver_available_flag = PONGPING_FLAG_UP;
	   al_uninstall_mouse();
	}
	else
	{
	   LOG("No mouse driver available.\n");
	}
	
	if( al_install_keyboard() == true )
	{
	   keyboard_driver_available_flag = PONGPING_FLAG_UP;
	   al_uninstall_keyboard();
	   
	}
	else
	{
	  
	    LOG("No keyboard driver available.\n");
	   
	}
	
    if(al_install_joystick() == true)
	{
	   joystick_or_joypad_driver_available_flag = PONGPING_FLAG_UP;
	   al_uninstall_joystick();
	}
    else
	{
	   LOG("No joystick driver avaiable.\n");
	}
 }
 
 
 
 int PongPingEssentials::process_configuration_file(void)
 {
    ALLEGRO_CONFIG *cfg = al_load_config_file("game_data/config_file.cfg");
    const char *string_from_config = NULL;
    int config_int;
	
	int control_type_found = 0;
	
	if(cfg == NULL)
    {
       goto abort;
    }
    
    if(get_int_from_config_file(cfg, 0, "use_log", &config_int) != 0 )
    {
	   goto abort;
	}
	else
	{
	   use_log = config_int;	
	}
	
	if( get_int_from_config_file(cfg, 0, "start_fullscreen", &config_int) != 0 )
    {
	   
       LOG("couldn't get start_fullscreen value from configuration file.\n");
		
       goto abort;
    }
	else if( config_int == 1 )
	{
	   LOG("Config file says start fullscreen : Setting run_fullscreen flag.\n");
	   run_fullscreen_flag = PONGPING_FLAG_UP;
	}
   
    #ifdef ENABLE_LEVEL_TO_START_AT_VARIABLE
    if(get_int_from_config_file(cfg, 0, "level_to_start_at", &config_int) != 0)
    {
	   
       LOG("Couldn't get number for which level to start at. (level_to_start_at) variable from config file.\n");
	     
       goto abort;
    }
    else
    {
	   if( config_int > PONGPING_MAX_LEVELS )
	   {
	    
		  LOG("Start level specified in configuration file too high.\n");
		  goto abort;
		  
	   }
	   else
	   {
	      LOG("Using start level specified in configuration file.\n");
		  start_level = config_int;
	   } 
    }
	#endif
    
	
	
   string_from_config = al_get_config_value(cfg, 0, "default_control_type");
   
   if( strcmp(string_from_config, "USE_JOYSTICK_OR_JOYPAD") == 0) /* if the string is USE_JOYSTICK_OR_JOYPAD */
   {
	
	   if(joystick_or_joypad_driver_available_flag == PONGPING_FLAG_UP)
	   {
		  if((number_of_joysticks = al_get_num_joysticks()) == 0)
		  {
		     LOG("No joysticks or joypads installed. Looking for alternative input device.\n");
		  }
		  else
		  {
		     LOG("Found a joystick.\n");
		    
			 current_control_type = PONGPING_USING_JOYPAD_OR_JOYSTICK;
			 control_type_found = 1;
		  }
	   }
   }
   
   if(control_type_found == 0 && strcmp(string_from_config, "USE_KEYBOARD") == 0 )
   {
	   if( keyboard_driver_available_flag == PONGPING_FLAG_UP && al_is_keyboard_installed() == true)
	   {
		   current_control_type = PONGPING_USING_KEYBOARD;
           control_type_found = 1;
	   }
   }
   
   if(control_type_found == 0 && strcmp(string_from_config, "USE_MOUSE") == 0)
   {
      if(mouse_driver_available_flag == PONGPING_FLAG_UP)
	  {
		 current_control_type = PONGPING_USING_MOUSE;
	     control_type_found = 1;
	  }
   }
 

   return 0;

   abort:

   al_destroy_config(cfg);   
 
   return 1;
}
 
 

 
int PongPingEssentials::get_display_details()
{
   vector <int> possible_display_modes(0);
   
   ALLEGRO_MONITOR_INFO monitor;
   ALLEGRO_DISPLAY_MODE mode;
   int number_of_display_modes = al_get_num_display_modes();
   int dw, dh;
   float minimum_w, minimum_h;
   int dmi = 0; /* Display mode index. */
   int index_to_use = -1;
   int window_found = 0;
   
   minimum_h = (float) dh * 0.5 ;
  
   int smallest_difference = dh;
   
   //logfile << "al_get_num_display_modes() = " << al_get_num_display_modes() << "\n";
   
   PPLOG(GETTING_DISPLAY_DETAILS);
   
   int pf; /* pixel format */
    
   int i;
	  
   for( i = 0; i < NUM_DESIRED_PIXEL_FORMATS; ++i)
   {
	   pf = pixel_formats[i];
		 
	   
	   LOG("*************************************************************************\n\nChecking display modes with pixel format : ");
		 
	   switch(i)
	   {
		  case 0:
			   LOG("ALLEGRO_PIXEL_FORMAT_RGBA_8888\n");
			   break;
		  case 1:
			   LOG("ALLEGRO_PIXEL_FORMAT_ARGB_8888\n");
			   break;
		  case 2:
			   LOG("ALLEGRO_PIXEL_FORMAT_ABGR_8888\n");
			   break;
		  case 3:
			   LOG("ALLEGRO_PIXEL_FORMAT_XBGR_8888\n");
			   break;
		  case 4:
			   LOG("ALLEGRO_PIXEL_FORMAT_RGBX_8888\n");
			   break;
		  case 5:
			   LOG("ALLEGRO_PIXEL_FORMAT_XRGB_8888\n");		   
			   break;
		 
	   }
		 
		 
	  
       dmi = 0;
		 
   
       while( dmi < al_get_num_display_modes())
       {
          if(al_get_display_mode( dmi, &mode) == NULL)
	      {
	         return 1;
	      }
            
	      if( mode.format == pf )
		  {
			  possible_display_modes.push_back(dmi);
		  }
			
		  ++dmi;
	   }
		 
	   smallest_difference = dh;
		 index_to_use = -1;
		 
		 for(dmi = 0; dmi < possible_display_modes.size(); ++dmi)
		 {
		    al_get_display_mode( possible_display_modes[dmi], &mode);
			
			
		    LOG("mode height = ");
		    LOGINT(mode.height);
		    LOG("\n");
	        LOG("mode format = ");
	        LOGINT(mode.format);
	        LOG("\n");
		   
			
			if(mode.height == screen_res_height && mode.width == screen_res_width)
			{
			   found_fullscreen_mode_flag = PONGPING_FLAG_UP;
			}
		 
		    if(mode.height == PONGPING_CANVAS_HEIGHT && mode.width == PONGPING_CANVAS_WIDTH)
			{
			
			   LOG("found desird width and height.\n");
			   
			   index_to_use = possible_display_modes[dmi];
			   logfile << "found desired window size (1024 * 178) \n";
			   break;
			}
			
			if( mode.height >= minimum_h && mode.height < dh)
	        {
	           if( dh - mode.height < smallest_difference )
		       {
		          logfile << "found window with height = " << mode.height << "and width = " << mode.width << "\n\n";
				  
	              smallest_difference = dh - mode.height;
			    
				  index_to_use = possible_display_modes[dmi];
		       }
	        }
		 
		 }
		 
		 
		 
		 if( index_to_use != -1)
         {
		    LOG("\nSetting the best window size found for this pixel format :-\n\n");
			
		    window_found = 1;
		 
            if(al_get_display_mode( index_to_use, &mode) == NULL)
	        {
	           return 1;
	        }
            else
	        {
			   window_pixel_format = pf;
	           window_width = mode.width;
		       window_height = mode.height;
			   
			   LOG("height = ");
			   LOGINT(mode.height);
			   LOG(" width = ");
			   LOGINT(mode.width);
			   
	        }
	  
   
         }
		 else
		 {
		    if(use_log)
			{
		       logfile << "\nno suitable window found for this pixel format.\n\n";
			}
		 }
	   
      }
   
     
      
   if(window_found == 0 && found_fullscreen_mode_flag == PONGPING_FLAG_DOWN)
   {
      if(use_log)
	  {
	     logfile << "Couldn't find a suitable window size or fullscreen mode.\n";
	  }
	  return 1;
   }
   
   
   PPLOG(FINISHED_GETTING_DISPLAY_DETAILS);
   
   return 0;
}





int PongPingEssentials::install_all_input_devices(void)
{
   if( keyboard_driver_available_flag == PONGPING_FLAG_UP && keyboard_installed_flag == PONGPING_FLAG_DOWN)
   {
       if(al_install_keyboard() == true)
	   {
	      keyboard_installed_flag = PONGPING_FLAG_UP;
	      al_register_event_source( queue, al_get_keyboard_event_source() );
	   }
   }

   if( mouse_driver_available_flag == PONGPING_FLAG_UP && mouse_installed_flag == PONGPING_FLAG_DOWN)
   {
      if(al_install_mouse() == true)
	  {
	     mouse_installed_flag = PONGPING_FLAG_UP;
	     al_register_event_source( queue, al_get_mouse_event_source() );
	  }
   }
   
   if( joystick_or_joypad_driver_available_flag == PONGPING_FLAG_UP && joystick_or_joypad_driver_installed_flag == PONGPING_FLAG_DOWN)
   {
      if(al_install_joystick() == true)
	  {
	     joystick_or_joypad_driver_installed_flag == PONGPING_FLAG_UP;
	     al_register_event_source( queue, al_get_joystick_event_source() );
	  }
   
   }
}
 
 
void PongPingEssentials::unregister_input_devices(void)
{












}
