#include <LiquidCrystal.h>

/**
 * FSX Panel by JOKUE2002
 * 
 * This Panel should be capable of handling the Autopilot and the 
 * Radios of a default 737 in FSX.
 * I'm using Link2FS, which can be found here:
 * http://www.jimspage.co.nz/intro.htm
 * 
 * PARTS:
 * 1 x Arduino Mega 2560
 * 1 x 1602 LCD Display
 * 1 x 0.96" OLED Display
 * 3 x Buttons
 * 1 x Switch
 * 7 x Rotary Encoders
 * 9 x LEDs
 * 1 x Potentiometer
 * 
 * 
 * WIRING:
 * 22-27 GearLEDs
 * 28-48 RotaryEncoders
 * 49-52 Buttons
 * 2-5/11-12 LCD 
 */

 #define GEAR_NOSE_RED 22
 #define GEAR_NOSE_GREEN 23
 #define GEAR_LEFT_RED 24
 #define GEAR_LEFT_GREEN 25
 #define GEAR_RIGHT_RED 26
 #define GEAR_RIGHT_GREEN 27
 //28 - 48  -->  rotary encoders
 #define AP_MASTER_BTN 49
 #define AP_ATHR_BTN 50
 #define SWAP_FREQ 51
 #define GEAR_SWITCH 52
 #define PAGE_UP 6
 #define PAGE_DOWN 7

struct encoder { 
   int pin1;
   int pin2;
   int button;
   int p1state;
   int p2state;
   int p1last;
   int p2last;
};

typedef struct encoder Rotary;

int charIn;

String com1active;
String com1standby;
String nav1active;
String nav1standby;
boolean ap_on;
boolean ap_athr;
String ap_alt;
String ap_hdg;
String ap_vs;
String ap_spd;
boolean app_on;
String app_hdg;
String ap_mode;
int gear_nose;
int gear_left;
int gear_right;
int fuel_percent;
boolean stall;

int brightness;
boolean freqmode; //true = MHz set; false = .25 MHz set
int page = 0; //0 = com1active com1s/b; 1 = nav1active nav1s/b; 2 = fuel% nav1hdg(app_hdg); 3 = apspd aphdg; 4 = apalt apvs; 5 = ap_on ap_athr; 6 = app_on ap_mode

Rotary FreqRotary;
Rotary NavRadioRotary;
Rotary NavHeadingRotary;
Rotary SpdRotary;
Rotary HDGRotary;
Rotary HeightRotary;
Rotary VSRotary;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(115200);
  lcd.begin(16,2);
  lcd.println("     F S X     ");
  lcd.println("(c)  by  JOKUE ");
  pinMode(GEAR_NOSE_RED, OUTPUT);
  pinMode(GEAR_NOSE_GREEN, OUTPUT);
  pinMode(GEAR_LEFT_RED, OUTPUT);
  pinMode(GEAR_LEFT_GREEN, OUTPUT);
  pinMode(GEAR_RIGHT_RED, OUTPUT);
  pinMode(GEAR_RIGHT_GREEN, OUTPUT);
  FreqRotary.pin1 = 28;
  FreqRotary.pin2 = 29;
  FreqRotary.button = 30;
  NavRadioRotary.pin1 = 31;
  NavRadioRotary.pin2 = 32;
  NavRadioRotary.button = 33;
  NavHeadingRotary.pin1 = 34;
  NavHeadingRotary.pin2 = 35;
  NavHeadingRotary.button = 36;
  SpdRotary.pin1 = 37;
  SpdRotary.pin2 = 38;
  SpdRotary.button = 39;
  HDGRotary.pin1 = 40;
  HDGRotary.pin2 = 41;
  HDGRotary.button = 42;
  HeightRotary.pin1 = 43;
  HeightRotary.pin2 = 44;
  HeightRotary.button = 45;
  VSRotary.pin1 = 46;
  VSRotary.pin2 = 47;
  VSRotary.button = 48;
}

void loop() {
  ROTARYS();
  if(Serial.available()){
    charIn = getChar();
    if(charIn == '='){
      EQUALS();
    }
    if(charIn == '<'){
      LESSTHAN();
    }
  }
  OUT();
}

char getChar()
{
  while(Serial.available() == 0);
  return((char)Serial.read());
}

