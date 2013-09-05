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
 

#include "pongping_headers/pongping_polygon_headers.h"

int calls_to_check_collision = 0;


vector <int> PongPingPolygon::ids_used;

void wait_for_key(void)
{


   ALLEGRO_EVENT ev;
   ALLEGRO_EVENT_QUEUE *q = al_create_event_queue();
   
   al_register_event_source(q, al_get_keyboard_event_source());
   
   do{
      al_wait_for_event( q, &ev);
      }
   while( ev.type != ALLEGRO_EVENT_KEY_DOWN );


}



PongPingPolygon::PongPingPolygon(int cx, int cy, int sides, float r, float ic, int direction_of_spin, float angle_, int _attributes)
  
   : cx(cx), cy(cy), number_of_sides(sides), radius_to_vertex(r), increment_angle(ic), direction_of_spin(direction_of_spin), angle(angle_)
   
{
   
   
   int vn; /* vertex number */

   float a; /* angle */
   
   attributes = _attributes;
   
   if(attributes & REGULAR) //covers all types of regular polygon
   {
   
      vertices = new Point [sides];
    
	  
      for(vn = 0; vn < number_of_sides; ++vn)
      {
      
         /* Get the angle to the vertex. */
      
         a = (( 2.0f * PONGPING_PI ) / number_of_sides) * (float) vn;
    
	     
	
         vertices[vn].x = cx + radius_to_vertex * cos(a);
         vertices[vn].y = cy + radius_to_vertex * sin(a);

		 //logfile << (vertices[vn].x) << " " << (vertices[vn].y) << "\n";
		 
		 
      }
	  
	  //eek();
	  
	  furthest_vertex = radius_to_vertex;
	  
   }
   else
   {
      //logfile << "allocating " << sides << " vertices.\n\n";
   
      vertices = new Point [sides];
	  //logfile << "calling update.\n";
	  
	  //logfile << "number of unrotated points is " << unrotated_points.size() << "\n";
	  
	  //update();
   }
   
   //logfile << "allocated vertices.\n\n";
   
   //get rid of this
   type = _attributes;
 
   //change  
   destroyed = 0;
   
   //change
   contains_collectable = false;
   
   
   
   polygon_id = ids_used.size();
   
   ids_used.push_back(polygon_id);
   
   //logfile << "assigned " << polygon_id << "to this polygon.\n";
   
   //current_angle = angle_;
   
   //logfile << "leaving polygon constructor.\n\n";
   
   
   
   set_feature_type( PONGPING_POLYGON_FEATURE );
   
   draw_priority = PONGPING_FEATURE_G_DRAW;
   
   time_to_delete = 0;
   
};





void PongPingPolygon::update_by(double pfot)
{
   if(direction_of_spin == 0)
   {
      angle += increment_angle * pfot;
   
      //logfile << " added " << increment_angle << "to angle.\n\n";
   
      if(angle > PONGPING_PI* 2.0f)
      {
         angle -= PONGPING_PI* 2.0f;
      }
   }
   else
   {
      angle -= increment_angle * pfot;
      
      if(angle < 0.0f)
      {
         angle = PONGPING_PI* 2.0f + angle;
      }
   }
   
   int vn;
   
   float a;
   
   if(attributes & REGULAR)
   {
   
      for(vn = 0; vn < number_of_sides; ++vn)
      {
      
         /* Get the angle to the vertex. */
      
         a = (( 2.0f * PONGPING_PI ) / number_of_sides) * (float) vn + angle;
      
	     //logfile << "type = " << type << "\n\n";
	  
	     if( !(attributes & PONGPING_VANISHING_POLYGON) )
	     {
	        //logfile << "updating.\n\n";
            vertices[vn].x = cx + radius_to_vertex * cos(a);
            vertices[vn].y = cy + radius_to_vertex * sin(a);
	     }
	     else if( attributes & PONGPING_VANISHING_POLYGON )
	     {
	        if( polygon_colours.size() == 0)
		    {
		       //logfile << "PongPingPolygon::Update() : polygon_colours.size() == 0.\n" << endl;
               eek();
	        }
	        vertices[vn].x = cx + radius_to_vertex * ( (float) active_layers / (float) polygon_colours.size()) * cos(a);
		    vertices[vn].y = cy + radius_to_vertex * ( (float) active_layers / (float) polygon_colours.size()) * sin(a);
	     }
      }
   }
   else // IRREGULAR
   {
     
	  
      for(vn = 0; vn < number_of_sides; ++vn)
      {
	     //if(polygon_id == 9) logfile << "a = " << a * 180.0f / PONGPING_PI<< "\n";
	     a = unrotated_points[vn].angle + angle;
	  
	     if(attributes & ONE_COLOURED_POLYGON || attributes & PLAY_AREA || attributes & PONGPING_HIT_X_TIMES_POLYGON)
         {
		    vertices[vn].x = cx + unrotated_points[vn].radius * cos(a);
            vertices[vn].y = cy + unrotated_points[vn].radius * sin(a);
         }
         else if(attributes & PONGPING_VANISHING_POLYGON) {
		 
            
            vertices[vn].x = cx + unrotated_points[vn].radius * ( (float) active_layers / (float) polygon_colours.size() ) * cos(a);
	        vertices[vn].y = cy + unrotated_points[vn].radius * ( (float) active_layers / (float) polygon_colours.size() ) * sin(a);
		    
			if(polygon_id == 9) 
			{
			   //logfile << "here 121 : " << vertices[vn].x << " " << vertices[vn].y << "\n";
			   //logfile << active_layers << "\n";
			}
         }		 
	  }
   }
   
   
};







