// ====================================================================================
// Webots Arduino Interface (wai)
//   This program provides an interfave between Arduino code and a Webots robot
//   so that the simulated robot can be controlled using unaltered Arduino code for 
//   a similar real robot
//
// Module: waiSerial
//         Mimics the Arduino Library - Serial
//         Michael Schofield March 2022
//         UNSW Computer Science & Engineering
// ====================================================================================

//-------------------------------------------------------------------------------------
// Libraries
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
// Header
//-------------------------------------------------------------------------------------
class SerialCom {
public:
  SerialCom();
  ~SerialCom();
  void begin(int baud);
  void print(int value);
  void print(double value);
  void print(string value);
  void println(int value);
  void println(double value);
  void println(string value);
  
};

//-------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------
SerialCom::SerialCom() {


}


//-------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------
SerialCom::~SerialCom() {

  // Free the resources
  
}

//-------------------------------------------------------------------------------------
// begin
//-------------------------------------------------------------------------------------
void SerialCom::begin(int baud) {

  // Initialise the serial comms
  printf("Serial coms initialised at %d\n", baud);
  
}

//-------------------------------------------------------------------------------------
// print
//-------------------------------------------------------------------------------------
void SerialCom::print(int value) {

  printf("%d", value);
  
}

void SerialCom::print(double value) {

  printf("%f", value);
  
}

void SerialCom::print(string value) {

  printf("%s", value.c_str());
  
}

//-------------------------------------------------------------------------------------
// println
//-------------------------------------------------------------------------------------
void SerialCom::println(int value) {

  printf("%d\n", value);
  
}

void SerialCom::println(double value) {

  printf("%f\n", value);
  
}

void SerialCom::println(string value) {

  printf("%s\n", value.c_str());
  
}

//-------------------------------------------------------------------------------------
// Serial
//-------------------------------------------------------------------------------------
SerialCom Serial;
