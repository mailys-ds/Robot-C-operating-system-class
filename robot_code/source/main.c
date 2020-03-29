#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/sensors.h"
#include "../include/movement.h"
#include "../include/bt_client.h"
#include "../ev3dev-c/source/ev3/ev3.h"
#include "../ev3dev-c/source/ev3/ev3_port.h"
#include "../ev3dev-c/source/ev3/ev3_tacho.h"
#include "../ev3dev-c/source/ev3/ev3_sensor.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif
const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))



void *main_action (void *x_void_ptr){

  connect_to_server(); 
  float beginPhase, currentPhase;
  while(1){
  read_gyro_sensor( &currentPhase );
  printf ("turn left currentPhase %f \n", currentPhase);}
  float face_basket = get_angle();
  float current_angle = get_angle();
  // please don't touch this part, this is for the beginnig of the game.
  go_straight_and_throw_ball();
  sleep(8);
  grab_the_ball(0);
  sleep(5);
  throw_the_ball_dist();
  sleep(2);
  go_back_from_basket_to_center();
  float angle = get_angle();
  // long term routine
  while(1){
      int find_the_ball_result = 1;
      while(find_the_ball_result){
         find_the_ball_result = find_Ball(1 );
         sleep(20);
      }
      go_to_the_ball( 2 );
      current_angle = get_angle();
      back_to_angular_position(5 , current_angle, face_basket);
      go_straight_and_throw_ball();
      go_back_from_basket_to_center();
      

   }
}

int main(int argc, char **argv) {

  sensor_init();
  init_motors(); 
  int ret;
  pthread_t thread_action;
  ret = pthread_create (  &thread_action, NULL, main_action, NULL); 
  while( robot_kickout_or_game_end() ) {
  
  }
  pthread_exit ( &ret);
  stop_motors();
  printf("does it work , are we able to stop the robot from the server");
 
  printf( "*** ( EV3 ) Bye! ***\n" ); 
}





