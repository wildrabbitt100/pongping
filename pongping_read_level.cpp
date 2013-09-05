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
 
 
#include "pongping_headers/pongping_read_level_headers.h"


/* read_level() : read a level 
                  return 0 for success, 1 for error. */

int OneGame::read_level()
{ 
    const char *file_name = "game_data/levels_file.xml";
    
    TiXmlDocument level_file( file_name );
    
    TiXmlHandle hDoc(&level_file);
    
    TiXmlHandle hLevel(0), hLevelDefinition(0), hPongPingPolygon(0);
    
    TiXmlElement *pLevelDefinitionElement, *pElement, *pLevelElement, *pColorElement;
    
    int level_number;
    
    const char *s;
    
    int i1, i2, i3, i4, i5, i6, i7, i8, i9, i10;
    
    double f1, f2, f3;
    
    
    COULEUR colour;
    
    vector<COULEUR> colour_vector(0);
    
    int r, g, b;
    

    /*********************************************************************************/
    
    #ifdef USE_LOG_FILE
    PPLOG(READ_LEVEL_READING_LEVEL); // say that level is being read
    #endif
    
    if(level_file.LoadFile() == false)
    {
       logfile << "Couldn't load " << file_name << endl << level_file.ErrorDesc() << endl; 
       
       return 1;
    }
    
    #ifdef USE_LOG_FILE
    PPLOG(READ_LEVEL_LOADED_XML_FILE);
    #endif
    
    pLevelDefinitionElement = hDoc.FirstChild("level_definition_for_willam_labbetts_awesome_new_game" ).Element();
    
    if(pLevelDefinitionElement == NULL)
    {
       logfile << "reading xml file - couldn't find a level_definition_for_willam_labbetts_awesome_new_game element.\n";
       return 1;
    }
    
    
    
    hLevelDefinition = TiXmlHandle(pLevelDefinitionElement);
    pElement = hLevelDefinition.FirstChild("level").Element();
   
    do
    {
       if( pElement->QueryIntAttribute( "level_number", &level_number) != TIXML_SUCCESS )
       {
          logfile << "couldn't fins level_number attribute in level_number tag.\n\n";
          return 1;
       }
       
       if(level_number == current_level)
       {
          break;
       }
       else
       {
          if((pElement = pElement->NextSiblingElement()) == NULL)
          {
             logfile << "couldn't find level " << current_level << "." << endl;
             return 1;
          }
       }
       
    } while(1);

    /* Found the right level */
    
    #ifdef USE_LOG_FILE
    PPLOG(READ_LEVEL_FOUND_LEVEL);
    //PPLOGINT(
    #endif
    

    
    //logfile << "found the right level.\n" << level_number << "\n";
    
    //eek();
    
    hLevel = TiXmlHandle(pElement);

    if((pLevelElement = hLevel.FirstChild("task").Element()) == NULL || (s = pLevelElement->GetText()) == NULL)
    {
       
    
       return 1;
    }

    
    if(strlen(s) > 99) 
    {
       logfile << "task string too long." << endl;
       eek();
    }
    else
    {
       /*
       if(s != NULL)
       {
          printf(" s is not NULL.\n\n");
       }
       */
       //printf("copying string.\n\n");
       //printf("string is <%s>", s);
    
       strcpy(task, s);
       
      
    }
    
    samples.clear();
    
    if((pLevelElement = hLevel.FirstChild("song").Element()) != NULL)
    {
       if((s = pLevelElement->GetText()) == NULL)
       {
          logfile << "song element but no filename\n\n";
          eek();
       }
       else
       {
          ALLEGRO_SAMPLE *sample = al_load_sample(s);
          if(sample == NULL)
          {
             logfile << "couldn't load sample " << s << "\n\n";
             eek();
          }
          else
          {
             logfile << "loaded sample.\n\n";
             
          }
          samples.push_back(sample);
       }
    }
    
    
    //logfile << "read the task.\n";
    
    if((pLevelElement = hLevel.FirstChild("ball_position").Element()) == NULL) return 1;
    
    if( pLevelElement->QueryIntAttribute("x", &i1) != TIXML_SUCCESS || pLevelElement->QueryIntAttribute("y", &i2) != TIXML_SUCCESS)
    {
       return 1;
    }
    else
    {
       initial_ball_x = i1;
       if(i2 < BALL_ARENA_TOP_EDGE_Y)
       {
          initial_ball_y = BALL_ARENA_TOP_EDGE_Y;
       }
       else
       {
          initial_ball_y = i2;
       }
    }
    
	if((pLevelElement = hLevel.FirstChild("border_colour").Element()) == NULL)
	{
	   logfile << "no border colour specified in level file.\n";
	   return 1;
	}
	else 
	{
	    if( pLevelElement->QueryIntAttribute("r", &i1) != TIXML_SUCCESS ||
	        pLevelElement->QueryIntAttribute("g", &i2) != TIXML_SUCCESS ||
			pLevelElement->QueryIntAttribute("b", &i3) != TIXML_SUCCESS )
	    {
		   logfile << "no r, g, and / or b in border colour element.\n";
		   return 1;
		}
	    else
		{
		    border_colour = al_map_rgb(i1, i2, i3);
		}
		
	}
	
	if((pLevelElement = hLevel.FirstChild("background_colour").Element()) == NULL)
	{
	   logfile << "no backgound colour specified in level file.\n";
	   return 1;
	}
	else 
	{
	    if( pLevelElement->QueryIntAttribute("r", &i1) != TIXML_SUCCESS ||
	        pLevelElement->QueryIntAttribute("g", &i2) != TIXML_SUCCESS ||
			pLevelElement->QueryIntAttribute("b", &i3) != TIXML_SUCCESS )
	    {
		   logfile << "no r, g, and / or b in background colour element.\n";
		   return 1;
		}
	    else
		{
		    //logfile << "setting background_colour.\n";
			
			
			//logfile << "i1 = " << i1 << " i2 = " << i2 << " i3 = " << i3 << "\n";
			
			//eek();
			
		    background_colour = al_map_rgb(i1, i2, i3);
		}
		
	}
	
	if((pLevelElement = hLevel.FirstChild("ball_colour").Element()) == NULL)
	{
	   logfile << "no ball colour specified in level file.\n";
	   return 1;
	}
	else 
	{
	    if( pLevelElement->QueryIntAttribute("r", &i1) != TIXML_SUCCESS ||
	        pLevelElement->QueryIntAttribute("g", &i2) != TIXML_SUCCESS ||
			pLevelElement->QueryIntAttribute("b", &i3) != TIXML_SUCCESS )
	    {
		   logfile << "no r, g, and / or b in ball colour element.\n";
		   return 1;
		}
	    else
		{
		    //logfile << "setting background_colour.\n";
			
			
			//logfile << "i1 = " << i1 << " i2 = " << i2 << " i3 = " << i3 << "\n";
			
			//eek();
			
		    ball_colour = al_map_rgb(i1, i2, i3);
		}
		
	}
	
	
	
	
    if((pLevelElement = hLevel.FirstChild("initial_angle").Element()) == NULL)
    {
        /* no angle specified so make the ball go in the direction of the bat. */
        
        /* get the distance from initial bat x to middle of screen */
        
        i1 = ( PONGPING_CANVAS_WIDTH / 2 ) - initial_ball_x;
        i2 = - BAT_Y_POSITION;
    
        f1 = atan( (float) i2 / (float) i1 );
        principal_value(&f1);
     
        initial_ball_angle = f1;
        

    }
    else
    {
       if(pLevelElement->QueryDoubleAttribute("angle", &f1) != TIXML_SUCCESS)
       {
          return 1;
       }
       else
       {
          principal_value(&f1);
          initial_ball_angle = f1;
       }
    }
    
    /* increment for ball (initial speed) */
    
    if((pLevelElement = hLevel.FirstChild("ball_increment").Element()) == NULL)
    {
       initial_ball_increment = 5.0f;
    }
    else
    {
       if( pLevelElement->QueryDoubleAttribute("increment", &f1) != TIXML_SUCCESS)
       {
          return 1;
       }
       else
       {
          initial_ball_increment = f1;
       }
    }

    /* ball speeds */
    
    //logfile << "getting ball speeds.\n";
    
	/*
    if((pLevelElement = hLevel.FirstChild("ball_speed_change").Element()) != NULL)
    {
       BallSpeedData b;
       
       int i = 0;
       
       for( ; ; pLevelElement = pLevelElement->NextSiblingElement("ball_speed_change"))
       {
           if(pLevelElement == NULL)
           {
              break;
           }
           
           if(pLevelElement->QueryIntAttribute("time", &i1) != TIXML_SUCCESS || pLevelElement->QueryDoubleAttribute("difference", &f1) != TIXML_SUCCESS)           
           {
              logfile << "query issue with 'ball_speed_change' element.\n";
              return 1;
           }
           else
           {
              b = BallSpeedData(i1, f1);
              bsd.push_back(b);  
           }
       }
    }
    */
    
    if((pLevelElement = hLevel.FirstChild("survive_until").Element()) != NULL )
    {
       if(pLevelElement->QueryIntAttribute("time", &i1) == TIXML_SUCCESS)
       {
          end_when_time_reached = true;
          time_to_reach = i1;
       }
    }
    else
    {
       end_when_time_reached = false;
    }
    
    int cn; //collectable number
    
    
    int destructable_shaded;
    
    int rings;
    TiXmlElement *shaded;
    
	int polygon_number = 0;
	
    if((pLevelElement = hLevel.FirstChild("polygon").Element()) != NULL)
    {
       PongPingPolygon *rp = NULL;
       
       int i = 0;
       
       int number_of_colors;
       
       int number_of_layers = 0;
       
       rings = destructable_shaded = 0;
       
       
       
	  
	   
       //logfile << "found another polygon.\n\n";
       
       for( ; ; pLevelElement = pLevelElement->NextSiblingElement("polygon"))
       {
	       ++polygon_number;
		   
           //logfile << "checking if pLevelElement NULL.\n\n";
           
		   i8 = 0; // must reset i8 for this polygon because it might have a value left over from last run of this loop
		   
           if(pLevelElement == NULL)
           {
              logfile << "breaking.\n\n";
              break;
           }
           
           //logfile << "got level feature.\n\n";
           
           if(   pLevelElement->QueryIntAttribute("x", &i1) != TIXML_SUCCESS 
              || pLevelElement->QueryIntAttribute("y", &i2) != TIXML_SUCCESS
              || pLevelElement->QueryIntAttribute("sides", &i3) != TIXML_SUCCESS              
              || pLevelElement->QueryIntAttribute("direction", &i4) != TIXML_SUCCESS
              || pLevelElement->QueryDoubleAttribute("increment_angle", &f2) != TIXML_SUCCESS
              || pLevelElement->QueryDoubleAttribute("initial_angle", &f3) != TIXML_SUCCESS
              || pLevelElement->QueryIntAttribute("layers", &number_of_layers) != TIXML_SUCCESS
                          )
           {
              logfile << "query issue with 'regular_polygon' element. one of x, y, sides, direction, increment_anglem initial_angle or layers missing.\n";
              return 1;
           }
           else
           {
              s = pLevelElement->Attribute("regular");
              if(s == NULL)
              {
                 logfile << "no regular attribute in polygon tag\n\n";
                 return 1;
              }
              
              //logfile << "got string for regular. \n\n";
           
              if(strcmp( s, "yes") == 0)
              {
                 //logfile << "regular.\n\n";
                 
                 
                 i8 |= REGULAR;
                 
                 if(pLevelElement->QueryIntAttribute("sides", &i3) != TIXML_SUCCESS)
                 {
                    logfile << "query issue with 'regular_polygon' type polygon. No sides attribute. Polygon number :- " << polygon_number << ">.\n";
                    return 1;
                 }
				 
				 if(pLevelElement->QueryDoubleAttribute("width", &f1) != TIXML_SUCCESS)
                 {
                    logfile << "query issue with 'regular_polygon' type polygon. No width attribute. Polygon number :- " << polygon_number << ">.\n";
                    return 1;
                 }
                 
                 
              }
              else if(strcmp( s, "no") == 0)
              {
                 i8 |= IRREGULAR;
              }
              else
              {
                 logfile << "value of 'regular' attribute not recognised. It was " << s << "\n\n";
                 return 1;
              }
           
              //logfile << "worked out regularness.\n\n";
              
              hPongPingPolygon = TiXmlHandle( pLevelElement );
              /*
              if(hPongPingPolygon == NULL)
              {
                  logfile << "handle NULL in read_level() .\n\n";
                  return 1;
              
              
              }
              */
              
			  //logfile << "i8 = " << i8 << " before type read\n";
              
              s = pLevelElement->Attribute("type");
              if(s == NULL)
              {
                 logfile << "no type for polygon.\n";
                 return 1;
              }
              else
              {
               
                 if(strcmp( s, "one_coloured") == 0)
                 {
                    i8 |= ONE_COLOURED_POLYGON;
                 }
                 else if(strcmp( s, "shaded") == 0)
                 {
                    i8 |= SHADED_POLYGON;
                 }
                 else if(strcmp( s, "destructable") == 0)
                 {
                    i8 |= PONGPING_VANISHING_POLYGON;
                    
                    //logfile << "got a DESTRUCTABLE polygon.\n";
                    
                 }
                 else if(strcmp( s, "reducable_polygon") == 0)
                 {
				    logfile << "or ing with PONGPING_HIT_X_TIMES_POLYGON.\n";
                    i8 |= PONGPING_HIT_X_TIMES_POLYGON;
                 }
                 else
                 {
                    logfile << "unrecognised type of polygon in level file.\n";
                    return 1;
                 }
                
              }
              
			  //logfile << "i8 = " << i8 << " after type read\n";
			  
              //logfile << "got type of polygon.\n\n";
              
              
              
              
            
              rp = new PongPingPolygon(  i1, i2, i3,    i8 & REGULAR ? f1 : 0,              f2,        i4, f3, i8);
              
              if(rp == NULL)
              {
                 logfile << "rp polygon is NULL.\n\n";
                 return 1;
              }
              
              //logfile << "got pointer to new polygon.\n\n";
           
              if(i8 & IRREGULAR) //get the vertices
              {
                 TiXmlElement *pVertexElement;
                 
                 //logfile << "irregular - getting vertex element.\n\n";
                 
                 if( (pVertexElement = hPongPingPolygon.FirstChild("vertex").Element()) != NULL)
                 {
                    logfile << "got vertex.\n\n";
                 
                    int x, y, vn;
                    
                    vn = 0;
                    rp->furthest_vertex = 0;
                    
					int vc = 0;
					
                    for( ; ; pVertexElement = pVertexElement->NextSiblingElement( "vertex" ))
                    {
					   ++vc;
                       logfile << " vc = " << vc << "\n";
                       
                       double furthest_vertex = 0.0;
                       
                       if(pVertexElement == NULL) break;
                       
                       if( (pVertexElement->QueryIntAttribute("x", &x) != TIXML_SUCCESS ) || (pVertexElement->QueryIntAttribute("y", &y) != TIXML_SUCCESS ) )
                       {
                          logfile << "no x and y elements for vertex of polygon.\n\n";
                          return 1;
                       }
                       else
                       {
                          
                       
                          logfile << "doing vertex " << vc << " x = " << x << " y = " << y << "\n\n";
                       
                          polar_coordinates new_point;// = new Point [1];
                          
                          new_point.radius = sqrt( (x - i1) * (x - i1) + (y - i2) * (y - i2) );
                          
						  logfile << "radius = " << (x - i1) * (x - i1) << " * " << (y - i2) * (y - i2) << " = " << new_point.radius << "\n"; 
                          
                          
                          if(new_point.radius > rp->furthest_vertex)
                          {
                             //logfile << new_point.radius << "was greater than " << rp->furthest_vertex << "\n";
                             //logfile << "changing furthest vertex to " << new_point.radius << "\n";
                             rp->furthest_vertex = new_point.radius;
                          }
                          
                          if( x == i1 ) // line from centre to vertex is vertical
                          {
                             if( y - i2 > 0.0 )
                             {
                                new_point.angle = PONGPING_PI/ 2.0;
                             }
                             else if( y - i2 < 0.0 )
                             {
                                new_point.angle = 3.0 * PONGPING_PI/ 2.0;
                             }
							 
							 logfile << "new_point.angle = " << (new_point.angle * 180.0f / PONGPING_PI)<< "\n";
                          }
                          else
                          {
                             new_point.angle = principal_value_2(atan2( (y - i2) , (x - i1) ));
                             
							 logfile << "atan(" << (y - i2) << " , " << (x - i1) << "\n";
							 
							 /*
                             if(new_point.angle < 0.0)
                             {
                                new_point.angle += ALLEGRO_PI;
                             }
							 */
							 
							 logfile << "new_point.angle = " << (new_point.angle * 180.0f / PONGPING_PI)<< "\n";
                          }
                          
                          //logfile << "pushing point (radius) " << new_point.radius << " angle " << new_point.angle << "\n\n";
                          //
						  
						  logfile << "pushing point.\n";
                          rp->unrotated_points.push_back(new_point);
                       
                       }
                       
                       logfile << "finished.\n\n";
                    }
					
					rp->update_irregular_polygon();
                    
                    //logfile << "out of for loop.\n\n";
           
                 }
                 else
                 { 
                    logfile << "no vertex elements for irregular polygon.\n\n";
                    return 1;
                 }
              }
              else
              {
              
                 rp->furthest_vertex = f1;
                 
                 
                 logfile << "polygon  regular.\n\n";
              }
              
              /* Get the colors for the polygon. */

              number_of_colors = 0;
              
              hPongPingPolygon = TiXmlHandle(pLevelElement);
              
			  colour_vector.clear();
			  
              if( (pColorElement = hPongPingPolygon.FirstChild("color").Element()) != NULL)
              {
                 for( ; ; pColorElement = pColorElement->NextSiblingElement("color") )
                 {
                    
                    
                    
                    if( pColorElement == NULL) 
                    {
                       break;
                    }
                    /*
                    else if(i8 & REDUCABLE_POLYGON && number_of_colors == 1)
                    {
                       logfile << "too many colours for one_coloured POLYGON.\n\n";
                       return 1;
                    }
                    else if(i8 & PONGPING_VANSHING_POLYGON && number_of_colors == 1)                     {
                       logfile << "too many colours for one_coloured POLYGON.\n\n";
                       return 1;
                    }
                    */
                    else
                    {
                       //logfile << "got a color.\n\n" << "rings = " << rings;
                       ++number_of_colors;
                       if( pColorElement->QueryIntAttribute("r", &r) != TIXML_SUCCESS ||
                           pColorElement->QueryIntAttribute("g", &g) != TIXML_SUCCESS ||
                           pColorElement->QueryIntAttribute("b", &b) != TIXML_SUCCESS )
                       {
                           logfile << "trouble reading r, g or b from color element for polygon.\n";
                           return 1;
                       }
                       else
                       {
                          colour_vector.push_back(al_map_rgb(r, g, b));
                          /*
                          if(i8 & REDUCABLE_POLYGON)
                          {
                             int i;
                             int r2, g2, b2;
                             for(i = 0; i < NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES; ++i)
                             {
                                r2 = (float) r * ( (float) i / (float) (NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES - 1) );
                                g2 = (float) g * ( (float) i / (float) (NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES - 1));
                                b2 = (float) b * ( (float) i / (float) (NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES - 1));
                                colour = al_map_rgb( r2, g2, b2);
                             
                                rp->polygon_colours.push_back(colour);
                                
                                //logfile << "r = " << r2 << " g = " << g2 << " b = " << b2 << "\n\n";
                             }
                             
                             //logfile << "pushed all colours.\n\n";
                             
                          
                          }
           
                          else
                          {
                             colour = al_map_rgb(r, g, b);
                             
                             //logfile << "pushing colour.\n";
                          
                             rp->polygon_colours.push_back(colour);
                          
                             //logfile << "pushed colour.\n";
                          }
                          */
                       }
                    }
                  }
                  
                  /*
                  if(rp->polygon_colours.size() == 0)
                  {
                     logfile << "read_level : no colours for polygon. Will cause serious issues.\n";
                     
                  
                  }
                  //logfile << "setting active_layers to " << number_of_colors << "\n";
                  
                  if(i8 & REDUCABLE_POLYGON)
                  {
                     rp->active_layers = NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES - 1;
                     //logfile << "set active_layers to " << rp->active_layers << "\n\n"; 
                  }
                  else if(i8 & PONGPING_VANSHING_POLYGON && shaded != NULL)
                  {
                     rp->active_layers = rings;
                  }
                  else
                  {
                     rp->active_layers = number_of_colors;
                  }
                  */
                  
                  
                    
              }
              else
              {
                 logfile << "no color element for polygon.\n";
                 return 1;
              }
              
			  //logfile << "colours counted = " << number_of_colors << "\n colour_vector.size() = " << colour_vector.size() << "\n";
			  
              if(i8 & PONGPING_VANISHING_POLYGON || i8 & PONGPING_HIT_X_TIMES_POLYGON )
              {
                 if(number_of_layers <= 0)
                 {
                    logfile << "number of layers specified for destructable type polygon less than or equal to zero.\n"; 
                    return 1;
                 }
              
                 if(colour_vector.size() == 1 && number_of_layers > 1) //  make the colours from the one colour specified.
                 {
                    int i;
                    int r2, g2, b2;
					
					unsigned char cr, cg, cb;
					
					double fraction;
					
					if( i8 & PONGPING_VANISHING_POLYGON )
					{
					   fraction *= 0.75;
                       fraction += 0.25;					   
					}
					
					al_unmap_rgb(colour_vector[0], &cr, &cg, &cb);
					
                    for(i = 0; i < number_of_layers; ++i)
                    {
					    double fraction = ( (float) i / (float) (number_of_layers - 1));
					
					    if( i8 & PONGPING_VANISHING_POLYGON )
					    {
					       fraction *= 0.75;
                           fraction += 0.25;					   
					    }
						
                        r2 = (float) cr * fraction;
                        g2 = (float) cg * fraction;
                        b2 = (float) cb * fraction;
                        colour = al_map_rgb( r2, g2, b2);
                             
                        rp->polygon_colours.push_back(colour);
                                
                        //logfile << "r = " << r2 << " g = " << g2 << " b = " << b2 << "\n\n";
                        //logfile << "rings = " << rings;  
                    }
                 }
                 else if(colour_vector.size() == number_of_layers)
                 {
                    int i;
               
                    for(i = 0; i < number_of_layers; ++i)
                    {
                        rp->polygon_colours.push_back(colour_vector[i]);
                                
                    }
					
					
                 
                 }
                 else if(colour_vector.size() != number_of_layers)
                 {
                    logfile << "colour_vector.size() greater than one but not equal to number of layers specified. colour_vector.size() = \n" << colour_vector.size();
                    return 1;
                 }
				 
				 rp->active_layers = ( i8 & PONGPING_VANISHING_POLYGON ) ? number_of_layers : number_of_layers - 1;
				 //logfile << "rp->active_layers = " << rp->active_layers << "\n";
              }
			  else if(i8 & ONE_COLOURED_POLYGON)
			  {
			     if(number_of_colors != 1) 
				 {
				    logfile << "more than on colour specified for one coloured polygon.\n";
					return 1;
			     }
				 else
				 {
				    rp->polygon_colours.push_back(colour_vector[0]);
				 }
			  }
			  
			 
              
              
              //logfile << "pushing LevelFeature onto vector.\n";
              
              features_for_level.push_back((LevelFeature *) rp);
             

              //logfile << "pushed LevelFeature.\n";           
           }
           
           
           
           
           
           
       }
    }
    
    
    if((pLevelElement = hLevel.FirstChild("catchment_area").Element()) != NULL)
    {
       for( ; ; pLevelElement = pLevelElement->NextSiblingElement("catchment_area"))
       {
           if(pLevelElement == NULL) // no more catchment areas
           {
              logfile << "breaking.\n\n";
              break;
           }
          
         CatchmentArea *ca = NULL;

         if( (pLevelElement->QueryIntAttribute( "x", &i1) != TIXML_SUCCESS) ||
             (pLevelElement->QueryIntAttribute( "y", &i2) != TIXML_SUCCESS) ||
             (pLevelElement->QueryIntAttribute( "w", &i3) != TIXML_SUCCESS) ||
             (pLevelElement->QueryIntAttribute( "h", &i4) != TIXML_SUCCESS) ||
             (pLevelElement->QueryIntAttribute( "bitmap_number", &i5) != TIXML_SUCCESS) )
         {
            logfile << "couldn't read one of the integer attributes of the catchment_area element.\n";
            return 1;
         }
         else
         {
            ca = new CatchmentArea( i1, i2, i3, i4, i5 );
            
            if(ca->init_okay == 0)
            {
               logfile << "problem initialising catchment area.\n";
               return 1;
            }
            
            features_for_level.push_back( (LevelFeature *) ca );
         }
      }
    
    }
    
    
    
    //logfile << "getting quota variables.\n";
    
    if((pLevelElement = hLevel.FirstChild("quotas").Element()) != NULL)
    {
       int number = 0;
       
       int total_number = 0;
       
       logfile << "got quotas element.\n";
       
       if( pLevelElement->QueryIntAttribute("letter_quota", &number) == TIXML_SUCCESS )
       {
          collectable_type_quotas[PONGPING_EXTRA_LIFE_LETTER_CIRCLE_TYPE - 1] = number;
          
          total_number += number;
         
       }
       else
       {
          logfile << "no value for letter_quota in quotas element.\n";
          return 1;
       }
       
       if( pLevelElement->QueryIntAttribute("bat_size_up_quota", &number) == TIXML_SUCCESS )
       {
          collectable_type_quotas[PONGPING_BAT_SIZE_UP_CIRCLE_TYPE - 1] = number;
          
          total_number += number;
       }
       else
       {
          logfile << "no value for bat_size_up_quota in quotas element.\n";
          return 1;
       }
       
       if( pLevelElement->QueryIntAttribute("ball_speed_up_quota", &number) == TIXML_SUCCESS )
       {
          collectable_type_quotas[PONGPING_BALL_SIZE_UP_CIRCLE_TYPE - 1] = number;
          
          total_number += number;
       }
       else
       {
          logfile << "no value for ball_speed_up_quota in quotas element.\n";
          return 1;
       }
       
       if( pLevelElement->QueryIntAttribute("ball_speed_down_quota", &number) == TIXML_SUCCESS )
       {
          collectable_type_quotas[PONGPING_BALL_SPEED_DOWN_CIRCLE_TYPE - 1] = number;
          
          total_number += number;
       }
       else
       {
          logfile << "no value for ball_speed_down_quota in quotas element.\n";
          return 1;
       }
       
       collectable_quota = total_number;
    }
    else
    {
       logfile << "reading level : no quota element needed for how many of different types of collectables there are.\n";
       return 1;
    }
    
    
    
    
    
    
    //#ifdef USE_LOG_FILE
    //PPLOG(READ_LEVEL_FINISHED_READING_LEVEL);
    //#endif
    
    //logfile << "finished reading level.\n\n";
    
  
    return 0;
}
