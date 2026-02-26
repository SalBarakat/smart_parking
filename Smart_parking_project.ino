#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>   // Libraries for servo motors and I2C LCD

// Initialize 16x2 I2C LCD at address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo objects for entry and exit barriers
Servo IN_Door; 
Servo Out_Door;

// IR sensor pins (entry, exit, parking slots)
int IrIN = 10;
int IrOUT = 6;
int IrPark1 = 9;
int IrPark2 = 8;
int IrPark3 = 7;

// Parking slot state variables
int park1State;
int park2State;
int park3State;

// Hazard detection sensors and buzzer
int flame = 13;
int Buzzer = 12;
int gas = 11;
int water = A0;   // Analog water level sensor

int Counter;      // Stores number of occupied slots

void setup() {

  Serial.begin(9600);   // Start serial communication for debugging

  // Configure IR sensors as input
  pinMode(IrIN, INPUT);
  pinMode(IrOUT, INPUT);
  pinMode(IrPark1, INPUT);
  pinMode(IrPark2, INPUT);
  pinMode(IrPark3, INPUT);

  // Attach servos to PWM pins
  IN_Door.attach(3);
  Out_Door.attach(5);

  // Configure hazard sensors and buzzer
  pinMode(flame, INPUT);
  pinMode(gas, INPUT);
  pinMode(water, INPUT);
  pinMode(Buzzer, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Display startup message
  lcd.setCursor(4,0);
  lcd.print("Welcome");
  lcd.setCursor(0,1);
  lcd.print(" Smart Parking");
  delay(2000);

  // Display main interface
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Smart Parking");
  lcd.setCursor(0,1);
  lcd.print(" Available: ");
}

void loop() {

  // ================= ENTRY GATE CONTROL =================
  if(digitalRead(IrIN)==0){
    Serial.println("Car on Enterance");
    IN_Door.write(90);     // Open entry gate
    delay(3000);           // Keep open for 3 seconds
  } else {
    IN_Door.write(0);      // Close entry gate
  }

  // ================= EXIT GATE CONTROL =================
  if (digitalRead(IrOUT)==0){
    Serial.println("Car on Exit");
    Out_Door.write(90);    // Open exit gate
    delay(3000);           // Keep open for 3 seconds
  } else {
    Out_Door.write(0);     // Close exit gate
  }

  // ================= PARKING SLOT STATUS =================
  // Each IR sensor returns LOW when slot is occupied

  if (digitalRead(IrPark1)== 0){
    park1State = 1;
  } else {
    park1State = 0;
  }

  if (digitalRead(IrPark2)== 0){
    park2State = 1;
  } else {
    park2State = 0;
  }

  if (digitalRead(IrPark3)== 0){
    park3State = 1;
  } else {
    park3State = 0;
  }

  // Count total occupied slots
  Counter = park1State + park2State + park3State; 
  Serial.println(Counter);

  // Display available slots on LCD
  lcd.setCursor(11,1);
  lcd.print(3 - Counter);

  // ================= EMERGENCY OVERRIDE =================
  // Triggered if flame OR gas detected OR water exceeds threshold

  if (digitalRead(flame)==0 || digitalRead(gas)==0 || analogRead(water)>500){

    // Open both gates for emergency evacuation
    IN_Door.write(90);
    Out_Door.write(90);

    // Display danger message
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" DANGER");

    // Generate sweeping alarm tone
    for(int i=0;i < 1000;i++){
      tone(Buzzer,i);
      delay(2);
    }
    for(int i = 1000;i>0; i--){
      tone(Buzzer,i);
      delay(2);
    }

    // Restore normal display
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Smart Parking");
    lcd.setCursor(0,1);
    lcd.print(" Available: ");

    noTone(Buzzer);   // Stop buzzer
  }
}
