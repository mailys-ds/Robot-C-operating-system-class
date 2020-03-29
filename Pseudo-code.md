# Algorithms

In this document, the most important algorithms will be presented in pseudo-code.

## How to play the game

The strategy of Justin is as follows

```c
    void main() {
        initialize all motors and sensors;
        take the current phase (the robot faces the basket);
        while (there are still balls in the arena) {
            find a ball;
            pick up the ball;
            score the ball;
        }
    }
```

How the single steps are achieved will be explained in the following subsections.

## How to find a ball
This algorithm combines two functions. One is to find a jump. The second function is used
to find the minimum distance of the found object, so the robot faces the middle of the ball.

```c
    int find_Ball() {
        while ( true ) {
            measure and save the current distance;
            turn right;
            while( true ) {
                measure and save the current distance;
                if ( difference between two previous measurements above a threshold )
                    break;
            }
            
            int result = turn_towards_minimum_Distance( speed, angle );
            if ( result == success)
                ball was found, return success
            else
                ball was not found, start from beginning of outer while loop;
        }
    }
    
    int turn_towards_minimum_Distance( int speed, int angle ) {
        set flag found_jump to false;
        turn left for half of the given angle with given parameter speed;
        turn right for the angle with given speed and save the minimum distance in minimum_distance;
        
        while( turned_value < angle ) {
            if (jump detected) {
                set flag found_jump to true;
            }
            if (measure_Distance() < (minimum_distance + margin) ) {
                break;
            }
            update turned_value;
        }
        
        if ( found_jump == true ) 
            return success;
        else
            return failure;
    }
```

## How to pick up a ball

This functions presumes, that the robot is facing the ball

```c
    void go_to_the_ball( int speed ) {
        start_time = current time in milliseconds;
        while ( measured distance < threshold ) {
            go forward with given speed;
        }
        taken_time = current time in milliseconds - start_time;
        grab_the_ball();
        go back with given speed for taken_time in milliseconds;
    }
    
    int grab_the_ball(void) {
	    while (the press sensor has not been pushed){
    		read the value of the press sensor;
            move the arm little by little
    	}
    	move to be a little bit further than the touch-sensor;
    	return success;
    }
```

## How to score a ball
The goal is to always score from the far area. The algorithm for doing so is as follows:

```c
    void main() {
        Start with the robot facing the basket
        Save the phase value of the gyroscope in the beginning
        while (balls are still in the arena) {
            Find and get a ball
            Turn the robot with the gyroscope so it faces the basket again
            go_to_second_line();
            throw ball into basket from far area;
        }
    }
    
    void go_to_second_line() {
        move robot forward without stopping;
        while (color sensor does not measure black) {
            keep going forward;
        }
        move for a certain distance in order to cross the black line;
        while (color sensor does not measure black) {
            keep going forward;
        }
        stop all motors;
    }
```