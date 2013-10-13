

#include <XOBXOB_Ethernet.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Time.h>

///////////////////////////////////////////////////////////
//
// Change these for your Ethernet Shield and your APIKey
//
// NOTE: the MAC address for your Ethernet Shield might be
// printed on a label on the bottom of the shield. Your
// APIKey will be found on your account dashboard when you
// login to XOBXOB)
//

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x97, 0xF9 };
String APIKey = "c37beb47-da43-4868-bf3e-23f957b9264c";

///////////////////////////////////////////////////////////


// Create XOBXOB object (for the Ethernet shield)
XOBXOB_Ethernet XOB (mac, APIKey);

// Variables for request timing
boolean lastResponseReceived = true;
long lastRequestTime = -20000;


// Create motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 1);
boolean blinds_open = false; //intital state of blinds are closed
boolean time_set = false;



void setup() { 
  
  // Initialize XOBXOB
  XOB.init();
  Serial.begin(9600);
  
  AFMS.begin(4500);  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  myMotor->setSpeed(1);  // 10 rpm
  
}

void loop()
{
    // updown = 1 for up, 0 for down
    //boolean updown = getupdown(XOB.getMessage("switch"));
    int waketime = getwaketime();//(XOB.getMessage("value")); 
    //Serial.println(waketime);
    //Serial.println(second());
    
    // Set time if it is not set.
    if (!time_set) {
       setthetime();
       time_set = true; 
    }
    
    //Check for roll up. Then Roll up blinds
   /* if (updown && blinds_open == false) {
      Serial.println("Rolling Up Blinds");
      rollup_blinds_fast();
      blinds_open = true;
    }
  */
    // check roll down conditions
  /*   if (!updown && blinds_open == true) {
      Serial.println("Rolling Down Blinds");
      rolldown_blinds_fast();
      blinds_open = false;
    }
    */
    // rise up blinds slowly if the wake time is + or - 1 minute and if the blinds are closed.
    Serial.println(waketime);
    Serial.println(getcurrenttime());
    Serial.println(blinds_open);
   if(waketime == getcurrenttime() && blinds_open == false){
        Serial.println("Natural Wakeup");
        rollup_blinds_slow();
        blinds_open = true; // blinds should be open now.
    }
    
    //rolldown blinds at 20:00
    if (blinds_open == true && getcurrenttime() >= 1200 ) {
      rolldown_blinds_fast();
      blinds_open = false;
    }

    
    
  }



int getcurrenttime (){
 int current_time = minute() + (hour() * 60);
 return current_time; 
}

void setthetime () {
  //input hour(0-24) and minute(0-60)
  setTime(01,27,0,22,9, 2013);
  
  
  // if xobxob is working, pull set time from xobxob
  //insert NTP code here if you have more ram
  /*int sethour = text.substring(1,3).toInt();
  int setminute = text.substring(3,5).toInt();
  setTime(sethour, setminute, 0, 22, 9, 2013); 
  //Serial.print(hour());
  //Serial.print(":");
  //Serial.print(minute());*/
  
}


boolean getupdown(String s) {
 if (s.equalsIgnoreCase("\"ON\"")) {
   boolean updown = true; // go up
   return updown;
 }
   else {
     boolean updown = false;
    return updown; 
    }
}

int getwaketime() {
  int wakehour = 10;
  int wakeminute = 00;
  int waketime = (wakehour * 60) + wakeminute;
  return waketime;
}

void rollup_blinds_slow() {
  myMotor->setSpeed(5);
  myMotor->step(2000, BACKWARD, MICROSTEP);
  myMotor->release();
}

void rollup_blinds_fast() {
  myMotor->setSpeed(100);
  myMotor->step(2000, BACKWARD, SINGLE);
  myMotor->release();
}

void rolldown_blinds_fast() {
  myMotor->setSpeed(100);
  myMotor->step(2000, FORWARD, SINGLE);
  myMotor->release();
}
