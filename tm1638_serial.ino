//Arduino用
//shiftOut関数でTM1638を使う
//https://www.amazon.co.jp/Rasbee-TM1638-%E3%83%87%E3%82%A3%E3%82%B9%E3%83%97%E3%83%AC%E3%82%A4-%E9%9B%BB%E5%AD%90%E3%83%A2%E3%82%B8%E3%83%A5%E3%83%BC%E3%83%AB-Arduino/dp/B06XV9STV8/ref=sr_1_3?__mk_ja_JP=%E3%82%AB%E3%82%BF%E3%82%AB%E3%83%8A&crid=1XU8NSNRH1G9&keywords=tm1638&qid=1661095979&sprefix=tm1638%2Caps%2C171&sr=8-3
//STB：  シフトレジスタのラッチピンに当たる
//      ローにしてデータ送信。送信後ハイにすると表示が更新
//DIO,CLK：データ　クロックのピン
/*
ArduinoでコンパイルしたらPowerShellからUSB(シリアル通信)でつないで次のようにつかう。なおポートがCOM5だったとする
$c = New-Object System.IO.Ports.SerialPort "COM5", 115200, ([System.IO.Ports.Parity]::None)
$c.Encoding=[System.Text.Encoding]::GetEncoding("Shift_JIS")
$d = Register-ObjectEvent -InputObject $c -EventName "DataReceived" -Action {param([System.IO.Ports.SerialPort]$sender, [System.EventArgs]$e) Write-Host -NoNewline $sender.ReadExisting()}
$c.open()
$c.writeline("18746.730")
$c.close()

ポートしらべるコマンド
[System.IO.Ports.SerialPort]::getportnames()

*/


const int STB = 6;
const int CLK = 7;
const int DIO = 8;
uint32_t disp;
uint16_t dly=50;
String str;
  /*Font配列　0-9 Commaなし。10-19Commaあり　20　ブランク*/
                     /*0*/ /*1*/ /*2*/ /*3*/ /*4*/ /*5*/ /*6*/ /*7*/ /*8*/ /*9*/
uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 
                     /*0.*//*1.*//*2.*/ /*3.*//*4.*//*5.*//*6.*//*7.*//*8.*//*9.*//*blank*/
                     0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x87, 0xff, 0xef, 0x00   };
uint32_t dig[8];
uint32_t num,tmpnum;
uint8_t dip[8],DP[8];



//コマンド送信用の関数を作っておく。
void sendCommand(uint8_t value)
{
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, value);
  digitalWrite(STB, HIGH);
}

//リセット＝表示文字を全てブランクにする。表示メモリの開始アドレス0xc0。
void reset()
{
  sendCommand(0x40); //アドレス自動インクリメント
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(DIO, CLK, LSBFIRST, 0x00);//ブランクのデータ
  }
  digitalWrite(STB, HIGH);
}
bool counting();
void setup()
{
  Serial.begin(115200);
  pinMode(STB, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  sendCommand(0x8f);  // activate　起動
  reset();
}
void display(/*uint32_t nm, */ int lng ,int fnds){
      tmpnum=num;
      for(int i=lng-1;i>=0;i--){
        dip[i]=tmpnum/dig[i];
        tmpnum=tmpnum-dip[i]*dig[i];    
        }
      if (fnds==-1){
          for(int i=lng-1;i>=0;i--){
      }
  sendCommand(0x40);//自動インクリメント
  digitalWrite(STB, LOW);
  shiftOut(DIO, CLK, LSBFIRST, 0xc0);//表示開始アドレス
    for (int i=0;i<8-lng;i++){
      shiftOut(DIO, CLK, LSBFIRST, digits[20]);
      shiftOut(DIO, CLK, LSBFIRST, digits[20]);
    }
    for (int i=lng-1;i>=0;i--){
      shiftOut(DIO, CLK, LSBFIRST, digits[dip[i]]);
      shiftOut(DIO, CLK, LSBFIRST, digits[20]);
    }
  digitalWrite(STB, HIGH);
  delay(dly);
  }


}/*end of display*/


void loop()
{
  int leng,fnd;//桁数　　と　"."を探すための変数
  if (Serial.available() > 0) {
    // シリアルの読み込み
    str = Serial.readString();
    Serial.println(str);
    str.trim();
    leng = str.length();
    fnd = str.indexOf(".");
    if (fnd == -1)/*整数*/  {
      if(leng > 9) {
        str = str.substring(1,8);leng=8;
    Serial.println(str);        
      num=str.toInt();
    Serial.println(str.toInt());
      display(leng,fnd);
      }/*if の終わり*/
    else/*小数点あり*/       {
    Serial.println(str);        
      str.remove(fnd,fnd);
    Serial.println(str.toInt());
      if(leng > 9) {
        str = str.substring(1,8);leng=8;
        }
      num=str.toInt();
        display(leng,fnd);
        }/*else の終わり*/  
    }/*if (Serial.available() > 0)の終わり*/
  delay(dly);
}
}