void EQUALS(){ // First Identifier was '='
  charIn = getChar();
  String tmp = "";
  switch(charIn){
    case 'A'://COM 1 Freq - 7 chars
    delay(11);
    com1active = getString(7);
    break;
    case 'B'://COM 1 s/b - 7 chars
    delay(11);
    com1standby = getString(7);
    break;
    case 'E'://Nav 1 Freq - 6 chars
    delay(11);
    nav1active = getString(6);
    break;
    case 'F'://Nav 1 s/b - 6 chars
    delay(11);
    nav1standby = getString(6);
    break;
    case 'a'://AP active - 1 char
    delay(11);
    tmp = getString(1);
    if(tmp == "1"){
      ap_on = true;
    }else{
      ap_on = false;
    }
    break;
    case 'b'://AP Alt Set - 5 chars
    delay(11);
    ap_alt = getString(5);
    break;
    case 'c'://AP VS Set - 6 chars
    delay(11);
    ap_vs = getString(6);
    break;
    case 'd'://AP Heading Set - 3 chars
    delay(11);
    ap_hdg = getString(3);
    break;
    case 'e'://APP Heading Set - 3 chars
    delay(11);
    app_hdg = getString(3);
    break;
    case 'f'://AP Speed Set - 3 chars
    delay(11);
    ap_spd = getString(3);
    break;
    case 'l'://AP Mode - 1 char
    delay(11);
    tmp = getString(1);
    if(tmp == "1"){
      ap_mode = "GPS";
    }else{
      ap_mode = "NAV";
    }
    break;
    case 'm'://APP Active - 1 char
    delay(11);
    tmp = getString(1);
    if(tmp == "1"){
      app_on = true;
    }else{
      app_on = false;
    }
    break;
  }
}

void LESSTHAN() { //First Identifier was '<'
  charIn = getChar();
  switch(charIn){
    case 'A'://GearNose
    delay(11);
    gear_nose = getString(3).toInt();
    break;
    case 'B'://GearLeft
    delay(11);
    gear_left = getString(3).toInt();
    break;
    case 'c'://GearRight
    delay(11);
    gear_right = getString(3).toInt();
    break;
  }
}

void OUT(){
  if(gear_nose < 1){ // UP
    digitalWrite(GEAR_NOSE_RED,LOW);
    digitalWrite(GEAR_NOSE_GREEN,LOW);
  }else if(gear_nose > 1 && gear_nose < 99){ // MOVING
    digitalWrite(GEAR_NOSE_RED,HIGH);
    digitalWrite(GEAR_NOSE_GREEN,LOW);
  }else if(gear_nose > 99){ // DOWN
    digitalWrite(GEAR_NOSE_RED,LOW);
    digitalWrite(GEAR_NOSE_GREEN,HIGH);
  }
  if(gear_left < 1){ // UP
    digitalWrite(GEAR_LEFT_RED,LOW);
    digitalWrite(GEAR_LEFT_GREEN,LOW);
  }else if(gear_left > 1 && gear_left < 99){ // MOVING
    digitalWrite(GEAR_LEFT_RED,HIGH);
    digitalWrite(GEAR_LEFT_GREEN,LOW);
  }else if(gear_left > 99){ // DOWN
    digitalWrite(GEAR_LEFT_RED,LOW);
    digitalWrite(GEAR_LEFT_GREEN,HIGH);
  }
  if(gear_right < 1){ // UP
    digitalWrite(GEAR_RIGHT_RED,LOW);
    digitalWrite(GEAR_RIGHT_GREEN,LOW);
  }else if(gear_right > 1 && gear_right < 99){ // MOVING
    digitalWrite(GEAR_RIGHT_RED,HIGH);
    digitalWrite(GEAR_RIGHT_GREEN,LOW);
  }else if(gear_right > 99){ // DOWN
    digitalWrite(GEAR_RIGHT_RED,LOW);
    digitalWrite(GEAR_RIGHT_GREEN,HIGH);
  }
  lcd.clear();

  switch(page){
    case 0:
    lcd.setCursor(0,0);
    lcd.print("COM1  ");
    lcd.print(com1active);
    lcd.setCursor(0,1);
    lcd.print("Standby");
    lcd.print(com1standby);
    break;
    case 1:
    lcd.setCursor(0,0);
    lcd.print("NAV1  ");
    lcd.print(nav1active);
    lcd.setCursor(0,1);
    lcd.print("Standby");
    lcd.print(nav1standby);
    break;
    case 2:
    lcd.setCursor(0,0);
    lcd.print("Fuel: ");
    lcd.print(fuel_percent);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("Nav1 Cours: ");
    lcd.print(app_hdg);
    break;
    case 3:
    lcd.setCursor(0,0);
    lcd.print("AP SPD: ");
    lcd.print(ap_spd);
    lcd.print("kts");
    lcd.setCursor(0,1);
    lcd.print("AP HDG: ");
    lcd.print(ap_hdg);
    break;
    case 4:
    lcd.setCursor(0,0);
    lcd.print("AP Alt: ");
    lcd.print(ap_alt);
    lcd.setCursor(0,1);
    lcd.print("AP V/S: ");
    lcd.print(ap_vs);
    break;
    case 5:
    lcd.setCursor(0,0);
    lcd.print("AP Master: ");
    if(ap_on){
      lcd.print("ON");
    }else{
      lcd.print("OFF");
    }
    lcd.setCursor(0,1);
    lcd.print("Athr: ");
    if(ap_athr){
      lcd.print("ON");
    }else{
      lcd.print("OFF");
    }
    break;
    case 6:
    lcd.setCursor(0,0);
    lcd.print("APP: ");
    if(app_on){
      lcd.print("ON");
    }else{
      lcd.print("OFF");
    }
    lcd.setCursor(0,1);
    lcd.print("Mode: ");
    lcd.print(ap_mode);
    break;
  }
}

