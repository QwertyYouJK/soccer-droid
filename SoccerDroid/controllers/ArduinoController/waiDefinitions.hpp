// ====================================================================================
// Webots Arduino Interface (wai)
//   This program provides an interfave between Arduino code and a Webots robot
//   so that the simulated robot can be controlled using unaltered Arduino code for 
//   a similar real robot
//
// Module: waiDefinitions
//         Program definitions and global objects
//         Michael Schofield March 2022
//         UNSW Computer Science & Engineering
// ====================================================================================

//-------------------------------------------------------------------------------------
// Libraries
//-------------------------------------------------------------------------------------
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/Camera.hpp>

using namespace std;
using namespace webots;

//-------------------------------------------------------------------------------------
// Definitions
//-------------------------------------------------------------------------------------
#define TIME_STEP 64
#define MAX_SPEED 6.28

//-------------------------------------------------------------------------------------
// Webots devices
//-------------------------------------------------------------------------------------
struct devicePinConnection {
  string deviceDef;
  int pin1;
  int pin2;
  int pin3;
};
  
Robot* robot = NULL;
Camera* pixy2Cam = NULL;
vector < devicePinConnection > devicePinConnections;

//-------------------------------------------------------------------------------------
// Webots virtual time
//-------------------------------------------------------------------------------------
int virtualTimeStep = 0;


