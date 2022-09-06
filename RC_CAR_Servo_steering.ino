#include <AFMotor.h>
#include <Servo.h>

Servo myServo1;
Servo myServo2;

AF_DCMotor motor1(1);
AF_DCMotor motor2(4);

int angle=90;       //Servo will stay at the center by default (90 is the center of 0 and 180)
int angleStep=30;   //This will be reduced from "angle" and the value obtained is the desired value

char bt='S';
void setup()
{
  Serial.begin(9600);

  myServo1.attach(9);
  myServo2.attach(10);

  myServo1.write(angle);
  myServo2.write(angle);
  
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  Stop();
}


void loop() {
 
  bt=Serial.read();
  
  //Straight movement
  if(bt=='F')
  {
   forward();
   myServo1.write(angle); 
  }
  if(bt=='B')
  {
   backward();
   myServo1.write(angle); 
  }

  //Forward directional movement
  if(bt=='G')
  {
   left();
   forward();
  }
  if(bt=='I')
  {
   right();
   forward();
  }
  
  //Backward directional movement
  if(bt=='H')
  {
   left();
   backward(); 
  }
  if(bt=='J')
  {
   right(); 
   backward();
  }
  
  //Steering only movement
  if(bt=='L')
  {
   left();
  }
  if(bt=='R')
  {
   right(); 
  }

  //Door ops
  if(bt=='X')
  {
   doorOpen(); 
  }

  if(bt=='x')
  {
   doorClose(); 
  }
  
  //Stop the motors!!!
  if(bt=='S')
  {
   Stop(); 
  }
}

//Straight movement
void forward()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}


//Steering movement
void left(){
  myServo1.write(angle-angleStep);
}
void right(){
  myServo1.write(angle+angleStep);
}


//Door ops
void doorOpen(){
  myServo2.write(angle-angleStep);
}

void doorClose(){
  myServo2.write(angle);
}


//Stop the motors!!!
void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
