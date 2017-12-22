void setup() {
  Serial.begin(9600);      // 9600bpsでシリアルポートを開く
}

void loop() {
  Serial.print("NO FORMAT");       // 文字列を送信
  Serial.print("\t");              // タブを送信
  Serial.print("DEC");
  Serial.print("\t");
  Serial.print("HEX");
  Serial.print("\t");
  Serial.print("OCT");
  Serial.print("\t");
  Serial.print("BIN");
  Serial.print("\t");

  for(int x=0; x< 64; x++){    // ASCIIコード表を出力
    Serial.print(x);          // ASCIIコードを十進数で出力
    Serial.print("\t");
    Serial.print(x, DEC);  // ASCIIコードを十進数で出力
    Serial.print("\t");
    Serial.print(x, HEX);  // ASCIIコードを十六進数で出力
    Serial.print("\t");
    Serial.print(x, OCT);  // ASCIIコードを八進数で出力
    Serial.print("\t");
    Serial.println(x, BIN);  // ASCIIコードを二進数で出力し改行
    delay(200);
  }
  Serial.println("");      // 改行
}