void PongPingPolygon::draw() const
{
   int vn;
   
   float r, g, b;
   
   float muliplier;
   int layer;
         float a;
   
   const float minimum = 100.0f / 255.0f;
   
   Point vtx_temp[2];	     
   
   COULEUR c;
   
   if(!(attributes & PLAY_AREA))
      al_unmap_rgb_f(polygon_colours[0], &r, &g, &b);
   
   //logfile << "unmapped colour.\n";
   
   if(attributes & REGULAR)
   {
   for(vn = 0; vn < number_of_sides; ++vn)
   {
      
	  c = al_map_rgba_f( (double) r * (double) vn / (double) number_of_sides, (double) g * (double) vn / (double) number_of_sides, (double) b * (double) vn / (double) number_of_sides, 255 );
   
      if( attributes & PONGPING_HIT_X_TIMES_POLYGON)
	  {
	     /* Color 9 is the actual color from the level file. This is for the largest triangle drawn which should be drawn first
		    as the others are drawn over it. */
	     
		 a = (( 2.0f * PONGPING_PI ) / number_of_sides) * (float) (vn % number_of_sides) + angle;
	     
		 vtx_temp[0].x = cx + radius_to_vertex * cos(a);
	     vtx_temp[0].y = cy + radius_to_vertex * sin(a);
		 
		 a = (( 2.0f * PONGPING_PI ) / number_of_sides) * (float) ((vn + 1) % number_of_sides) + angle;
	   
	     vtx_temp[1].x = cx + radius_to_vertex * cos(a);
	     vtx_temp[1].y = cy + radius_to_vertex * sin(a);
		 
		 al_draw_filled_triangle( cx, cy, vtx_temp[0].x, vtx_temp[0].y, vtx_temp[1].x, vtx_temp[1].y, polygon_colours[NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES - 1]);
	     
	     for( layer = active_layers; layer > 0; --layer)
		 {
		    //logfile << "layer = " << layer << "\n\n";
			eek();
		 
		    a = (( 2.0f * PONGPING_PI ) / number_of_sides) * (float) (vn % number_of_sides) + angle;
		 
		    vtx_temp[0].x = (float) cx + (float) radius_to_vertex * ( (float) layer / ((float) NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES)) * cos(a);
			vtx_temp[0].y = (float) cy + (float) radius_to_vertex * ( (float) layer / ((float) NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES)) * sin(a);
		 
		    a = (( 2.0f * PONGPING_PI ) / number_of_sides) * (float) ((vn + 1) % number_of_sides) + angle;
		 
		    vtx_temp[1].x = (float) cx + (float) radius_to_vertex * ( (float) layer / ((float) NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES)) * cos(a);
			vtx_temp[1].y = (float) cy + (float) radius_to_vertex * ( (float) layer / ((float) NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES)) * sin(a);
		 
            al_draw_filled_triangle( cx, cy, vtx_temp[0].x, vtx_temp[0].y, vtx_temp[1].x, vtx_temp[1].y, polygon_colours[layer - 1] );
			
			
	     }
      }
	  else if( attributes & SHADED_POLYGON  )
	  {
	  
         al_draw_filled_triangle( cx, cy, vertices[vn].x, vertices[vn].y, vertices[vn + 1].x, vertices[vn + 1].y, 
		 al_map_rgb_f( (r * (((float) vn + minimum )/ (float) number_of_sides)),
                       (g * (((float) vn + minimum )/ (float) number_of_sides)), 
                       (b * (((float) vn + minimum )/ (float) number_of_sides)) ) );
      }
	  else if( attributes & PONGPING_VANISHING_POLYGON )
	  {
	     logfile << "drawing vanishing polygon*******************.\n";
	  
	     logfile << "active_layers = " << active_layers << "\n";
	  
		 for(layer = active_layers; layer > 0; --layer)
		 {
		    a = (( 2.0f * PONGPING_PI ) / number_of_sides) * (float) (vn % number_of_sides) + angle;
		    vtx_temp[0].x = cx + radius_to_vertex * ( (float) layer / (float) polygon_colours.size()) * cos(a);
            vtx_temp[0].y = cy + radius_to_vertex * ( (float) layer / (float) polygon_colours.size()) * sin(a);
			
			a = (( 2.0f * PONGPING_PI ) / number_of_sides) * (float) ((vn + 1) % number_of_sides)  + angle;
			vtx_temp[1].x = cx + radius_to_vertex * ( (float) layer / (float) polygon_colours.size()) * cos(a);
            vtx_temp[1].y = cy + radius_to_vertex * ( (float) layer / (float) polygon_colours.size()) * sin(a);
				
			al_draw_filled_triangle(cx, cy, vtx_temp[0].x, vtx_temp[0].y, vtx_temp[1].x, vtx_temp[1].y, c /*polygon_colours[layer - 1]*/); 
		 }
	  }
	  else if( attributes & ONE_COLOURED_POLYGON )
	  {
	     al_draw_filled_triangle( cx, cy, vertices[vn].x, vertices[vn].y, vertices[(vn + 1) % number_of_sides].x, vertices[ (vn + 1) % number_of_sides].y, c /*polygon_colours[0]*/ );
	  
	  }
	  
   }
   
   
   if( attributes & SHADED_POLYGON )
   {
      al_draw_filled_triangle( cx, cy, vertices[number_of_sides - 1].x, vertices[number_of_sides - 1].y, vertices[0].x, vertices[0].y, polygon_colours[0]);
   }
   
   }
   else // polygon should be IRREGULAR
   {
      
	  //logfile << "got irregular.\n";
	  
      if(attributes & PLAY_AREA)
	  {
	     return;
	  }
      
	  int triangle_count = 0;
   
      if( attributes & ONE_COLOURED_POLYGON )
	  {
	     for(vn = 0; vn < number_of_sides; ++vn)
         {
		     ++triangle_count;
	         al_draw_filled_triangle( cx, cy, vertices[vn].x, vertices[vn].y, vertices[(vn + 1) % number_of_sides].x, vertices[(vn + 1) % number_of_sides].y, polygon_colours[0] );
	     
		     
		 }
		 //logfile << "drawn " << triangle_count << " triangles.\n";
	  }
	  else if( attributes & PONGPING_HIT_X_TIMES_POLYGON )
	  {
	     for(vn = 0; vn < number_of_sides; ++vn)
	     {
	        a = unrotated_points[vn].angle + angle;
	     
		    vtx_temp[0].x = cx + unrotated_points[vn].radius * cos(a);
	        vtx_temp[0].y = cy + unrotated_points[vn].radius * sin(a);
		 
			a = unrotated_points[(vn + 1) % number_of_sides].angle + angle;
		 
		    vtx_temp[1].x = cx + unrotated_points[(vn + 1) % number_of_sides].radius * cos(a);
	        vtx_temp[1].y = cy + unrotated_points[(vn + 1) % number_of_sides].radius * sin(a);
		 
		    al_draw_filled_triangle( cx, cy, vtx_temp[0].x, vtx_temp[0].y, vtx_temp[1].x, vtx_temp[1].y, polygon_colours[NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES - 1]);
	     
	        for( layer = active_layers; layer > 0; --layer)
		    {
		    
		       a = unrotated_points[vn].angle + angle;
		 
		       vtx_temp[0].x = (float) cx + (float) unrotated_points[vn].radius * ( (float) layer / ((float) NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES)) * cos(a);
			   vtx_temp[0].y = (float) cy + (float) unrotated_points[vn].radius * ( (float) layer / ((float) NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES)) * sin(a);
		 
		       a = unrotated_points[(vn + 1) % number_of_sides].angle + angle;
		    
		 
		       vtx_temp[1].x = (float) cx + (float) unrotated_points[(vn + 1) % number_of_sides].radius * ( (float) layer / ((float) NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES)) * cos(a);
			   vtx_temp[1].y = (float) cy + (float) unrotated_points[(vn + 1) % number_of_sides].radius * ( (float) layer / ((float) NUMBER_OF_LAYERS_FOR_POLYGONS_WITH_COLLECTABLES)) * sin(a);
		 
               al_draw_filled_triangle( cx, cy, vtx_temp[0].x, vtx_temp[0].y, vtx_temp[1].x, vtx_temp[1].y, polygon_colours[layer - 1] );
			
			
	        }
         }
	  
	  }
	  else if(attributes & PONGPING_VANISHING_POLYGON)
	  {
	     //logfile << "destructable.\n";
		 for(vn = 0; vn < number_of_sides; ++vn)
	     {
	        for( layer = active_layers; layer > 0; --layer)
	        {
		    
			   a = unrotated_points[vn].angle + angle;
	     
		       vtx_temp[0].x = cx + unrotated_points[vn].radius * ( (float) layer / (float) polygon_colours.size() ) * cos(a);
	           vtx_temp[0].y = cy + unrotated_points[vn].radius * ( (float) layer / (float) polygon_colours.size() ) * sin(a);
		 
			   a = unrotated_points[(vn + 1) % number_of_sides].angle + angle;
		 
		       vtx_temp[1].x = cx + unrotated_points[(vn + 1) % number_of_sides].radius * ( (float) layer / (float) polygon_colours.size() ) * cos(a);
	           vtx_temp[1].y = cy + unrotated_points[(vn + 1) % number_of_sides].radius * ( (float) layer / (float) polygon_colours.size() ) * sin(a);
			
               al_draw_filled_triangle( cx, cy, vtx_temp[0].x, vtx_temp[0].y, vtx_temp[1].x, vtx_temp[1].y, polygon_colours[layer - 1] );
			
	        }
		 }
	  }
	  
   
   }
}




