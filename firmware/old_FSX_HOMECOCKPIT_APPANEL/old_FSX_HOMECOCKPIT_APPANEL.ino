#include <RotaryEncoder.h>

#define spdPinA 42
#define spdPinB 43
#define spdPinC 41
#define hdgPinA 38
#define hdgPinB 39
#define hdgPinC 40
#define altPinA 22
#define altPinB 23
#define altPinC 24
#define vsPinA 26
#define vsPinB 27
#define vsPinC 25
#define crsPinA 45
#define crsPinB 46
#define crsPinC 44

#define APMasterSW 28
#define APNavGpsSW 47
#define APAthrSW 48
#define APFDSW 49

#define AP_ALT_LED 29
#define AP_VS_LED 30
#define AP_CRS_LED 34
#define AP_Athr_LED 35
#define AP_HDG_LED 36
#define AP_SPD_LED 37

#define AP2DATA 33
#define AP2CLK 32
#define AP2LATCH 31

#define AP1DATA 50
#define AP1CLK 51
#define AP1LATCH 52

RotaryEncoder rotarySpeed(spdPinA, spdPinB);
RotaryEncoder rotaryHeading(hdgPinA, hdgPinB);
RotaryEncoder rotaryAltitude(altPinA, altPinB);
RotaryEncoder rotaryVerticalSpeed(vsPinA, vsPinB);
RotaryEncoder rotaryCourse(crsPinA, crsPinB);

static int spdPos = 0;
static int hdgPos = 0;
static int altPos = 0;
static int crsPos = 0;
static int vsPos = 0;

float activeCom1 = 100.00;
float standCom1 = 100.00;

float activeCom2 = 100.00;
float standCom2 = 100.00;

float activeNav1 = 100.00;
float standNav1 = 100.00;

float activeNav2 = 100.00;
float standNav2 = 100.00;

char AP_ALT[5] = {'0','0','0','0','0'};
char AP_VS[5] = {'+','0','0','0','0'};
char AP_HDG[3] = {'0','0','0'};
char AP_CRS[3] = {'0','0','0'};
char AP_SPD[3] = {'0','0','0'};

char AP_ALT_OLD[5] = {'0','0','0','0','0'};
char AP_VS_OLD[5] = {'+','0','0','0','0'};
char AP_HDG_OLD[3] = {'0','0','0'};
char AP_CRS_OLD[3] = {'0','0','0'};
char AP_SPD_OLD[3] = {'0','0','0'};

int refreshAP1 = 0;
int refreshAP2 = 0;

/*
 * 0 through 9; -; (none)
 */
byte dataArray[12] = 
{B00111111,
B00000110,
B01011011,
B01001111,
B01100110,
B01101101,
B01111101,
B00000111,
B01111111,
B01101111,
B01000000,
B00000000
};

byte dataArrayWithDot[10] = 
{B10111111,
B10000110,
B11011011,
B11001111,
B11100110,
B11101101,
B11111101,
B10000111,
B11111111,
B11101111
};


/**
 * 22-36 Rotary
 *   PinA |  PinB | PinC
 * RotPin1 RotPin2 Button
 * 
 * xxx  xxx  xxxxx  xxxxx  xxx
 * xxx  xxx  xxx00  xxx00  xxx
 * 
 * SPD  HDG   ALT    V/S   CRS
 */

