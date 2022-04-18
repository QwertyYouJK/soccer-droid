//####################################################################################
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

// Ball variables
int ballX;
int ballY;
int ballWidth;
int ballArea;

// Goal variables
int goalX;
int goalY;
int goalWidth;
int goalArea;

int lastSeen = 149;

// Robot actions
enum enAction {
  enActionNone,
  enActionStop,
  enActionFindTheBall,
  enActionGoToTheBall,
  enActionKickTheBall,
  enActionFindGoal,
  enActionGoToGoal,
  enActionTurnAround
} robotAction;

// Timer 
int timeLimit;
int lostTime = 0;


// Go to ball flag
bool gbflag = false;

// Go to goal flag
bool ggflag = false;

// Find goal flag
bool fgflag = false;

// Turn around flag
int turnflag = 0;

//-------------------------------------------------------------------------------------
// Arduino functions and subroutines
//-------------------------------------------------------------------------------------

//-------------------------------------------------
// Locate Ball
//-------------------------------------------------
bool locateBall() {

  int area;
  bool result = false;

  // Scan for signature blocks
  pixy.ccc.getBlocks();

    // Enumerate any blocks
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      // Is this block the ball, ie. signature 1
      if (pixy.ccc.blocks[i].m_signature == 1) {
        area = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
        ballArea = area;
        ballWidth = pixy.ccc.blocks[i].m_width;
        ballX = pixy.ccc.blocks[i].m_x; 
        ballY = pixy.ccc.blocks[i].m_y;
        result = true;
      }
    }

  //Serial.print("Area of ball is ");
  //Serial.println(ballArea);
  return result;
   
}

//-------------------------------------------------
// Locate my Goal
//-------------------------------------------------
bool locateGoal() {

  int area;
  bool result = false;

  // Scan for signature blocks
  pixy.ccc.getBlocks();

    // Enumerate any blocks
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      // Is this block the goal, ie. signature 2
      if (pixy.ccc.blocks[i].m_signature == 2) {
        area = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
        goalArea = area;
        goalWidth = pixy.ccc.blocks[i].m_width;
        goalX = pixy.ccc.blocks[i].m_x; 
        goalY = pixy.ccc.blocks[i].m_y;
        result = true;
      }
    }

  return result;
   
}

//-------------------------------------------------
// Locate opponent's Goal
//-------------------------------------------------
bool locateoppGoal() {

  int area;
  bool result = false;

  // Scan for signature blocks
  pixy.ccc.getBlocks();

    // Enumerate any blocks
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      // Is this block the goal, ie. signature 4
      if (pixy.ccc.blocks[i].m_signature == 4) {
        area = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
        goalArea = area;
        goalWidth = pixy.ccc.blocks[i].m_width;
        goalX = pixy.ccc.blocks[i].m_x; 
        goalY = pixy.ccc.blocks[i].m_y;
        result = true;
      }
    }

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
// Stop (enActionStop)
//-------------------------------------------------
void stopMotors() {
  // Stop the motors
  leftMotorDriver.stop();
  rightMotorDriver.stop();
}

//-------------------------------------------------
// Find the ball (enActionFindTheBall)
//-------------------------------------------------
enAction findTheBall(int robotAction) {

  int speedDifference;

  // Is this the first call to this function
  if (robotAction != enActionFindTheBall) {
    timeLimit = millis() + 20000;
  }
  
  // Locate the position of the ball on the camera screen
  if (locateBall()) {
  
    // Calculate the speeds of the left and right wheels
    // Screen is 300 wide, so this varies from -150 to +150
    speedDifference = (ballX - 150) / 2;
    lastSeen = ballX;
    runMotors(0 + speedDifference, 0 - speedDifference);
    
    // If ball is close, begin defend the goal
    if (ballArea > 1500) {
       return enActionGoToTheBall;
    }
    
  } else {
  
    // Lost the ball?  then stop
    stopMotors();
    lastSeen = ballX;
    //Serial.println("Ball lost");
    return enActionNone;
    
  }
  
  // Have we run out of time
  if (millis() > timeLimit) {
    stopMotors();         
    return enActionNone;
  }

  return enActionFindTheBall;
  
}