/* circle_near() : checks to see if a circle is too far away from a polygon to hit it */

int PongPingPolygon::circle_near(Circle *circle)
{
   
   /* If the distance from centre of polygon to centre of circle ... */
   
   if( sqrt( (cx - circle->cc_x) * (cx - circle->cc_x) + (cy - circle->cc_y) * (cy - circle->cc_y) ) < furthest_vertex + circle->distance_increment + circle->radius )
   {
      
      return 1;
   }
   else
   {
      return 0;
   }
   
}




int PongPingPolygon::check_for_collision(Circle *circle, PotentialMove **pm, int hit_this_polygon_last_time, int side_hit, int vertex_hit, double ftr)
{
   int side;
   int v0, v1, v2;
   int collision_value;
   int debug_flag = 0;
   int circle_location;
   int line_number;
   
 
   unsigned char line_movement;
   
   PotentialMove *pm1, *pm2;
   
   double d1, d2;
   
   pm1 = new PotentialMove();
   pm2 = new PotentialMove();
   
   if(increment_angle == 0.0)
   {
      line_movement = PONGPING_TRANSLATION;
   }
   else 
   {
      line_movement = PONGPING_ROTATION;
   }
   
   
   
   //logfile << "this polygon has " << number_of_sides << " sides.\n";
   
   
   
   for(side = 0; side < number_of_sides; ++side) /* eg number_of_sides = 7 would mean side goes from 0 to 6 */
   {
      
      if(hit_this_polygon_last_time && ((side_hit == side && circle->hit_type == PONGPING_ROTATING_SIDE_POLYGON_COLLISION) || 
	                                    (vertex_hit == side && circle->hit_type == PONGPING_ROTATING_VERTEX_COLLISION)))
	  {
	  
	     //logfile << "not checking side " << side << "\n";
	     continue;
	  }
	  
   

	  //circle_location = attributes & CIRCLES_INSIDE_ONLY ? CIRCLE_ALWAYS_INSIDE : CIRCLE_INSIDE_OR_OUTSIDE;
	  
	  /*
	  
	  if( polygon_id == 3 )
	  {
	     debug_flag = 1;
	     logfile << "checking line from vertex " << v1 << " to " << v2 << ".\n\n"; 
	  }
	  
	  */
	  
	  //logfile << "Checking circle hits polygon side " << side << "\n"; //<< "attributes & CIRCLES_INSIDE_ONLY = " << (attributes & CIRCLES_INSIDE_ONLY) << "\n";
	  //
	  //logfile << "v1 = (" << vertices[side].x << " , " << vertices[side].y << ")\n";
	  //logfile << "v2 = (" << vertices[( side + 1 ) % number_of_sides].x << " , " << vertices[( side + 1 ) % number_of_sides].y << ")\n";
	  /* Check collision between circle and all the sides. */
	  //logfile << "v1 = " << side << "v2 = " << ( side + 1 ) % number_of_sides << "\n";
	  //logfile << "bc : " << vertices[v1].x << " " << vertices[v1].y << " " << vertices[v2].x << " " << vertices[v2].y << "\n";
	  
	  //int circle_and_line_collision(const unsigned char line_move, const Point v1, const Point v2,  
      //const double cx, const double cy, const double polygon_increment_angle, const int dx, const int dy, Circle *circle, PotentialMove& move, int circle_inside);
							  
	  if(circle_and_line_collision(line_movement, vertices[side], vertices[( side + 1 ) % number_of_sides], cx, cy, increment_angle * (direction_of_spin == 0 ? 1.0 : -1.0), 0.0, 0.0, circle, pm1, attributes & CIRCLES_INSIDE_ONLY, ftr ))                    
	  {
	     
		 //logfile << "circle hits side " << side << " checking next side\n";
	  
		 /* Check next line. */
	  
	     if(circle_and_line_collision(line_movement, vertices[(side + 1) % number_of_sides], vertices[( side + 2 ) % number_of_sides], cx, cy, increment_angle * (direction_of_spin == 0 ? 1.0 : -1.0), 0.0, 0.0, circle, pm2, attributes & CIRCLES_INSIDE_ONLY, ftr ))
         {
		 
		    /* Make pm point to the which of pm1 and pm2 is the shortest move (so the one which would happen first). */
			
			*pm = pm1->pfot < pm2->pfot ? pm1 : pm2;
			
			(*pm)->set_hit_type_and_side(PONGPING_ROTATING_SIDE_POLYGON_COLLISION, side);
			
         }
         else
         {		
            //logfile << "pm1->x = " << pm1->x << "\n"; 
		 
	        (*pm) = pm1;
	        
			(*pm)->set_hit_type_and_side(PONGPING_ROTATING_SIDE_POLYGON_COLLISION, side);
	        
	        return side;
		 }
	  }
	 
	
   }
   
   for(side = 0; side < number_of_sides; ++side) /* eg number_of_sides = 7 would mean side goes from 0 to 6 */
   {
      
	  //logfile << "checking vertex " << side << "\n";
	  
      if(hit_this_polygon_last_time && circle->vertex_or_side_hit == side)
	  {
	     //logfile << "not checking vertex " << side << "\n";
	     continue;
	  }
	  
	  //logfile << "(check_for_collision) circle distance increment = " << circle->distance_increment << "\n";
	  
	  if(cvc( line_movement, cx, cy, PONGPING_DONT_USE_CENTRE_OF_MASS, 0, 0, 0, vertices[side], increment_angle * (direction_of_spin == 0 ? 1.0 : -1.0), 0.0, 0.0, circle, *pm, ftr ))
	  {
	     (*pm)->set_hit_type_and_side(PONGPING_ROTATING_VERTEX_COLLISION, side);
		 
	     return side;
	  }
	  
	  
   }
   
   //logfile << "*********************************BACK IN PonPingPolygon::check_for_collision\n\n";
   //logfile << "number of sides was " << number_of_sides << "\n";
   //logfile << "returning -1.\n";
   
   return -1;
}




