// Woody the Friendly Social Distancing Robot
// His name is Woody and he really likes making friends! With everything!! Unfortunately, he is also afraid of Coronavirus and practises social distancing :)
// He also wanted me to tell you that he wishes everyone health and luck!

#include<NewPing.h>         // library for ultrasonic sensor
#include<Servo.h>           // library for servo motor
#include<AFMotor.h>         // library for dc motor

#define RIGHT A2            // right (IR sensor) -> A2
#define LEFT A3             // left (IR sensor) -> A3

#define TRIGGER_PIN A1      // (Ultrasonic sensor Trigger) -> A1
#define ECHO_PIN A0         // (Ultrasonic sensor Echo) -> A0
#define MAX_DISTANCE 200    // max distance (cm) I wish to sense with the (Ultrasonic Sensor)

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);   // Object: sonar <- Class: NewPing


AF_DCMotor Motor1(1,MOTOR12_1KHZ);    // Object: Motor1 <- Class: AF_DCMotor (motornum=1, DC_MOTOR_PWM_RATE=MOTOR12_1KHZ)
                                        // #define MOTOR12_1KHZ _BV(CS22) // BV is Bit Value
                                        // CS = Clock Source
                                        // #define _BV(bit) (1 << (bit))
                                        // #define bit(b) (1UL << (b))
                                        // 1UL = 1 Unsigned Long Int = 00000000000000000000000000000001
                                        // << (b) : left shift 5 bits
AF_DCMotor Motor2(2,MOTOR12_1KHZ);    // Object: Motor2 <- Class: AF_DCMotor
AF_DCMotor Motor3(3,MOTOR34_1KHZ);    // Object: Motor3 <- Class: AF_DCMotor
                                      // MOTOR34_1KHZ _BV(CS01) | _BV(CS00)
AF_DCMotor Motor4(4,MOTOR34_1KHZ);    // Object: Motor4 <- Class: AF_DCMotor

Servo myservo;    // Object: myservo <- Class : Servo  
 
int pos = 0; // position of servo motor

