/**
 * Arduino Smart Car Sample by Bluemoon inc.
 * LED点灯サンプル
 * デジタル端子13番にLEDのプラス（長い方）端子・GNDにLEDのマイナス端子を接続
 */
#include <Wire.h>

void setup() {
  pinMode( 13 , OUTPUT );
}

void loop() {
  digitalWrite(13,LOW);
  delay(1000);
  digitalWrite(13,HIGH);
  delay(1000);
}

