#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>

float initial_phase;

uint8_t sn;
uint8_t sn_touch;
uint8_t sn_color;
uint8_t sn_compass;
uint8_t sn_sonar;
uint8_t sn_mag;
uint8_t sn_gyro;

/**
 * \brief       detect connected sensors
 * \return      Number of found sensors of -1 in case of an error
 */
extern int sensor_init (void);

/**
 * \brief	returns angle relative to position of beginning
 *
 * \return Value of calculation, between 0° and 360°
 */
extern float get_angle( void );

/**
 * \brief       tries to read value of sonar
 * \param[out]  value Current value of sonar if detected
 *
 * \return      0 if reading suceeded, otherwise return -1
 */
extern int read_sonar( float *value );

/**
 * \brief       tries to read value of gyrosensor
 * \param[out]  value Current value of gyrosensor if detected
 *
 * \return      0 if reading suceeded, otherwise return -1
 */
extern int read_gyro_sensor( float *value );

/**
 * \brief       tries to read value of magnetic sensor 
 * \param[out]  value Current value of magnetic sensor, if detected
 *
 * \return      0 if reading suceeded, otherwise return -1
 */
extern int read_magnetic_sensor( float *value );

/**
 * \brief       tries to read value of compass (from 0 to 360)
 * \param[out]  value Current value of compass, if detected
 *
 * \return      0 if reading suceeded, otherwise return -1
 */
extern int read_compass( float *value );

/**
 * \brief       tries to read value of touch sensor
 * \param[out]  1 if sensor pressed, 0 if sensor released
 *
 * \return      0 if reading suceeded, otherwise return -1
 */
extern int read_check_pressed( int *value );


/**
 * \brief       tries to read value of Color sensor 
 * \param[out]  numerical value for Color if detected
 * \param       maxNumberColors Number of colors available
 *
 * \return      0 if reading suceeded, otherwise return -1
 */
extern int read_color( int *value, int maxNumberColors );

#endif
