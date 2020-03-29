#ifndef CLIENT_H
#define CLIENT_H
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/socket.h>
#include <math.h>
#include <bluetooth/bluetooth.h> 
#include <bluetooth/rfcomm.h>

#define SERV_ADDR   "2C:33:7A:3A:BA:D4"     /* Whatever the address of the server is */
#define TEAM_ID     8                       /* Your team ID */

#define MSG_ACK     0
#define MSG_START    1
#define MSG_STOP   2
#define MSG_KICK    3
#define MSG_SCORE 	    4
#define MSG_CUSTOM 	    8

#define Sleep( msec ) usleep(( msec ) * 1000 )


extern void robot ();

extern int read_from_server (int sock, char *buffer, size_t maxSize);

extern int connect_to_server(void);

extern int send_score_to_server(int points);

extern int robot_kickout_or_game_end(void);

/**
 * \listen the server;
 *
 * \param sn defines which motor is to be inspected
 * \return 0 is the game end, 1 otherwise
 */

#endif
