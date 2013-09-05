

#pongping makefile



OBJECT_FILES = pongping_one_game.o                                  \
               pongping_main.o                                      \
               pongping_bat.o                                       \
               pongping_miscellaneous.o                             \
               pongping_polygon.o                                   \
               pongping_read_level.o                                \
			   pongping_line_collisions.o                           \
			   pongping_get_ready_and_level_finished_bitmaps.o      \
			   pongping_circle.o                                    \
			   pongping_pixel_pusher.o                              \
			   pongping_credits.o                                   \
			   pongping_vertex_collisions.o                         \
			   pongping_updates.o                                   \
			   pongping_gui.o                                       \
			   pongping_circle_circle_collisions.o                  \
			   pongping_math.o                                      \
			   pongping_resource_check.o                            \
			   pongping_essentials.o                                \
			   pongping_log_strings.o                               \
               tinyxml.o                                            \
			   tinyxmlerror.o                                       \
			   tinyxmlparser.o                                      \
			   tinystr.o                                           
         
TINY_XML_OBJECT_FILES = tinyxml.o tinyxmlerror.o tinystr.o tinyxmlparser.o

EXTRA_SEARCH_DIRECTORIES = -I tiny_xml -I pongping_independent_headers -I pongping_headers

LINKER_FLAGS =  -static-libgcc -static-libstdc++ -lallegro-5.0.5-monolith-md-debug -lgsl

CPP_FLAGS = -Wshadow


pongping.exe : $(TINY_XML_OBJECT_FILES) $(OBJECT_FILES) Makefile
	echo :
	echo building pong_game.exe
	g++ -o $@ $(OBJECT_FILES) $(LINKER_FLAGS)
	
	
	
pongping_one_game.o : pongping_one_game.cpp                       \
pongping_headers/pongping_one_game_headers.h                      \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_gui.h                        \
pongping_headers/individual/pongping_essentials.h                 \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_level_feature.h              \
pongping_headers/individual/pongping_polygon.h                    \
pongping_headers/individual/pongping_bat.h                        \
pongping_headers/individual/pongping_one_game.h                   \
pongping_headers/individual/pongping_collectables.h               \
pongping_headers/individual/pongping_catchment_areas.h            \
pongping_headers/individual/pongping_ball.h                       \
pongping_headers/individual/pongping_pixel_pusher.h				 
	g++ -c $(CPP_FLAGS) $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	
pongping_main.o : pongping_main.cpp                               \
pongping_headers/pongping_main_headers.h                          \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_gui.h                        \
pongping_headers/individual/pongping_essentials.h                 \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_collectables.h               \
pongping_headers/individual/pongping_level_feature.h              \
pongping_headers/individual/pongping_bat.h                        \
pongping_headers/individual/pongping_one_game.h                   \
pongping_headers/individual/pongping_info_bitmaps.h               \
pongping_headers/individual/pongping_control_options_menu.h       \
pongping_headers/individual/pongping_credits.h                    \
pongping_headers/individual/pongping_resource_check.h             \
pongping_headers/individual/pongping_main.h

	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@

	
	
pongping_bat.o : pongping_bat.cpp                                 \
pongping_headers/pongping_bat_headers.h                           \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_bat.h
	echo : building bat.o
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@


	
pongping_miscellaneous.o : pongping_miscellaneous.cpp             \
pongping_headers/pongping_miscellaneous_headers.h                 \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h
	echo : compiling pongping_miscellaneous.cpp
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	
	
	
pongping_polygon.o : pongping_polygon.cpp                         \
pongping_headers/pongping_polygon_headers.h                       \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_level_feature.h              \
pongping_headers/individual/pongping_line_collisions.h            \
pongping_headers/individual/pongping_vertex_collisions.h          \
pongping_headers/individual/pongping_all_collisions.h             \
pongping_headers/individual/pongping_polygon.h 
	echo : compiling polygon.cpp
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@

	
	
pongping_read_level.o : pongping_read_level.cpp                   \
pongping_headers/pongping_read_level_headers.h                    \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_gui.h                        \
pongping_headers/individual/pongping_essentials.h                 \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_bat.h                        \
pongping_headers/individual/pongping_level_feature.h              \
pongping_headers/individual/pongping_one_game.h                   \
pongping_headers/individual/pongping_polygon.h                    \
pongping_headers/individual/pongping_catchment_areas.h 
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	

pongping_get_ready_and_level_finished_bitmaps.o : pongping_get_ready_and_level_finished_bitmaps.cpp \
pongping_headers/pongping_get_ready_and_level_finished_bitmaps_headers.h                            \
pongping_headers/individual/pongping.h                                                              \
pongping_headers/individual/pongping_potential_move.h                                               \
pongping_headers/individual/pongping_miscellaneous.h                                                \
pongping_headers/individual/pongping_info_bitmaps.h 

	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	
