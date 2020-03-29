#include <stdio.h>
#include <stdlib.h>
#include "../include/movement.h"
#include "../include/sensors.h"
#include "../include/bt_client.h"
#include "../ev3dev-c/source/ev3/ev3.h"
#include "../ev3dev-c/source/ev3/ev3_port.h"
#include "../ev3dev-c/source/ev3/ev3_tacho.h"
#include "../ev3dev-c/source/ev3/ev3_sensor.h"
#include <time.h>
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

#define COLOR_COUNT 8

enum { L, R };
uint8_t motor[3] = { DESC_LIMIT, DESC_LIMIT, DESC_LIMIT };

uint8_t sn_arm;
uint8_t sn_hand;

int get_max_speed( uint8_t sn) {
	get_tacho_max_speed( sn, &max_speed );
	return 0;
}

int init_motors() {
        if ( ev3_init() == -1 ) return ( -1 );
        if ( ev3_tacho_init() < 1 ) return ( -1 );

	if ( ev3_search_tacho_plugged_in(MOTOR_PORT_L, MOTOR_EXT_PORT_L, motor + L, 0 )) {
		get_tacho_max_speed( motor[L], &max_speed );
		set_tacho_command_inx( motor[L], TACHO_RESET );
	} else {
		printf("Left motor was NOT found");
		return -1;
	}
	if ( ev3_search_tacho_plugged_in(MOTOR_PORT_R, MOTOR_EXT_PORT_R, motor + R, 0 )) {
		set_tacho_command_inx( motor[R], TACHO_RESET );
	} else {
		printf("Right motor was NOT found");
		return -1;
	}
//	if ( ev3_search_tacho_plugged_in(PORT_ARM, EXT_PORT_ARM, &sn_arm, 0 )) {
//		set_tacho_command_inx( sn_arm, TACHO_RESET );
//	} else {
//		printf("Arm motor was NOT found");
//		return -1;
//	}
	if ( ev3_search_tacho_plugged_in(PORT_HAND, EXT_PORT_HAND, &sn_hand, 0 )) {
		set_tacho_command_inx( sn_hand, TACHO_RESET );
	} else {
		printf("Hand motor was NOT found");
		return -1;
	}
        printf("Initialization successful\n");
	return 0;
}

int run_motors_without_stopping( int speed_l, int speed_r ) {
	if ( (speed_l < -100) || (speed_l > 100) || (speed_r < -100) || (speed_r > 100) ) {
		printf("speed has to be between -100 and 100");
		return -1;
	}
	get_max_speed(motor[L]);
        set_tacho_speed_sp(motor[L], speed_l/100.0 * max_speed);
        set_tacho_speed_sp(motor[R], speed_r/100.0 * max_speed);
        multi_set_tacho_command_inx( motor, TACHO_RUN_FOREVER );
	return 0;
}

void stop_motors() {
	multi_set_tacho_command_inx(motor, TACHO_STOP);
}

int run_motors_to_relative_position( int speed_l, int pos_l, int speed_r, int pos_r ) {
	if ( (speed_l < 0) || (speed_l > 100) || (speed_r < 0) || (speed_r > 100) ) {
		printf("speed has to be between 0 and 100");
		return -1;
	}
	get_max_speed(motor[L]);
	set_tacho_speed_sp(motor[L], speed_l/100.0 * max_speed);
	set_tacho_speed_sp(motor[R], speed_r/100.0 * max_speed);
	set_tacho_position_sp(motor[L], pos_l);
	set_tacho_position_sp(motor[R], pos_r);
	multi_set_tacho_command_inx(motor, TACHO_RUN_TO_REL_POS);
	return 0;
}

int run_motors_timed( int speed_l, int speed_r, int ms ) {
	if ( (speed_l < -100) || (speed_l > 100) || (speed_r < -100) || (speed_r > 100) ) {
		printf("speed has to be between 0 and 100\n");
		return -1;
	}
	get_max_speed(motor[L]);
	set_tacho_speed_sp(motor[L], speed_l/100.0 * max_speed);
	set_tacho_speed_sp(motor[R], speed_r/100.0 * max_speed);
	multi_set_tacho_time_sp(motor, ms);
	multi_set_tacho_command_inx(motor, TACHO_RUN_TIMED);
	return 0;
}

int run_motors_turn( int speed, int direction, int ms) {
	if ( (direction != 0) && (direction != 1)) {
		printf("Direction of turn has to be either 0 or 1!");
		return -1;
	}
	if (direction) {
		run_motors_timed( speed, -1*speed, ms);
	} else {
		run_motors_timed( -1*speed, speed, ms);
	}
	return 0;
}