void setup() {
  Serial.begin(115200);

  pinMode(AP1LATCH,OUTPUT);
  pinMode(AP1CLK,OUTPUT);
  pinMode(AP1DATA,OUTPUT); 
  pinMode(AP2LATCH,OUTPUT);
  pinMode(AP2CLK,OUTPUT);
  pinMode(AP2DATA,OUTPUT);
  pinMode(AP_ALT_LED,OUTPUT);
  pinMode(AP_VS_LED,OUTPUT);
  pinMode(AP_CRS_LED,OUTPUT);
  pinMode(AP_Athr_LED,OUTPUT);
  pinMode(AP_HDG_LED,OUTPUT);
  pinMode(AP_SPD_LED,OUTPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  //ROTARYS();
  char charIn = getChar();

  if(charIn == '='){
    //Serial.println("Equals");
    EQUALS();
  }
  if(refreshAP1 >= 1){
    DISPLAYAP1();
  }
  if(refreshAP2  >= 1){
    DISPLAYAP2();
  }
}

void ROTARYS(){
   rotarySpeed.tick();
   rotaryHeading.tick();
   rotaryAltitude.tick();
   rotaryVerticalSpeed.tick();
   rotaryCourse.tick();

   int currentPos = rotarySpeed.getPosition();
   if(spdPos != currentPos){
    if(spdPos < currentPos){
      //DECREASE SPEED
      Serial.println("B1&");
    }else{
      //INCREASE SPEED
      Serial.println("B15");
    }
    spdPos = currentPos;
   }

   currentPos = rotaryHeading.getPosition();
   if(hdgPos != currentPos){
    if(hdgPos < currentPos){
      //DECREASE HEADING
      Serial.println("A58");
    }else{
      //INCREASE HEADING
      Serial.println("A57");
    }
    hdgPos = currentPos;
   }

   currentPos = rotaryAltitude.getPosition();
   if(altPos != currentPos){
    if(altPos < currentPos){
      //DECREASE ALTITUDE
      Serial.println("B12");
    }else{
      //INCREASE ALTITUDE
      Serial.println("B11");
    }
    altPos = currentPos;
   }

   currentPos = rotaryVerticalSpeed.getPosition();
   if(vsPos != currentPos){
    if(vsPos < currentPos){
      //DECREASE VERTICALSPEED
      Serial.println("B14");
    }else{
      //INCREASE VERTICALSPEED
      Serial.println("B13");
    }
    vsPos = currentPos;
   }

   currentPos = rotaryCourse.getPosition();
   if(crsPos != currentPos){
    if(crsPos < currentPos){
      //DECREASE COURSE
      Serial.println("A55");
    }else{
      //INCREASE COURSE
      Serial.println("A56");
    }
    crsPos = currentPos;
   }
}

void EQUALS(){
  char charIn = getChar();
  if(charIn == 'A'){//COM1 ACTIVE
    delay(15);
    String tmp = "";
    tmp += getString(7);
    activeCom1 = tmp.toFloat();
  }
  if(charIn == 'B'){//COM1 S/B
    delay(15);
    String tmp = "";
    tmp += getString(7);
    standCom1 = tmp.toFloat();
  }
  if(charIn == 'C'){//COM2 ACTIVE
    delay(15);
    String tmp = "";
    tmp += getString(7);
    activeCom2 = tmp.toFloat();
  }
  if(charIn == 'D'){//COM2 S/B
    delay(15);
    String tmp = "";
    tmp += getString(7);
    standNav2 = tmp.toFloat();
  }
  if(charIn == 'E'){//NAV1 ACTIVE
    delay(15);
    String tmp = "";
    tmp += getString(6);
    activeNav1 = tmp.toFloat();
  }
  if(charIn == 'F'){//NAV1 S/B
    delay(15);
    String tmp = "";
    tmp += getString(6);
    standNav1 = tmp.toFloat();
  }
  if(charIn == 'G'){//NAV2 ACTIVE
    delay(15);
    String tmp = "";
    tmp += getString(6);
    activeNav2 = tmp.toFloat();
  }
  if(charIn == 'H'){//NAV2 S/B
    delay(15);
    String tmp = "";
    tmp += getString(6);
    standNav2 = tmp.toFloat();
  }

  
  if(charIn == 'b'){//AP ALT SET
    delay(15);
    String tmp = "";
    tmp += getString(5);
    boolean changed = compare(tmp, AP_ALT_OLD);
    if(changed){
      refreshAP2 = 1;
    }
    copy(AP_ALT,AP_ALT_OLD);
    tmp.toCharArray(AP_ALT, 6); 
  }
  if(charIn == 'c'){//AP VS SET
    delay(15);
    String tmp = "";
    tmp += getString(5);
    //tmp = "+0100"
    boolean changed = compare(tmp, AP_VS_OLD);
    if(changed){
      refreshAP2 = 1;
    }
    copy(AP_VS,AP_VS_OLD);
    tmp.toCharArray(AP_VS, 6);
  }
  if(charIn == 'd'){//AP HDG SET
    delay(15);
    String tmp = "";
    tmp += getString(3);
    tmp += " ";
    boolean changed = compare(tmp, AP_HDG_OLD);
    if(changed){
      refreshAP1 = 1;
    }
    copy(AP_HDG,AP_HDG_OLD);
    tmp.toCharArray(AP_HDG, 4);
  }
  if(charIn == 'e'){//AP CRS SET
    delay(15);
    String tmp = "";
    tmp += getString(3);
    tmp += " ";
    boolean changed = compare(tmp, AP_CRS_OLD);
    if(changed){
      refreshAP1 = 1;
    }
    copy(AP_CRS,AP_CRS_OLD);
    tmp.toCharArray(AP_CRS, 4);
  }
  if(charIn == 'f'){//AP SPD SET
    delay(15);
    String tmp = "";
    tmp += getString(3);
    tmp += " ";
    boolean changed = compare(tmp, AP_SPD_OLD);
    if(changed){
      refreshAP1 = 1;
    }
    copy(AP_SPD,AP_SPD_OLD);
    tmp.toCharArray(AP_SPD, 4);
  }
}

void DISPLAYAP1() {
  digitalWrite(AP1LATCH,LOW); 
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_CRS[2])]);
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_CRS[1])]);
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_CRS[0])]);
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_HDG[2])]);
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_HDG[1])]);
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_HDG[0])]);
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_SPD[2])]);
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_SPD[1])]);
  delay(10);
  shiftOut(AP1DATA,AP1CLK,MSBFIRST,~dataArray[charToInt(AP_SPD[0])]);
  delay(10);
  digitalWrite(AP1LATCH,HIGH); 
}

