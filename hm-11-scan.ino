/*
*  Bluetooth LE device scanner using HM-11
*  Richard Hawthorn
*  6th June 2015
*/

#include <SoftwareSerial.h>
#include <Time.h>

//define software serial pins, for the HM-11 device
SoftwareSerial mySerial(2, 3); // RX, TX

//define string to collect serial data
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

//used for timing
int lastMinute = -1;

void setup()  
{
  //setup the serial ports
  Serial.begin(19200);
  mySerial.begin(19200);
  
  //reserve space for the incoming string data
  inputString.reserve(200);
  
  //let the user know the program has started
  Serial.println("Starting");
  
  //send setup bluetooth commands
  at("AT"); // check if working, always returns OK
  at("AT+ROLE1"); // select master = central
  at("AT+RESET"); // actually more a restart than a reset .. needed after ROLE
  at("AT+SHOW1"); // include Bluetooth name in response
  //at("AT+FILT0"); // show all BLE devices, not only HM ones
  delay(1000); // wait a bit, NECESSARY!!
  at("AT+IMME1"); // "work immediately", not sure what this does
  
}

//scan for devices
void scan(){
  
  Serial.println("Scanning");
  at("AT+DISC?");
  
}

//sending commands
void at(char* cmd) {
  
  mySerial.write(cmd);
  
  while(!mySerial.find("OK")){
    //probably shouldn't wait here forever
  };
  
}

//process the incoming serial string
void processString(String string){
  
 String deviceId = "";
 String deviceName = "";
  
 int startId = string.indexOf("+DIS1:") + 6;
 int endId = string.indexOf("+NAME:");
 int startName = endId + 6;
 int endName = string.indexOf("\n");
 
 deviceId = string.substring(startId, endId);
 deviceName = string.substring(startName, endName);
 
 Serial.println(deviceId);
 Serial.println(deviceName);
  
}
  
//check if we have any response from the bluetooth device
void checkBtResponse(){  
 
  while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  
  //if we have received a serial string
  if (stringComplete == true){
    processString(inputString);
    stringComplete = false;
    inputString = "";
  }
  
}

//run the scan every minute
void timer(){
  
  int minuteNow = minute();
  if (minuteNow != lastMinute){  
    lastMinute = minuteNow;
    scan();    
  }
  
}

void loop()
{
  timer(); 
  checkBtResponse();
}
