//This program is made to operate with "Bluetooth RC Controller" app
//Available on the playstore(Android) and appstore(iOS)
//Can work on both L293D Motor driver shield and L298N Motor Driver module
//-------------------------------------------------------------------------

#include <AFMotor.h>
#include <Servo.h>

//HCSR04
#define echo A5
#define trig 2

//Servo motor
Servo myServo1;
Servo myServo2;

//DC Motor
//With shield
AF_DCMotor motor1(1);
AF_DCMotor motor2(4);

//With module/driver
//L298n Motor Driver pins.
#define in1 3
#define in2 5
#define in3 6
#define in4 11

int angle=90;       //Servo will stay at the center by default (90 is the center of 0 and 180)
int angleStep=30;   //This will be reduced from "angle" and the value obtained is the desired value for turning the video

int minDist = 10;        //Collision avoidance
int maxDist = 4000;        //Collision avoidance

//Bluetooth part
char bt='S';        //To keep the car at stop by default, S in the app means STOP

//Lights
int Headlight = 4;        //These digital pins are not at all connected to the L293D module so can be used
int Taillight = 13;

//Horn
int Honky = 7;      //Connected to the M2 which is connected to the digital pin 3

//Speed variable
int Speed = 204; // 0 - 255.

void setup()
{
  Serial.begin(9600);

  //Motor driver module
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  myServo1.attach(9);
  myServo2.attach(10);

  myServo1.write(angle);
  myServo2.write(angle);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  Stop();

  pinMode(Headlight, OUTPUT);
  pinMode(Taillight, OUTPUT);

  pinMode(Honky, OUTPUT);
}


void loop() {
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(2);
  long t = pulseIn(echo,HIGH);
  long cm = t / 29 / 2;

  Serial.println(cm);
  
  bt=Serial.read();
  switch(bt){
  
  //Straight movement
  case 'F': if(cm>minDist && cm<maxDist){forward(); myServo1.write(angle);} 
            else{Stop();} break;
            
  case 'B': backward(); myServo1.write(angle); break;  

  //Forward directional movement
  case 'G': left(); forward(); break;
  case 'I': right(); forward(); break;
  
  //Backward directional movement
  case 'H': left(); backward(); break;
  case 'J': right(); backward(); break;
  
  //Steering only movement
  case 'L': left(); break;
  case 'R': right(); break; 
 
  //Door ops
  case 'X': doorOpen(); break; 
  case 'x': doorClose(); break;
  
  //Stop the motors!!!
  case 'S': Stop(); break;

  //Motor speed
  case '0': motor1.setSpeed(0); motor2.setSpeed(0); Speed=0; break;
  case '1': motor1.setSpeed(25); motor2.setSpeed(25); Speed=25; break;
  case '2': motor1.setSpeed(50); motor2.setSpeed(50); Speed=50; break;
  case '3': motor1.setSpeed(75); motor2.setSpeed(75); Speed=75; break;
  case '4': motor1.setSpeed(100); motor2.setSpeed(100); Speed=100; break;
  case '5': motor1.setSpeed(125); motor2.setSpeed(125); Speed=125; break;
  case '6': motor1.setSpeed(150); motor2.setSpeed(150); Speed=150; break;
  case '7': motor1.setSpeed(175); motor2.setSpeed(175); Speed=175; break;
  case '8': motor1.setSpeed(200); motor2.setSpeed(200); Speed=200; break;
  case '9': motor1.setSpeed(225); motor2.setSpeed(225); Speed=225; break;
  case 'q': motor1.setSpeed(255); motor2.setSpeed(255); Speed=255; break;

  //Front Lights control
  case 'W': digitalWrite(Headlight, HIGH); break;
  case 'w': digitalWrite(Headlight, LOW); break;

  //Back Lights control
  case 'U': digitalWrite(Taillight,HIGH); break;
  case 'u': digitalWrite(Taillight,LOW); break;

  //Horn Honky
  case 'V': digitalWrite(Honky,HIGH); break;
  case 'v': digitalWrite(Honky,LOW); break;
  }
}


//------FUNCTIONS------
//Straight movement
void forward()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);

  //Motor driver module
  //Motor driver module
  analogWrite(in1, Speed);
  analogWrite(in2, 0);
  analogWrite(in3, Speed);
  analogWrite(in4, 0);
}

void backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);

  //Motor driver module
  analogWrite(in1, 0);
  analogWrite(in2, Speed);
  analogWrite(in3, 0);
  analogWrite(in4, Speed);
}


//Steering movement
void left(){
  myServo1.write(angle-angleStep);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  
  //Motor driver module
  analogWrite(in1, Speed);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}
void right(){
  myServo1.write(angle+angleStep);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);

  //Motor driver module
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, Speed);
  analogWrite(in4, 0);
}


//Door ops
void doorOpen(){
  myServo2.write(angle-(2*angleStep));
}

void doorClose(){
  myServo2.write(angle);
}


//Stop the motors!!!
void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);

  //Module part
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}
