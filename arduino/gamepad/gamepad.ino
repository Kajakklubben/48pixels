void setup(){
  Serial.begin(9600); 

  for(int i=8; i<=12;i++){
    pinMode(i, INPUT);
    digitalWrite(i,HIGH); 
  }
}


boolean pinState[14];

void loop(){
  for(int i=8; i<=12;i++){
    boolean r = digitalRead(i);
    if(r != pinState[i]){
      pinState[i] = r;
      Serial.write(i+ ((pinState[i])?128:0));
      delay(50);
    }
  }
}


