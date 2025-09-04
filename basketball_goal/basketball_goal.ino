#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>

// ---------- Pin definitions ----------
const int irReceiverPin = 2;    // Grove IR receiver
const int ky032VCC      = 4;    // KY-032 IR zender VCC
const int platePin      = 3;    // Startknop
const int ledPin        = 13;   // Indicator LED
const int neoPin        = 6;    // NeoPixel data pin
const int piezzoPin     = A0;   // Piezzo sensor op het bord

// ---------- NeoPixel ----------
const int numPixels = 8;
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

// ---------- DFPlayer ----------
DFRobotDFPlayerMini myDFPlayer;

// ---------- Game state ----------
bool countdownActive = false;
unsigned long countdownStartTime = 0;
bool beamBroken = false;
bool piezzoHit = false;

int currentPixel = 0;
int score = 0;             
int scoreWithoutBoard = 0; 

// ---------- Piezzo drempel ----------
const int piezzoThreshold = 100; // afhankelijk van piezzo, moet getest worden

void setup() {
  // ---------- Input/Output ----------
  pinMode(irReceiverPin, INPUT);
  pinMode(ky032VCC, OUTPUT);
  pinMode(platePin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  // Zet KY-032 zender aan
  digitalWrite(ky032VCC, HIGH);

  // ---------- Serial ----------
  Serial.begin(9600);     // USB serial (debugging)
  Serial1.begin(9600);    // Hardware serial for DFPlayer

  // ---------- NeoPixel ----------
  pixels.begin();
  pixels.clear();
  pixels.show();

  // ---------- DFPlayer ----------
  if (!myDFPlayer.begin(Serial1)) {
    Serial.println("DFPlayer Mini niet gevonden!");
    while (true); // stop als module niet gevonden
  }
  Serial.println("DFPlayer Mini online.");
  myDFPlayer.volume(20); // volume 0-30
}

void loop() {
  // ---------- Lees inputs ----------
  int beamState   = digitalRead(irReceiverPin);
  int startState  = digitalRead(platePin);
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
    // Check IR beam (Grove IR sensor)
    if (beamState == HIGH) { 
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
          score++;
          Serial.print("Score + bord! Totaal = ");
          Serial.println(score);

          digitalWrite(ledPin, HIGH);
          pixels.setPixelColor(currentPixel, pixels.Color(0, 255, 0)); // Groen
          myDFPlayer.play(1); // Score geluid
        } else {
          scoreWithoutBoard++;
          Serial.print("Score zonder bord! Totaal = ");
          Serial.println(scoreWithoutBoard);

          digitalWrite(ledPin, LOW);
          pixels.setPixelColor(currentPixel, pixels.Color(255, 0, 0)); // Rood
          myDFPlayer.play(2); // Miss/zonder bord geluid
        }
      } else {
        Serial.println("Miss!");
        digitalWrite(ledPin, LOW);
        pixels.setPixelColor(currentPixel, pixels.Color(0, 0, 255)); // Blauw
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
