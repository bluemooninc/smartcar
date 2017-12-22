/**
 * Arduino Smart Car Sample by Bluemoon inc.
 * 超音波センサー・DCモータの組み合わせサンプル
 */

#include <Wire.h>

#define LEFT_FORWARD 2
#define LEFT_BACKWARD 3
#define RIGHT_FORWARD 4
#define RIGHT_BACKWARD 5
#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin

// HC-SR04
double Duration = 0; //受信した間隔
int Distance = 0; //距離

void setup() {
  Serial.begin(9600);
  Serial.print("Start:");
  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );
  // initialize the digital pin as an output.
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A   
}
void go(){
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);
  while (1) {
    Distance = getDistance();
    if (Distance < 10) {
      Serial.println("Stop");
      stop();
      break;
    }
  }
}
void stop(){
  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);
}

/*
 * 距離を取得
 */
int getDistance(){
  int priv_dist;
  while(1){
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite( trigPin, HIGH ); //超音波を出力
    delayMicroseconds( 10 ); //
    digitalWrite( trigPin, LOW );
    Duration = pulseIn( echoPin, HIGH ); //センサからの入力
    Duration = Duration/2; //往復距離を半分にする
    Distance = Duration*340*100/1000000; // 音速を340m/sに設定
    if (Distance > 0 && Distance < 300 && abs(priv_dist-Distance)<3) {
        break;
    }
    priv_dist = Distance;
  }
  return Distance;
}

void loop() {
  Distance = getDistance();
  Serial.print("Distance:");
  Serial.print(Distance);
  Serial.println(" cm");
  // Print a message to the LCD.
  // set cursor to first line
  if (Distance>10 && Distance<150) {
    Serial.println("GO");
    digitalWrite(0, HIGH); //LED on Model A
    digitalWrite(1, LOW); //LED on Model B
    go();
  }
  delay(500);
}

