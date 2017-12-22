/**
 * Arduino Smart Car Sample by Bluemoon inc.
 * DCモータ・サーボ・超音波センサーの組み合わせサンプル
 */
#include <Servo.h> 
#include <Wire.h>
 
#define LEFT_FORWARD 2
#define LEFT_BACKWARD 3
#define RIGHT_FORWARD 4
#define RIGHT_BACKWARD 5
#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin

// create servo object to control a servo 
Servo myServo;  
char dist[3];
char rot[3];
int adjust = 0;
String output = "";
int search_wait = 250;
int see_left_dig = 120;
int see_right_dig = 60;
int see_left_dig_wide = 135;
int see_right_dig_wide = 45;

// HC-SR04
double Duration = 0; //受信した間隔
int Distance = 0; //距離

void setup() {
  Serial.begin( 9600 );
  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );
  myServo.attach(10);  // attaches the servo on pin 0 to the servo object
  myServo.write(90); 
}
/**
 * mx,mn 指定の距離に入るまで状態を保つ
 */
void keep(int mn, int mx){
  while(1){
    Distance = getDistance();
    if (Distance < mn or Distance > mx ) {
      Serial.println("STOP");
      stop();
      break;
    }
  }
}
/**
 * 10cmから300cmまでの距離なら進む
 */
void go(){
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);
  keep(10,300);
}
/**
 * 10cmから80cmまでの距離なら後退
 */
void back(){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);
  keep(1,80);
}

void stop(){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);
}
/**
 * 右に向きを変える
 */
void turnRight(int wait){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,LOW);
  delay(wait);
}
/**
 * 左に向きを変える
 */
void turnLeft(int wait){
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,HIGH);
  delay(wait);
}
/*
 * 距離を取得
 */
int getDistance(){
  int priv_dist,i;
  for(i=0;i<5;i++){
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite( trigPin, HIGH ); //超音波を出力
    delayMicroseconds( 10 ); //
    digitalWrite( trigPin, LOW );
    Duration = pulseIn( echoPin, HIGH ); //センサからの入力
    Duration = Duration/2; //往復距離を半分にする
    Distance = Duration*340*100/1000000; // 音速を340m/sに設定
    if (Distance > 0 && Distance < 300 && abs(priv_dist-Distance)<5) {
        break;
    }
    priv_dist = Distance;
  }
  return Distance;
}

void loop() {
  int mind, mini, i;
  mind = 999;
  mini = 90;
  /**
   * 一番近くにある物体の角度・距離を取得
   */
  for(i=0;i<180;i++){
    myServo.write(i); 
    Distance = getDistance();
    // 10cmより遠くにある一番近いものの距離と角度を計測
    if (mind > Distance && Distance > 10){
      mind = Distance;
      mini = i;
    }
    Serial.print("Distance:");
    Serial.print(Distance);
    Serial.println(" cm");
  }
  /**
   * 近くにある物体に頭を向け近づく
   */
  if (mind < 180) {
    myServo.write(mini); 
    if (mini<90){
      turnLeft((90-mini)*3);
    }else{
      turnRight((mini-90)*3);
    }
    myServo.write(90); 
    Serial.println(mini);
    Serial.println("Digree Go");
    Serial.println(mind);
    if (mind<20){
      // 近すぎれば後ろへ
      back();
    }else{
      // 遠くなら前へ
      go();
    }
    delay(500);
  }
}