int run_motors_infinite_turn( int speed, int direction ) {
	if ( (direction != 0) && (direction != 1)) {
		printf("Direction of turn has to be either 0 or 1!");
		return -1;
	}
	if (direction) {
		run_motors_without_stopping( speed, -1*speed );
	} else {
		run_motors_without_stopping( -1*speed, speed );
	}
	return 0;
}

int run_arm_to_relative_position( int speed, int pos ) {
	if ( (speed < 0) || (speed > 100) ) {
		printf("speed has to be between 0 and 100");
		return -1;
	}
	get_max_speed(sn_arm);
	set_tacho_speed_sp(sn_arm, speed/100.0 * max_speed);
	set_tacho_position_sp(sn_arm, pos);
	multi_set_tacho_command_inx( &sn_arm, TACHO_RUN_TO_REL_POS);
	return 0;
}

int run_hand_to_relative_position( int speed, int pos ) {
	if ( (speed < 0) || (speed > 100) ) {
		printf("speed has to be between 0 and 100");
		return -1;
	}
	get_max_speed(sn_hand);
	set_tacho_speed_sp(sn_hand, speed/100.0 * max_speed);
	set_tacho_position_sp(sn_hand, pos);
	multi_set_tacho_command_inx( &sn_hand, TACHO_RUN_TO_REL_POS);
	return 0;
}

int turn_degree( int speed, int direction, int degree, float* minDistanceReturn ) {
	float previousDistance, tempDistance;
	printf("turning\n");
	float phase;
	read_gyro_sensor( &phase );
	float target = 0;
	float beginPhase, currentPhase;
	read_gyro_sensor( &beginPhase );
	read_gyro_sensor( &currentPhase );
	float currentDistance=9999.9;
	float minimumDistance=9999.9;	
	if (direction == 1) {
		run_motors_infinite_turn( speed, 1 );
		printf("turn right\n");
	} else {
		printf("turn left\n");
		run_motors_infinite_turn( speed, 0 );
	}
	while ( abs(beginPhase - currentPhase) < degree ) { //make sure the robot turns for the given degree
		read_sonar( &tempDistance );
		if ( tempDistance != 326.0 ) {
			previousDistance = currentDistance;
			currentDistance = tempDistance;
		}
		if ( (currentDistance != 326.0) && (currentDistance < minimumDistance) ) { //Update the minimum distance
			minimumDistance = currentDistance;
		}
		read_gyro_sensor( &currentPhase ); //Update the current angle
       	//printf ("turn left currentPhase %f\n", currentPhase);
	}
	stop_motors();
	*minDistanceReturn = minimumDistance;
	return 0;
}

int turn_towards_minimum_Distance( int speed, int degree ) {
	int found_jump = 0;
	float minimumDistance = 9999.0;
	float currentDistance, previousDistance, tempDistance;
	float beginPhase, currentPhase;
    time_t start, end;
    time(&start);
    time(&end);
	int retValue;
	retValue = turn_degree( speed, 0, (int) degree/2, &minimumDistance ); // turn left to get the object into the sweep of the area
	sleep(3);
	retValue = turn_degree( speed, 1, degree, &minimumDistance ); // turn right and save the minimum observed distance
	sleep(5);
    printf ("mini dist %f\n", minimumDistance);

        read_sonar( &currentDistance );
	printf ("CurrentDistance: %f\n", currentDistance);

	read_gyro_sensor( &beginPhase );    
	read_gyro_sensor( &currentPhase );    

        run_motors_infinite_turn( speed, 0 ); 
	if ( currentDistance < minimumDistance + 16 ) {
		retValue = turn_degree( speed, 1, 10, &tempDistance ); //If the distance is minimal at the end of the sweep, go a little bit further (include the object with minimum distance)
		sleep(1);
	}
	while ( (currentDistance > minimumDistance + 15) && ((int)(end - start) < 35) && (abs(currentPhase-beginPhase) < degree) ) {
		time(&end);
		read_sonar( &tempDistance );
		if ( tempDistance != 326.0 ) { //Ignore the value 326.0
			previousDistance = currentDistance;
			currentDistance = tempDistance;
		}
		printf("currentDistance: %f\n", tempDistance);
		if ( (abs(currentDistance-previousDistance) > 100 ) && (tempDistance != 326.0) ) { //A jump is detected
			printf("Jump detected (turn_towards_minimum_Distance)\n");
			found_jump = 1; //Set the flag that shows if a jump was detected during 3rd sweep
		}
		read_gyro_sensor( &currentPhase );
	}
	printf("scanned area, returning result\n");
	stop_motors();

	if ( abs(currentPhase-beginPhase) < degree && found_jump == 1 ) { //A jump has to be detected, otherwise the robot could see the wall with minimum distance as "ball"
		retValue = turn_degree( speed, 0, 10, &tempDistance );
		return 0; 
	} else {
		return -1;
	}
}

