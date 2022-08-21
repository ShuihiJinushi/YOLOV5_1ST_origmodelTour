/* aruuinoから * 74HC595へシリアルに7SEG数値の表現を送る
 * 7SEGのa,b,c...が595のQA,QB,QC...のピンに対応する（Ｋコモン）
 * OE(bar) : GNDとする（ローアクティブのアウトプット可）
 */
int const SDI = 6;//シリアルデータイン
int const LAT = 7;//ラッチピン
int const CLK = 8;//クロックピン
//int const CLR = 9;//クリアピン
int i;//７SEGのa,b,c,d,e,f,g,DPとする
String str;

int changeOut(int i);

void setup() {
  Serial.begin(115200);
  pinMode(SDI, OUTPUT);
  pinMode(LAT, OUTPUT);
  pinMode(CLK, OUTPUT);
}
void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    str = Serial.readString();
    // say what you got:
    if (str.charAt(1)=='.'){
      String temp;
      temp="1"+str;
      str=temp;
      }
    Serial.print("I received: ");
    Serial.println(str.toInt(), DEC);
  }
//シリパラ変換でLED光らせる。
    digitalWrite(LAT, LOW);
    shiftOut(SDI,CLK, LSBFIRST,changeOut(str.toInt()));
    digitalWrite(LAT, HIGH);
    delay(250);
}
int changeOut(int i){
  int tmp;tmp=18;
  //7SEGの部品ごとのピンのばらつきはジャンパピンで調整
  //Ａコモンなら０１を逆転させてデータ作ること　10は小数点つきの０、11は小数点つきの１...
  if (i==0) tmp=B11111100;
  if (i==1) tmp=B01100000;
  if (i==2) tmp=B11011010;
  if (i==3) tmp=B11110010;
  if (i==4) tmp=B01100110;
  if (i==5) tmp=B10110110;
  if (i==6) tmp=B10111110;
  if (i==7) tmp=B11100000;
  if (i==8) tmp=B11111110;
  if (i==9) tmp=B11110110;
  if (i==10) tmp=B11111101;//For 1 with DP on 
  if (i==11) tmp=B01100001;//For 2 with DP on
  if (i==12) tmp=B11011011;//below are the same as above
  if (i==13) tmp=B11110011;
  if (i==14) tmp=B01100111;
  if (i==15) tmp=B10110111;
  if (i==16) tmp=B10111111;
  if (i==17) tmp=B11100001;
  if (i==18) tmp=B11111111;
  if (i==19) tmp=B11110111;
  return tmp; 
  }
