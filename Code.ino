#include <dht.h>

dht DHT;  
#define DHT11_PIN 6      // DHT11 Sensor Pin
#define FAN_PIN 11       // Fan Relay Pin
#define LED_PIN 7        // LED controlled by LDR & Ultrasonic
#define LDR_PIN A0       // LDR connected to Analog pin A0
#define TRIG_PIN 9       // Ultrasonic Sensor Trig Pin
#define ECHO_PIN 10      // Ultrasonic Sensor Echo Pin

// Thresholds
const int LIGHT_THRESHOLD = 15; // Adjust based on environment
const int DISTANCE_THRESHOLD = 15; // Distance threshold in cm
const int TEMP_THRESHOLD = 29; // Temperature threshold in °C

void setup() {
  Serial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(FAN_PIN, LOW);  // Ensure fan is OFF initially
}

void loop() {
  // Read LDR sensor
  int sensorValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(sensorValue);
  
  // Measure distance using Ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Read temperature from DHT11
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);

  // LED Control (Turn on if it's dark OR object is close)
  if (sensorValue < LIGHT_THRESHOLD || distance < DISTANCE_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Fan Control (Turn on if temp > 30°C OR object is close)
  if (DHT.temperature > TEMP_THRESHOLD || distance < DISTANCE_THRESHOLD) {
    digitalWrite(FAN_PIN, HIGH);  // Turn fan ON
    Serial.println("Fan ON");
  } else {
    digitalWrite(FAN_PIN, LOW);   // Turn fan OFF
    Serial.println("Fan OFF");
  }

  delay(1000); // Wait before next loop
}


