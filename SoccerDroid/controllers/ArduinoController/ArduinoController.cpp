 
 // ====================================================================================
// Webots Arduino Interface (wai)
//   This program provides an interface between Arduino code and a Webots robot
//   so that the simulated robot can be controlled using unaltered Arduino code for 
//   a similar real robot
//
// Module: ArduinoController
//         Controller entry point
//         Michael Schofield March 2022
//         UNSW Computer Science & Engineering
// ====================================================================================

//-------------------------------------------------------------------------------------
// Webots Arduino Interface Libraries
//-------------------------------------------------------------------------------------
#include <waiDefinitions.hpp>
#include <waiArduino.hpp>
#include <waiL298N.hpp>
#include <waiHCSR04.hpp>
#include <waiPixy2.hpp>
#include <Arduino.ino>

// ====================================================================================
// Entry point for controller
// ====================================================================================
int main(int argc, char **argv) {

  int timeStep;

//-------------------------------------------------------------------------------------
// Initialise the robot
//-------------------------------------------------------------------------------------

  // Initialise the robot
  robot = new Robot();
  timeStep = (int)robot->getBasicTimeStep();

  // Make logical connections between Webots devices and Arduino pins
  makeConnections();
  
//-------------------------------------------------------------------------------------
// Run Arduino code
//-------------------------------------------------------------------------------------

  // Run the Arduino setup()
  setup();

  while (robot->step(timeStep) != -1) {

    // Run the Arduino loop()
    loop();
    
    // Advance the virtual clock
    virtualTimeStep++;

  }
  
//-------------------------------------------------------------------------------------
  
  // Tidy up
  if (pixy2Cam != NULL) delete pixy2Cam;
  delete robot;

  return 0;
  
}