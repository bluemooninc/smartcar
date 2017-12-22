
/* 
 * Project : Smart Car Project
 * Device  : Arduino + Shield V5 + L298N + Ultrasonic sencer + SG90 survo motor 
 * Athor   : Yoshi Sakai 
 * Project start : 14-Oct-2016
*/
#include <Servo.h> 
#include <NewPing.h>
#include <Wire.h>

#define MAX_DISTANCE 300
#define TRIG_PIN A0
#define ECHO_PIN A1
#define LEFT_FORWARD 2
#define LEFT_BACKWARD 3
#define RIGHT_FORWARD 4
#define RIGHT_BACKWARD 5

// For I2C wire
int SLAVE_ADDRESS = 0x04;
int wire_cmd = 0;
int wire_val = 0;

// for Servo and Sonor
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
Servo myServo;  // create servo object to control a servo 
char dist[3];
char rot[3];
int rotation = 0;
String output = "";
int max_dist = 0;
int max_dist_dig = 90;
int min_dist = 0;
int min_dist_dig = 90;
int distance = 0;
int centerDistance = 0;
int leftDistance = 0;
int rightDistance = 0;
int search_wait = 40;
int see_left_dig = 20;
int see_right_dig = 160;
int see_left_dig_f = 80;
int see_right_dig_f = 100;
/**
 * Set up LED and Sonor and Servo
 */
void setup() {
  // for I2C wire
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveAnalogReading);
  Wire.onRequest(sendData);
  Serial.begin(9600);

  // for motor
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode (TRIG_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  myServo.attach(10);  // attaches the servo on pin 0 to the servo object 
  myServo.write(90);
  //Serial.begin(115200);
  //Serial.println("start¥n");
  Serial.println("Ready!");
}
void turnRight(){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,LOW);
}
void turnLeft(){
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,HIGH);
}
void goOn(){
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);  
}
void stopMotor(){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);
}
int getSonar() {
  distance = sonar.ping_cm();
  if (distance < 10){
    distance = 0; 
  }
  return distance;
}

void debugDist(int d){
  sprintf(dist,"%3d",d);
  Serial.print("Range:");
  Serial.print(dist);
  Serial.print("cm");
}
void debugDig(int d){
  sprintf(rot,"%3d",d);
  Serial.print(rot);
  Serial.println("deg");
}

void moveBackward(int d){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);
  while(1){
    myServo.write(90);  
    delay(search_wait*2);
    distance = getSonar();
    debugDist(distance);
    if (distance < d){
      delay(search_wait*2);
      stopMotor();
      break;
    }
    delay(search_wait);
  }
}

/*
 * maxDeg 10 to 80
 */
void searchLeft(int maxDeg, int mode){
  int til = 0;
  // scan center to left
  min_dist = 300;
  max_dist = 0;
  if (mode==0){
    til = see_left_dig;
  }else{
    til = see_left_dig_f;
  }
  for (int deg = maxDeg; deg >= til; deg-=10) {
    myServo.write(deg);
    delay(search_wait);
    distance = getSonar();
    //debugDist(distance);
    //debugDig(deg);
    if (max_dist < distance){
      max_dist = distance;
      max_dist_dig = deg;
    }
    if (min_dist > distance){
      min_dist = distance;
      min_dist_dig = deg;
    }    
  }
}
/*
 * 170 to 100
 */
void searchRight(int maxDeg, int mode){
  int til = 0;
  // scan center to right
  min_dist = 300;
  max_dist = 0;
  if (mode==0){
    til = see_right_dig;
  }else{
    til = see_right_dig_f;
  }
  for (int deg = maxDeg; deg <= til; deg+=10) {
    myServo.write(deg);
    delay(search_wait);
    distance = getSonar(); 
    //debugDist(distance);
    //debugDig(deg);       
    if (max_dist < distance){
      max_dist = distance;
      max_dist_dig = deg;
    }
    if (min_dist > distance){
      min_dist = distance;
      min_dist_dig = deg;
    }   
  }
}

void moveFoward(){
  goOn();
  while(1){
    searchLeft(see_left_dig_f,1);
    delay(search_wait);
    leftDistance = getSonar();
    if(leftDistance<50){
        turnRight();
        delay(200);
        stopMotor();
        goOn();
    }
    searchRight(see_right_dig_f,1); 
    delay(search_wait);
    rightDistance = getSonar();
    if(rightDistance<50){
        turnLeft();
        delay(200);
        stopMotor();
        goOn();
    }
    myServo.write(90);
    delay(search_wait);
    centerDistance = getSonar();
    if (centerDistance < 25){
        stopMotor();
        break;
    }
  }
}
void search_and_move(){
  max_dist = 0;
  min_dist = 300;
  searchLeft(20,0);
  searchRight(160,0);
  myServo.write(90);    
  debugDist(max_dist);  
  debugDig(max_dist_dig);  
  if (min_dist<20){
    Serial.print("moveBackward:");
    debugDist(min_dist); 
    moveBackward(25);
  }else if(max_dist_dig>90){
    // turnRight
    Serial.println("turnRight");
    turnRight();
    if(max_dist<20){
      delay((max_dist_dig-90)*20);
    }else{
      delay((max_dist_dig-90)*10);
    }
    stopMotor();
    // moveFoward
    moveFoward();
  }else if(max_dist_dig<90){
    // turnLeft
    turnLeft();
    Serial.println("turnLeft");
    if(max_dist<20){
      delay((90-max_dist_dig)*20);
    }else{
      delay((90-max_dist_dig)*10);
    }
    stopMotor();
    // moveFoward
    moveFoward();
  }      
}
/**
 * Main loop
 */
void loop() {
  Serial.println(".");
  if (wire_val==1){
    Serial.println("Go");
    search_and_move();
  }  
  if (wire_val==2){
    Serial.println("Stop");
    stopMotor();
    wire_val=0;
  }  
  delay(1000);
}

void receiveAnalogReading(int byteCount){
  while(Wire.available()) {
    wire_val = Wire.read();
    Serial.print("data received: ");
    Serial.println(wire_val);
  }
}

void sendData(){
  Serial.print("send data: ");
  Serial.println(wire_val);
  Wire.write(wire_val);
  wire_val = 0;
}
