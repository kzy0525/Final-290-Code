#include <Wire.h>
#include <ArduinoJson.h>
#include "Adafruit_SGP30.h"  // Include the SGP30 sensor library
#include "DHT.h"
 
// DHT sensor setup
#define DHTPIN 3       // Pin connected to the DATA line of the DHT sensor
#define DHTTYPE DHT11  // DHT11 or DHT22 sensor type
DHT dht(DHTPIN, DHTTYPE);
 
// TMP36 Temperature sensor pin
int sensePin = A2;  // Pin that reads the TMP36 output
int sensorInput;     // Variable to store TMP36 sensor input
double temp;         // Variable to store TMP36 temperature in degrees Celsius
 
// Sound sensor pins
const int soundAnalogPin = A0;  // Analog output from the sound sensor
const int soundDigitalPin = 2;  // Digital output from the sound sensor
 
// Buzzer pin (not used in this version)
const int buzzerPin = 4; // Digital pin connected to the buzzer
 
// SPG30 sensor setup
Adafruit_SGP30 sgp;  // Instantiate the SPG30 sensor
 
 
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}
 
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  while (!Serial) { delay(10); } // Wait for serial console to open!
 
  // Initialize the DHT sensor
  dht.begin();
 
  // Set the buzzer pin as output (not used in this code version)
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // Ensure the buzzer is off initially
  sgp.begin();
  // // Initialize the SPG30 sensor
  // if (! sgp.begin()) {
  //   Serial.println("Sensor not found :(");
  //   while (1);
  // }
  // Serial.print("Found SGP30 serial #");
  // Serial.print(sgp.serialnumber[0], HEX);
  // Serial.print(sgp.serialnumber[1], HEX);
  // Serial.println(sgp.serialnumber[2], HEX);
}
 
void loop() {
  JsonDocument doc;
  // --- Read and Display Sound Sensor Data ---
  int soundLevel = analogRead(soundAnalogPin);
  int soundDetected = digitalRead(soundDigitalPin);
  doc["sound"] = soundLevel;
  // --- Read and Display Humidity Sensor Data ---
  int humidity = dht.readHumidity();
  int temperatureDHT = dht.readTemperature()*0.8;
  if (isnan(humidity) || isnan(temperatureDHT)) {
    //Serial.println("Failed to read from DHT sensor!");
  } else {
    //Serial.print("Humidity: ");
    doc["temperature"] = temperatureDHT;
    //Serial.print("%  Temperature (DHT): ");
    doc["humidity"] = humidity;
    //Serial.println("°C");
  }
  if (Serial.available() > 0) {
    // read the incoming byte:
  char incomingByte = Serial.read();
  if incomingByte = "A"{
      digitalWrite(buzzerPin, HIGH);
    }
  }
  else{
    digitalWrite(buzzerPin, LOW);
  }
 
  // --- Read and Display Temperature Module Data (TMP36) ---
  sensorInput = analogRead(sensePin);  // Read the analog sensor (TMP36)
  double voltage = sensorInput * (5.0 / 1023.0);  // Convert the analog value to voltage
  temp = (voltage - 0.5) * 100*0.8302;  // Convert the voltage to temperature in Celsius
 
  // --- Read and Display Air Quality Data (SPG30) ---
  //   if (! sgp.IAQmeasure()) {
  //    Serial.println("Measurement failed");
  //    return;
  //  }
  sgp.IAQmeasure();
  doc["tvoc"] = sgp.TVOC;
  doc["ECO2"] = sgp.eCO2;
  serializeJson(doc, Serial);
  Serial.println();
  delay(1000);  // 1-second delay between measurements
}
