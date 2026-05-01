#include <DHT_Async.h>
#include <Arduino.h>

//#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
#define DHT_SENSOR_TYPE DHT_TYPE_22

static const int DHT_SENSOR_PIN = 11;
static const int LED_PIN = 12;
static const int BUTTON_PIN = 9;
static const int RELAY_PIN = 6;
static const int HUMIDITY_LIMIT = 70;
static const int HUMIDITY_LIMIT_THRESHOLD = 2;

static const long NORMAL_DELAY = 5; // 5 sec
static const long LONG_DELAY = 1200; // 20 minutes

DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
}

bool running = false;
unsigned long wait_time = NORMAL_DELAY * 1000;
unsigned long measurement_timestamp = millis();

static bool measure_environment(float *temperature, float *humidity) {
  if (millis() - measurement_timestamp > wait_time) {
    if (dht_sensor.measure(temperature, humidity)) {
      
      Serial.print("T = ");
      Serial.print(*temperature, 1);
      Serial.print(" deg. C, H = ");
      Serial.print(*humidity, 1);
      Serial.println("%");

      return (true);
    }
  }
  return (false);
}

void start() {
  if (running) return;

  Serial.println("Start dehumidification...");
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(RELAY_PIN, HIGH);

  running = true;
}

void stop() {
  if (!running) return;

  Serial.println("Stop dehumidification...");
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  running = false;
}

void reset() {
  Serial.println("Reset!");
  set_timer(NORMAL_DELAY);
  
  running = true; // Force stop to run
  stop();
}

void set_timer(unsigned long d) {
  wait_time = d * 1000;
  measurement_timestamp = millis();
}

void loop() {
  float temperature;
  float humidity;
  int count;
  bool blink;

  if (measure_environment(&temperature, &humidity)) {
    set_timer(NORMAL_DELAY);
    if (humidity >= HUMIDITY_LIMIT + HUMIDITY_LIMIT_THRESHOLD) start();
    else if (humidity <= HUMIDITY_LIMIT - HUMIDITY_LIMIT_THRESHOLD) stop();
  }

  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button press");
    set_timer(LONG_DELAY);

    if (running) stop();
    else start();

    count = 0;
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(100);
      
      if (count >= 20) {
        blink = !blink;
        blink ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
      } else {
        count++;
      }
    }

    if (count >= 20) {
      reset();
    }
  }
}