#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// Pin definitions
const int gasSensorPin = A0;  // Analog pin for gas sensor
const int flameSensorPin = 2; // Digital pin for flame sensor
const int ldrPin = A1;         // Analog pin for LDR sensor
const int Buzzer = 12;
const int Load = 13;


SoftwareSerial SIM800L(7, 8); // RX, TX
String phoneNumber = "01305514344";
// DHT11 setup
#define DHTTYPE DHT11
const int dhtPin = 4; // Digital pin for DHT11

DHT dht(dhtPin, DHTTYPE);

// I2C LCD Display setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address, columns, rows


void setup() {
  Serial.begin(9600);
  SIM800L.begin(9600);
  dht.begin();
  
  lcd.init();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("Ind. Protection");
  lcd.setCursor(0, 1);
  lcd.print("System OK");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Project by");
  lcd.setCursor(0, 1);
  lcd.print("TE-SP23-08 ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Md.Sujon Islam");
  lcd.setCursor(0, 1);
  lcd.print("Masud Rana");
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Md.Ripon Sarker");
  lcd.setCursor(0, 1);
  lcd.print("Md.Rafial Arafat");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Supervised by");
  lcd.setCursor(0, 1);
  lcd.print("Shahariar Kabir");
  delay(2000);
  

  pinMode(flameSensorPin, INPUT);
  pinMode(gasSensorPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Load, OUTPUT);
}

void loop() {
  digitalWrite(Buzzer,LOW);
  digitalWrite(Load,HIGH);
  
  // Gas detection
  int gasValue = analogRead(gasSensorPin);
  if (gasValue >= 175) {
    sendSMS("Gas Detected!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas Detected!");
    digitalWrite(Buzzer,HIGH);
    digitalWrite(Load,LOW);
    delay(5000);
  }

  // Fire detection
  int flameValue = digitalRead(flameSensorPin);
  if (flameValue == HIGH) {
    sendSMS("Fire Detected!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fire Detected!");
    digitalWrite(Buzzer,HIGH);
    digitalWrite(Load,LOW);
    delay(5000);
  }

  // Temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(8, 0);
  lcd.print("Hum:");
  lcd.print(humidity);
  lcd.print("%");
   if (temperature>= 35) {
    sendSMS("Over Temperature");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Over Temp Detect");
    digitalWrite(Buzzer,HIGH);
    digitalWrite(Load,LOW);
    delay(5000);
  }

  // Light level
  int ldrValue = analogRead(ldrPin);
  int light= (ldrValue);
  lcd.setCursor(0, 1);
  lcd.print("Light:");
  lcd.print(light);
  lcd.print(" lux");
  if (light<= 40) {
    sendSMS("Low Light detected");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Low Light Detect");
    digitalWrite(Buzzer,HIGH);
    digitalWrite(Load,LOW);
    delay(5000);
  }
Serial.print(gasValue);
  // You can add conditions to trigger alerts based on sensor values

  delay(500); // Delay for 10 seconds before rechecking sensors
}

void sendSMS(String message) {
  SIM800L.println("AT+CMGF=1"); // Set SMS mode to text
  delay(500);
  
  SIM800L.print("AT+CMGS=\"");
  SIM800L.print(phoneNumber);
  SIM800L.println("\"");

  delay(500);

  SIM800L.print(message);
  delay(100);

  SIM800L.write(26); // Send Ctrl+Z to end the SMS message
  delay(500);
}
