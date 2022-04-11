// ====================================================================================
// Webots Arduino Interface (wai)
//   This program provides an interfave between Arduino code and a Webots robot
//   so that the simulated robot can be controlled using unaltered Arduino code for 
//   a similar real robot
//
// Module: waiElectrical
//         Define all the device connections to Arduino pins
//         Michael Schofield March 2022
//         UNSW Computer Science & Engineering
// ====================================================================================


//-------------------------------------------------------------------------------------
// Libraries
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
// getDeviceName
//-------------------------------------------------------------------------------------
string getDeviceName(int pin1, int pin2, int pin3) {

  for (auto& connection: devicePinConnections) {
    if ((connection.pin1 == pin1) && (connection.pin2 == pin2) && (connection.pin3 == pin3)) {
      return connection.deviceDef;
    }
  }
  
  char msg[80];
  snprintf(msg, 79, "Devive not found for pins %d, %d, %d", pin1, pin2, pin3);
  throw runtime_error(msg); 
  
}

//-------------------------------------------------------------------------------------
// connectDevice
//-------------------------------------------------------------------------------------
void connectDevice(string deviceDef, int pin1, int pin2 = -1, int pin3 = -1) {

  devicePinConnection newConnection;
  
  newConnection.deviceDef = deviceDef;
  newConnection.pin1 = pin1;
  newConnection.pin2 = pin2;
  newConnection.pin3 = pin3;
  
  devicePinConnections.push_back(newConnection);

}

