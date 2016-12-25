#include <Servo.h>
#include <Wire.h> // Must include Wire library for I2C
#include <SparkFun_MMA8452Q.h> // Includes the SFE_MMA8452Q library

Servo myservo;
MMA8452Q accel;

int pos = 0;
int left_tail_pos = 160;
int right_tail_pos = 100;
int current_tail_pos = 0; //0=LEFT 1=RIGHT
int velocity = 450;

unsigned long time;


void setup() {
  time = millis();
  myservo.attach(9); //we connect the servo to pin 9
  accel.init();
  
   //  2. Initialize with FULL-SCALE setting. You can set the scale
  //     using either SCALE_2G, SCALE_4G, or SCALE_8G as the value.
  //     That'll set the scale to +/-2g, 4g, or 8g respectively.
  //accel.init(SCALE_4G); // Uncomment this out if you'd like
  //  3. Initialize with FULL-SCALE and DATA RATE setting. If you
  //     want control over how fast your accelerometer produces
  //     data use one of the following options in the second param:
  //     ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12,
  //     ODR_6, or ODR_1. 
  //     Sets to 800, 400, 200, 100, 50, 12.5, 6.25, or 1.56 Hz.
  //accel.init(SCALE_2G, ODR_200);
}

void loop() {

  unsigned long current_time = millis();
  
  // Use the accel.available() function to wait for new data
  //  from the accelerometer.
  

  if (accel.available())
  {
    // First, use accel.read() to read the new variables:
    accel.read();

    // accel.read() will update two sets of variables.
    // * int's x, y, and z will store the signed 12-bit values
    //   read out of the accelerometer.
    // * floats cx, cy, and cz will store the calculated
    //   acceleration from those 12-bit values. These variables
    //   are in units of g's.
    // Check the two function declarations below for an example
    // of how to use these variables.
    printCalculatedAccels();
    //printAccels(); // Uncomment to print digital readings

    // The library also supports the portrait/landscape detection
    //  of the MMA8452Q. Check out this function declaration for
    //  an example of how to use that.
    //printOrientation();

    Serial.println(); // Print new line every time.
  }else{
    Serial.println("accle not available");
  }


  

  if ((current_time - time) >= velocity) {
    time = current_time;
    if (current_tail_pos == 0) {
      current_tail_pos = 1;
      myservo.write(right_tail_pos);
    } else {
      current_tail_pos = 0;
      myservo.write(left_tail_pos);
    }

  }

}

// The function demonstrates how to use the accel.x, accel.y and
//  accel.z variables.
// Before using these variables you must call the accel.read()
//  function!
void printAccels()
{
  Serial.print(accel.x, 3);
  Serial.print("\t");
  Serial.print(accel.y, 3);
  Serial.print("\t");
  Serial.print(accel.z, 3);
  Serial.print("\t");
}

// This function demonstrates how to use the accel.cx, accel.cy,
//  and accel.cz variables.
// Before using these variables you must call the accel.read()
//  function!
void printCalculatedAccels()
{
  Serial.print(accel.cx);
  Serial.print(",");
  Serial.print(accel.cy);
  Serial.print(",");
  Serial.println(accel.cz);
  //Serial.print("\t");
  //Serial.print(accel.cy, 3);
  //Serial.print("\t");
  //Serial.print(accel.cz, 3);
  //Serial.print("\t");
}


// This function demonstrates how to use the accel.readPL()
// function, which reads the portrait/landscape status of the
// sensor.
void printOrientation()
{
  // accel.readPL() will return a byte containing information
  // about the orientation of the sensor. It will be either
  // PORTRAIT_U, PORTRAIT_D, LANDSCAPE_R, LANDSCAPE_L, or
  // LOCKOUT.
  byte pl = accel.readPL();
  switch (pl)
  {
    case PORTRAIT_U:
      Serial.print("Portrait Up");
      break;
    case PORTRAIT_D:
      Serial.print("Portrait Down");
      break;
    case LANDSCAPE_R:
      Serial.print("Landscape Right");
      break;
    case LANDSCAPE_L:
      Serial.print("Landscape Left");
      break;
    case LOCKOUT:
      Serial.print("Flat");
      break;
  }
}
