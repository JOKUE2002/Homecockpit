/**
 * FSX MULTIPANEL FIRMWARE v4
 * (c) 2012 JOKUE2002 aka Jonas Kuehn
 * 
 * This panel can handle the autopilot and the radios of a 737 in FSX.
 * It's currently NOT using any extrenal library but some programs.
 * 
 * 
 * LIBRARIES AND EXTRENAL PROGRAMS NEEDED(*) +NICE TO HAVES(+)
 *  + Rotary Encoder Library - https://github.com/buxtronix/arduino
 *  * Link2FS                - http://www.jimspage.co.nz/link2fs_multi_fsx_v5h.zip
 *  + LiquidCrystal          - Standard Arduino Library 
 *                                (preinstalled with Arduino Software)
 *  * Arduino Software       - https://www.arduino.cc/en/Main/Software
 *  * FSUIPC                 - Preinstalled on FSX; 
 *                                You have to install it later, 
 *                                if you want to use FSX:SteamEdition
 *  * FSX / FSX:SE           - http://store.steampowered.com/app/314160/
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
 *       into a folder, that you won't "lose" or delete by accident. 
 *  7.  Upload this code to your Arduino MEGA 2560, with the hardware all built.
 *  8.  Start Link2FS and select all extractions in the list below at the 
 *      "Extractions(1)" and "Extractions(2)" tab.
 *  9.  Start FSX or FSX:SteamEdition. Go to freeflight, select your airport,
 *        and the 737 as the plane. Now start the flight.
 *  10. Start the card with the correct COM-Port after you connected the arduino 
 *      to your computer. 
 *  11. Enjoy the flight and your FSXPanel.
 *  
 * EXTRACTIONS THAT NEED TO BE ACTIVE
 *  =a AP(Autopilot)active
 *  =b AP altitude set
 *  =c AP vertical speed set
 *  =d AP heading set
 *  =e AP course (CRS) set
 *  =f AP speed set knots
 *  
 * PARTS NEEDED
 *  01x Arduino MEGA 2560
 *  19x 7Segment Displays
 *  19x 74ls595 or 74hc595 ICs
 *  01x AP1 PCB (found at the github repository)
 *  01x AP2 PCB (found at the github repository)
 *  06x LED 5mm
 *  21x 220Ohm resisitor (THT)
 * 
 * WIRING
 *  Solder the parts to the pcbs
 *  connect pin 1 (3 pin connector at bottom-left corner (seen from top); ap1)
 *    to pin 2 (on the arduino)
 *  connect pin 2 (same connector) to pin 3 (arduino)
 *  connect pin 3 (same connector) to pin 4 (arduino)
 *  
 *  connect pin 1 (3 pin connector at middle-left side (seen from top); ap2)
 *    to pin 5 (on the arduino)
 *  connect pin 6 (same connector) to pin 3 (arduino)
 *  connect pin 7 (same connector) to pin 4 (arduino)
 *  
 * RUNNING / USING
 *  1. Start FSX (as usual) and start into a flight. 
 *      Now pause the game (press "P")
 *  2. Start Link2FS (from your "link2fs" folder)
 *  3. Connect arduino to USB
 *  4. Connect arduino to link2s 
 *    (click connect on page 1 of link2fs and select com port of arduino)
 *  5. Resume FSX Game (Tab to FSX and press "P" again)
 *  
 */

/**
 * Pinout definition
 */

#define AP1DATA 22
#define AP1CLK 23
#define AP1LATCH 24

#define AP2DATA 25
#define AP2CLK 26
#define AP2LATCH 27


#define COM1DATA 28
#define COM1CLK 29
#define COM1LATCH 30

#define COM2DATA 31
#define COM2CLK 32
#define COM2LATCH 33


#define NAV1DATA 34
#define NAV1CLK 35
#define NAV1LATCH 36

#define NAV2DATA 37
#define NAV2CLK 38
#define NAV2LATCH 39

/**
 * Definition of data w/o dot on
 * Data: "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "-"
 */
byte dataArray[11] = 
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
B01000000
};

byte dotOn = B10000000;

/**
 * all the data, which gets passed by Link2FS
 */
String hdg;
String crs;
String spd;
String alt;
String vs;
String c1a;
String c1s;
String c2a;
String c2s;
String n1a;
String n1s;
String n2a;
String n2s;

String oldhdg;
String oldcrs;
String oldspd;
String oldalt;
String oldvs;
String oldc1a;
String oldc1s;
String oldc2a;
String oldc2s;
String oldn1a;
String oldn1s;
String oldn2a;
String oldn2s;

enum callsForUpdate {
  HDG,
  SPD,
  ALT,
  CRS,
  VS,
  C1A,
  C1S,
  C2A,
  C2S,
  N1A,
  N1S,
  N2A,
  N2S
};


/**
 * MAIN SETUP
 * 1. Begin serial transmission
 * 2. define pin-outputs
 */
