#include <Servo.h>
const int GREEN = 7; // could also use: #define GREEN 7
const int YELLOW = 12; // yellow LED pin number
const int RED = 13; // red LED pin number
const int LEFT = 4; // left button pin number
const int RIGHT = 2; // right button pin number
const signed int leftServoStop = 85; // number for left servo to stop moving
const signed int rightServoStop = 83; // number for right servo to stop moving
const int leftServoPin = 6; // left servo pin number
const int rightServoPin = 5; // right servo pin number
int green = 0; // initialises green LED to off
int yellow = 0; // initialises yellow LED to off
int red = 0; // // initialises red LED to off
int white = 0; // white background array number
int black = 1; // black background array number
int IRPin = 3; // IR pin number
int IRPulse = 38000; // IR pulse frequency
Servo leftServo; // setup left servo
Servo rightServo; // setup right servo
int takeValue[3][10]; // initialising array takeValue with size for calibrating LDRs
/*int whiteBackgroundUpper[3] = {};
int whiteBackgroundLower[3] = {};
int blackBackgroundUpper[3] = {};
int blackBackgroundLower[3] = {};*/
int average[2][3] = { // initialising 2D array average filled with standardized numbers for use when no calibration
  {593,664,506},
  {315,408,217}
};
int sumWhite[3]; // array for sum of white background LDR readings
int sumBlack[3]; // array for sum of black background LDR readings
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // serial begin
  pinMode(GREEN, OUTPUT); // setup green LED pin as output
  pinMode(YELLOW, OUTPUT); // setup yellow LED pin as output
  pinMode(RED, OUTPUT); // setup red LED pin as output
  pinMode(LEFT, INPUT); // setup left button
  pinMode(RIGHT, INPUT); // setup right button
  pinMode(A0, INPUT); // setup LDR sensor #0
  pinMode(A1, INPUT); // setup LDR sensor #1
  pinMode(A2, INPUT); // setup LDR sensor #2
  pinMode(IRPin, OUTPUT); // setup IR LED pin as output
  leftServo.attach(leftServoPin); // attaching left servo to corresponding pin
  rightServo.attach(rightServoPin); // attaching right servo to corresponding pin
  
  //turn180();
  start(); // sets speed to 50,50 to start
  //delay(1000);
  //stop();
  /*int temp0 = analogRead(A0);
  int temp1 = analogRead(A1);
  int temp2 = analogRead(A2);
  Serial.println(temp0);
  Serial.println(temp1);
  Serial.println(temp2);*/
  
  //Serial.println("press left button to calibrate light sensors");
  //Serial.println("or press right button to skip");
  //setupLightValues();
  //printLightValues();
}
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("hello");
  //whiteOrBlack();
  lineFollowing();

  
}
void lineFollowing() { // function for robot to follow black line on white background
  //setSpeed(50,50);
  if(analogRead(A0) < 400) { // if left LDR is on black
    setSpeed(50,0);
  }
  else if(analogRead(A2) < 400) { // if right LDR is on black
    setSpeed(0,50);
  }
  if(analogRead(A0) >= 400) { // if left LDR is on white
    if(analogRead(A2) >= 400) { // AND if right LDR is on white
      start(); // THEN set speed back to 50,50 (straight line)
    }
  }
  obstacleDetection();
}
void obstacleDetection() {
  tone(IRPin,IRPulse); // start IR pulses
  delay(20);
  if (digitalRead(2) == LOW){ // obstacle detected
    turn90(1);
    delay(3600);
    turn90(0);
    delay(3600);
    turn90(0);
    delay(3600);
    turn90(1);
  }
  else {
    // do nothing
  }
  noTone(IRPin);
}
void turn90(int direction) {
  if(direction == 0) {
    setSpeed(-50,50);
    delay(1600);
    setSpeed(50,50);
  }
  else if(direction == 1){
    setSpeed(50,-50);
    delay(1600);
    setSpeed(50,50);
  }
}
void turn180() { // function to make the robot turn 180 degrees
  setSpeed(50,-50); // set servos to turn in opposite directions at same speed
  delay(3200); // wait 3200ms
  start(); // continue in a straight line at speed 50,50
}
void start(){ // function to start speed
  setSpeed(50,50); // speed starts at 50,50
}
void whiteOrBlack() { // function that outputs to serial monitor if LDR A1 is on white or black background
  if(analogRead(A1) < 400){ // if LDR A1 is on black background
    Serial.println("black"); // print black
  }
  else {
    Serial.println("white"); // or print white
  }
}
void setupLightValues() { // function to calibrate LDR values
  Serial.println("press left button to calibrate white background");
  while (digitalRead(LEFT) == HIGH) { // while the left button isn't pressed, do nothing
    // do nothing
  } // when the left button is pressed:
  for(int loop = 0; loop < 10; loop ++) { // takes 10 readings from all three of the LDRs
    takeValue[0][loop] = analogRead(A0);
    takeValue[1][loop] = analogRead(A1);
    takeValue[2][loop] = analogRead(A2);
  }
  for(int loop = 0; loop < 3; loop++) { // takes the average of the 10 readings from the 3 LDRs
    for(int loop0 = 0; loop0 < 10; loop++) { // nested loop calculates the sum of the 10 readings from the 3 LDRs
      sumWhite[loop] = sumWhite[loop] + takeValue[loop][loop0];; // calculates sum of ten readings
    }
    average[white][loop] = sumWhite[loop] / 10; // calculates average for each LDR
  }
  for(int loop = 0; loop < 3; loop++) { // prints the average white reading of each LDR
    Serial.println(average[white][loop]);
  }
  Serial.println("press right button to calibrate black background");
  while (digitalRead(RIGHT) == HIGH) { // while the right button isn't pressed, do nothing
    // do nothing
  }
  for(int loop = 0; loop < 10; loop ++) { // takes 10 readings from all three of the LDRs
    takeValue[0][loop] = analogRead(A0);
    takeValue[1][loop] = analogRead(A1);
    takeValue[2][loop] = analogRead(A2);
  }
  for(int loop = 0; loop < 3; loop++) { // takes the average of the 10 readings from the 3 LDRs
    for(int loop0 = 0; loop0 < 10; loop0++) { // nested loop calculates the sum of the 10 readings from the 3 LDRs
      sumBlack[loop] = sumBlack[loop] + takeValue[loop][loop0];; // calculates sum of ten readings
    }
    average[black][loop] = sumBlack[loop] / 10; // calculates average for each LDR
  }  
  for(int loop = 0; loop < 3; loop++) { // prints the average black reading of each LDR
    Serial.println(average[black][loop]);
  }
  
  /*for(int loop = 0; loop < 3; loop++) {
    Serial.print("white upper >> ");
    Serial.println(whiteBackgroundUpper[loop]);
    Serial.print("white lower >> ");
    Serial.println(whiteBackgroundLower[loop]);
    Serial.print("black upper >> ");
    Serial.println(blackBackgroundUpper[loop]);
    Serial.print("black lower >> ");
    Serial.println(blackBackgroundLower[loop]);
  }
  */
}
/*void printLightValues() {
  for(int loop = 0; loop < 2; loop++){
    Serial.println(average[loop]);
  }
}*/
void setSpeed(signed int left, signed int right) { // function to set speed of servos
  left = leftServoStop + left; // sets speed of left servo using parameter left
  right = rightServoStop - right; // sets speed of right servo using parameter right - negative because in opposite direction
  leftServo.write(left); // adjusted value written to left servo
  rightServo.write(right); // adjusted value written to right servo
}
void stop() { // function to make both servos stop
  setSpeed(0, 0);
}
void setLEDs(int green_state, int yellow_state, int red_state) { // function to set the status of the LEDs
  digitalWrite(GREEN, green_state); // set status of green LED using parameter
  digitalWrite(YELLOW, yellow_state); // set status of yellow LED using parameter
  digitalWrite(RED, red_state); // set status of green red using parameter
}
void waitKey(int pin) {
  while (digitalRead(pin) == HIGH ) {
    //do nothing
  }
  delay(20);
  while (digitalRead(pin) == LOW ) {
    //do nothing
  }
  delay(20);
  if (pin == LEFT) {
    setupLightValues();
  }
  else {
    //do nothing
  }
  delay(20);
}
