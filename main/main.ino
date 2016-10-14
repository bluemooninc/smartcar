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
                                                                                  
void setup() {
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode (TRIG_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  myServo.attach(10);  // attaches the servo on pin 9 to the servo object 
  myServo.write(90);
  Serial.begin(115200);
  Serial.println("===== Ultrasonic rotating sonar =====");
}
void moveFoward(){
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
void moveBackward(){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);
}
void loop() { 
  // scan right to left
  moveFoward();
  for (int deg = 90; deg < 170; deg+=5) {
    myServo.write(deg);
    delay(300);
    displaySonar(deg);
  }
  moveBackward();
  // scan left to right
  for (int deg = 90; deg > 10; deg-=5) {
    myServo.write(deg);
    delay(300);
    displaySonar(deg);
  }
}
void displaySonar(int degrees) {
  int distance = sonar.ping_cm(); 
  delay(30);
  if (distance < 10){
    distance = 0; 
    stopMotor();
  }  
  sprintf(dist,"%3d",distance);
  Serial.print("Range:");
  Serial.print(dist);
  Serial.print("cms/");
  sprintf(rot,"%3d",degrees);
  Serial.print(rot);
  Serial.print("deg:");

  for (int dloop = 0; dloop < distance/4; dloop++) {
    Serial.print("-");
  }
  Serial.println("=");
}
