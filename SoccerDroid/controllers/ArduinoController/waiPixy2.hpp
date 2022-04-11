// ====================================================================================
// Webots Arduino Interface (wai)
//   This program provides an interfave between Arduino code and a Webots robot
//   so that the simulated robot can be controlled using unaltered Arduino code for 
//   a similar real robot
//
// Module: waiSerial
//         Mimics the Arduino Library - Pixy2
//         Michael Schofield March 2022
//         UNSW Computer Science & Engineering
// ====================================================================================

//-------------------------------------------------------------------------------------
// Libraries
//-------------------------------------------------------------------------------------
#include <webots/Supervisor.hpp>

//-------------------------------------------------------------------------------------
// Structures
//-------------------------------------------------------------------------------------
struct block {
  int m_signature;
  int m_height;
  int m_width;
  int m_x;
  int m_y;
};

//-------------------------------------------------------------------------------------
// Header
//-------------------------------------------------------------------------------------
class CCC {
public:
  CCC();
  ~CCC();
  void getBlocks();
  int numBlocks;
  block blocks[16];
  
};

//-------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------
CCC::CCC() {


}


//-------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------
CCC::~CCC() {

  // Free the resources
  
}

//-------------------------------------------------------------------------------------
// getBlocks
//-------------------------------------------------------------------------------------
void CCC::getBlocks() {

  const CameraRecognitionObject* objects;
  int numObjects;
  
  // Reset the block count
  numBlocks = 0;

  // Set the block characteristics
  numObjects = pixy2Cam->getRecognitionNumberOfObjects();
  objects = pixy2Cam->getRecognitionObjects();
  
  // Populate the blocks array
  for (int i = 0; (i < numObjects) && (i < 16); i++) {
    blocks[i].m_signature = atoi(objects[i].model);
    blocks[i].m_height = objects[i].size_on_image[1];
    blocks[i].m_width = objects[i].size_on_image[0];
    blocks[i].m_x = objects[i].position_on_image[0];
    blocks[i].m_y = objects[i].position_on_image[1];
    numBlocks = i + 1;
  }  
  
}

//-------------------------------------------------------------------------------------
// Header
//-------------------------------------------------------------------------------------
class Pixy2 {
public:
  Pixy2();
  ~Pixy2();
  void init();
  CCC ccc;
  
};

//-------------------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------------------
Pixy2::Pixy2() {


  
}


//-------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------
Pixy2::~Pixy2() {

  // Free the resources
  
}

//-------------------------------------------------------------------------------------
// begin
//-------------------------------------------------------------------------------------
void Pixy2::init() {

  if (robot == NULL) {
    printf("FAILED: pixy2Cam initialisation\n");
    return;
  }

  // Initialise the pixy2 camera
  pixy2Cam = robot->getCamera("pixy2Cam");
  pixy2Cam->enable(TIME_STEP);
  pixy2Cam->recognitionEnable(TIME_STEP);
  printf("pixy2Cam initialised\n");
  
 
}

