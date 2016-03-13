/*****************************************************************************/
// IO via Bluetooth LE on the Genuino 101 (LE stack on the Intel Curie chip)
// Author: Anthony Abbot
// Date: 30 Jan 2016
// 
// This code has been developed for use by the Olney Coder Dojo club
//
// This requires the following components:
//  - An Arduino Genuino 101
//
//
/*****************************************************************************/

#include <CurieBLE.h>
#include <CurieUart.h>
#include <CuriePing.h>

CuriePing pinger(2, 3);

CurieUart uart("dojoCurie");    // Create a named UART peripheral
String str;                     // Data read from the local serial input
String data;                    // Data received from the LE device


/*****************************************************************************/
// Setup function - runs once at startup
void setup() 
{
  while(!Serial);
  Serial.begin(9600);  
  
  Serial.print("Initialising UART...");
  uart.begin();
  Serial.println("Done");
}

/*****************************************************************************/
// Main loop - runs repeatedly
void loop() 
{  
  BLECentral central = uart.central();

  if (central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // Loop while connected
    while (central.connected()) 
    {
      // read if there is any data on the serial console and send it on the uart channel
      if(Serial.available() > 0)
      {
          str = Serial.readStringUntil('\n');
          uart.sendString(str);
          Serial.print("--> ");
          Serial.println(str);
      }
      
      // Check if the uart has a string and print to the serial console
      if (uart.hasString())
      {
        data = uart.getString();
        Serial.print("<-- ");
        Serial.println(data);

        int dist = pinger.ping();
        String ret{">" + data};
        uart.sendString(ret);
        ret = "dst=" + String(dist);
        uart.sendString(ret);
      } 
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
  
}