int find_Ball( int speed ) {
	float previousDistance= 9999.0;
	float currentDistance = 9999.0;
	float minimumDistance = 9999.0;
	float tempDistance    = 9999.0;
	printf("Start find Ball\n");
	while (true) {
		read_sonar( &previousDistance );
		read_sonar( &currentDistance );
		stop_motors();
		run_motors_infinite_turn(speed, 1);
		while (true) {
			read_sonar( &tempDistance );
			if ( tempDistance != 326.0 ) {
				previousDistance = currentDistance;
				currentDistance = tempDistance;
			}
			printf("%f\n",tempDistance);
			if ( currentDistance < 800 ) { //Ignore things that are too far away
				if ( (abs(currentDistance-previousDistance) > 100 ) && (tempDistance != 326.0) ) { //Detect a jump
					printf("Jump detected\n");
					break;
				}
			}
		}
		stop_motors();
	
		// Found the beginning of a potential ball, check if it is a ball and turn towards it
		printf("Found jump, checking if it is a ball\n");
		int result = turn_towards_minimum_Distance( speed, 30 ); //Call the function that checks if the found object is a ball and turn towards it if so
		if (result == 0) {
			printf("Ball found\n");
			return 0;
		} else {
			printf("No minimum distance detected, start routine from beginning\n");

		}
	}		
	printf("Current distance: ,%f\n", currentDistance);
	printf("Found minimum distance\n");
	stop_motors();	
}

int go_to_the_ball( int speed ){
    float currentDistance;
	struct timeval tm1, tm2;
	int color=0;
	int lineCount = 0;

	gettimeofday(&tm1, NULL); //Get time for measuring how long the robot moves forward
        read_sonar( &currentDistance );
	read_color( &color, COLOR_COUNT );
	float angle = get_angle();
	float phase;
	read_gyro_sensor( &phase );

	printf("Current angle: %f\n",angle);
	printf("Current phase: %f\n",phase);
	while( currentDistance > 50 ) {
		run_motors_without_stopping( speed, speed );
		read_sonar( &currentDistance );
		if ( color != 0 && angle < 270 && angle > 90) { //Don't go into opponent's field, based on robot facing in the beginning the own basket
			lineCount++;
			printf("Line detected: Number of lines: %d\n", lineCount);
			if ( lineCount >= 2 ) { //The second 
				break;
				printf("Second black line detected, opponent field, break \n");
			}
			Sleep(1500);
		}
		read_color( &color, COLOR_COUNT );
		
	}

	gettimeofday(&tm2, NULL);
	unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
	printf("%llu ms taken for getting close\n", t);
	if ( lineCount >= 2 ) { // Robot found line to opponent's field, go back. No ball was found
	  	run_motors_timed( -1*speed, -1*speed, t );
		sleep(3);
		return -1; 
	}
	else //Found ball, grab it
	{
	  	run_motors_timed( speed, speed, 1000 );
          	sleep(2);
          	grab_the_ball(0);
          	sleep(2);
	  	run_motors_timed( -speed, -speed, 1000 );
	  	run_motors_timed( -1*speed, -1*speed, t ); //Get back to original position
		return 0;
	}
}



int grab_the_ball(int n) {
	struct timeval tm1, tm2;
	int valcomp;
	long int t=0;
	gettimeofday(&tm1, NULL);
	while(valcomp==0&&t<2000){      //wait for the touch sensor to be pushed or for the time to be higher than expected
		read_check_pressed(&valcomp);
		printf("%i",valcomp);
        	run_hand_to_relative_position(50, 5);
		gettimeofday(&tm2, NULL);
		t =1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000 ;
		printf("%li ms\n", t);
	}
	sleep(1);
	if(valcomp==1){  //touch sensor is touched
		if(n==0){  //there was no wall
			run_hand_to_relative_position(50, 10);
			printf("grab the ball\n");
			return 0;
		}
		else{  //there was a wall
			printf("Give the value to go to the ball");
			printf("%i", n);
			run_motors_timed( 2, 2, 1500*n );//go forward to reinitialize the position
			return 1;
		}
	}
	else{ //time is higher, a wall is blocking the arm
		printf("touched a wall\n");
		n=n+1;
		run_hand_to_relative_position(50, -70);
		run_motors_timed( -1, -1, 3000 );  //go backward
		sleep(4);
		grab_the_ball(n);
	}
}





int drop_the_ball(void) {
	run_hand_to_relative_position(30, 40);
	sleep(3);
	run_hand_to_relative_position(30, -40); /*if no place when we are in the front zone*/
	return 0;
	printf("too far from the ball");
	return -1;
}


