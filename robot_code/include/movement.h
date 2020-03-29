#ifndef MOVEMENT_H
#define MOVEMENT_H
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include "../ev3dev-c/source/ev3/ev3.h"

#define motor_Port_A 65
#define motor_Port_B 66
#define motor_Port_C 67
#define motor_Port_D 68

#define MOTOR_PORT_L     motor_Port_B
#define MOTOR_EXT_PORT_L EXT_PORT__NONE_

#define MOTOR_PORT_R     motor_Port_C
#define MOTOR_EXT_PORT_R EXT_PORT__NONE_

#define PORT_ARM         motor_Port_A
#define EXT_PORT_ARM     EXT_PORT__NONE_

#define PORT_HAND        motor_Port_D
#define EXT_PORT_HAND    EXT_PORT__NONE_

int     max_speed;
uint8_t sn;

/**
 * \brief gets maximum speed;
 *
 * \param sn defines which motor is to be inspected
 * \return 0 if successful, -1 otherwise
 */
int get_max_speed( uint8_t sn );

/**
 * \brief checks if all motors are connected; if yes, reset them; has to be called before using motors
 * 
 * \return 0 if initialization was succesful, -1 otherwise
 */
extern int init_motors( void );

/**
 * \brief Run motors for moving without time limit
 *
 * \param speed_l speed for left wheel relative to maximum speed (in percentage out of 100)
 * \param speed_r speed for right wheel relative to maximum speed (in percentage out of 100)
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int run_motors_without_stopping( int speed_l, int speed_r );

/**
 * \brief Stops motors
 * 
 * \return void
 */
extern void stop_motors( void );

/**
 * \brief runs motor to relative position
 *
 * \param speed_l speed for left wheel relative to maximum speed (in percentage out of 100)
 * \param pos_l   position for left wheel
 * \param speed_r speed for right wheel relative to maximum speed (in percentage out of 100)
 * \param pos_r   position for right wheel
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int run_motors_to_relative_position( int speed_l, int pos_l, int speed_r, int pos_r );

/**
 * \brief Run motors for moving with time limit
 *
 * \param speed_l speed for left wheel relative to maximum speed (in percentage out of 100)
 * \param speed_r speed for right wheel relative to maximum speed (in percentage out of 100)
 * \param ms      duration to run the wheels
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int run_motors_timed( int speed_l, int speed_r, int ms );

/**
 * \brief Turns robot
 *
 * \param speed     defines speed for turn relative to maximum speed (in percentage out of 100)
 * \param direction turns clockwise for 1; turns counter-clockwise for 0
 * \param ms        duration of the turn
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int run_motors_turn( int speed, int direction, int ms);

/**
 * \brief Turns robot without limit
 *
 * \param speed     defines speed for turn relative to maximum speed (in percentage out of 100)
 * \param direction turns clockwise for 1; turns counter-clockwise for 0
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int run_motors_infinite_turn( int speed, int direction );

/**
 * \brief Turns robot for defined angle
 *
 * \param speed     defines speed for turn relative to maximum speed (in percentage out of 100)
 * \param direction turns clockwise for 1; turns counter-clockwise for 0
 * \param degree    angle of the turn
 * \param[out] minDistanceReturn Minimum distance during of the turn
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int turn_degree( int speed, int direction, int degree, float* minDistanceReturn);

/**
 * \brief Turns towards minimum distance (turns left first for 0.5*degree, then right for 1*degree and then left for 1*degree again)
 *
 * \param speed     defines speed for turn relative to maximum speed (in percentage out of 100)
 * \param degree    defines the angle of the sweep
 *
 * \return Returns 0 if robot found minimum Distance again and faces the minimum Distance. Returns -1 otherwise
 */
extern int turn_towards_minimum_Distance( int speed, int degree );

/**
 * \brief runs arm motor to relative position
 *
 * \param speed speed for arm motor relative to maximum speed (in percentage out of 100)
 * \param pos   position for arm
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int run_arm_to_relative_position( int speed, int pos );

/**
 * \brief runs hand motor to relative position
 *
 * \param speed speed for hand motor relative to maximum speed (in percentage out of 100)
 * \param pos   position for hand
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int run_hand_to_relative_position( int speed, int pos );


/**
 * \brief trys to find the ball, if it is a ball it calls go_to_the_ball()
 *
 * \param speed speed for wheels, relative to maximum speed (in percentage out of 100)
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int find_Ball(int speed);

/**
 * \brief Goes to the ball, if it would enter the opponent's field, it will go back
 *
 * \param speed speed for wheels, relative to maximum speed (in percentage out of 100)
 *
 * \return Returns 0 on success (with ball), -1 on failure (without ball, e.g. when ball is in enemy field)
 */
extern int go_to_the_ball( int speed );

/**
 * \brief open the hand
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int open_hand();

/**
 * \brief close the hand
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int close_hand();

/**
 * \brief grab the ball and put the arm up, avoid a wall going backward, reinitialize the position going forward once the ball is grabbed in case of a wall
 *
 * \param n : has to be 0
 *
 * \return Returns 0 on success without wall, 1 on suuccess with wall
 */
extern int grab_the_ball(int n);

/**
 * \brief drop the ball in the basket and put the arm at the initial position
 *
 * \return Returns 0 on success, -1 on failure
 */

extern int drop_the_ball();

/**
 * \brief throw the ball from the distant area and put the arm at the initial position
 *
 * \return Returns 0 on success, -1 on failure
 */
extern int drop_the_ball();

extern int throw_the_ball_dist();


extern int back_to_angular_position(int speed, float initial_angle, float ball_angle);

/**
 * \go to score from the center 
 *
 * \no param
 *
 * \return Returns 0 
 */
extern int go_straight_and_throw_ball(void);

/**
 * \brief use to come back in the center after scoring
 *
 * \return Returns 0 
 */
extern int go_back_from_basket_to_center(void);



extern int detect_line(void);

extern unsigned long long measure_time_to_reach_opposite_line(int speed);

extern int go_until_line_found(int speed);

extern int find_calibrage_angle(int speed);
#endif
