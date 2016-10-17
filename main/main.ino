/* 
 * Project : Smart Car Project
 * Device  : Arduino + Shield V5 + L298N + Ultrasonic sencer + SG90 survo motor 
 * Athor   : Yoshi Sakai 
 * Project start : 14-Oct-2016
*/
#include <Servo.h> 
#include <NewPing.h>
#define MAX_DISTANCE 300
#define TRIG_PIN A0
#define ECHO_PIN A1
#define LEFT_FORWARD 2
#define LEFT_BACKWARD 3
#define RIGHT_FORWARD 4
#define RIGHT_BACKWARD 5

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
Servo myServo;  // create servo object to control a servo 
char dist[3];
char rot[3];
int rotation = 0;
String output = "";
int max_dist = 0;
int max_dist_dig = 90;
int distance = 0;
                                                                                    
void setup() {
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode (TRIG_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  myServo.attach(10);  // attaches the servo on pin 10 to the servo object 
  myServo.write(90);
  Serial.begin(115200);
  Serial.println("===== Ultrasonic rotating sonar =====");
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
void moveFoward(){
  myServo.write(90);
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);
  while(1){
    delay(30);
    distance = getSonar();
    debugDist(distance);
    if (distance < 10){
        stopMotor();
        break;
    }
  }
}
void stopMotor(){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);
}
void moveBackward(int d){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);
  while(1){
    delay(30);
    distance = getSonar();
    if (distance > d){
        stopMotor();
        debugDist(distance);
        break;
    }
  }
}
void searchLeft(){
  // scan center to left
  for (int deg = 10; deg < 90; deg+=5) {
    myServo.write(deg);
    delay(100);
    distance = getSonar();
    debugDist(distance);
    debugDig(deg);
    if (max_dist < distance){
      max_dist = distance;
      max_dist_dig = deg;
    }
  }
}
void searchRight(){
  // scan center to right
  for (int deg = 170; deg > 90; deg-=5) {
    myServo.write(deg);
    delay(100);
    distance = getSonar(); 
    debugDist(distance);
    debugDig(deg);       
    if (max_dist < distance){
      max_dist = distance;
      max_dist_dig = deg;
    }
  }
}
int getSonar() {
  distance = sonar.ping_cm();
  delay(30);
  if (distance < 10){
    distance = 0; 
  }
  return distance;
}
void debugDist(int d){
  sprintf(dist,"%3d",d);
  Serial.print("Range:");
  Serial.print(dist);
  Serial.print("cms/");
}
void debugDig(int d){
  sprintf(rot,"%3d",d);
  Serial.print(rot);
  Serial.print("deg/");
}

void loop() { 
  distance = getSonar();
  debugDist(distance);    
  if (distance < 10){
    Serial.print("moveBackward:");
    moveBackward(50);
  } else {
    max_dist = 0;
    searchLeft();
    searchRight();    
    debugDist(max_dist);  
    debugDig(max_dist_dig);  
    delay(30);
    if(max_dist_dig>90){
      Serial.print("turnRight:");
      turnRight();
      delay((max_dist_dig-90)*5);
      stopMotor();
    }else{
      Serial.print("turnLeft:");
      turnLeft();
      delay((90-max_dist_dig)*5);
      stopMotor();
    }
    Serial.print("moveFoward:");
    moveFoward();
  } 
  delay(3000);
}

