#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo IN_Door ; 
Servo Out_Door;
int IrIN = 10;
int IrOUT = 6;
int IrPark1 = 9;
int IrPark2 = 8;
int IrPark3 = 7;
int park1State;
int park2State;
int park3State;
int flame = 13;
int Buzzer = 12;
int gas = 11;
int water = A0;
int Counter;
void setup() {
  Serial.begin(9600);
  pinMode(IrIN, INPUT);
  pinMode(IrOUT, INPUT);
  IN_Door.attach(3);
  Out_Door.attach(5);
  pinMode(IrPark1, INPUT);
  pinMode(IrPark2, INPUT);
  pinMode(IrPark3, INPUT);
  pinMode(flame, INPUT);
  pinMode(water, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(gas, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Welcome");
  lcd.setCursor(0,1);
  lcd.print(" Smart Parking");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Smart Parking");
  lcd.setCursor(0,1);
  lcd.print(" Available: ");

}

void loop() {
  if(digitalRead(IrIN)==0){
    Serial.println("Car on Enterance");
    IN_Door.write(90);
    delay(3000);
  }else{
      IN_Door.write(0);
    }
  if (digitalRead(IrOUT)== 0){
    Serial.println("Car on Exit");
    Out_Door.write(90);
    delay(3000);
  }
    else{
      Out_Door.write(0);
  }
  if (digitalRead(IrPark1)== 0){
    park1State = 1;
  }
    else {
    park1State = 0;
    }
  if (digitalRead(IrPark2)== 0){
    park2State = 1;
  }
    else {
    park2State = 0;
    }
  if (digitalRead(IrPark3)== 0){
    park3State = 1;
  }
    else {
    park3State = 0;
    }
  Counter = park1State + park2State + park3State; 
  Serial.println(Counter);
  lcd.setCursor(11,1);
  lcd.print(3 - Counter);
  if (digitalRead(flame)==0 || digitalRead(gas)==0 || analogRead(water)>500){
    IN_Door.write(90);
    Out_Door.write(90);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" DANGER");

    for(int i=0;i < 1000;i++){
      tone(Buzzer,i);
      delay(2);
    }
    for(int i = 1000;i>0; i--){
      tone(Buzzer,i);
      delay(2);
    }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Smart Parking");
  lcd.setCursor(0,1);
  lcd.print(" Available: ");
  noTone(Buzzer);
  
  }
}