//-------------------------------------------------
// Goto The Ball (enActionGoToTheBall)
//-------------------------------------------------
enAction gotoTheBall(int robotAction) {

  int speedDifference;

  // Is this the first call to this function
  if (!gbflag) {
    timeLimit = millis() + 2000;
    gbflag = true;
  }
  
  // Have we run out of time
  if (millis() > timeLimit) {
    gbflag = false;
    return enActionKickTheBall;
  }
  
  // Locate the position of the ball on the camera screen
  if (locateBall()) {
  
    // Calculate the speeds of the left and right wheels
    // Screen is 300 wide, so this varies from -75 to +75
    speedDifference = (ballX - 150) / 2;
    runMotors(180 + speedDifference, 180 - speedDifference);
    
  } else {
  
    // Lost the ball?  then stop
    stopMotors();
    lastSeen = ballX;
    Serial.println("Ball lost");
    lostTime = timeLimit - millis();
    gbflag = false;
    return enActionFindGoal;
    
  }

  return enActionGoToTheBall;
  
}

//-------------------------------------------------
// Find the Goal (enActionFindGoal)
//-------------------------------------------------
enAction findGoal(int robotAction) {
    
  // Is this the first call to this function
  if (!fgflag) {
    timeLimit = millis() + 10000;
    fgflag = true;
    // Spin to find goal
    runMotors(-100, 100);
  }
  
  // Is this the ball in the middle of the frame?  Then we have found it!
  if (locateGoal()) {
    if ((goalX > 120) && (goalX < 180)) {
      stopMotors();
      fgflag = false;
      return enActionGoToGoal;
    }
  }
  
  // Have we run out of time
  if (millis() > timeLimit) {
    stopMotors();
    Serial.println("Can't find ball");         
    return enActionNone;
  }
   
  return enActionFindGoal;

}

//-------------------------------------------------
// Go back to the Goal (enActionGoToGoal)
//-------------------------------------------------
enAction gotoGoal(int robotAction){
  
  int speedDifference;
  
  // Is this the first call to this function
  if (!ggflag) {
    if (lostTime > 0){
      timeLimit = millis() + lostTime;
      ggflag = true;
    } else {
      timeLimit = millis() + 2500;
      ggflag = true;
    }
    
  }
  
  // Have we run out of time
  if (millis() > timeLimit) {
    ggflag = false;
    lostTime = 0;
    return enActionTurnAround;
  }
  
  // Locate the position of the ball on the camera screen
  if (locateGoal()) {
  
    // Calculate the speeds of the left and right wheels
    speedDifference = goalX - 150;
    runMotors(180 + speedDifference, 180 - speedDifference);
  }

  return enActionGoToGoal;

}

//-------------------------------------------------
// Turn Around (enActionTurnAround)
//-------------------------------------------------
enAction turnAround(int robotAction){

  // First go back
  if (turnflag == 0) {
    timeLimit = millis() + 300;
    runMotors(-255,-255);
    turnflag = 1;
  }
  
  // Second turn around 180
  if (turnflag == 2) {
    runMotors(255,-255);
    turnflag = 3;
  }
  
  // Have we run out of timeLimit
  if (millis() > timeLimit && turnflag == 1) {
    stopMotors();
    turnflag = 2;
  }
  
  // If opponent's goal is at centre, stop turning
  if (locateoppGoal() && turnflag == 3) {
    if (locateBall()){
      stopMotors();
      turnflag = 0;
      return enActionNone;
    } else if ((goalX + goalWidth / 2 > 100) && (goalX + goalWidth / 2 < 200)) {
      stopMotors();
      turnflag = 0;
      return enActionNone;
    }
  }

  
  
  return enActionTurnAround;

}

//-------------------------------------------------
// Kick the ball (enActionKickTheBall)
//-------------------------------------------------
enAction kickTheBall(int robotAction) {
  
  Serial.println("Kick the ball");
  stopMotors();
  return enActionFindGoal;

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
      robotAction = kickTheBall(robotAction);
      break;
    case enActionFindGoal:
      robotAction = findGoal(robotAction);
      break;
    case enActionGoToGoal:
      robotAction = gotoGoal(robotAction);
      break;
    case enActionTurnAround:
      robotAction = turnAround(robotAction);
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

//Test edit