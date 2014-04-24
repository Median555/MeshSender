
#include <math.h>

float J1Xmin = 512 - 0.5 * JOYSTICK_DEADZONE; //joystick 1, x-axis minimum
float J1Xmax = 512 + 0.5 * JOYSTICK_DEADZONE; 
float J1Ymin = 512 - 0.5 * JOYSTICK_DEADZONE;
float J1Ymax = 512 + 0.5 * JOYSTICK_DEADZONE;
float J2Xmin = 512 - 0.5 * JOYSTICK_DEADZONE; //joystick 2, x-axis minimum
float J2Xmax = 512 + 0.5 * JOYSTICK_DEADZONE; 
float J2Ymin = 512 - 0.5 * JOYSTICK_DEADZONE;
float J2Ymax = 512 + 0.5 * JOYSTICK_DEADZONE;

//joystickNumber is the joystick we wan't to check
uint8_t joystickSector(uint8_t joystickNumber)
{
  float xin, yin;
  float x, y, angle;
  const int deadzone = JOYSTICK_DEADZONE;
  
  if (joystickNumber == 1)
  {
    xin = analogRead(JOYSTICK_1_X_PIN);
    yin = analogRead(JOYSTICK_1_Y_PIN);
    
    J1Xmin = min(J1Xmin, xin);
    J1Xmax = max(J1Xmax, xin);
    J1Ymin = min(J1Ymin, yin);
    J1Ymax = max(J1Ymax, yin);
    
    x = xin - ( (J1Xmax - J1Xmin)/2 );
    y = yin - ( (J1Ymax - J1Ymin)/2 );
    
    if (sqrt(x * x + y * y) < deadzone) 
      return -1;
    
    //rotate vector input 22.5 degrees
    float xtemp = x * cos(PI/8) - y * sin(PI/8);
    float ytemp = x * sin(PI/8) + y * cos(PI/8);
    x = xtemp;
    y = ytemp;
    
    angle = atan2(y, x);
    angle *= 180/PI; //convert to degress
    angle = (angle * -1) + 180;
    
    return ((int)angle) / 45;
  }
  
  if (joystickNumber == 2)
  {
    xin = analogRead(JOYSTICK_2_X_PIN);
    yin = analogRead(JOYSTICK_2_Y_PIN);
    
    J2Xmin = min(J2Xmin, xin);
    J2Xmax = max(J2Xmax, xin);
    J2Ymin = min(J2Ymin, yin);
    J2Ymax = max(J2Ymax, yin);
    
    x = xin - ( (J2Xmax - J2Xmin)/2 );
    y = yin - ( (J2Ymax - J2Ymin)/2 );
    
    if (sqrt(x * x + y * y) < deadzone)
      return -1;
    
    //rotate vector input 22.5 degrees
    float xtemp = x * cos(PI/8) - y * sin(PI/8);
    float ytemp = x * sin(PI/8) + y * cos(PI/8);
    x = xtemp;
    y = ytemp;
    
    angle = atan2(y, x);
    angle *= 180/PI; //convert to degress
    angle = (angle * -1) + 180;
    
    return ((int)angle) / 45;
  }
  
  return -1;
}
