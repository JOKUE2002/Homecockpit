/**
 * FSX MULTIPANEL by JOKUE2002
 * 
 * This panel can handle the autopilot and the radios of a 737 in FSX.
 * I'm using several extrenal libraries and programs.
 * 
 * 
 * LIBRARIES AND EXTRENAL PROGRAMS NEEDED
 *  Rotary Encoder Library - https://github.com/buxtronix/arduino
 *  Link2FS                - http://www.jimspage.co.nz/link2fs_multi_fsx_v5h.zip
 *  LiquidCrystal          - Standard Arduino Library 
 *                                (preinstalled with Arduino Software)
 *  Arduino Software       - https://www.arduino.cc/en/Main/Software
 *  FSUIPC                 - Preinstalled on FSX; 
 *                                You have to install it later, 
 *                                if you want to use FSX:SteamEdition
 *  FSX / FSX:SE           - http://store.steampowered.com/app/314160/Microsoft_Flight_Simulator_X_Steam_Edition/
 *  
 * INSTALLATION
 *  0.  Download all necessary files/installers.
 *  1.  Install Arduino and all its drivers.
 *  2.  Restart the Computer.
 *  3.  Install 
 *     a.  FSX 
 *     b.  Steam and FSX:SteamEdition
 *  4.  Copy the Rotary Encoder Library to the "Arduino/libraries/" folder, 
 *        which can be found in your "Documents" folder.
 *  5.  If you use FSX:SteamEdition, than you have to install FSUIPC. Therefor,
 *       go to: "C:\Program Files (x86)\Steam\steamapps\common\FSX\SDK\
 *        Core Utilities Kit\SimConnect SDK\LegacyInterfaces\FSX-RTM\"
 *       and double click on "SimConnect.msi" 
 *       (if there are multiple, just do all of them)
 *  6.  Now "install" Link2FS. You basically just have to download it and copy it 
 *       into a folder, that you won't lost or delete by accident. 
 *  7.  Upload this code to your Arduino MEGA 2560, with the hardware all built.
 *  8.  Start Link2FS and select all extractions in the list at the 
 *      "Extractions(1)" and "Extractions(2)" tab.
 *  9.  Start FSX or FSX:SteamEdition. Go to freeflight, select your airport,
 *        and the 737 as the plane. Now start the flight.
 *  10. Start the card with the correct COM-Port after you connected the arduino 
 *      to your computer. 
 *  11. Enjoy the flight and your FSXPanel.
 *  
 * EXTRACTIONS THAT NEED TO BE ACTIVE
 * 
 *  
 * PARTS NEEDED
 * 1x Arduino MEGA 2560
 * 1x 1602 LCD Display
 * 5x Switch On/Off (SPST)
 * 2x Pushbutton
 * 3x LEDs (3x RGB)
 * 2x Rotary Encoders
 * 
 * WIRING
 * Red LED for Nose Gear       - 22
 * Green LED for Nose Gear     - 23
 * Red LED for Left Gear       - 24
 * Green LED for Left Gear     - 25
 * Red LED for Right Gear      - 26
 * Green LED for Right Gear    - 27
 * Pageselect Rotary pin 1     - 28
 * Pageselect Rotary pin 2     - 29
 * Pageselect Rotary button    - 30
 * AP Master switch            - 31
 * Gear switch                 - 32
 * Autothrottle switch         - 33
 * NAV/GPS Mode switch         - 34
 * Approach hold switch        - 35
 * Freq. mode button           - 36
 * Info button                 - 37
 * Selectionset Rotary pin 1   - 38
 * Selectionset Rotary pin 2   - 39
 * Selectionset Rotary button  - 40
 * LCD RS                      - 12
 * LCD enable                  - 11
 * LCD D4                      - 5
 * LCD D5                      - 4
 * LCD D6                      - 3
 * LCD D7                      - 2
 */

#include <Rotary.h>
#include <LiquidCrystal.h>

#define GEAR_NOSE_RED 22
#define GEAR_NOSE_GREEN 23
#define GEAR_LEFT_RED 24
#define GEAR_LEFT_GREEN 25
#define GEAR_RIGHT_RED 26
#define GEAR_RIGHT_GREEN 27
 
#define pageSelectPinA 28
#define pageSelectPinB 29
#define pageSelectButton 30
#define setSelectPinA 38
#define setSelectPinB 39
#define setSelectButton 40
#define MAXPAGE 10
#define MAXCOMSUBPAGE 3

Rotary pageSelect = Rotary(pageSelectPinA, pageSelectPinB);
Rotary setSelect = Rotary(setSelectPinA, setSelectPinB);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

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

int charIn;
int PAGE = 0;
int COMSUBPAGE = 0;
boolean leftMode = true;
/**
 * PAGEs
 * 
 * 0  Info  (Flaps        | Current Fuel in %)
 * 1  AP1   (Master       | Mode)
 * 2  AP2   (F/D          | ATHR)
 * 3  AP3   (SPD          | HDG)
 * 4  AP4   (ALT          | V/S)
 * 5  AP5   (APP Master   | APP Crs)
 * 6  Com1  (Com1 Active  | Com1 S/B)
 * 7  Com2  (Com2 Active  | Com2 S/B)
 * 8  Nav1  (Nav1 Active  | Nav1 S/B)
 * 9  Nav2  (Nav2 Active  | Nav2 S/B)
 * 10 Com   (Com1on Com2on Nav1on Nav2on | Cursor)
 */

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
}

