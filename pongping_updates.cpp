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


 
#include "pongping_headers/pongping_updates_headers.h"
 
 
/* need to free the memory used for the potential moves for the circle to circle collisions. */


void erase_circle(vector<Circle *>& circles, int circle_number);



static int get_collisions(int *one_or_more, vector<LevelFeature *>& features_for_level, vector <Circle*>& circles, vector <PotentialMove *>& pms, Bat& TheBat, double ftr);

//static inline void mark_circle_as_moved( int32_t *circle_list, int cn );



int OneGame::update_objects()
{
   int cn1, cn2; // circle number
   
   int cn3;
   
   int Lfn; // level feature number
   
   vector <PotentialMove *> moves;
   
   int nc; // number of collisions
   
   double ftr = 1.0; /* fraction of time remaining */
   
   double fraction_of_time_used_up;
   
   double tupf;
   
   /* time_reciprocal - this is used for ... */
   
   double time_reciprocal = 0.0;
   
   int feature_number;
   
   /* Reset the remaining distance for each circle. It should be 0 from the last time it was moved (unless the circle hasn't been
      moved since the level started).   */
             
   for(cn1 = 0; cn1 < circle_vector.size(); ++cn1)
   {
      circle_vector[cn1]->reset_remaining_distance();
   }
             
   
   
   //logfile << " reset remaining distances.\n";
         
   /* Go through the circle vector and see if any circles have been collected by a catchment area and if so
      either update it (the animation of it exploding), or if it's time to delete it, remove it from
	  the array. */
   
   
   for(cn1 = 0; cn1 < circle_vector.size(); ++cn1)
   {
       if(circle_vector[cn1]->phase == PONGPING_CIRCLE_COLLECTED_PHASE)
       {
           circle_vector[cn1]->update_dying_circle();       
       }
                
       if(circle_vector[cn1]->phase == PONGPING_CIRCLE_DELETE_PHASE)
       {         
           erase_circle(circle_vector, cn1);         
       }
   }


   //logfile << "point 2.\n";
   
   
   
   while( ftr > 0.0 )
   {
   
      
	  /* Need to clear away all the PotentialMoves from the last look at things. */
	  
	  if(moves.size() != 0)
      {
	     for(cn1 = 0; cn1 < moves.size(); ++cn1)
         {
            delete moves[cn1];
         }
	  }
	  
	  //logfile << "point 3\n";
	  
	  moves.clear();
   
   
      /* circles_inside_polygon is a member of the OneGame class (which is a singleton) - it's a record of whether or not
	     any circles are inside a polygon */
   
   
      if(circles_inside_polygon)
      {
         for(cn1 = 0; cn1 < circle_vector.size(); ++cn1)
         {
		     
			 /* is circle_vector[cn]->circle_is_collectable() needed : */
			 
		 
             if( circle_vector[cn1]->circle_is_collectable() && circle_vector[cn1]->inside_polygon )
             {  
                for(Lfn = 0; Lfn < features_for_level.size(); ++Lfn)
                {
                   if( Lfn == ((FallingCollectable *) circle_vector[cn1])->polygon_id_it_came_from )
                   {
                      break;
                   }

                }  
                                        
                #ifdef PONGPING_DEBUG_BUILD
                if(Lfn == features_for_level.size())
                {
                    logfile << "can't find polygon id, collectable circle came from.\n";
                    eek();
                }
                #endif                   
  
                //logfile << "checking circle " << cn1 << "is clear. Lfn = " << Lfn << "\n";
                      
                if(circle_vector[cn1]->check_circle_is_clear_of_polygon( (PongPingPolygon *) features_for_level[Lfn]))
                {
                    circle_vector[cn1]->inside_polygon = 0;
                    --circles_inside_polygon;
                }
                         
              }
           }
                
       } // if(circles_inside_polygon
	   
	   //logfile << "point 4\n";
	   
	   nc = 0;
	   
	   //logfile << "first nc = " << nc << "\n";
	   
	   //logfile << "************************************************************************************************************ Calling get_collisions.\n";
	   
	   get_collisions(&nc, features_for_level, circle_vector, moves, TheBat, ftr);
	   
	   logfile << "got all collisions.\n";
	   
	   //logfile << "************************************************************************************************************ Got collisions.\n";
	      
	   if( nc != 0 ) /* At least one of the circles hits something else. */
	   {
	      logfile << "nc = " << nc << "\n";
	   
	      //logfile << "got circle collisions.\n";
	   
	      
		  sort( moves.begin(), moves.end(), compare_distance );
	   
	      //logfile << "sorted moves.\n";
		
		  //logfile << moves[0] << "\n";
		  
		  //logfile << "cn3 = " << cn3 << "\n";
		  //logfile << "hit_type = " << moves[0]->hit_type << "\n";
		  
		  //logfile << "applying move for circle " << moves[0]->circle_id_1 << "\n\n";
		  
		  logfile << "applying move for circle " << moves[0]->circle_id_1 << " ftr for which is " << moves[0]->pfot << "\n\n";
		  
		  circle_vector[moves[0]->circle_id_1]->apply_move( moves[0], 1 ) ;
         
		  if(moves[0]->number_of_circles == 2)
		  {
		     circle_vector[moves[0]->circle_id_2]->apply_move( moves[0], 2 ) ;
			
		  }
		  
		  //logfile << "applied move.\n";
		  
          ftr -= moves[0]->pfot;
		  
		  logfile << "ftr = " << ftr << "\n";
		  
          /* Move all the other circles by however far they travel in moves[0]->pfot */
	  
          for(cn1 = 0; cn1 < circle_vector.size(); ++cn1)
		  {
		     if( cn1 == moves[0]->circle_id_1 || (moves[0]->number_of_circles == 2 && cn1 == moves[0]->circle_id_2) )
			 {
			    /* Don't want to move the circle (or circles) that's already been moved again. */
			    continue;
			 }
			 else
			 {
			    //logfile << "moving circle. pfot = \n";
				
				//eek();
			 
			    circle_vector[cn1]->move_circle_by( moves[0]->pfot );
				
				//logfile << "called move_circle_by() " << "\n";
			 }
			 
		  }
		  
		  for(feature_number = 0; feature_number < features_for_level.size(); ++feature_number)
          {
                
             features_for_level[feature_number]->update_by(moves[0]->pfot);
          }
		
		
	   }
	   else /* None of the circle's anything so just move everything by ftr (how much time there is remaining). */
	   {
	      logfile << "no circles hit anything.\n";
	   
	      for(cn1 = 0; cn1 < circle_vector.size(); ++cn1)
		  {
			 circle_vector[cn1]->move_circle_by( ftr );
		  }
		  
		  /* Update the features and the bat. */
            
          for(feature_number = 0; feature_number < features_for_level.size(); ++feature_number)
          {
                
             features_for_level[feature_number]->update_by( ftr );
          }
		  
		  ftr = 0.0;
	   
	   }
	   
	   
	   
	   
	   
	   
	   
   }
   
   logfile << "UPDATED ALL OBJECTS.\n";
   
   //eek();

   /* The pointer in the vector <PotentialMove *> moves; will all point to memory than has been allocated so need to call delete
      for all the PotentialMove classes which were allocated. */
   

   for(cn1 = 0; cn1 < moves.size(); ++cn1)
   {
   
       delete moves[cn1];
   }
   
}









