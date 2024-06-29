#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <DHT.h>
DHT dht(8,DHT22);
//#define 5 5
char inputvalue[5]; 
char Passcode[5] = "1234"; 
byte inputcount = 0, passcodecount = 0;
char inputKey;
Servo myservo;
byte rowpins[4]={36,37,38,39};
byte columnpins[4]={40,41,42,43};
char key[4][4]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};
LiquidCrystal_I2C lcd(0x27,16,2);
Keypad keys=Keypad(makeKeymap(key),rowpins,columnpins,4,4);

void setup() {
  lcd.init();
  dht.begin();
  myservo.attach(4);
  myservo.write(0);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, INPUT);
  pinMode(30, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(1, OUTPUT);
}
 
void loop() {
  int attempt=0;
  do{
  digitalWrite(7, LOW);
  delayMicroseconds(2);
  digitalWrite(7, HIGH);
  delayMicroseconds(10);
  digitalWrite(7, LOW);
  
  long duration=pulseIn(6,HIGH);
  int distance=(duration*0.034)/2;
  if (distance<30){
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print("Enter Password:");
      inputKey = keys.getKey(); 
      if (inputKey){
        inputvalue[inputcount] = inputKey; 
        lcd.setCursor(inputcount,1);
        lcd.print(inputvalue[inputcount]); 
        inputcount++; 
        }

    if(inputcount == 5-1){
      lcd.clear();

    if(!strcmp(inputvalue, Passcode)){
        lcd.setCursor(5,0);
        lcd.print("ACCESS");
        lcd.setCursor(5,1);
        lcd.print("GRANTED");
        myservo.write(90);  
        delay(10000);
        myservo.write(0);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("DOOR IS LOCKED");
        delay(1000);
      }
    else{
      digitalWrite(30, HIGH);
      lcd.setCursor(5,0);
      lcd.print("ACCESS");
      lcd.setCursor(5,1);
      lcd.print("DENIED");
      delay(2000);
      digitalWrite(30, LOW);
      attempt+=1;
      }
    
    lcd.clear();
    while(inputcount !=0){
    inputvalue[inputcount--] = 0; 
  }

  }
  }
  else{
    lcd.clear();
    lcd.noBacklight();
  }
    int l= analogRead(A0);
  int ldra=l/4;
  if(ldra<15){
     digitalWrite(9, LOW);
  }
  else {
    analogWrite(9,ldra);
  }
  float temp=dht.readTemperature();
  if(temp<0){
    digitalWrite(3, HIGH);//blue
    digitalWrite(2, LOW);//yellow
    digitalWrite(1, LOW);//red
    //delay(1000);
  }
  else if(temp>0 && temp<40){
    digitalWrite(3, HIGH);//blue
    digitalWrite(2, HIGH);//yellow
    digitalWrite(1, LOW);//red
    //delay(1000);
  }
  else if(temp>40) {
    digitalWrite(3, HIGH);//blue
    digitalWrite(2, HIGH);//yellow
    digitalWrite(1, HIGH);//red
    //delay(1000);
  }
  } while(attempt<3);
  if(attempt==3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MAX ATTEMPTS");
    lcd.setCursor(0,1);
    lcd.print("REACHED");
    attempt=0;
    delay(2000);
    lcd.clear();
    lcd.noBacklight();
    delay(7000);
  }



}
