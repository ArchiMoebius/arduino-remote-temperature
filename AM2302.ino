#include "cactus_io_AM2302.h"
#include <SoftwareSerial.h>

#define DEBUG 0

#define RX 10
#define TX 9

#define AM2302_PIN 2

SoftwareSerial Bluetooth(RX, TX);

AM2302 dht(AM2302_PIN);

char b[10];
String buf = "";

void setup() {

  if (DEBUG) {
    Serial.begin(9600); 
    Serial.println("AM2302 Humidity - Temperature Sensor");
    Serial.println("RH\t\tTemp (C)\tTemp (F)\tHeat Index (C)\t Heat Index (F)");
  }

  Bluetooth.begin(9600);

  dht.begin();
}

void loop() {
  Bluetooth.listen();

  dht.readHumidity();
  dht.readTemperature();
  
  if (
    isnan(dht.humidity) ||
    isnan(dht.temperature_C)
  ) {

    if (DEBUG) {
      Serial.println("DHT sensor read failure!");
    }

    Bluetooth.println("Read failure for DHT sensor!");
    return;
  }
  
  if (DEBUG) {
    Serial.print(dht.humidity); Serial.print(" %\t\t");
    Serial.print(dht.temperature_C); Serial.print(" *C\t");
    Serial.print(dht.temperature_F); Serial.print(" *F\t");
    Serial.print(dht.computeHeatIndex_C()); Serial.print(" *C\t");
    Serial.print(dht.computeHeatIndex_F()); Serial.println(" *F");
  }

  dtostrf(dht.humidity, 4, 2, b);
  buf += b;
  buf += ",";
  dtostrf(dht.temperature_F, 4, 2, b);
  buf += b;
  buf += ",";
  dtostrf(dht.computeHeatIndex_F(), 4, 2, b);
  buf += b;

  Bluetooth.println(buf);
  Bluetooth.flush();
  delay(300000);// 5min
  buf = "";
}
