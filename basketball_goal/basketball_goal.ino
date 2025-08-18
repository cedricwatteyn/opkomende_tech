#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// ---------- Pins ----------
const int irReceiverPin = 2;   // IR beam sensor
const int platePin = 3;        // Startknop
const int ledPin = 13;         // Indicator LED
const int neoPin = 6;          // NeoPixel data pin

// ---------- NeoPixel ----------
const int numPixels = 8;
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

// ---------- DFPlayer ----------
SoftwareSerial mySerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// ---------- Game state ----------
bool countdownActive = false;
unsigned long countdownStartTime = 0;
bool beamBroken = false;

int currentPixel = 0;
int score = 0; // Score teller

void setup() {
  // ---------- Input/Output ----------
  pinMode(irReceiverPin, INPUT);
  pinMode(platePin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);

  // ---------- NeoPixel ----------
  pixels.begin();
  pixels.clear();
  pixels.show();

  // ---------- DFPlayer ----------
  mySerial.begin(9600);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini niet gevonden!");
    while (true); // stop als module niet gevonden
  }
  Serial.println("DFPlayer Mini online.");
  myDFPlayer.volume(20); // volume 0-30
}

void loop() {
  // ---------- Lees inputs ----------
  int beamState = digitalRead(irReceiverPin);
  int startState = digitalRead(platePin);

  // ---------- Start countdown ----------
  if (!countdownActive && startState == LOW) {
    countdownActive = true;
    countdownStartTime = millis();
    beamBroken = false;
    Serial.println("Countdown gestart!");
  }

  if (countdownActive) {
    // Check of IR beam onderbroken is
    if (beamState == HIGH) {
      beamBroken = true;
    }

    // Na 5 seconden countdown
    if (millis() - countdownStartTime >= 5000) {
      if (beamBroken) {
        // Score
        score++;
        Serial.print("Score! Totaal = ");
        Serial.println(score);

        digitalWrite(ledPin, HIGH);
        pixels.setPixelColor(currentPixel, pixels.Color(0, 255, 0)); // Groen
        myDFPlayer.play(1); // Score geluid
      } else {
        // Miss
        Serial.print("Miss! Totaal = ");
        Serial.println(score);

        digitalWrite(ledPin, LOW);
        pixels.setPixelColor(currentPixel, pixels.Color(255, 0, 0)); // Rood
        myDFPlayer.play(2); // Miss geluid
      }
      pixels.show();

      // Volgende pixel
      currentPixel++;
      if (currentPixel >= numPixels) currentPixel = 0;

      // Reset countdown
      countdownActive = false;
    }
  }
}
