# Contributions

This file shows the contributions of the single team members.

## Colombe 
- Setting up the architecture of the robot
- Writing functions to
    - grab the ball
    - drop the ball
    - throw the ball
    - detect a black line
    - avoid entering the opponent field (not used because not included in our strategy)
    - go to the ball (1st version with threads, one for the detection of a black line, one for the movement towards the ball, not used because simpler way found)
- Writing the sensors and the architecture parts of the website
- Adding pseudo code for some algorithms
- Adding images and videos to the website


## Mailys
- Setting up the architecture of the robot
- Writing functions: 
     -to connect the sensors: set-up color-sensor mode, executable file to test the sensors 
     -related to the robot actions :
	 - score and throw the initial two balls, 
	 - come back to its initial position and angular position once the balls are scored,
	 - find_a_ball_score_and_go_back_to_initial_position() (not used but inspired)
     -to make the calibration of the robot: (finally not used because too uncertain and too long and mainly because our robot has already uncertainties with higher priority to work on)
	 -go forward until a black line in found
	 -measure the time to reach the opposite line
- Final tests and debbuging of the robot


## Thomas
- design the structure of the robot
- Writing functions to
    - find the Ball
    - turn towards the minimum distance in a certain area
    - go towards the ball until the robot is close enough to grab the ball
    - going back to the center after leaving the center for getting the ball
    - get angle relative to the position facing the basket
- Writing pseudo-code for most important algorthims
- writing the code to deal with the bluetooth server ()
- design the final sequences of action to play the game and 
- adding threads to deals with the server


## Jakob 
- Setting up the gitlab project
- Creating the directories for the project, including all files and the crosscompiling commands
- Writing all functions to read values from sensors
- Writing all functions to access actuators
- Writing functions to
    - initialize sensors and motors
    - move the robot (forwards, backwards, turns, timed, relative position)
    - detect black lines
    - find the Ball
    - turn towards the minimum distance in a certain area
    - go towards the ball until the robot is close enough to grab the ball
    - going back to the center after leaving the center for getting the ball
    - get angle relative to the position facing the basket
- Writing pseudo-code for most important algorithms
- Writing the README file
- Designing structure of the robot
