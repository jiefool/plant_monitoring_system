#include <dht.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define DHT11_PIN 22

SoftwareSerial mySerial(11,10);
const int rs = 36, en = 38, d4 = 46, d5 = 44, d6 = 42, d7 = 40;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
dht DHT;

long notifInterval=120000;
float temp = 0.0;
float humidity = 0.0;
unsigned long period = 60000;
unsigned long time_now = 0;
String phoneNumber = "+639467254060";


void setup() {
  Serial.begin(9600);
  
  mySerial.begin(9600); 
  lcd.begin(16, 2);

  printLCD(0,0, "Plant Monitoring");
  printLCD(0,1, "System");

  delay(2000);

  lcd.clear();
  printSensorReading();
}

void loop() {
  // put your main code here, to run repeatedly:
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  temp = DHT.temperature;
  Serial.println(temp);
  
  Serial.print("Humidity = ");
  humidity = DHT.humidity;
  Serial.println(humidity);

  
  if( (temp >= 32) ){
    Serial.println("Too Hot!");
     
    if(millis() >= time_now + period){
      time_now += period;
      lcd.clear();
      printLCD(0,0, "Too Hot!");
      sendSMS("Too Hot!");
    } 
    
  }

  if( (temp <= 15) ){
    Serial.println("Too Cold!");
     
    if(millis() >= time_now + period){
      time_now += period;
      lcd.clear();
      printLCD(0,0, "Too Cold!");
      sendSMS("Too Cold!");
    } 
    
  }
  
  if (humidity > 60 ){
    Serial.println("Too Humid!");
    

     if(millis() >= time_now + period){
      time_now += period;
      lcd.clear();
      printLCD(0,0, "Too Humid!");
      sendSMS("Too Humid!");
    } 
  }
  
  
  if (humidity < 50 ){
    Serial.println("Low Humid!");

    if(millis() >= time_now + period){
        time_now += period;
       lcd.clear();
       printLCD(0,0, "Low Humid!");
       sendSMS("Low Humid!");
    }
  }

  delay(1000);
  printSensorReading();
  delay(1000);

}

void printLCD(int x, int y, String msg){
   lcd.setCursor(x, y);
   lcd.print(msg);
}

void printSensorReading(){
  printLCD(0,0, "Temp:" + (String) temp );
  printLCD(0,1, "Hmdty:" + (String) humidity );
}


void sendSMS(String msg){
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\""+phoneNumber+"\"");
  updateSerial();
  mySerial.print(msg);
  updateSerial();
  mySerial.write(26);
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