void erase_circle(vector<Circle *>& circles, int circle_number)
{
   vector<Circle*>::iterator circle_it = circles.begin();

   do
   {
      if(circle_number == 0)
      {
          circles.erase(circle_it);
          break;
      }
      else
      {
          --circle_number;
          ++circle_it;
      }
                     
   } while(1);
}







/* 

Something someone said on the allegro.cc forum :

Since you only want to modify the data within the array, you should use int32_t *circle_list. 
This will pass a copy of the memory address that the array is located at so that you can change 
the data contained in the array. The other way means you are passing a pointer to the pointer to 
the array memory. This would be useful if you needed to, for example, allocate a chunk of memory 
and return the pointer to it through one of the arguments.

*/







/** The old way I don't want to remember or explain but it needed improving. The pseudo code for the new way is :-



1. If all circles have been moved, stop. Get all the circle to circle collisions.

   Hypothetical example :  There's 4 circles on the screen.
   
   circle number    0      1      2       3 
   
   0                
   
   1                *
   
   2                *      *
   
   3                *      *      *
   
   The stars show all the collisions which need to checked.



2. If there are any, identify the shortest one. This needs to be done because.......

3. Check to see if one or both of the circles hit anything else first before they hypothetically would
   have hit eachother.
   
4. If they do - 
      if just one hits an object first - move it up to where the object is.
	  

	  
*/