int turn = 0; // which way to turn, when searching friends; 0=left, 1=right
bool frend = false; // variable for if a friend was found in front of Woody

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // Initialize the serial connection at 9600 bits per second (9600 is the default for Arduino). 
                        // Both sides of the serial connection need to have the same speed.  
  
  myservo.attach(10);   // Servo motor <- I/O pin 10
  
  for(pos = 90; pos <= 180; pos += 1){
    myservo.write(pos);   // Servo motor angle <- [90~180] degrees
    delay(10);    // pause for 15 miliseconds (0.015 sec)
  } 
  for(pos = 180; pos >= 0; pos-= 1) {
    myservo.write(pos);   // Servo motor angle <- [180~0] degrees
    delay(10);
  }
  for(pos = 0; pos<=90; pos += 1) {
    myservo.write(pos);   // Servo motor angle <- [0~90] degrees
    delay(10);
  }
  
  pinMode(RIGHT, INPUT);  // configure RIGHT (A2) pin <- input
  pinMode(LEFT, INPUT); // configure LEFT (A3) pin <- input
  
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(50);

  int k;

  unsigned int distance = sonar.ping_cm();    // Send a ping and get the distance in whole centimeters
  Serial.print("distance:");   // Prints data to the serial port as human-readable ASCII text
  Serial.println(distance);   // Prints data to the serial port as human-readable ASCII text followed 
                                // by a carriage return character (ASCII 13, or '\r') and a 
                                // newline character (ASCII 10, or '\n')


  int Right_Value = digitalRead(RIGHT);   // read the value from the digital pin RIGHT (A2)  (HIGH/1 = sense nothing or LOW/0 = something)
  int Left_Value = digitalRead(LEFT);     // read the value from the digital pin LEFT  (A3)  (HIGH or LOW)

  Serial.print("RIGHT:");
  Serial.println(Right_Value);
  Serial.print("LEFT:");
  Serial.println(Left_Value);
  Serial.print("pos:");
  Serial.println(pos);

  if((Right_Value==1) && (Left_Value==0)) {   // only the Left IR sensor senses something
    frend = true; // Woody found a friend

    if (pos>86 && pos<94){   // go left
      Motor1.setSpeed(120);
      Motor1.run(BACKWARD);
      Motor2.setSpeed(120);
      Motor2.run(BACKWARD);
      Motor3.setSpeed(120);
      Motor3.run(FORWARD);
      Motor4.setSpeed(120);
      Motor4.run(FORWARD);
    }else{
      k = straightenHead(Right_Value, Left_Value);   // straighten Woody's head as you turn  
    }
    
  }else if((Right_Value==0)&&(Left_Value==1)) {   // only the Right IR sensor senses something
    frend = true; // Woody found a friend
    
    if (pos>86 && pos<94){   // go right
      Motor1.setSpeed(120);
      Motor1.run(FORWARD);
      Motor2.setSpeed(120);
      Motor2.run(FORWARD);
      Motor3.setSpeed(120);
      Motor3.run(BACKWARD);
      Motor4.setSpeed(120);
      Motor4.run(BACKWARD);
    }else{
      k = straightenHead(Right_Value, Left_Value);   // straighten Woody's head as you turn  
    }
    
  }else if( distance>=5 && distance<10 ) {  // Ultrasonic Sensor distance 5<=x<10
    frend = true; // woody has a friend

    if (pos>86 && pos<94){
      Motor1.setSpeed(0);
      Motor1.run(RELEASE);
      Motor2.setSpeed(0);
      Motor2.run(RELEASE);
      Motor3.setSpeed(0);
      Motor3.run(RELEASE);
      Motor4.setSpeed(0);
      Motor4.run(RELEASE);    
    }else{
      k = straightenHead(Right_Value, Left_Value);
    }
    
  }else if( distance>=1 && distance<5 ) {  // Ultrasonic Sensor distance <5
    frend = true; // woody has a friend

    if (pos>86 && pos<94){
      Motor1.setSpeed(120);
      Motor1.run(BACKWARD);
      Motor2.setSpeed(120);
      Motor2.run(BACKWARD);
      Motor3.setSpeed(120);
      Motor3.run(BACKWARD);
      Motor4.setSpeed(120);
      Motor4.run(BACKWARD);    
    }else{
      k = straightenHead(Right_Value, Left_Value);
    }
    
  }else if( distance>50 || distance==0 ) {  // Ultrasonic Sensor distance >50 cm
    frend = false; // no friend is found in less than 50cm distance
    
    Motor1.setSpeed(0);
    Motor1.run(RELEASE);
    Motor2.setSpeed(0);
    Motor2.run(RELEASE);
    Motor3.setSpeed(0);
    Motor3.run(RELEASE);
    Motor4.setSpeed(0);
    Motor4.run(RELEASE);

    searchFriend();

  }else if( distance>=10 && distance<=50 ) {  // Ultrasonic sensor distance = [10~50]
    frend = true; // a friend was found and Woody is approaching to say Hello!

    if (pos>86 && pos<94) { // Woody is looking straight. Lets approach friend.
      Motor1.setSpeed(120); // switch (motornum) {
                              // case 1:
                              // setPWM1(speed); break;
                          
      Motor1.run(FORWARD);  // switch (motornum) {
                              // case 1:
                              // a = MOTOR1_A; b = MOTOR1_B; 
                              // break;

                            // Bit positions in the 74HCT595 shift register output
                              // #define MOTOR1_A 2

                            // switch (cmd) {
                             // case FORWARD:
                              // latch_state |= _BV(a);
                              // latch_state &= ~_BV(b); 
                              // MC.latch_tx();
                              // break;

                            // The latch_tx() function is responsible for updating the outputs of the shift 
                              // register with the bits of the global variablelatch_state,which is declared 
                              // as:static uint8_t latch_state
    
      Motor2.setSpeed(120);
      Motor2.run(FORWARD);
      Motor3.setSpeed(120);
      Motor3.run(FORWARD);
      Motor4.setSpeed(120);
      Motor4.run(FORWARD);
    }else{
      k = straightenHead(Right_Value, Left_Value);
    }
  }
  
}


