#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27,16,2);  
// run ic2_scanner sketch and get the IC2 address, which is 0x3f in my case,it could be 0x3f in many cases

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
  Serial.begin( 9600 );
  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );
  lcd.init();                      // initialize the lcd
  // initialize the digital pin as an output.
  pinMode(0, OUTPUT); //LED on Model B
  pinMode(1, OUTPUT); //LED on Model A   
  lcd.backlight();
}
void go(){
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,LOW);
  digitalWrite(RIGHT_BACKWARD,LOW);
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
  // Print a message to the LCD.
  // set cursor to first line
  lcd.setCursor(0, 0);
  lcd.print("   ");    
  lcd.setCursor(0, 0);
  lcd.print(Distance);
  lcd.setCursor(0,1);
  lcd.print("cm");    
  if (Distance < 200) {
    Serial.println("GO");
    digitalWrite(0, HIGH); //LED on Model A
    digitalWrite(1, LOW); //LED on Model B
    go();
  }else{
    Serial.println("STOP");
    digitalWrite(1, HIGH); //LED on Model A   
    digitalWrite(0, LOW); //LED on Model B
    stop();
  }
  delay(500);
}

