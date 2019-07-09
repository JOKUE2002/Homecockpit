#define radioLatch 22
#define radioClock 23
#define radioData 24

#define AP1Latch 25
#define AP1Clock 26
#define AP1Data 27

#define AP2Latch 28
#define AP2Clock 29
#define AP2Data 30

//display 0,1,2,3,4,5,6,7,8,9,Blank,Minus
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
B00000000,
B01000000
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

float active = 100.00;
float stand = 100.00;
int spd = 000;
int hdg = 000;
int crs = 000;
int alt = 00000;
int vs = 00000;

void setup() {
  Serial.begin(115200);
  pinMode(radioLatch,OUTPUT);
  pinMode(radioClock,OUTPUT);
  pinMode(radioData,OUTPUT); 
  
  pinMode(AP1Latch, OUTPUT);
  pinMode(AP1Clock, OUTPUT);
  pinMode(AP1Data, OUTPUT);
  
  pinMode(AP2Latch, OUTPUT);
  pinMode(AP2Clock, OUTPUT);
  pinMode(AP2Data, OUTPUT);
}

/**
 * =A - Com1Active
 * =B - Com2Active
 * =a - AP Active
 * =b - AP Alt set
 * =c - AP VS set
 * =d - AP HDG set
 * =e - AP Crs set
 * =f - AP Spd set
 */
void loop() {
  if(Serial.available()){
    char charIn = getChar();
    if(charIn == '='){
      charIn = getChar();
      if(charIn == 'A'){//COM1 ACTIVE
        String tmp = "";
        tmp += getString(6);
        active = tmp.toFloat();
      }
      if(charIn == 'B'){//COM1 S/B
        String tmp = "";
        tmp += getString(6);
        stand = tmp.toFloat();
      }
      if(charIn == 'b'){//Alt set
        String tmp = "";
        tmp += getString(5);
        alt = tmp.toFloat();
      }
      if(charIn == 'c'){//VS set
        String tmp = "";
        tmp += getString(5);
        vs = tmp.toFloat();
      }
      if(charIn == 'd'){//HDG set
        String tmp = "";
        tmp += getString(3);
        hdg = tmp.toFloat();
      }
      if(charIn == 'e'){//Crs set
        String tmp = "";
        tmp += getString(3);
        crs = tmp.toFloat();
      }
      if(charIn == 'f'){//Spd set
        String tmp = "";
        tmp += getString(3);
        spd = tmp.toFloat();
      }
    }
  }
  printRadio(active, stand);
  printAP1(spd,hdg,crs);
  printAP2(alt,vs);
}

  void printRadio(float active, float stand){
    long activeFull = active * 100;
    long standFull = stand * 100;

    activeFull *= 100000;

    long all = standFull + activeFull;
    printRadioDisplay(all);
  }

  void printAP1(long spd, long hdg, long crs){
    spd *= 1000000L;
    hdg *= 1000L;

    long all = spd + hdg + crs;
    printAP1Display(all);
  }

  void printAP2(long alt, long vs){
    alt *= 100000L;

    long all = alt + vs;
    printAP2Display(all);
  }

  void printRadioDisplay(long zahl){
    
    long temp = zahl % 10;
    int z1 = temp;
    zahl = zahl - temp; 
    temp = zahl % 100L;
    int z2 = temp / 10L;
    zahl = zahl - temp; 
    temp = zahl % 1000L;
    int z3 = temp / 100L;
    zahl = zahl - temp; 
    temp = zahl % 10000L;
    int z4 = temp / 1000L;
    zahl = zahl - temp; 
    temp = zahl % 100000L;
    int z5 = temp / 10000L;
    zahl = zahl - temp; 
    temp = zahl % 1000000L;
    int z6 = temp / 100000L;
    zahl = zahl - temp; 
    temp = zahl % 10000000L;
    int z7 = temp / 1000000L;
    zahl = zahl - temp; 
    temp = zahl % 100000000L;
    int z8 = temp / 10000000L;
    zahl = zahl - temp; 
    temp = zahl % 1000000000L;
    int z9 = temp / 100000000L;
    zahl = zahl - temp; 
    temp = zahl % 10000000000L;
    int z10 = temp / 1000000000L;
  
    digitalWrite(radioLatch,LOW); 
    shiftOut(radioData,radioClock,MSBFIRST,~dataArray[z1]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArray[z2]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArrayWithDot[z3]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArray[z4]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArray[z5]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArray[z6]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArray[z7]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArrayWithDot[z8]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArray[z9]);
    shiftOut(radioData,radioClock,MSBFIRST,~dataArray[z10]);
    digitalWrite(radioLatch,HIGH); 
  }

  void printAP1Display(long zahl){
    
    long temp = zahl % 10;
    int z1 = temp;
    zahl = zahl - temp; 
    temp = zahl % 100L;
    int z2 = temp / 10L;
    zahl = zahl - temp; 
    temp = zahl % 1000L;
    int z3 = temp / 100L;
    zahl = zahl - temp; 
    temp = zahl % 10000L;
    int z4 = temp / 1000L;
    zahl = zahl - temp; 
    temp = zahl % 100000L;
    int z5 = temp / 10000L;
    zahl = zahl - temp; 
    temp = zahl % 1000000L;
    int z6 = temp / 100000L;
    zahl = zahl - temp; 
    temp = zahl % 10000000L;
    int z7 = temp / 1000000L;
    zahl = zahl - temp; 
    temp = zahl % 100000000L;
    int z8 = temp / 10000000L;
    zahl = zahl - temp; 
    temp = zahl % 1000000000L;
    int z9 = temp / 100000000L;
  
    digitalWrite(AP1Latch,LOW); 
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z1]);
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z2]);
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z3]);
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z4]);
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z5]);
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z6]);
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z7]);
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z8]);
    shiftOut(AP1Data,AP1Clock,MSBFIRST,~dataArray[z9]);
    digitalWrite(AP1Latch,HIGH); 
  }

  void printAP2Display(long zahl){
    
    long temp = zahl % 10;
    int z1 = temp;
    zahl = zahl - temp; 
    temp = zahl % 100L;
    int z2 = temp / 10L;
    zahl = zahl - temp; 
    temp = zahl % 1000L;
    int z3 = temp / 100L;
    zahl = zahl - temp; 
    temp = zahl % 10000L;
    int z4 = temp / 1000L;
    zahl = zahl - temp; 
    temp = zahl % 100000L;
    int z5 = temp / 10000L;
    zahl = zahl - temp; 
    temp = zahl % 1000000L;
    int z6 = temp / 100000L;
    zahl = zahl - temp; 
    temp = zahl % 10000000L;
    int z7 = temp / 1000000L;
    zahl = zahl - temp; 
    temp = zahl % 100000000L;
    int z8 = temp / 10000000L;
    zahl = zahl - temp; 
    temp = zahl % 1000000000L;
    int z9 = temp / 100000000L;
    zahl = zahl - temp; 
    temp = zahl % 10000000000L;
    int z10 = temp / 1000000000L;
  
    digitalWrite(AP2Latch,LOW); 
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z1]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z2]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z3]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z4]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z5]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z6]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z7]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z8]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z9]);
    shiftOut(AP2Data,AP2Clock,MSBFIRST,~dataArray[z10]);
    digitalWrite(AP2Latch,HIGH); 
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