void ROTARYS(){
  FreqRotary.p1state = digitalRead(FreqRotary.pin1);
  if(FreqRotary.p1state != FreqRotary.p1last){
    if(FreqRotary.p1state != digitalRead(FreqRotary.pin2)){
      //CLOCKWISE (+)
      freqIncrement();
    }else{
      //COUNTERCLOCKWISE (-)
      freqDecrement();
    }
  }

  NavRadioRotary.p1state = digitalRead(NavRadioRotary.pin1);
  if(NavRadioRotary.p1state != NavRadioRotary.p1last){
    if(NavRadioRotary.p1state != digitalRead(NavRadioRotary.pin2)){
      //CLOCKWISE (+)
      Serial.println("A57");
    }else{
      //COUNTERCLOCKWISE (-)
      Serial.println("A58");
    }
  }

  NavHeadingRotary.p1state = digitalRead(NavHeadingRotary.pin1);
  if(NavHeadingRotary.p1state != NavHeadingRotary.p1last){
    if(NavHeadingRotary.p1state != digitalRead(NavHeadingRotary.pin2)){
      //CLOCKWISE (+)
      Serial.println("A57");
    }else{
      //COUNTERCLOCKWISE (-)
      Serial.println("A58");
    }
  }
  
  SpdRotary.p1state = digitalRead(SpdRotary.pin1);
  if(SpdRotary.p1state != SpdRotary.p1last){
    if(SpdRotary.p1state != digitalRead(SpdRotary.pin2)){
      //CLOCKWISE (+)
      Serial.println("B15");
    }else{
      //COUNTERCLOCKWISE (-)
      Serial.println("B16");
    }
  }

  HDGRotary.p1state = digitalRead(HDGRotary.pin1);
  if(HDGRotary.p1state != HDGRotary.p1last){
    if(HDGRotary.p1state != digitalRead(HDGRotary.pin2)){
      //CLOCKWISE (+)
      Serial.println("A57");
    }else{
      //COUNTERCLOCKWISE (-)
      Serial.println("A58");
    }
  }
  
  HeightRotary.p1state = digitalRead(HeightRotary.pin1);
  if(HeightRotary.p1state != HeightRotary.p1last){
    if(HeightRotary.p1state != digitalRead(HeightRotary.pin2)){
      //CLOCKWISE (+)
      Serial.println("B11");
    }else{
      //COUNTERCLOCKWISE (-)
      Serial.println("B12");
    }
  }
  
  VSRotary.p1state = digitalRead(VSRotary.pin1);
  if(VSRotary.p1state != VSRotary.p1last){
    if(VSRotary.p1state != digitalRead(VSRotary.pin2)){
      //CLOCKWISE (+)
      Serial.println("B13");
    }else{
      //COUNTERCLOCKWISE (-)
      Serial.println("B14");
    }
  }
  

  FreqRotary.p1last = FreqRotary.p1state;
  NavRadioRotary.p1last = NavRadioRotary.p1state;
  NavHeadingRotary.p1last = NavHeadingRotary.p1state;
  HDGRotary.p1last = HDGRotary.p1state;
  SpdRotary.p1last = SpdRotary.p1state;
  HDGRotary.p1last = HDGRotary.p1state;
  HeightRotary.p1last = HeightRotary.p1state;
  VSRotary.p1last = VSRotary.p1state;

  if(digitalRead(FreqRotary.button) == HIGH){
    freqmode = !freqmode;
  }
  if(digitalRead(SpdRotary.button) == HIGH){
    Serial.println("B26");
  }
  if(digitalRead(HDGRotary.button) == HIGH){
    Serial.println("B04");
  }
  if(digitalRead(HeightRotary.button) == HIGH || digitalRead(VSRotary.button) == HIGH){
    Serial.println("B05");
  }
}

void freqIncrement(){
  if(freqmode == true){
    Serial.println("A02");
  }else{
    Serial.println("A04");
  }
}

void freqDecrement(){
  if(freqmode == true){
    Serial.println("A01");
  }else{
    Serial.println("A03");
  }
}

String getString(int chars){
  String tmp = "";
  for(int i = 0; i < chars; i++){
    tmp += getChar();
  }
  return tmp;
}


