#include <stdio.h>
#include <stdlib.h>
#include "../include/sensors.h"
#include "../ev3dev-c/source/ev3/ev3.h"
#include "../ev3dev-c/source/ev3/ev3_port.h"
#include "../ev3dev-c/source/ev3/ev3_tacho.h"
#include "../ev3dev-c/source/ev3/ev3_sensor.h"

//const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
//#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

int sensor_init( void ) {

	if (ev3_search_sensor (LEGO_EV3_GYRO, &sn_gyro, 0)) {
	set_sensor_mode( sn_gyro, "GYRO-RATE");
		if ( set_sensor_mode (sn_gyro, "GYRO-CAL" ) == 0) {
			printf("Gyro not on GYRO-ANG");
		}
	}

	if (ev3_search_sensor (LEGO_EV3_COLOR, &sn_color, 0)) {
	set_sensor_mode( sn_color, "COL-REFLECT");
	printf("sensor en mode COL-REFLECT");
		if ( set_sensor_mode (sn_color, "COL-REFLECT" ) == 0) {
			printf("Color not on COL-REFLECT");
		}
	}
	read_gyro_sensor( &initial_phase ); // save initial phase
	return ev3_sensor_init();
    
}

float get_angle() {
	float phase = 9999.0;
	read_gyro_sensor( &phase );
	float temp = (float)((int)(phase - initial_phase) % 360);
	while (temp < 0) {
		temp += 360;
	}
	return temp;
}

int read_gyro_sensor( float *value ) {
	if ( ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro,0) ) {
		if ( !get_sensor_value0( sn_gyro, value )) {
			*value = 0.0;
		}
	} else {
		return -1;
	}
	return 0;
}

int read_sonar( float *value ) {
	if ( ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0) ) {
		if ( !get_sensor_value0( sn_sonar, value )) {
			*value = 0.0;
		}
	} else {
		return -1;
	}
	return 0;
}

int read_magnetic_sensor( float *value ) {
	if (ev3_search_sensor( NXT_ANALOG, &sn_mag, 0 )) {
		if ( !get_sensor_value0(sn_sonar, value )) {
			*value = 0;
		}
		return 0;
	} else {
		return -1;
	}
}

int read_compass( float *value ) {
	if (ev3_search_sensor( HT_NXT_COMPASS, &sn_compass, 0 )) {
		if ( !get_sensor_value0(sn_compass, value )) {
			*value = 0;
		}
		return 0;
	} else {
		return -1;
	}
}

int read_check_pressed( int *value ) {
	if ( ev3_search_sensor( LEGO_EV3_TOUCH, &sn_touch, 0 )) {
		int val;
		if ( sn_touch == SENSOR__NONE_ ) {
			*value = ( ev3_read_keys(( uint8_t *) &val ) && ( val & EV3_KEY_UP ));
		} else {
			*value = ( get_sensor_value( 0, sn_touch, &val) && (val != 0));
		}		
		return 0;
	} else {
		return -1;
	}
}
		

int read_color( int *value, int maxNumberColors ) {
//uint8_t sn_color;
	if ( ev3_search_sensor(LEGO_EV3_COLOR, &sn_color,0) ) {
		if ( !get_sensor_value( 0,sn_color, value ) || ( *value < 0 ) || ( *value >= maxNumberColors)) {
			*value = 0;
		}
	} else {
		return -1;
	}
	return 0;
}
