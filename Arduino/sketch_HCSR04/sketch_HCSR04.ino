/**
 * Arduino Smart Car Sample by Bluemoon inc.
 * 超音波センサー・液晶LCD（1602A）の組み合わせサンプル
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27,16,2);  
// run ic2_scanner sketch and get the IC2 address, which is 0x3f in my case,it could be 0x3f in many cases

#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin

// HC-SR04
double Duration = 0; //受信した間隔
int Distance = 0; //距離

void setup() {
  Serial.begin( 9600 );
  Serial.print("Start:");
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  // initialize the digital pin as an output.
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A   
  lcd.init();                      // initialize the lcd
  lcd.backlight();
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
      // +-3cmは誤差とする
      if (Distance < 3000 && abs(priv_dist-Distance) < 3) {
        break;
      }
      priv_dist = Distance;
    }
  }
  return Distance;
}

void loop() {
  digitalWrite(0, HIGH); //LED on Model A
  digitalWrite(1, LOW); //LED on Model B

  Distance = getDistance();
  Serial.print("Distance:");
  Serial.print(Distance);
  Serial.println(" cm");
  // Print a message to the LCD.
  // set cursor to first line
  lcd.setCursor(0, 0);
  lcd.print("   ");    
  lcd.setCursor(0, 0);
  lcd.print(Distance);
  lcd.setCursor(0,1);
  lcd.print("cm");    
  digitalWrite(1, HIGH); //LED on Model A   
  digitalWrite(0, LOW); //LED on Model B
  delay(500);
}

