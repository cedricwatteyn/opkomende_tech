#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// ---------- Pins ----------
const int ky032Pin = 2;       // KY-032 IR sensor
const int groveIRPin = 4;     // Grove IR sensor
const int platePin = 3;       // Startknop
const int ledPin = 13;        // Indicator LED
const int neoPin = 6;         // NeoPixel data pin
const int piezzoPin = A0;     // Piezzo sensor op het bord

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
bool piezzoHit = false;

int currentPixel = 0;
int score = 0;             // Score teller
int scoreWithoutBoard = 0; // Score zonder bord

// ---------- Piezzo drempel ----------
const int piezzoThreshold = 100; // waarde afhankelijk van piezzo, moet getest worden

void setup() {
  // ---------- Input/Output ----------
  pinMode(ky032Pin, INPUT);
  pinMode(groveIRPin, INPUT);
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
  int ky032State = digitalRead(ky032Pin);
  int groveState = digitalRead(groveIRPin);
  int startState = digitalRead(platePin);
  int piezzoValue = analogRead(piezzoPin);

  // ---------- Start countdown ----------
  if (!countdownActive && startState == LOW) {
    countdownActive = true;
    countdownStartTime = millis();
    beamBroken = false;
    piezzoHit = false;
    Serial.println("Countdown gestart!");
  }

  if (countdownActive) {
    // Check beide IR beams: als één HIGH is, is de beam doorbroken
    if (ky032State == HIGH || groveState == HIGH) {
      beamBroken = true;
    }

    // Check piezzo
    if (piezzoValue > piezzoThreshold) {
      piezzoHit = true;
    }

    // Na 5 seconden countdown
    if (millis() - countdownStartTime >= 5000) {
      if (beamBroken) {
        if (piezzoHit) {
          score++; // Score met bord
          Serial.print("Score + bord! Totaal = ");
          Serial.println(score);

          digitalWrite(ledPin, HIGH);
          pixels.setPixelColor(currentPixel, pixels.Color(0, 255, 0)); // Groen
          myDFPlayer.play(1); // Score geluid
        } else {
          scoreWithoutBoard++; // Score zonder bord
          Serial.print("Score zonder bord! Totaal = ");
          Serial.println(scoreWithoutBoard);

          digitalWrite(ledPin, LOW);
          pixels.setPixelColor(currentPixel, pixels.Color(255, 0, 0)); // Rood
          myDFPlayer.play(2); // Miss/zonder bord geluid
        }
      } else {
        Serial.println("Miss!");
        digitalWrite(ledPin, LOW);
        pixels.setPixelColor(currentPixel, pixels.Color(0, 0, 255)); // Blauw = geen score
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
