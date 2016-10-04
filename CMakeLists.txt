project(click_drag_draw_select)

cmake_minimum_required(VERSION 3.4)

execute_process(COMMAND /usr/bin/glib-compile-resources cdds.gresource.xml --target=resources.c --generate-source 
                WORKING_DIRECTORY /home/btardio/projects/click_drag_draw_select
                OUTPUT_VARIABLE resources_output_variable
                RESULT_VARIABLE resources_result_variable
                ERROR_VARIABLE resources_error_variable)
#message(STATUS ${resources_output_variable})
#message(STATUS ${resources_result_variable})
#message(STATUS ${resources_error_variable})

if(${resources_result_variable})
  message(FATAL_ERROR ${resources_error_variable})
endif(${resources_result_variable})

add_subdirectory(dialogs)
add_subdirectory(geometry)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(cairo REQUIRED cairo)
pkg_check_modules(GSL REQUIRED gsl)

include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${CAIRO_INCLUDE_DIRS})
include_directories(${GSL_INCLUDE_DIRS})

link_directories(${GTK3_LIBRARY_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})
link_directories(${GSL_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})
add_definitions(${CAIRO_CFLAGS_OTHER})
add_definitions(${GSL_CFLAGS_OTHER})

# removal of definitions declared for source highlighting in cdds_geometry.h
add_definitions(-U CDDS_DEBUG)
add_definitions(-U CDDS_TEST)

add_executable(click_drag_draw_select cdds_window.c 
                                      cdds_application.c
                                      main.c 
                                      resources.c
                                      cdds_drawingarea.c 
                                      menus/point_right_click.c
                                      geometry/cdds_geometry_point.c
                                      geometry/cdds_geometry_pointmanager.c 
                                      geometry/cdds_geometry_selection.c
                                      geometry/cdds_geometry_action.c
                                      geometry/cdds_geometry_actionmanager.c
                                      geometry/cdds_geometry_line.c
                                      geometry/cdds_geometry_linemanager.c
                                      
                                      bresenhamlinealgorithm/bresenham.c
                                      
                                      subclassingtypes.c
                                      
                                      cdds_test_helper.c
                                      
                                      dialogs/dialog_new.c)

target_compile_definitions(click_drag_draw_select PUBLIC -D CDDS_DEBUG)
target_compile_options(click_drag_draw_select PUBLIC -g)

add_executable(click_drag_draw_select_test cdds_test.c
                                           cdds_test_helper.c
                                           cdds_window.c 
                                           cdds_application.c
                                           resources.c
                                           cdds_drawingarea.c
                                           menus/point_right_click.c
                                           dialogs/dialog_new.c
                                           
                                           geometry/cdds_geometry_point.c
                                           geometry/cdds_geometry_pointmanager.c
                                           geometry/cdds_geometry_selection.c
                                           geometry/cdds_geometry_action.c
                                           geometry/cdds_geometry_actionmanager.c
                                           geometry/cdds_geometry_line.c
                                           geometry/cdds_geometry_linemanager.c
                                           
                                           subclassingtypes.c
                                           )

target_compile_definitions(click_drag_draw_select_test PUBLIC -D CDDS_DEBUG -D CDDS_TEST)
target_compile_options(click_drag_draw_select_test PUBLIC -g)

target_link_libraries(click_drag_draw_select ${GTK3_LIBRARIES})
target_link_libraries(click_drag_draw_select ${CAIRO_LIBRARIES})
target_link_libraries(click_drag_draw_select ${GSL_LIBRARIES})
target_link_libraries(click_drag_draw_select -lm)

target_link_libraries(click_drag_draw_select_test ${GTK3_LIBRARIES})
target_link_libraries(click_drag_draw_select_test ${CAIRO_LIBRARIES})
target_link_libraries(click_drag_draw_select_test ${GSL_LIBRARIES})
target_link_libraries(click_drag_draw_select_test -lm)

install(TARGETS click_drag_draw_select RUNTIME DESTINATION bin)
