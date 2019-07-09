void setup() {
  Serial.begin(9600);

  Serial.println("TEST A");
  String t0 = "TEST";
  char t1[4] = {'T','E','S','T'};
  boolean t2 = compare(t0, t1);
  boolean t3 = false;
  boolean t4 = true;

  String t5 = "255";
  char t6[3] = {'2','8','5'};
  boolean t7 = compare(t5, t6);
  String t8 = "255";
  char t9[3] = {'2','5','5'};
  boolean t10 = compare(t8, t9);

  Serial.println("FALSE - TRUE");
  Serial.println(t3);
  Serial.println(t4);

  Serial.println("TEST Nr. 1");
  Serial.println(t2);
  Serial.println("TEST Nr. 2");
  Serial.println(t7);
  Serial.println("TEST Nr. 3");
  Serial.println(t10);
}

void loop(){}

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
