

#ifndef POTENTIAL_MOVE_H
#define POTENTIAL_MOVE_H





class PotentialMove {
   
   
   public:
     
	  int number_of_circles;
	  
      double a1, a2;
	  
	  int feature_id;                 // for storing which feature if any was hit.
       
	  int hit_type;
	  
	  int polygon_vertex_or_side_hit;
	  
	  double new_di_1, new_di_2;
	  
	  
	  double pfot; // proper fraction of time
	  
	  
	  int circle_id_1;
	  int circle_id_2;
	  
	  
	  
	  
	  PotentialMove() { ; }
	  
	  
	  /* set members :- nc - number of circles (involved in the move) */
	  
	  int set_members(int nc, double _pfot, double _a1, double _new_di_1, double _a2, double _new_di_2 )
	  {
	     pfot = _pfot;
	     
		 a1 = _a1;
		 new_di_1 = _new_di_1; 
		
		 if( (number_of_circles = nc) == 2 )
		 {
		    a2 = _a2;
		    new_di_2 = _new_di_2;
	     }
	  }
	  
	  void set_feature_id( int id)
	  {
	     feature_id = id;
	  }
	  
	  //
	  
	  void set_hit_type_and_side(int ht, int side)
	  {
	     hit_type = ht;
         polygon_vertex_or_side_hit = side;		 
	  }
	  
	  void set_circle_one_number(int id)
	  {
	     circle_id_1 = id;
	  }
	  
	  void set_circle_two_number(int id)
	  {
	     circle_id_2 = id;
	  }
	  
	  
	  /* set_circle_numbers() : when the potential move is for two circles, this function is for setting the circle
	     numbers - so that if the move is applied, then the information is there about which circle 
		 
		 The hit type also gets set to PONGPING_CIRCLE_TO_CIRCLE_COLLISION. */
	  void set_circle_numbers( int cn1, int cn2 )
	  {
	     circle_id_1 = cn1;
		 circle_id_2 = cn2;
		 
		 hit_type = PONGPING_CIRCLE_TO_CIRCLE_COLLISION;
	  }
	  
};






#endif
