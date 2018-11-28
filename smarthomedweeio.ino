#include <SoftwareSerial.h>
#define RX 10
#define TX 11
#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to
#include <JeeLib.h>
ISR(WDT_vect) { Sleepy::watchdogEvent(); }

String AP = "FLIA RIVEROS BENITEZ";       // AP
String PASS = "lulilove06012018"; // OCULTANDING PASSWORD
String API = "VZHYBF4M4LDNA67F";   // OCULTANDING API
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensorTemp = 1;
int valSensorHumid = 1;
SoftwareSerial esp8266(RX,TX); 
dht DHT; 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}
void loop() {
 valSensorTemp = getSensorTemperature();
 valSensorHumid = getSensorHumidity();
 
 String getData = "GET /update?api_key="+ API +"&"+ field1 +"="+String(valSensorTemp);
 String getData1 = "GET /update?api_key="+ API +"&"+ field2 +"="+String(valSensorHumid);
 
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 
 delay(1500);
 
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData1.length()+4),4,">");
 esp8266.println(getData1);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 Sleepy::loseSomeTime(5000);
  
}

int getSensorTemperature(){
  
   DHT.read11(dht_apin);  
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    return DHT.temperature;
}

int getSensorHumidity(){
  
   DHT.read11(dht_apin);  
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    
    return DHT.humidity;
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
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
