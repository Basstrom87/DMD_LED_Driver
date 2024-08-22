
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "Arial_black_16.h"
#include <SoftwareSerial.h>
#include "Arial14.h"

//Fire up the DMD library as dmd
DMD dmd(1,1);

SoftwareSerial espSerial(2,3);

String recvData = "";
String oldTime = "";

void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}


void setup(void)
{
   
   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 3000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
   dmd.selectFont(Arial_14);

  Serial.begin(9600);
  espSerial.begin(9600);
  Serial.println("End Setup Loop");

}


void loop(void){
  // Recieve data
  if (espSerial.available()) {
    // Receive a single character from the software serial port
    char character = espSerial.read(); 
     // Add the received character to the receive buffer
    recvData.concat(character);
    
    // Read until the new line character
    if (character == '\n') {
      // Check to see if time has updated, if time has changed, print to serial console and update display
      if (recvData != oldTime) {
        Serial.print("Time: ");
        Serial.println(recvData);
        oldTime = recvData;
        dmd.clearScreen (true);

        char __recvData[recvData.length()+1];
        recvData.toCharArray(__recvData, sizeof(__recvData));
        
        dmd.drawString(2, 2, __recvData, 5, GRAPHICS_NORMAL);
      }
    // Clear receive buffer so we're ready to receive the next line
    recvData = "";
    }
  }
}
