#include <Keypad.h>
#include <Rotary.h>

char keys[4][4] = {
  {'A','B','C','D'},
  {'E','F','G','H'},
  {'I','J','K','L'},
  {'M','N','O','P'}
};
byte colPins[4] = {15, 14, 16, 10};
byte rowPins[4] = {A3, A2, A1, A0};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 );

Rotary rotary1 = Rotary(2, 3);
Rotary rotary2 = Rotary(4, 5);
Rotary rotary3 = Rotary(6, 7);

int prevResult = 2;

void setup() {
  Serial.begin(115200);
  pinMode(9,INPUT_PULLUP);
}

void loop() {
  KEYS();
  ROTARYS();
  SWITCH();
}

void KEYS(){
  char key = keypad.getKey();
  if(key){
    switch(key){
      case 'A': Serial.println("C04"); break; //Parkingbrake
      case 'B': Serial.println("C08"); break; //pause
      case 'C': Serial.println("E16"); break; //master battery
      case 'D': Serial.println("C29"); break; //pushback
      case 'E': Serial.println("A45"); break; //Com  I to transmit
      case 'F': Serial.println("A46"); break; //Com II to transmit
      case 'G': Serial.println("A06"); break; //Swap Com  I s/b
      case 'H': Serial.println("A12"); break; //Swap Com II s/b
      case 'I': Serial.println("A02"); break; //Incr. Com  I by 1Mhz
      case 'J': Serial.println("A04"); break; //Incr. Com  I by .25kHz
      case 'K': Serial.println("A03"); break; //Decr. Com  I by .25kHz
      case 'L': Serial.println("A01"); break; //Decr. Com  I by 1Mhz
      case 'M': Serial.println("A08"); break; //Incr. Com II by 1Mhz
      case 'N': Serial.println("A10"); break; //Incr. Com II by .25kHz
      case 'O': Serial.println("A09"); break; //Decr. Com II by .25kHz
      case 'P': Serial.println("A07"); break; //Decr. Com II by 1Mhz
    }
  }
}

void ROTARYS(){
  unsigned char result = rotary1.process();
  if (result == DIR_CW) {
    Serial.println("B15"); //spd hold +
  } else if (result == DIR_CCW) {
    Serial.println("B16"); //spd hold -
  }

  result = rotary2.process();
  if (result == DIR_CW) {
    Serial.println("A57"); //hdg hold +
  } else if (result == DIR_CCW) {
    Serial.println("A58"); //hdg hold -
  }

  result = rotary3.process();
  if (result == DIR_CW) {
    Serial.println("B11"); //alt hold +
  } else if (result == DIR_CCW) {
    Serial.println("B12"); //alt hold -
  }
}

void SWITCH(){
  int result = digitalRead(9);

  if(result == 0){//SWITCH DOWN
    if(result != prevResult){
      Serial.println("C02");
    }
  }else{//SWITCH UP
    if(result != prevResult){
      Serial.println("C01");
    }
  }
  prevResult = result;
}

