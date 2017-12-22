//********************************************************************
//*超音波センサを使って距離を表示するプログラム
//********************************************************************
#include <Servo.h> 
#include <Wire.h>
 
//********************************************************************
//*超音波センサを使って距離を表示するプログラム
//********************************************************************

#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin

#define led_green 9
#define led_blue 10
#define led_yellow 11
#define led_red 12

double Duration = 0; //受信した間隔
int Distance = 0; //距離

void setup() {
  Serial.begin( 9600 );
  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );
  // initialize the digital pin as an output.
  //pinMode(0, OUTPUT); //LED on Model B
  //pinMode(1, OUTPUT); //LED on Model A   
  pinMode( led_green , OUTPUT );
  pinMode( led_blue  , OUTPUT );
  pinMode( led_yellow, OUTPUT );
  pinMode( led_red   , OUTPUT );
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
    if (Duration > 0) {
      Duration = Duration/2; //往復距離を半分にする
      Distance = Duration*340*100/1000000; // 音速を340m/sに設定
      if (Distance < 3000 && priv_dist == Distance) {
        break;
      }
      priv_dist = Distance;
    }
  }
  return Distance;
}

void loop() {
  Distance = getDistance();
  Serial.print("Distance:");
  Serial.print(Distance);
  Serial.println(" cm");
  digitalWrite(led_green,LOW);
  digitalWrite(led_blue,LOW);
  digitalWrite(led_yellow,LOW);
  digitalWrite(led_red,LOW);
  if (Distance < 50) {
    digitalWrite(led_green,HIGH);
  }else if (Distance < 100) {
    digitalWrite(led_blue,HIGH);
  }else if (Distance < 150) {
    digitalWrite(led_yellow,HIGH);
  }else{
    digitalWrite(led_red,HIGH);
  }
  delay(1000);
}

