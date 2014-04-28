
#include <math.h>
float J1Xmin = 512 - 0.5 * JOYSTICK_DEADZONE; //joystick 1, x-axis minimum
float J1Xmax = 512 + 0.5 * JOYSTICK_DEADZONE; //joystick 1, x-axis maximum
float J1Ymin = 512 - 0.5 * JOYSTICK_DEADZONE; //joystick 1, y-axis minimum
float J1Ymax = 512 + 0.5 * JOYSTICK_DEADZONE; //joystick 1, y-axis maximum
float J2Xmin = 512 - 0.5 * JOYSTICK_DEADZONE; //joystick 2, x-axis minimum
float J2Xmax = 512 + 0.5 * JOYSTICK_DEADZONE; //joystick 2, x-axis maximum
float J2Ymin = 512 - 0.5 * JOYSTICK_DEADZONE; //joystick 2, y-axis minimum
float J2Ymax = 512 + 0.5 * JOYSTICK_DEADZONE; //joystick 2, y-axis maximum


//joystickNumber is the joystick we wan't to check
//Method can give us number of section for Joystick 1 or 2
//by knowing angles we can turn it into sections
int joystickSector(uint8_t joystickNumber, uint8_t sectorCount)
{
  float xin, yin;
  float x, y, angle, rotation = - PI / sectorCount; //Makes variables and create the variable rotation as a calculation because we wan't our sections to start when we got fx right.
  const int deadzone = JOYSTICK_DEADZONE; // Creation of deadzone. This deadzone is the zone were our joystick shall return -1
  
  if (joystickNumber == 1)  //For Joystick 1
  {
    xin = analogRead(JOYSTICK_1_X_PIN);
    yin = analogRead(JOYSTICK_1_Y_PIN);
    
    J1Xmin = min(J1Xmin, xin);  //Takes the lowest number of the values J1Xmin, xin, and uses as the J1Xmin.
    J1Xmax = max(J1Xmax, xin);  //Takes the highest number of the values J1Xmax, xin, and uses as the J1Xmax.
 
    J1Ymin = min(J1Ymin, yin);  //Takes the lowest number of the values J1Ymin, yin, and uses as the J1Ymin.
    J1Ymax = max(J1Ymax, yin);  //Takes the highest number of the values J1Ymax, yin, and uses as the J1Xmax.
    
    //To get the correct direction for our vector.
    x = xin - ( (J1Xmax - J1Xmin)/2 );  //Our calculation of our x-value for our vector.
    y = yin - ( (J1Ymax - J1Ymin)/2 );  //Our calculation of our y-value for our vector.
    
    if (sqrt(x * x + y * y) < deadzone) //If our vector lenght is smaller than our deadzones radius just return -1.
      return -1;
    
    //rotate vector input 22.5 degrees
    float xtemp = x * cos(rotation) - y * sin(rotation);
    float ytemp = x * sin(rotation) + y * cos(rotation);
    x = xtemp;
    y = ytemp;
    
    angle = atan2(y, x);
    angle *= 180/PI; //convert to degress
    angle = (angle * -1) + 180;
    
    return ((int)angle) / (360 / sectorCount);
  }
  
  if (joystickNumber == 2)  //For Joystick 2
  {
    xin = analogRead(JOYSTICK_2_X_PIN);
    yin = analogRead(JOYSTICK_2_Y_PIN);
    
    J2Xmin = min(J2Xmin, xin);  //Takes the lowest number of the values J2Xmin, xin, and uses as the J2Xmin.
    J2Xmax = max(J2Xmax, xin);  //Takes the highest number of the values J2Xmax, xin, and uses as the J2Xmax.
    
    J2Ymin = min(J2Ymin, yin);  //Takes the lowest number of the values J2Ymin, yin, and uses as the J2Ymin.
    J2Ymax = max(J2Ymax, yin);  //Takes the highest number of the values J2Ymax, yin, and uses as the J2Xmax.
    
    //To get the correct direction for our vector.
    x = xin - ( (J2Xmax - J2Xmin)/2 );  //Our calculation of our x-value for our vector.
    y = yin - ( (J2Ymax - J2Ymin)/2 );  //Our calculation of our y-value for our vector.
    
    if (sqrt(x * x + y * y) < deadzone)  //If our vector lenght is smaller than our deadzones radius just return -1.
      return -1;
    
    //rotate vector input 22.5 degrees
    float xtemp = x * cos(rotation) - y * sin(rotation);
    float ytemp = x * sin(rotation) + y * cos(rotation);
    x = xtemp;
    y = ytemp;
    
    angle = atan2(y, x);  //Calculate the angle between the positive x-axis and our vector in radians.
    angle *= 180/PI; //convert to degress
    angle = (angle * -1) + 180; // Flips it to positive direction of our angles.
    
    return ((int)angle) / (360 / sectorCount);
  }
  
  return -1;
}