void DISPLAYAP2() {
  /*
   * AP_ALT[5] = {'0','0','0','0','0'};
char AP_VS[6]
   */
  digitalWrite(AP2LATCH,LOW); 
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_VS[4])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_VS[3])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_VS[2])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_VS[1])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_VS[0])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_ALT[4])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_ALT[3])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_ALT[2])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_ALT[1])]);
  delay(10);
  shiftOut(AP2DATA,AP2CLK,MSBFIRST,~dataArray[charToInt(AP_ALT[0])]);
  delay(10);
  digitalWrite(AP2LATCH,HIGH); 
}

/*
#define AP2DATA 31
#define AP2CLK 32
#define AP2LATCH 33

#define AP1DATA 50
#define AP1CLK 51
#define AP1LATCH 52
 */








int charToInt(char c){
  switch(c){
  case '0': return 0;  break;
  case '1': return 1;  break;
  case '2': return 2;  break;
  case '3': return 3;  break;
  case '4': return 4;  break;
  case '5': return 5;  break;
  case '6': return 6;  break;
  case '7': return 7;  break;
  case '8': return 8;  break;
  case '9': return 9;  break;
  case '-': return 10; break;
  case '+': return 11; break;
  }
}

char getChar(){
  while(Serial.available() == 0);
  return((char)Serial.read());
}

String getString(int chars){
  String tmp = "";
  for(int i = 0; i < chars; i++){
    tmp += getChar();
  }
  return tmp;
}

/*
 * true if changed
 */
boolean compare(String tmp_a, char tmp_b[]){

  char tmp_1[sizeof(tmp_a)];
  tmp_a.toCharArray(tmp_1, sizeof(tmp_a));

  for(int i = 0; i < sizeof(tmp_b); i++){
    if(tmp_1[i] != tmp_b[i]){
      return true;
    }
  }
  
  return false;
}

void copy(char* src, char* dst) {
  memcpy(dst, src, sizeof(src[0]));
}