void setup() {
  Serial.begin(115200);
  
  pinMode(AP1DATA,OUTPUT);
  pinMode(AP1CLK,OUTPUT);
  pinMode(AP1LATCH,OUTPUT);
  digitalWrite(AP1LATCH, HIGH);
  digitalWrite(AP1DATA, LOW);
  digitalWrite(AP1CLK, LOW);

  pinMode(AP2DATA,OUTPUT);
  pinMode(AP2CLK,OUTPUT);
  pinMode(AP2LATCH,OUTPUT);
  digitalWrite(AP2LATCH, HIGH);
  digitalWrite(AP2DATA, LOW);
  digitalWrite(AP2CLK, LOW);

  clearData();
}

/**
 * MAIN LOOP
 * 
 * 1. clear all data
 * 2. check for serial signal
 * 3. decode signal (2-char-code, e.g. "=b")
 * 4. read and save data
 * 5. update displays
 */
void loop() {
  if(Serial.available()){
    
    char charIn = getChar();
    
    if(charIn == '='){
      
      charIn = getChar();
      
      if(charIn == 'd'){//HDG
        hdg = "";
        hdg += getChar();
        hdg += getChar();
        hdg += getChar();

        if(hdg != oldhdg) {
          updateDisplays(HDG);
          oldhdg = hdg;
        }
      }
      
      if(charIn == 'e'){//CRS
        crs = "";
        crs += getChar();
        crs += getChar();
        crs += getChar();

        if(crs != oldcrs) {
          updateDisplays(CRS);
          oldcrs = crs;
        }
      }
      
      if(charIn == 'f'){//SPD
        spd = "";
        spd += getChar();
        spd += getChar();
        spd += getChar();

        if(spd != oldspd) {
          updateDisplays(SPD);
          oldspd = spd;
        }
      }

      if(charIn == 'b'){//ALT
        alt = "";
        alt += getChar();
        alt += getChar();
        alt += getChar();
        alt += getChar();
        alt += getChar();

        if(alt != oldalt) {
          updateDisplays(ALT);
          oldalt = alt;
        }
      }

      if(charIn == 'c'){//VS
        vs = "";
        vs += getChar();
        vs += getChar();
        vs += getChar();
        vs += getChar();
        vs += getChar();
        vs += getChar();

        if(vs != oldvs) {
          updateDisplays(VS);
          oldvs = vs;
        }
      }

      if(charIn == 'A'){//Com1Active
        c1a = "";
        c1a += getChar();
        c1a += getChar();
        c1a += getChar();
        c1a += getChar();
        c1a += getChar();
        c1a += getChar();
        c1a += getChar();

        if(c1a != oldc1a) {
          updateDisplays(C1A);
          oldc1a = c1a;
        }
      }

      if(charIn == 'B'){//Com1Standby
        c1s = "";
        c1s += getChar();
        c1s += getChar();
        c1s += getChar();
        c1s += getChar();
        c1s += getChar();
        c1s += getChar();
        c1s += getChar();

        if(c1s != oldc1s) {
          updateDisplays(C1S);
          oldc1s = c1s;
        }
      }

      if(charIn == 'C'){//Com2Active
        c2a = "";
        c2a += getChar();
        c2a += getChar();
        c2a += getChar();
        c2a += getChar();
        c2a += getChar();
        c2a += getChar();
        c2a += getChar();

        if(c2a != oldc2a) {
          updateDisplays(C2A);
          oldc2a = c2a;
        }
      }

      if(charIn == 'D'){//Com2Standby
        c2s = "";
        c2s += getChar();
        c2s += getChar();
        c2s += getChar();
        c2s += getChar();
        c2s += getChar();
        c2s += getChar();
        c2s += getChar();

        if(c2s != oldc2s) {
          updateDisplays(C2S);
          oldc2s = c2s;
        }
      }

      if(charIn == 'E'){//Nav1Active
        n1a = "";
        n1a += getChar();
        n1a += getChar();
        n1a += getChar();
        n1a += getChar();
        n1a += getChar();
        n1a += getChar();

        if(n1a != oldn1a) {
          updateDisplays(N1A);
          oldn1a = n1a;
        }
      }

      if(charIn == 'F'){//Nav1Standby
        n1s = "";
        n1s += getChar();
        n1s += getChar();
        n1s += getChar();
        n1s += getChar();
        n1s += getChar();
        n1s += getChar();

        if(n1s != oldn1s) {
          updateDisplays(N1S);
          oldn1s = n1s;
        }
      }

      if(charIn == 'G'){//Nav2Active
        n2a = "";
        n2a += getChar();
        n2a += getChar();
        n2a += getChar();
        n2a += getChar();
        n2a += getChar();
        n2a += getChar();

        if(n2a != oldn2a) {
          updateDisplays(N2A);
          oldn2a = n2a;
        }
      }

      if(charIn == 'H'){//Nav2Standby
        n2s = "";
        n2s += getChar();
        n2s += getChar();
        n2s += getChar();
        n2s += getChar();
        n2s += getChar();
        n2s += getChar();

        if(n2s != oldn2s) {
          updateDisplays(N2S);
          oldn2s = n2s;
        }
      }
    }
  }
}

