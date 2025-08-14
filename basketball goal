#include <Adafruit_NeoPixel.h>

const int irReceiverPin = 2;   // IR receiver pin
const int ledPin = 13;         //  LED
const int platePin = 3;        // Pressure plate pin

// Neopixel settings
const int neoPin = 6;          // Data pin voor NeoPixel stick
const int numPixels = 8;       // Aantal LED's
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

bool countdownActive = false;
unsigned long countdownStartTime = 0;
bool beamBroken = false;

int currentPixel = 0; // Welke pixel als vervolgens moet branden

void setup() {
  pinMode(irReceiverPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(platePin, INPUT_PULLUP);
  Serial.begin(9600);

  // Initialize Neopixel
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void loop() {
  int beamState = digitalRead(irReceiverPin);
  int plateState = digitalRead(platePin); // LOW wanneer ingedrukt

  // Start tellen wanneer pressure plate ingedrukt is
  if (!countdownActive && plateState == LOW) {
    countdownActive = true;
    countdownStartTime = millis();
    beamBroken = false;
    Serial.println("Countdown started!");
  }

  if (countdownActive) {
    // Check of de straal onderbroken wordt
    if (beamState == HIGH) { // Beam broken
      beamBroken = true;
    }

    // Na 5 seconden, bekijk resultaat
    if (millis() - countdownStartTime >= 5000) {
      if (beamBroken) {
        Serial.println("Score!");
        digitalWrite(ledPin, HIGH);
        pixels.setPixelColor(currentPixel, pixels.Color(0, 255, 0)); // Green
      } else {
        Serial.println("Miss!");
        digitalWrite(ledPin, LOW);
        pixels.setPixelColor(currentPixel, pixels.Color(255, 0, 0)); // Red
      }
      pixels.show();

      // Move to the next pixel for the next round
      currentPixel++;
      if (currentPixel >= numPixels) currentPixel = 0;

      countdownActive = false; // Reset for next round
    }
  }
}