#include <fstream>

void PongPingPolygon::print_out_vertex_data()
{
   ofstream file("vertex_data.txt");

   ALLEGRO_BITMAP *bmp = al_create_bitmap(1024 + 8, 768 + 8);
   
   int i;
   
   int v;
   
   
   update_by(0.0);
   
   for(i = 0; i < number_of_sides; ++i)
   {
      logfile << "vertex " << i << "    x = " << vertices[i].x << "   y = " << vertices[i].y << endl << endl;
   }

   al_set_target_bitmap(bmp);
   al_clear_to_color(PONGPING_WHITE);

   al_draw_filled_rectangle(      0, 0,       1024 + 8, 4,          BLACK);
   al_draw_filled_rectangle(      0, 0,        4, 768 + 8,          BLACK);
   al_draw_filled_rectangle(  1024 + 4,       0, 1024 + 8, 768 + 8, BLACK);
   al_draw_filled_rectangle(0, 768 + 4, 1024 + 8, 768 + 8,          BLACK);
   
   
   for( v = 0; v < number_of_sides; ++v)
   {
      al_draw_filled_circle( vertices[v].x, vertices[v].y, 2.0, BLACK);   
   }
   
   al_save_bitmap("vertex_plot.png", bmp);
   
  
}






void PongPingPolygon::set_convex_or_concave_flag()
{
   int vn;
   
   double angle1, angle2;
   
   for(vn = 0; vn < number_of_sides; ++vn)
   {
      /* side 1 is  vertex vn to vertex (vn + 1) % number_of_sides */
   
      if( vertices[vn].x == vertices[(vn + 1) % number_of_sides].x )
	  {
	     if( vertices[vn].y < vertices[(vn + 1) % number_of_sides].y )
		 {
		    angle1 = PONGPING_PI/ 2.0;
		 }
		 else
		 {
		    angle1 = 3.0 * PONGPING_PI/ 2.0;
		 }
	  }
	  else
	  {
	     angle1 = atan( (vertices[vn].y - vertices[(vn + 1) % number_of_sides].y)  / (vertices[vn].x - vertices[(vn + 1) % number_of_sides].x) );
		 
		 if( (vertices[vn].x - vertices[(vn + 1) % number_of_sides].x) < 0.0)
		 {
		    angle1 += ALLEGRO_PI;
	     }
	  
	  }

	  if( vertices[(vn + 1) % number_of_sides].x == vertices[(vn + 2) % number_of_sides].x )
	  {
	     if( vertices[(vn + 1) % number_of_sides].y < vertices[(vn + 2) % number_of_sides].y )
		 {
		    angle2 = PONGPING_PI/ 2.0;
		 }
		 else
		 {
		    angle2 = 3.0 * PONGPING_PI/ 2.0;
		 }
	  }
	  else
	  {
	     angle2 = atan( (vertices[(vn + 1) % number_of_sides].y - vertices[(vn + 2) % number_of_sides].y)  / (vertices[(vn + 1) % number_of_sides].x - vertices[(vn + 2) % number_of_sides].x) );
		 
		 if( (vertices[(vn + 1) % number_of_sides].x - vertices[(vn + 2) % number_of_sides].x) < 0.0)
		 {
		    angle2 += ALLEGRO_PI;
	     }
	  
	  }

	  if( !(angle2 > angle1 && angle2 < angle1 + ALLEGRO_PI) )
	  {
	     attributes |= CONCAVE;
	  }
	  

   }





}


