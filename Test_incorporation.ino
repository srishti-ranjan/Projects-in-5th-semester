
#include <SoftwareSerial.h>
#include <dht11.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

float pressure = 0.0;
float altitudee = 0.0;
Adafruit_BMP085 bmp;
#define RX 2
#define TX 3
#define dht_apin 11 
dht11 dhtObject;
String AP = "HiraChak";       
String PASS = "SnehLata79"; 
String API = "4V8TGO5TT855YACW";   
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
int buzzer = 10;
int smokeA0 = A1;
int sensorThres = 400;

SoftwareSerial esp8266(RX,TX); 
String pressureread();
String altituderead();

  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  bmp.begin(9600);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() {
   
 String getData = "GET /update?api_key="+ API +"&field1="+getTemperatureValue()+"&field2="+getHumidityValue()+"&field3="+getGasdata()+"&field4="+pressureread()+"&field5="+altituderead();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}

String getTemperatureValue(){

   dhtObject.read(dht_apin);
   Serial.print(" Temperature(C)= ");
   int temp = dhtObject.temperature;
   Serial.println(temp); 
   delay(50);
   return String(temp); 
  
}


String getHumidityValue(){

   dhtObject.read(dht_apin);
   Serial.print(" Humidity in %= ");
   int humidity = dhtObject.humidity;
   Serial.println(humidity);
   delay(50);
   return String(humidity); 
  
}

String getGasdata(){
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  Serial.begin(9600); 
  int analogSensor = analogRead(smokeA0);

  Serial.print("Gas sensor: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    tone(buzzer, 1000, 200);
    return String(analogSensor); 
  }
  else
  {
    noTone(buzzer);
    return String(analogSensor);
  }
  Serial.println(analogSensor);
  delay(100);
}

String pressureread(){
  pressure = bmp.readPressure() / 100.0;
  Serial.print("Pressure = ");
  Serial.print(pressure / 100.0);
  Serial.println(" hPa");
  return String(pressure);
}

String altituderead(){
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  altitudee = bmp.readAltitude();
  Serial.print("Altitude = ");
  Serial.print(altitudee);
  Serial.println(" meters");
  return String(altitudee);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("pass");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("...");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
