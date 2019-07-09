/**
 * FSX MULTIPANEL FIRMWARE v3
 * (c) 2019 JOKUE2002 aka Jonas Kuehn
 * 
 * This panel can handle the autopilot and the radios of a 737 in FSX.
 * It's currently NOT using any extrenal library but one programs.
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
 *  01x AP1 PCB (found at: "")
 *  01x AP2 PCB (found at: "")
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
#define AP2DATA 5
#define AP2CLK 6
#define AP2LATCH 7

#define AP1DATA 2
#define AP1CLK 3
#define AP1LATCH 4

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

/**
 * Definition of data w/ dot on
 * Data: "0.", "1.", "2.", "3.", "4.", "5.", "6.", "7.", "8.", "9.", "-."
 */
byte dataArrayWithDot[11] = 
{B10111111,
B10000110,
B11011011,
B11001111,
B11100110,
B11101101,
B11111101,
B10000111,
B11111111,
B11101111,
B11000000
};

/**
 * bytearrays holding the data for displays (1 and 2)
 */
byte ap1data[9];
byte ap2data[11];

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
  clearData();
  
  if(Serial.available()){
    
    char charIn = getChar();
    
    if(charIn == '='){
      
      charIn = getChar();
      
      if(charIn == 'd'){//HDG
        ap1data[3] = getDataForNextChar();
        ap1data[4] = getDataForNextChar();
        ap1data[5] = getDataForNextChar();
      }
      
      if(charIn == 'e'){//CRS
        ap1data[0] = getDataForNextChar();
        ap1data[1] = getDataForNextChar();
        ap1data[2] = getDataForNextChar();
      }
      
      if(charIn == 'f'){//SPD
        ap1data[6] = getDataForNextChar();
        ap1data[7] = getDataForNextChar();
        ap1data[8] = getDataForNextChar();
      }

      if(charIn == 'b'){//ALT
        ap2data[0] = getDataForNextChar();
        ap2data[1] = getDataForNextChar();
        ap2data[2] = getDataForNextChar();
        ap2data[3] = getDataForNextChar();
        ap2data[4] = getDataForNextChar();
      }

      if(charIn == 'c'){//VS
        ap2data[5] = getDataForNextChar();
        ap2data[6] = getDataForNextChar();
        ap2data[7] = getDataForNextChar();
        ap2data[8] = getDataForNextChar();
        ap2data[9] = getDataForNextChar();
        ap2data[10] = getDataForNextChar();
      }
    }

    updateAP1();
    updateAP2();
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
byte getDataForNextChar() {
  char nextChar = getChar();
  switch(nextChar){
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
 * Writes data from ap1data-array to display1
 */
void updateAP1() {
  digitalWrite(AP1LATCH, LOW);
  for(int i = 0; i < 9; i++){
    delay(1);
    shiftOut(AP1DATA, AP1CLK, MSBFIRST, ~ap1data[i]);
  }
  digitalWrite(AP1LATCH, HIGH);
}

/**
 * Writes data from ap2data-array to display2
 */
void updateAP2() {
  digitalWrite(AP2LATCH, LOW);
  for(int i = 0; i < 10; i++){
    delay(1);
    shiftOut(AP2DATA, AP2CLK, MSBFIRST, ~ap2data[i]);
  }
  digitalWrite(AP2LATCH, HIGH);
}

/**
 * Clears data from both ap*data-arrays and both displays
 */
void clearData() {
  //Clear Ap1 display + data-array
  digitalWrite(AP1LATCH, LOW);
  for(int i = 0; i < 9; i++){
    delay(1);
    shiftOut(AP1DATA, AP1CLK, MSBFIRST, ~B00000000);
    ap1data[i] = B00000000;
  }
  digitalWrite(AP1LATCH, HIGH);

  //Clear Ap2 display + data-array
  digitalWrite(AP2LATCH, LOW);
  for(int i = 0; i < 10; i++){
    delay(1);
    shiftOut(AP2DATA, AP2CLK, MSBFIRST, ~B00000000);
    ap2data[i] = B00000000;
  }
  ap2data[10] = B00000000;
  digitalWrite(AP2LATCH, HIGH);
}