pongping_line_collisions.o : pongping_line_collisions.cpp         \
pongping_headers/pongping_line_collisions_headers.h               \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_circles_base.h	              \
pongping_headers/individual/pongping_all_collisions.h             \
pongping_headers/individual/pongping_line_collisions.h 

	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	
	
pongping_vertex_collisions.o : pongping_vertex_collisions.cpp     \
pongping_headers/pongping_vertex_collisions_headers.h             \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_vertex_collisions.h          \
pongping_headers/individual/pongping_all_collisions.h 
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	
	
pongping_circle.o : pongping_circle.cpp                           \
pongping_headers/pongping_circle_headers.h                        \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_bat.h                        \
pongping_headers/individual/pongping_level_feature.h              \
pongping_headers/individual/pongping_polygon.h                    \
pongping_headers/individual/pongping_line_collisions.h            \
pongping_headers/individual/pongping_vertex_collisions.h          \
pongping_headers/individual/pongping_all_collisions.h 
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@


	
pongping_credits.o : pongping_credits.cpp                         \
pongping_headers/pongping_credits_headers.h                       \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_gui.h                        \
pongping_headers/individual/pongping_essentials.h                 \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_level_feature.h              \
pongping_headers/individual/pongping_bat.h                        \
pongping_headers/individual/pongping_one_game.h                   \
pongping_headers/individual/pongping_credits.h 
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	

pongping_pixel_pusher.o : pongping_pixel_pusher.cpp               \
pongping_headers/pongping_pixel_pusher_headers.h                  \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_pixel_pusher.h               \
pongping_headers/individual/pongping_miscellaneous.h             
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@

	
	
	
pongping_updates.o : pongping_updates.cpp                         \
pongping_headers/pongping_updates_headers.h                       \
pongping_headers/individual/pongping.h                            \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_gui.h                        \
pongping_headers/individual/pongping_essentials.h                 \
pongping_headers/individual/pongping_circles_base.h               \
pongping_headers/individual/pongping_collectables.h               \
pongping_headers/individual/pongping_level_feature.h              \
pongping_headers/individual/pongping_polygon.h                    \
pongping_headers/individual/pongping_bat.h                        \
pongping_headers/individual/pongping_one_game.h                   \
pongping_headers/individual/pongping_circle_circle_collisions.h
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@

	
pongping_resource_check.o : pongping_resource_check.cpp           \
pongping_headers/pongping_resource_check_headers.h                \
pongping_headers/individual/pongping_potential_move.h             \
pongping_headers/individual/pongping_miscellaneous.h              \
pongping_headers/individual/pongping_resource_check.h      
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@



pongping_math.o : pongping_math.cpp                               \
pongping_headers/pongping_math_headers.h                          \
pongping_headers/individual/pongping.h
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	
	
pongping_circle_circle_collisions.o : pongping_circle_circle_collisions.cpp \
pongping_headers/pongping_circle_circle_collisions_headers.h                \
pongping_headers/individual/pongping.h                                      \
pongping_headers/individual/pongping_potential_move.h                       \
pongping_headers/individual/pongping_miscellaneous.h                        \
pongping_headers/individual/pongping_circles_base.h                         \
pongping_headers/individual/pongping_circle_circle_collisions.h

pongping_gui.o : pongping_gui.cpp                                           \
pongping_headers/pongping_gui_headers.h                                     \
pongping_headers/individual/pongping.h                                      \
pongping_headers/individual/pongping_potential_move.h                       \
pongping_headers/individual/pongping_miscellaneous.h                        \
pongping_headers/individual/pongping_gui.h                                  \
pongping_headers/individual/pongping_essentials.h                           

	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@

	
pongping_essentials.o : pongping_essentials.cpp                             \
pongping_headers/individual/pongping.h                                      \
pongping_headers/individual/pongping_potential_move.h                       \
pongping_headers/individual/pongping_miscellaneous.h                        \
pongping_headers/individual/pongping_gui.h                                  \
pongping_headers/individual/pongping_essentials.h                           \
pongping_headers/pongping_essentials_headers.h                    
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@


pongping_log_strings.o : pongping_log_strings.cpp

tinyxml.o : tiny_xml/tinyxml.cpp
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@
	

tinyxmlerror.o : tiny_xml/tinyxmlerror.cpp
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@


tinystr.o : tiny_xml/tinystr.cpp
	g++ -c $(EXTRA_SEARCH_DIRECTORIES) $< -o $@

tinyxmlparser.o : tiny_xml/tinyxmlparser.cpp
	g++ -c $< -o $@
	

	
.PHONY: clean

clean:
	rm -f $(OBJECT_FILES)
	
	
	
	