void loop() {
  ROTARYS();
  if(Serial.available()){
    charIn = getChar();
    if(charIn == '='){
      EQUALS();
    }
    if(charIn == '<'){
      //LESSTHAN();
    }
  }
}

char getChar()
{
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

void EQUALS(){ // First Identifier was '='
  charIn = getChar();
  String tmp = "";
  switch(charIn){
    case 'A'://COM 1 Freq - 7 chars 127.275
    com1active = getString(7);
    break;
    case 'B'://COM 1 s/b - 7 chars
    com1standby = getString(7);
    break;
    case 'E'://Nav 1 Freq - 6 chars
    nav1active = getString(6);
    break;
    case 'F'://Nav 1 s/b - 6 chars
    nav1standby = getString(6);
    break;
    case 'a'://AP active - 1 char
    tmp = getString(1);
    if(tmp == "1"){
      ap_on = true;
    }else{
      ap_on = false;
    }
    break;
    case 'b'://AP Alt Set - 5 chars
    ap_alt = getString(5);
    break;
    case 'c'://AP VS Set - 6 chars
    ap_vs = getString(6);
    break;
    case 'd'://AP Heading Set - 3 chars
    ap_hdg = getString(3);
    break;
    case 'e'://APP Heading Set - 3 chars
    app_hdg = getString(3);
    break;
    case 'f'://AP Speed Set - 3 chars
    ap_spd = getString(3);
    break;
    case 'l'://AP Mode - 1 char
    tmp = getString(1);
    if(tmp == "1"){
      ap_mode = "GPS";
    }else{
      ap_mode = "NAV";
    }
    break;
    case 'm'://APP Active - 1 char
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
    gear_nose = getString(3).toInt();
    break;
    case 'B'://GearLeft
    gear_left = getString(3).toInt();
    break;
    case 'c'://GearRight
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
}

void ROTARYS(){
  unsigned char result = pageSelect.process();
  if(result == DIR_CW){
    PAGE++;
    if(PAGE > MAXPAGE){
      PAGE = MAXPAGE;
    }
  }else if(result == DIR_CCW){
    if(PAGE > 0){
      PAGE--;
    }
  }
  result = setSelect.process();
  if(result == DIR_CW){//+
    plus();
  }else if(result == DIR_CCW){//-
    minus();
  }
}

void plus(){
  switch(PAGE){
    case 0: break;
    case 1: break;
    case 2: break;
    case 3: 
    if(leftMode){
      //B15
      Serial.println("B15");
    }else{
      //A57
      Serial.println("A57");
    }
    break;
    case 4: 
    if(leftMode){
      //B11
      Serial.println("B11");
    }else{
      //B13
      Serial.println("B13");
    }
    break;
    case 5: 
    if(!leftMode){
      //A56
      Serial.println("A56");
    }
    break;
    case 6: 
    if(leftMode){
      //A02
      Serial.println("A02");
    }else{
      //A04
      Serial.println("A04");
    }
    break;
    case 7: 
    if(leftMode){
      //A08
      Serial.println("A08");
    }else{
      //A10
      Serial.println("A10");
    }
    break;
    case 8: 
    if(leftMode){
      //A14
      Serial.println("A14");
    }else{
      //A16
      Serial.println("A16");
    }
    break;
    case 9:
    if(leftMode){
      //A20
      Serial.println("A20");
    }else{
      //A22
      Serial.println("A22");
    }
    break;
    case 10:
    COMSUBPAGE++;
    if(COMSUBPAGE > MAXCOMSUBPAGE){
      COMSUBPAGE = MAXCOMSUBPAGE;
    }
    break;
  }
}

/**
 * PAGEs
 * 
 * 0  Info  (Flaps        | Current Fuel in %)
 * 1  AP1   (Master       | Mode)
 * 2  AP2   (F/D          | ATHR)
 * 3  AP3   (SPD          | HDG)
 * 4  AP4   (ALT          | V/S)
 * 5  AP5   (APP Master   | APP Crs)
 * 6  Com1  (Com1 Active  | Com1 S/B)
 * 7  Com2  (Com2 Active  | Com2 S/B)
 * 8  Nav1  (Nav1 Active  | Nav1 S/B)
 * 9  Nav2  (Nav2 Active  | Nav2 S/B)
 * 10 Com   (Com1on Com2on Nav1on Nav2on | Cursor)
 */

void minus(){
  switch(PAGE){
    case 0: break;
    case 1: break;
    case 2: break;
    case 3: 
    if(leftMode){
      //B16
      Serial.println("B16");
    }else{
      //A58
      Serial.println("A58");
    }
    break;
    case 4: 
    if(leftMode){
      //B12
      Serial.println("B12");
    }else{
      //B14
      Serial.println("B14");
    }
    break;
    case 5: 
    if(!leftMode){
      //A55
      Serial.println("A55");
    }
    break;
    case 6: 
    if(leftMode){
      //A01
      Serial.println("A01");
    }else{
      //A03
      Serial.println("A03");
    }
    break;
    case 7: 
    if(leftMode){
      //A07
      Serial.println("A07");
    }else{
      //A09
      Serial.println("A09");
    }
    break;
    case 8: 
    if(leftMode){
      //A13
      Serial.println("A13");
    }else{
      //A15
      Serial.println("A15");
    }
    break;
    case 9:
    if(leftMode){
      //A19
      Serial.println("A19");
    }else{
      //A21
      Serial.println("A21");
    }
    break;
    case 10:
    if(COMSUBPAGE > 0){
      COMSUBPAGE--;
    }
    break;
  }
}

/*
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
 */

