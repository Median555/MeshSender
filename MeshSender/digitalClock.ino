#include <Time.h>
#define TIME_HEADER "T"


void digitalClockDisplay()
  {
    DrawClear(vram);
    
    String urHour = String(hour(), DEC);
    char urHourText[50];
    urHour.toCharArray(urHourText, 50);
    DrawString(vram,urHourText,43,0);
    
    DrawString(vram, ":",54,0);
    
    String urMin = String(minute(), DEC);
    char urMinText[50];
    urMin.toCharArray(urMinText, 50);
    DrawString(vram,urMinText,58,0);
    
    DrawString(vram, ":",69,0);
    
    String urSec = String(second(), DEC);
    char urSecText[50];
    urSec.toCharArray(urSecText, 50);
    DrawString(vram,urSecText,73,0);
    
    LCDUpdate(vram);
  }
  
time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}
  
unsigned long processSyncMessage() 
{
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 
  
  if(Serial.find(TIME_HEADER)) 
  {
   pctime = Serial.parseInt();
   return pctime;
   if( pctime < DEFAULT_TIME) 
   { // check the value is a valid time (greater than Jan 1 2013)
     pctime = 0L; // return 0 to indicate that the time is not valid
   }  
  
  }
}

