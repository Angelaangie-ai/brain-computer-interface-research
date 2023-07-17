#include <RH_RF69.h>
#include <SPI.h>
#include <Wire.h>
#include <Zumo32U4.h>
/************ Components ***************/
Zumo32U4LCD display;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
/************ Radio Setup ***************/
#define RF69_FREQ 915.0
#define RFM69_CS 17
#define RFM69_INT 1
#define RFM69_RST 0

/*************** Enums ******************/
/**
 Declaring the states for the finite state machine
*/
enum State {
  Forward_Idle,//Stationary, but ready to move forward position
  Forward, //Move forward
  Right_Turn, //Follows the turnRight() logic
  Left_Turn, //Follows the turnLeft() logic
  Backward, //Move backward
  Backward_Idle,//Stationary, facing backward
  System_Off,//Robot is turned off
};
/************* Constants ****************/
const uint16_t motorSpeed = 100;
const uint16_t turnSpeed = 75;
const int acceleration = 2;
int16_t packetnum = 0;
/********** Global Variables ************/
State state = System_Off;
int currentSpeed = 0;
String Input = "";
/*************** Setup ****************/
/**
 This function is called once the arduino starts
*/
void setup() { receiverSetup(); }

/**************** Loop *****************/
void loop() {
  /**
  This function reads the signals from the transmitter through the radio receiver
  */
  receiverLoop();
  /**
   Handles the state machine logic for the robot
  */
  stateMachine();
}
/************* Functions ***************/
/**
 This function sets up the radio receiver
*/
void receiverSetup() {
  //Starts serial communication
  Serial.begin(115200);
  pinMode(RFM69_RST, OUTPUT);
  //Process of initialization
  digitalWrite(RFM69_RST, LOW);
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  if (!rf69.init()) {
    while (1)
      ;
  }
  //Sets up frequency, initializes encryption and configures power settings
  if (!rf69.setFrequency(RF69_FREQ)) {
  }
  rf69.setTxPower(
      20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW
  // The encryption key has to be the same as the one in the server uint8_t
  // key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03,
  // 0x04, 0x05, 0x06, 0x07, 0x08
};
rf69.setEncryptionKey(key);
}

/**
 Reading signals from the transmitter
*/
void receiverLoop() {
  //if a message is received
  if (rf69.available()) {
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN]; //array to store the message
    uint8_t len = sizeof(buf);
    if (rf69.recv(buf, &len)) {
      if (!len) return;
      buf[len] = 0;
      Input = (char*)buf;
      Serial.println(Input); //printing to the serial monitor
      53 uint8_t data[] = "Recieved";
      rf69.send(data, sizeof(data)); //sends a response message
      rf69.waitPacketSent();
    } else {
    }
  }
}
/**
 Making the robot turn left
*/
void turnLeft() {
  motors.setSpeeds(-turnSpeed, turnSpeed); //setting up motor speeds
  for (int i = 0; i < 3500; i++) {
    receiverLoop(); //calling receiverLoop to check for messages
    if (Input == "J") { 
      break;
    } else {
      delay(1);
    }
  }
  state = Forward_Idle;
  Input = "";
}

/**
 Making the robot turn right
 */
void turnRight() {
  motors.setSpeeds(turnSpeed, -turnSpeed);
  for (int i = 0; i < 3500; i++) {
    receiverLoop();
    if (Input == "J") {
      break;
    } else {
      delay(1);
    }
  }
  state = Forward_Idle;
  Input = "";
}

/**
 Logic of the finite state machine to control the behavior of the robot
 */
void stateMachine() {
  // System_Off
  if (state == System_Off) {
    motors.setSpeeds(0, 0);
    if (Input == "A") {
      state = Forward_Idle;
      Serial.print("State: Forward Idle");
    }
  }
  // Forward_Idle
  else if (state == Forward_Idle) {
    motors.setSpeeds(0, 0);
    if (Input == "J") {
      state = Forward;
    } else if (Input == "B") {
      state = Backward_Idle;
    } else if (Input == "L") {
      state = Left_Turn;
    } else if (Input == "R") {
      state = Right_Turn;
    } else if (Input == "A") {
      state = System_Off;
    }
  }
  // Backward_Idle
  else if (state == Backward_Idle) {
    motors.setSpeeds(0, 0);
    if (Input == "J") {
      state = Backward;
    } else if (Input == "B") {
      state = Forward_Idle;
    } else if (Input == "L") {
      state = Left_Turn;
    } else if (Input == "R") {
      state = Right_Turn;
    } else if (Input == "A") {
      state = System_Off;
    }
  }
  // Forward
  else if (state == Forward) {
    motors.setSpeeds(motorSpeed, motorSpeed);
    if (Input == "J") {
      state = Forward_Idle;
    } else if (Input == "B") {
      state = Backward;
    } else if (Input == "L") {
      state = Left_Turn;
      55
    } else if (Input == "R") {
      state = Right_Turn;
    }
  }
  // Backward
  else if (state == Backward) {
    motors.setSpeeds(-motorSpeed, -motorSpeed);
    if (Input == "J") {
      state = Backward_Idle;
    } else if (Input == "B") {
      state = Forward;
    } else if (Input == "L") {
      state = Left_Turn;
    } else if (Input == "R") {
      state = Right_Turn;
    }
  }
  // Left Turn
  else if (state == Left_Turn) {
    turnLeft();
  }
  // Right Turn
  else if (state == Right_Turn) {
    turnRight();
  }
  Input = "";
}