int trhow_the_ball_close(void) { 
        run_hand_to_relative_position(100, 60);
	sleep(3);
	run_hand_to_relative_position(100, -60);
	return 0;
	printf("too far from the ball");
	return -1;
}

int throw_the_ball_dist(void) {
        run_hand_to_relative_position(100, 80);
	sleep(3);
	run_hand_to_relative_position(100, -90);
    send_score_to_server(3); 
	return 0;
	printf("too far from the ball");
	return -1;
}

//int detect_line(void) {
//	int color_value;
//
//	read_color(&color_value, 8);
//
//	while (true) {
//        read_color(&color_value, 8);
//        printf("Color_value :%d \n", color_value);
//	printf("Color : %s \n", color[color_value]);
//	if (color_value==1) {
//		printf("line detected");
//		return 0
//	}
//  }

int go_straight_and_throw_ball(void) { 
	int sensor_value;
	int nb_lines_detected;
	//sensor_init();  // weird that the scolor sensor doesn't initialise correcylt and if it does it in this fuction, it bugs the others
	nb_lines_detected=0;
	while(nb_lines_detected!=2) {	
                run_motors_without_stopping(3, 3);
		read_color(&sensor_value,8); //ajouter un sleep à tester demain
		printf("color= %d\n", sensor_value);
		if(sensor_value!=0) {
			nb_lines_detected++;
			sleep(1);
		}
	}
	stop_motors();
	sleep(2);
	throw_the_ball_dist();
  	return 0;
}

int go_back_from_basket_to_center(void) { 
	int sensor_value;
	int nb_lines_detected;
	//sensor_init();  // weird that the scolor sensor doesn't initialise correcylt and if it does it in this fuction, it bugs the others
	nb_lines_detected=0;
	while(nb_lines_detected!=2) {	
                run_motors_without_stopping(-5, -5);
		read_color(&sensor_value,8); //ajouter un sleep à tester demain
		printf("color= %d\n", sensor_value);
		if(sensor_value!=0) {
			nb_lines_detected++;
			sleep(1);
		}
	}
    sleep(1);
	stop_motors();
  	return 0;
}

/*int back_to_angular_position(int speed, float initial_angle, float ball_angle) {
	float angle_difference;
	float fake_distance;
	if (initial_angle - ball_angle < 0) {
		turn_degree( speed, 0, ((int)(ball_angle - initial_angle)%360), &fake_distance ); //voir si c'est le bon sens
		return 0;
	}
	else {
		turn_degree( speed, 1,((int)(initial_angle - ball_angle)%360), &fake_distance);
		return 0;
	}
} */


int back_to_angular_position(int speed, float initial_angle, float ball_angle) {
	float angle_difference;
	float fake_distance;
        float difference=(int)(ball_angle - initial_angle)%360;                         //problementde difference négative
	printf("difference %f\n", difference);
	if (difference > 180 ) {
		turn_degree( speed, 1, (360-difference), &fake_distance ); 
		return 0;
	}
	if (difference < 0 ) {
		difference = -1*difference;
		turn_degree( speed, 1, difference, &fake_distance ); 
		return 0;
	}
	else {
		turn_degree( speed, 0, difference , &fake_distance);
		return 0;
	}
} 

int go_until_line_found(int speed) {
        run_motors_without_stopping(speed, speed);
	printf("début sleep");
	sleep(2);
        printf("fin sleep");
	stop_motors();
	int color_value; //faire un pas en arrière
	read_color(&color_value, 8);
	printf("color= %d\n", color_value);
	while(color_value==0) {   
		run_motors_without_stopping(speed, speed);
	        read_color(&color_value, 8);
		//printf("color: %d\n", color_value);
	}
	stop_motors();
        return 0;
}

unsigned long long measure_time_to_reach_opposite_line(int speed){ 
	go_until_line_found(-1*speed);
	struct timeval tm1, tm2;
	gettimeofday(&tm1, NULL);
	sleep(1);
	go_until_line_found(speed);
	gettimeofday(&tm2, NULL);
	unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
	printf("time necessary to reach opposite line =%llu\n", t);
	run_motors_timed( -1*speed, -1*speed, t/2 );
	return t;
}		

int find_calibrage_angle(int speed){ //à ajouter dans mvmt.h
	
}
/*
Full FCalibration
Etape1: être sûr qu'on est dans le bon axe
	- reparér la plus petit distance entre les lignes noirs, calculer 1, tourner à gauche +2° puis à droite 6é (+4)
Etape2: reculer à mi-distance et tourner de 90°
Etape3: se placer au centre de la ligne
/!\ rajouter des offsets pour vérifier qu'on est bien placé 

Semi calibrage
COnsidéré que le robot est latéralement dans le carré et juste vérifier son axe en bougeant de 2° grâce au temps mis.
*/
