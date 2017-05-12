
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

// For 8x8 Matrix LED
int Max7219_pinCLK = 8;
int Max7219_pinCS = 7;
int Max7219_pinDIN = 6;
unsigned char i;
unsigned char j; 
/**
 * LED Digree array
 * 20    40    60    80   100   120    140   160
 */
unsigned char led_dig[8]={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
/**
 * LED Distance array
 * 25    50    75    100   125   150   175   200
 */
unsigned char led_dist[8]={
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
};
unsigned char disp1[8][8]={
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 
  0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,  
};
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
/*
 * LED Section
 */
void Write_Max7219_byte(unsigned char DATA) 
{   
  unsigned char i;
  digitalWrite(Max7219_pinCS,LOW);  
  for(i=8;i>=1;i--)
  {    
    digitalWrite(Max7219_pinCLK,LOW);
    digitalWrite(Max7219_pinDIN,DATA&0x80);
    DATA = DATA<<1;
    digitalWrite(Max7219_pinCLK,HIGH);
  }                                 
}

void Write_Max7219(unsigned char address,unsigned char dat)
{
  digitalWrite(Max7219_pinCS,LOW);
  Write_Max7219_byte(address);          
  Write_Max7219_byte(dat);               
  digitalWrite(Max7219_pinCS,HIGH);
}

void Init_MAX7219(void)
{
  Write_Max7219(0x09, 0x00);      
  Write_Max7219(0x0a, 0x03);      
  Write_Max7219(0x0b, 0x07);       
  Write_Max7219(0x0c, 0x01);      
  Write_Max7219(0x0f, 0x00);      
} 
/**
 * Set up LED and Sonor and Servo
 */
void setup() {
  // for I2C wire
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveAnalogReading);
  Wire.onRequest(sendData);
  Serial.begin(9600);
  Serial.println("Ready!!");
  // for LED
  delay(50);
  pinMode(Max7219_pinCLK,OUTPUT);
  pinMode(Max7219_pinCS,OUTPUT);
  pinMode(Max7219_pinDIN,OUTPUT);
  delay(50);
  Init_MAX7219();
  // for motor
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode (TRIG_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  myServo.attach(10);  // attaches the servo on pin 0 to the servo object 
  myServo.write(90);
  //Serial.begin(115200);
  //Serial.println("start¥n");
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
  delay(25);
  if (distance < 10){
    distance = 0; 
  }
  return distance;
}
/**
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
*/
void moveBackward(int d){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);
  while(1){
    distance = getSonar();
    set_led_array(90,distance);
    if (distance > d){
        stopMotor();
        //debugDist(distance);
        break;
    }
    delay(30);
  }
}
int get_led_dist(int dist) {
  if (dist < 25){
    return led_dist[0];
  }else if(dist < 50){
    return led_dist[1];    
  }else if(dist < 75){
    return led_dist[2];    
  }else if(dist < 100){
    return led_dist[3];    
  }else if(dist < 125){
    return led_dist[4];    
  }else if(dist < 150){
    return led_dist[5];    
  }else if(dist < 175){
    return led_dist[6];    
  }else if(dist > 175){
    return led_dist[7];    
  }
}
void set_led_array(int deg,int dist){
  if (deg < 20){
    led_dig[0] = get_led_dist(dist);
  }else if (deg < 40){
    led_dig[1] = get_led_dist(dist);
  }else if (deg < 60){
    led_dig[2] = get_led_dist(dist);
  }else if (deg <= 80){
    led_dig[3] = get_led_dist(dist);
  }else if (deg == 90){
    led_dig[3] = get_led_dist(dist);
    led_dig[4] = get_led_dist(dist);
  }else if (deg <= 100){
    led_dig[4] = get_led_dist(dist);
  }else if (deg < 120){
    led_dig[5] = get_led_dist(dist);
  }else if (deg < 140){
    led_dig[6] = get_led_dist(dist);
  }else if (deg > 160){
    led_dig[7] = get_led_dist(dist);
  }
}
/*
 * maxDeg 10 to 80
 */
void searchLeft(int maxDeg){
  // scan center to left
  min_dist = 300;
  max_dist = 0;
  for (int deg = maxDeg; deg < 90; deg+=10) {
    myServo.write(deg);
    delay(25);
    distance = getSonar();
    //debugDist(distance);
    //debugDig(deg);
    set_led_array(deg,distance);
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
void searchRight(int maxDeg){
  // scan center to right
  min_dist = 300;
  max_dist = 0;
  for (int deg = maxDeg; deg > 90; deg-=10) {
    myServo.write(deg);
    delay(25);
    distance = getSonar(); 
    //debugDist(distance);
    //debugDig(deg);       
    set_led_array(deg,distance);
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
void write_led(){
  for(i=1;i<9;i++){
    Write_Max7219(i,led_dig[i-1]);  
  }
  delay(500);
}
void moveFoward(){
  goOn();
  while(1){
    searchLeft(80);
    delay(100);
    leftDistance = getSonar();
    set_led_array(80,leftDistance);
    if(leftDistance<50){
        turnRight();
        delay(200);
        stopMotor();
        goOn();
    }
    searchRight(100); 
    delay(100);
    rightDistance = getSonar();
    set_led_array(100,rightDistance);
    if(rightDistance<50){
        turnLeft();
        delay(200);
        stopMotor();
        goOn();
    }
    myServo.write(90);
    delay(100);
    centerDistance = getSonar();
    if (centerDistance < 25){
        stopMotor();
        break;
    }
    set_led_array(90,centerDistance);
    write_led(); 
  }
}

void loop() {
  for(j=0;j<8;j++)
  {
    for(i=1;i<9;i++){
      Write_Max7219(i,disp1[j][i-1]);
    }
    delay(250);
  }
    max_dist = 0;
    searchLeft(10);
    searchRight(170);    
    //debugDist(max_dist);  
    //debugDig(max_dist_dig);  
    write_led();
    if (min_dist<20){
      //Serial.print("moveBackward:");
      moveBackward(25);
    }else if(max_dist_dig>100){
      // turnRight
      turnRight();
      if(max_dist<20){
        delay((max_dist_dig-90)*20);
      }else{
        delay((max_dist_dig-90)*10);
      }
      stopMotor();
      // moveFoward
      moveFoward();
    }else if(max_dist_dig<80){
      // turnLeft
      turnLeft();
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

void receiveAnalogReading(int byteCount){
  while(Wire.available()){
    wire_cmd = Wire.read();
    Serial.print("data received: ");
    Serial.println(wire_cmd);
    
    switch(wire_cmd){
      case 0:
        wire_val = analogRead(A0);
        break;
      case 1:
        wire_val = analogRead(A1);
        break;
      case 2:
        wire_val = analogRead(A2);
        break;
      case 3:
        wire_val = analogRead(A3);
        break;
      case 4:
        wire_val = analogRead(A4);
        break;
      case 5:
        wire_val = analogRead(A5);
        break; 
    }
  }
}

void sendData(){
  Serial.print("send data: ");
  Serial.println(wire_val / 10);
  Wire.write(wire_val / 10);
}
