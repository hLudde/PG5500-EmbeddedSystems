#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE    DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;


const int numReadings = 10;

int readingsLightSensor[numReadings];
int readIndexLightSensor = 0;
int totalLightSensor = 0;
int averageLightSensor = 0;

int readingsFlameSensor[numReadings];
int readIndexFlameSensor = 0;
int totalFlameSensor = 0;
int averageFlameSensor = 0;

const int inputPinLightSensor = A0;
const int inputPinFlameSensor = A1;
const int inputPinTempSensor = A2;

const int outputPinTempSensor = 9;

int tempSensorValue = 0;
int tempOutputValue = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  delayMS = sensor.min_delay / 1000;
  dht.humidity().getSensor(&sensor);
  dht.temperature().getSensor(&sensor);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readingsLightSensor[thisReading] = 0;
    readingsFlameSensor[thisReading] = 0;
  }
}

void loop() {
  delay(delayMS);
  Serial.print("Light Level: ");
  Serial.print(lightSensor());
  Serial.print(" Flame Level: ");
  Serial.print(flameSensor());
  Serial.print(" Temp Sensor: ");
  Serial.print(tempSensor());
  Serial.print(" DHT Temp Sensor: ");
  Serial.print(dhtSensorTemp());
  Serial.print(" DHT Humidity Sensor: ");
  Serial.println(dhtSensorHumidity());
}

int lightSensor(){
  totalLightSensor = totalLightSensor - readingsLightSensor[readIndexLightSensor];
  readingsLightSensor[readIndexLightSensor] = analogRead(inputPinLightSensor);
  totalLightSensor = totalLightSensor + readingsLightSensor[readIndexLightSensor];
  readIndexLightSensor = readIndexLightSensor + 1;
  if (readIndexLightSensor >= numReadings) {
    readIndexLightSensor = 0;
  }
  averageLightSensor = totalLightSensor / numReadings;
  return averageLightSensor;
}

int flameSensor(){
  totalFlameSensor = totalFlameSensor - readingsFlameSensor[readIndexFlameSensor];
  readingsFlameSensor[readIndexFlameSensor] = analogRead(inputPinFlameSensor);
  totalFlameSensor = totalFlameSensor + readingsFlameSensor[readIndexFlameSensor];
  readIndexFlameSensor = readIndexFlameSensor + 1;
  if (readIndexFlameSensor >= numReadings) {
    readIndexFlameSensor = 0;
  }
  averageFlameSensor = totalFlameSensor / numReadings;
  return averageFlameSensor;
}

float tempSensor(){
  tempSensorValue = analogRead(inputPinTempSensor);
  float temperatureC = ( 4.9 * tempSensorValue * 100.0) / 1024.0;
  tempOutputValue = map(tempSensorValue, 0, 1023, 0, 255);
  analogWrite(outputPinTempSensor, tempOutputValue);
  return temperatureC;
}

float dhtSensorTemp(){
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  return event.temperature;
}

float dhtSensorHumidity(){
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  return event.relative_humidity;
}