/**
 * Received the next char over the Serial-bus (USB)
 */
char getChar() {
  while(Serial.available() == 0);
  return((char)Serial.read());
}

/**
 * Receives a String of given length, using the getChar method
 */
String getString(int numOfChars){
  String tmp = "";
  for(int i = 0; i < numOfChars; i++){
    tmp += getChar();
  }
  return tmp;
}

/**
 * Translates received char into a byte to be displayed
 */
byte getDataForChar(char c) {
  switch(c){
  case '0': return dataArray[0]; break;
  case '1': return dataArray[1]; break;
  case '2': return dataArray[2]; break;
  case '3': return dataArray[3]; break;
  case '4': return dataArray[4]; break;
  case '5': return dataArray[5]; break;
  case '6': return dataArray[6]; break;
  case '7': return dataArray[7]; break;
  case '8': return dataArray[8]; break;
  case '9': return dataArray[9]; break;
  case '-': return dataArray[10]; break;
  default: return B00000000; break;
  }
}

/**
 * Calls all display update functions
 * 
 * The number indicates, which one called
 */
void updateDisplays(callsForUpdate caller) {
  if(caller == HDG || caller == SPD || caller == CRS) {
    updateAP1();
  }
  if(caller == ALT || caller == VS){
    updateAP2();
  }
  if(caller == C1A || caller == C1S){
    updateCom1();
  }
  if(caller == C2A || caller == C2S){
    updateCom2();
  }
  if(caller == N1A || caller == N1S){
    updateNav1();
  }
  if(caller == N2A || caller == N2S){
    updateNav2();
  }
 }

/**
 * Writes data from ap1data-array to display1
 */
void updateAP1() {
  String full = spd + hdg + crs;
  
  int len = full.length(); 
  char char_array[len];
  
  full.toCharArray(char_array, len);
  
  digitalWrite(AP1LATCH, LOW);
  delay(20);
  for(int i = len; i >= 0; i--){
    delay(2);
    shiftOut(AP1DATA, AP1CLK, MSBFIRST, ~getDataForChar(char_array[i]));
  }
  delay(20);
  digitalWrite(AP1LATCH, HIGH);
}

/**
 * Writes data from ap2data-array to display2
 */
void updateAP2() {
  /*digitalWrite(AP2LATCH, LOW);
  for(int i = 10; i >= 0; i--){
    delay(1);
    shiftOut(AP2DATA, AP2CLK, MSBFIRST, ~ap2data[i]);
  }
  digitalWrite(AP2LATCH, HIGH);*/
}

/**
 * Writes data from com1data- & com1sbdata-array to display3
 */
void updateCom1() {
  /*digitalWrite(COM1LATCH, LOW);
  for(int i = 10; i >= 0; i--){
    delay(1);
    shiftOut(COM1DATA, COM1CLK, MSBFIRST, ~com1data[i]);
  }
  digitalWrite(COM1LATCH, HIGH);*/
}

/**
 * Writes data from com2data- & com2sbdata-array to display4
 */
void updateCom2() {
  /*digitalWrite(COM2LATCH, LOW);
  for(int i = 4; i >= 0; i--){
    delay(1);
    shiftOut(COM2DATA, COM2CLK, MSBFIRST, ~com2data[i]);
  }
  digitalWrite(COM2LATCH, HIGH);*/
}

/**
 * Writes data from nav1data- & nav1sbdata-array to display5
 */
void updateNav1() {
  /*digitalWrite(NAV1LATCH, LOW);
  for(int i = 10; i >= 0; i--){
    delay(1);
    shiftOut(NAV1DATA, NAV1CLK, MSBFIRST, ~nav1data[i]);
  }
  digitalWrite(NAV1LATCH, HIGH);*/
}

/**
 * Writes data from nav2data- & nav2data-array to display6
 */
void updateNav2() {
  /*digitalWrite(NAV2LATCH, LOW);
  for(int i = 10; i >= 0; i--){
    delay(1);
    shiftOut(NAV2DATA, NAV2CLK, MSBFIRST, ~nav2data[i]);
  }
  digitalWrite(NAV2LATCH, HIGH);*/
}

/**
 * Clears data from all *data-arrays and all displays
 */
void clearData() {
  spd = "";
  oldspd = "a";
  hdg = "";
  oldhdg = "a";
  crs = "";
  oldcrs = "a";
  
  //Clear Ap1 display + data-array
  digitalWrite(AP1LATCH, LOW);
  delay(20);
  for(int i = 0; i < 9; i++){
    delay(2);
    shiftOut(AP1DATA, AP1CLK, MSBFIRST, ~B00000000);
  }
  delay(20);
  digitalWrite(AP1LATCH, HIGH);
}

void fillData() {
  digitalWrite(AP1LATCH, LOW);
  delay(20);
  for(int i = 0; i < 9; i++){
    delay(2);
    shiftOut(AP1DATA, AP1CLK, MSBFIRST, B00000000);
  }
  delay(20);
  digitalWrite(AP1LATCH, HIGH);
}
