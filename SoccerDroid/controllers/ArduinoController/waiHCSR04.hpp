// ====================================================================================
// Webots Arduino Interface (wai)
//   This program provides an interfave between Arduino code and a Webots robot
//   so that the simulated robot can be controlled using unaltered Arduino code for 
//   a similar real robot
//
// Module: waiHCSR04
//         Mimics the Arduino Library - HCSR04 ultrasonic sensor by gamegine
//         Michael Schofield March 2022
//         UNSW Computer Science & Engineering
// ====================================================================================

//-------------------------------------------------------------------------------------
// Libraries
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
// Header
//-------------------------------------------------------------------------------------
class HCSR04 {
public:
  HCSR04(int out, int echo);			
  ~HCSR04();
  float dist();
private:
  int _out;	
  int _echo;
};

//-------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------
HCSR04::HCSR04(int out, int echo) {

  // Initialise the board
   _out = out;
   _echo = echo;

}


//-------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------
HCSR04::~HCSR04() {

  // Free the resources
  
}

