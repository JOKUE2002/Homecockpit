#define latchPin 5
#define clockPin 6
#define dataPin 7

#define rotary_A 2
#define rotary_B 3
int rotary_state;
int rotary_state_last;

//display 0,1,2,3,4,5,6,7,8,9
byte dataArray[10] = 
{B00111111,
B00000110,
B01011011,
B01001111,
B01100110,
B01101101,
B01111101,
B00000111,
B01111111,
B01101111
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
//Rotary rotary = Rotary(2, 3);

void setup () {
  //set pins to output
  Serial.begin(115200);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT); 

  pinMode(rotary_A,INPUT_PULLUP);
  pinMode(rotary_B,INPUT_PULLUP);
}
 
void loop() {
  ROTARY();
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
    }
  }

  /*if(activeReady && standReady){
    printSBOnDisplay(stand);
    printActiveOnDisplay(active);
    activeReady = false;
    standReady = false;
  }*/
  printStandByAndActive(active, stand);
}

void printStandByAndActive(float active, float stand){
  long activeFull = active * 100;
  long standFull = stand * 100;

  activeFull *= 100000;

  long all = standFull + activeFull;
  printOnDisplay(all);
}

void printOnDisplay(long zahl){
  
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

  digitalWrite(latchPin,LOW); 
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[z1]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[z2]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArrayWithDot[z3]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[z4]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[z5]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[z6]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[z7]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArrayWithDot[z8]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[z9]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[z10]);
  digitalWrite(latchPin,HIGH); 
}

void ROTARY(){
  rotary_state = digitalRead(rotary_A);
  Serial.print(rotary_state);
  Serial.print(", ");
  Serial.println(digitalRead(rotary_B));
  if(rotary_state != rotary_state_last){
    if(rotary_state != digitalRead(rotary_B)){
      //+
      Serial.println("A03");
    }else{
      //-
      Serial.println("A04");
    }
  }
  rotary_state_last = rotary_state;
}


/*void printActiveOnDisplay(String num){
  long firstDigit = toInt(substring(num,0,1));
  long secondDigit = toInt(substring(num,1,2)); 
  long thirdDigit = toInt(substring(num,2,3));
  int fourthDigit = toInt(substring(num,3,4));
  int lastDigit = toInt(substring(num,4,5));

  digitalWrite(latchPin,LOW); 
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[lastDigit]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[fourthDigit]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[thirdDigit]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[secondDigit]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[firstDigit]); 
  digitalWrite(latchPin,HIGH); 
}

void printSBOnDisplay(String num){
  long firstDigit = toInt(substring(num,0,1));
  long secondDigit = toInt(substring(num,1,2)); 
  long thirdDigit = toInt(substring(num,2,3));
  int fourthDigit = toInt(substring(num,3,4));
  int lastDigit = toInt(substring(num,4,5));

  digitalWrite(latchPin,LOW); 
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[lastDigit]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[fourthDigit]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[thirdDigit]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[secondDigit]);
  shiftOut(dataPin,clockPin,MSBFIRST,~dataArray[firstDigit]); 
  digitalWrite(latchPin,HIGH); 
}
*/
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

