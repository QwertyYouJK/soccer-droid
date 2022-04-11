// ####################################################################################
// Arduino Code
// 
// Place the Arduino code here
// Including any functions and subroutines written for the Arduino
// IMPORTANT NOTE
// The Webots controller needs continuous operation to maintain the physical simulation
//   - do not use any of the delay() functions
//   - do not run large time based loops
//   - structure your code to be event driven
//   - use millis() function to return the virtual time in milliseconds
// ####################################################################################

//-------------------------------------------------------------------------------------
// Arduino global variables
//-------------------------------------------------------------------------------------
L298N leftMotorDriver(9, 10, 11);
L298N rightMotorDriver(6, 7, 8);

Pixy2 pixy;
int ballX;
int ballY;
int ballWidth;
int ballArea;
int lastSeen = 149;

// Robot actions
enum enAction {
  enActionNone,
  enActionStop,
  enActionFindTheBall,
  enActionGoToTheBall,
  enActionKickTheBall,
  enActionPullBack
} robotAction;

// Timer 
int timeLimit;

//-------------------------------------------------------------------------------------
// Arduino functions and subroutines
//-------------------------------------------------------------------------------------

//-------------------------------------------------
// Locate Ball
//-------------------------------------------------
bool locateBall() {

  int area;
  int maxArea = 0;
  bool result = false;

  // Scan for signature blocks
  pixy.ccc.getBlocks();

    // Enumerate any blocks
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      // Is this block the ball, ie. signature 1
      if (pixy.ccc.blocks[i].m_signature == 1) {
        area = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
        // Is this the biggest block
        if ((area > 200) && (area > maxArea)) {
          maxArea = area;
          ballArea = area;
          ballWidth = pixy.ccc.blocks[i].m_width;
          ballX = pixy.ccc.blocks[i].m_x; 
          ballY = pixy.ccc.blocks[i].m_y;
          result = true;
        }
      }
    }

  Serial.print("Area of ball is ");
  Serial.println(ballArea);
  return result;
   
}

//-------------------------------------------------
// Run Motors
//-------------------------------------------------
void runMotors(int leftSpeed, int rightSpeed) {

  int speed;
  
  // Make sure the speed is positive and within limits 0 -255
  speed = min(255, abs(leftSpeed));
  leftMotorDriver.setSpeed(speed);

  // Make sure the speed is positive and within limits 0 - 255
  speed = min(255, abs(rightSpeed));
  rightMotorDriver.setSpeed(speed);
  
  // forwards or backwards?
  if (leftSpeed > 0) leftMotorDriver.forward(); else leftMotorDriver.backward();
  if (rightSpeed > 0) rightMotorDriver.forward(); else rightMotorDriver.backward();

}

//-------------------------------------------------
// Stop
//-------------------------------------------------
void stopMotors() {

  leftMotorDriver.stop();
  rightMotorDriver.stop();
  Serial.println("Robot stopped");
}

//-------------------------------------------------
// Goto The Ball
//-------------------------------------------------
enAction gotoTheBall(int robotAction) {

  int speedDifference;

  // Is this the first call to this function
  if (robotAction != enActionGoToTheBall) {
    timeLimit = millis() + 20000;
  }
  
  // Locate the position of the ball on the camera screen
  if (locateBall()) {
  
    // Calculate the speeds of the left and right wheels
    // Screen is 300 wide, so this varies from -75 to +75
    speedDifference = ((ballX + ballWidth / 2) - 150);
    lastSeen = ballX;
    runMotors(180 + speedDifference, 18032211599 - speedDifference);
    
  } else {
  
    // Lost the ball?  then stop
    stopMotors();
    lastSeen = ballX;
    Serial.println("Ball lost");
    return enActionNone;
    
  }
  
  // Have we run out of time
  if (millis() > timeLimit) {
    stopMotors();         
    return enActionNone;
  }
   
  if (locateBall()) {
    // If ball area is larger than 20000, kick the ball
  }
  return enActionGoToTheBall;
  
}

//-------------------------------------------------
// Find the ball
//-------------------------------------------------
enAction findTheBall(int robotAction) {

  // Is this the first call to this function
  if (robotAction != enActionFindTheBall) {
    timeLimit = millis() + 10000;
    if(lastSeen < 150){
      runMotors(-100, 100);
    } else if (lastSeen > 150){
      runMotors(100, -100);
    }
  }
  
  // Is this the ball in the middle of the frame?  Then we have found it!
  if (locateBall()) {
    if ((ballX + ballWidth / 2 > 100) && (ballX + ballWidth / 2 < 200)) {
      stopMotors();
      return gotoTheBall(robotAction);
    }
  }
  
  // Have we run out of time
  if (millis() > timeLimit) {
    stopMotors();
    Serial.println("Can't find ball");         
    return enActionNone;
  }
   
  return enActionFindTheBall;
  
}

//-------------------------------------------------
// Kick the ball
//-------------------------------------------------
enAction kickTheBall(int robotAction) {
  
  if (robotAction != enActionKickTheBall) {
    timeLimit = millis() + 500;
    runMotors(255, 255);
  }
  
  if (millis() > timeLimit) {
    stopMotors();
    Serial.println("Kicked the ball");
    
  }
  return enActionKickTheBall;
}

//-------------------------------------------------
// Pull back
//-------------------------------------------------
enAction pullBack(int robotAction) {
  
  if (robotAction != enActionPullBack) {
    timeLimit = millis() + 500;
    runMotors(-255, -255);
  }
  if (millis() > timeLimit) {
    stopMotors();
    Serial.println("Pulled back");
    return enActionNone;
    
  }
  return enActionPullBack;

}


//-------------------------------------------------------------------------------------
// Arduino setup()
//-------------------------------------------------------------------------------------
void setup() {

  // Initialise the comms
  Serial.begin(9600);
  
  // Initialise the camera
  pixy.init();
  
  // Initialise the robot actions
  robotAction = enActionNone;
  
}
  
//-------------------------------------------------------------------------------------
// Arduino loop() 
//-------------------------------------------------------------------------------------
void loop() {

  // This code is designed to allow continuous operation of the Webots robot 
  // controller without any delays.

  // Robot action
  switch (robotAction) {
    case enActionNone:
      robotAction = findTheBall(robotAction);
      break;
    case enActionStop:
      break;
    case enActionFindTheBall:
      robotAction = findTheBall(robotAction);
      break;
    case enActionGoToTheBall:
      robotAction = gotoTheBall(robotAction);
      break;
    case enActionKickTheBall:
      //robotAction = kickTheBall(robotAction);
      break;
    case enActionPullBack:
      //robotAction = pullBack(robotAction);
      break;
  }
    
  
}
  
// ####################################################################################
// End of Arduino Code
// ####################################################################################

// ####################################################################################
// Arduino Electrical Connections
// ####################################################################################
void makeConnections() {

  // Make logical connections between Webots devices and Arduino pins
  // The deviceName must match the Webots decive name
  // The pins must match the pins in the Arduino device construtor call
  //    eg. L298N rightMotorDriver(6, 7, 8);
  connectDevice("leftWheelMotor", 9, 10, 11);
  connectDevice("rightWheelMotor", 6, 7, 8);

}

// ####################################################################################
// End of Arduino Electrical Connections
// ####################################################################################