static int get_collisions(int *one_or_more, vector<LevelFeature *>& features_for_level, vector <Circle*>& circles, vector <PotentialMove *>& pms, Bat& TheBat, double ftr)
{
   int fn; // feature number
   PotentialMove *pm = new PotentialMove();
   int polygon_id;
   
   int side_not_to_check;
   
   int hit_polygon_last_time;
   
   int vertex_not_to_check;
   
   //logfile << "checking collision against bat.\n\n";
   
   int cn1 = 0;   /* circle number : used to iterate through circle pointers in (vector <Circle*> circles) */   
   
   
   int cn2;
   
   
   for( ; cn1 < circles.size(); ++cn1)
   {
      
	  //logfile << "\n circle number  " << cn1 << "\n\n";
	  
	  //logfile << "doing circle " << cn1 << "last circle it hit was " << circles[cn1]->circle_hit << "\n";
	  
	  /*
      if( circle->hit_type != PONGPING_BAT_COLLISION && TheBat.circle_near(circle) )
      {
         if( circle->check_collision_with_bat( TheBat, &pm, ftr) )
         {
            ++(*one_or_more); 

            pm->set_circle_one_number(cn1);
			
            pms.push_back(pm);
	        pm = new PotentialMove();
			
			
			
         }
      }
	  
      */
	  
      //logfile << "checking collision with feature.\n\n";
   
      
	  /* See if this circles hits any of the other circles. */
	  
	  
	  
	  for( cn2 = cn1 + 1; cn2 < circles.size(); ++ cn2)
	  {
	     //logfile << "calling it.\n";
		 
		 if( circles[cn1]->hit_type == PONGPING_CIRCLE_TO_CIRCLE_COLLISION && circles[cn1]->circle_hit == cn2 )
		 {
		    continue;
		 }
		 else if( circles[cn2]->hit_type == PONGPING_CIRCLE_TO_CIRCLE_COLLISION && circles[cn2]->circle_hit == cn1 )
		 {
		    continue;
		 }
		 
		 //logfile << "checking collision between circle " << cn1 << " and circle " << cn2 << "\n\n";
		 
	     if( check_two_circles_collide( circles[cn1], circles[cn2], pm, ftr) )
		 {
		    pm->set_circle_numbers(cn1, cn2);
		    
			pms.push_back(pm);
            
			pm = new PotentialMove();
			
		    ++(*one_or_more);
		 }
	  }
	  
	  
   
   
      for(fn = 0; fn < features_for_level.size(); ++fn)
      {
      
      
          if( features_for_level[fn]->feature_type == PONGPING_POLYGON_FEATURE )
	      {
	         polygon_id = ((PongPingPolygon *) features_for_level[fn])->polygon_id;
		 
		     if(polygon_id != 0 && polygon_id == circles[cn1]->id_of_last_polygon_hit)
		     {
		        //logfile << "setting vertex not to check : " << circle->vertex_hit << "\n";
		        side_not_to_check = circles[cn1]->side_hit;
			    vertex_not_to_check = circles[cn1]->vertex_hit;
			    hit_polygon_last_time = 1;
		     }
		     else
		     {
		        hit_polygon_last_time = 0;
		     }
		 
	      }
	  
	  /*
      if(fn != 0 && fn == circle->last_feature_hit)
      {
         logfile << "not checking polygon with id = " << fn << "\n\n"; 
      
         logfile << "last_feature_hit = " << circle->last_feature_hit << "\n";
         eek();
         continue;
      }
      */
      /* Action to take depends on the type of feature it is. */
      
	  /*
	  if(features_for_level[fn]->feature_type == PONGPING_POLYGON_FEATURE)
	  {
         logfile << " id : " << ((PongPingPolygon *) features_for_level[fn])->polygon_id << "\n";
      }
	  */
	  
         switch( features_for_level[fn]->feature_type )
         {
            case PONGPING_POLYGON_FEATURE:
            
               if( ( ((PongPingPolygon *) features_for_level[fn])->attributes & CIRCLES_INSIDE_ONLY)) 
               {
                  //logfile << "checking circle hits play area.\n";
            
			      //logfile << "**************************************************************************************************\n";
			      //logfile << "*********************** POLYGON NUMBER " << fn << " which has ";
                  //logfile << ((PongPingPolygon *) features_for_level[fn])->number_of_sides << " sides.\n";
				  
                  if( features_for_level[fn]->check_for_collision( circles[cn1], &pm, 0, circles[cn1]->side_hit, vertex_not_to_check, ftr) != -1)
                  {
				     
					 //logfile << " ++" << "\n";
				  
                     ++(*one_or_more);
                     
					 /* Could this be put in check_for_collision ? */
					 
					 
					 
                     pm->set_feature_id( ((PongPingPolygon *) features_for_level[fn])->get_polygon_id() ); 
                     
					 pm->set_circle_one_number( cn1 );
					 
					 pms.push_back(pm);
                     
					 pm = new PotentialMove();
				     
					 //ogfile << "feature hit (circles inside only).\n";
                     //logfile << "it hits.\n";
                  }
                  else
                  {
                     //logfile << "it doesn't.\n";
                  }
               }
               else 
               {
			      //logfile << "**************************************************************************************************\n";
			      //logfile << "*********************** POLYGON NUMBER " << fn << " which has ";
                  //logfile << ((PongPingPolygon *) features_for_level[fn])->number_of_sides << " sides. circle is outside polygon.\n\n";
                  //logfile << "checking from outside. fn = " << fn << "\n";
                  //logfile << "circle near = " << features_for_level[fn]->circle_near(circle) << "\n";
                  
				  if( features_for_level[fn]->circle_near(circles[cn1]) )
                  {
			         if(features_for_level[fn]->check_for_collision( circles[cn1], &pm, hit_polygon_last_time, circles[cn1]->side_hit, vertex_not_to_check, ftr ) != -1)
				     {
			   
                        //logfile << "got a collision from outside.\n\n";
                        
						//logfile << "feature.\n";
						//logfile << " ++" << "\n";
                        ++(*one_or_more);                  
         
                        pm->set_feature_id( ((PongPingPolygon *) features_for_level[fn])->get_polygon_id() ); 
					    
						pm->set_circle_one_number( cn1 );
						
                        pms.push_back(pm);
						
						
					    pm = new PotentialMove();
						
						
						//logfile << "feature hit (from outside).\n";
						
				     }
                  }
				  else
				  {
				     //logfile << "circle not near enough.\n";
				  }
               //logfile << "called circle_near and check_for_collision.\n";              
               
              }            
              break;
           case PONGPING_CATCHMENT_AREA:
              //logfile << "checking with catchment area.\n"; eek();
              if(features_for_level[fn]->check_for_collision(circles[cn1], &pm, 0, 0, 0, ftr))
              {
                 //logfile << "got collision with catchment area.\n"; eek();
                 //eek();
				 
				 pm->set_circle_one_number( cn1 );
				 //logfile << " ++" << "\n";
                 ++(*one_or_more);
              
                 pms.push_back(pm);
			  
			     pm = new PotentialMove();
           
              }
              break;
      
         }
      
      
      
      
      
      //logfile << "checking collision with bat.\n\n";
                    
      
      
      
                                   
       }
	   
	   
	  
   }
    
   
   
   //logfile << "one or more = " << (*one_or_more) << " leaving get collisions\n";
   
   //eek();
   
    //logfile << "checked against all features.\n";
    
}