// function to straighten the head (86~94 degrees)
int straightenHead (int Right_Value, int Left_Value) {
  if (pos>94 && Right_Value==0 && Left_Value==1) { // looks left and only right IR sensor senses something, turn Right 
    turn = 1; // start searching right
    /*
    Motor1.setSpeed(150);
    Motor1.run(BACKWARD);
    Motor2.setSpeed(150);
    Motor2.run(BACKWARD);
    Motor3.setSpeed(150);
    Motor3.run(FORWARD);
    Motor4.setSpeed(150);
    Motor4.run(FORWARD);
    */
    pos = pos - 2;
    myservo.write(pos);   
  }else if (pos<86 && Right_Value==1 && Left_Value==0){  // looks right and only left IR sensor senses something, turn Left
    turn = 0;
    /*
    Motor1.setSpeed(150);
    Motor1.run(FORWARD);
    Motor2.setSpeed(150);
    Motor2.run(FORWARD);
    Motor3.setSpeed(150);
    Motor3.run(BACKWARD);
    Motor4.setSpeed(150);
    Motor4.run(BACKWARD);
    */ 
    pos = pos + 2;
    myservo.write(pos);   
  }else if (pos>94 && Right_Value==1 && Left_Value==0){  // looks left and only left IR sensor senses something, go left & turn right
    turn = 1;
    
    Motor1.setSpeed(120);
    Motor1.run(BACKWARD);
    Motor2.setSpeed(120);
    Motor2.run(BACKWARD);
    Motor3.setSpeed(120);
    Motor3.run(FORWARD);
    Motor4.setSpeed(120);
    Motor4.run(FORWARD);
    
    pos = pos - 2;
    myservo.write(pos);   
  }else if (pos<86 && Right_Value==0 && Left_Value==1){  // looks right and only right IR sensor senses something, go right & turn left
    turn = 0;
    
    Motor1.setSpeed(120);
    Motor1.run(FORWARD);
    Motor2.setSpeed(120);
    Motor2.run(FORWARD);
    Motor3.setSpeed(120);
    Motor3.run(BACKWARD);
    Motor4.setSpeed(120);
    Motor4.run(BACKWARD);

    pos = pos + 2;
    myservo.write(pos);   
  }else if (pos>94 && Right_Value==0 && Left_Value==0) {  // looks left and both sensors sense something, so go left & turn right
    turn = 1;
    
    Motor1.setSpeed(120);
    Motor1.run(BACKWARD);
    Motor2.setSpeed(120);
    Motor2.run(BACKWARD);
    Motor3.setSpeed(120);
    Motor3.run(FORWARD);
    Motor4.setSpeed(120);
    Motor4.run(FORWARD);
    
    pos = pos - 2;
    myservo.write(pos);   // Servo motor angle <- [0~180] degrees
  }else if (pos<86 && Right_Value==0 && Left_Value==0) {  // looks right and both sensors sense something, so go right & turn left
    turn = 0;
    
    Motor1.setSpeed(120);
    Motor1.run(FORWARD);
    Motor2.setSpeed(120);
    Motor2.run(FORWARD);
    Motor3.setSpeed(120);
    Motor3.run(BACKWARD);
    Motor4.setSpeed(120);
    Motor4.run(BACKWARD);
    
    pos = pos + 2;
    myservo.write(pos);   // Servo motor angle <- [180~0] degrees
  }else if (pos>94 && Right_Value==1 && Left_Value==1) {  // looks left and both sensors sense nothing, so go left & turn right
    turn = 1;
    
    Motor1.setSpeed(120);
    Motor1.run(BACKWARD);
    Motor2.setSpeed(120);
    Motor2.run(BACKWARD);
    Motor3.setSpeed(120);
    Motor3.run(FORWARD);
    Motor4.setSpeed(120);
    Motor4.run(FORWARD);

    Serial.print("FUUUUUCK:");
    Serial.println(pos);  
    
    pos = pos - 2;
    myservo.write(pos);   // Servo motor angle <- [0~180] degrees
  }else if (pos<86 && Right_Value==1 && Left_Value==1) {  // looks right and both sensors sense nothing, so go right & turn left
    turn = 0;
    
    Motor1.setSpeed(120);
    Motor1.run(FORWARD);
    Motor2.setSpeed(120);
    Motor2.run(FORWARD);
    Motor3.setSpeed(120);
    Motor3.run(BACKWARD);
    Motor4.setSpeed(120);
    Motor4.run(BACKWARD);
    
    pos = pos + 2;
    myservo.write(pos);   // Servo motor angle <- [180~0] degrees
  }

  return 0;
}

// function to continuously search for friends by turning head left-right
void searchFriend(){
  // No friend was found in front of me. Let's look around
  if (pos<=180 && turn==0) {  // search left
    pos = pos + 2;
    myservo.write(pos);   // Servo motor angle <- [0~180] degrees
  }else if (pos>180) {
    pos = 180;
    turn = 1;
  }else if (pos>=0 && turn==1) {  // search right
    pos = pos - 2;
    myservo.write(pos);   // Servo motor angle <- [180~0] degrees
  }else if (pos<0) { 
    pos = 0;
    turn = 0; // noone was found left or right
  }
}
