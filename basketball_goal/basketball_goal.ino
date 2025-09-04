#include <Adafruit_NeoPixel.h>

const int irReceiverPin = 2;    // Grove IR receiver
const int ky032VCC = 4;         // KY-032 IR zender VCC
const int platePin = 3;         // Startknop
const int ledPin = 13;          // Indicator LED
const int neoPin = 6;           // NeoPixel data pin
const int piezzoPin = A0;       // Piezzo sensor op het bord

// ---------- NeoPixel ----------
const int numPixels = 8;
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

// ---------- DFPlayer ----------
SoftwareSerial mySerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// ---------- Game state ----------
>>>>>>> 22b6c9342c6c4a2b64494c2aec90478451b7b878
bool countdownActive = false;
unsigned long countdownStartTime = 0;
bool beamBroken = false;
bool piezzoHit = false;


int currentPixel = 0;
int score = 0;            // Score teller
int scoreWithoutBoard = 0; // Score zonder bord

// ---------- Piezzo drempel ----------
const int piezzoThreshold = 100; // waarde afhankelijk van piezzo, moet getest worden

void setup() {
  // ---------- Input/Output ----------
>>>>>>> 22b6c9342c6c4a2b64494c2aec90478451b7b878
  pinMode(irReceiverPin, INPUT);
  pinMode(ky032VCC, OUTPUT);
  pinMode(platePin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
<<<<<<< HEAD
  pinMode(platePin, INPUT_PULLUP);
  Serial.begin(9600);

  // Initialize Neopixel
  pixels.begin();
  pixels.clear();
  pixels.show();
=======

  // Zet KY-032 zender aan
  digitalWrite(ky032VCC, HIGH);

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
>>>>>>> 22b6c9342c6c4a2b64494c2aec90478451b7b878
}

void loop() {
  // ---------- Lees inputs ----------
  int beamState = digitalRead(irReceiverPin);
<<<<<<< HEAD
  int plateState = digitalRead(platePin); // LOW wanneer ingedrukt

  // Start tellen wanneer pressure plate ingedrukt is
  if (!countdownActive && plateState == LOW) {
=======
  int startState = digitalRead(platePin);
  int piezzoValue = analogRead(piezzoPin);

  // ---------- Start countdown ----------
  if (!countdownActive && startState == LOW) {
>>>>>>> 22b6c9342c6c4a2b64494c2aec90478451b7b878
    countdownActive = true;
    countdownStartTime = millis();
    beamBroken = false;
    piezzoHit = false;
    Serial.println("Countdown gestart!");
  }

  if (countdownActive) {
<<<<<<< HEAD
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
=======
    // Check IR beam (Grove IR sensor)
    if (beamState == HIGH) { // HIGH betekent onderbroken
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
>>>>>>> 22b6c9342c6c4a2b64494c2aec90478451b7b878
      }
      pixels.show();

<<<<<<< HEAD
      // Move to the next pixel for the next round
      currentPixel++;
      if (currentPixel >= numPixels) currentPixel = 0;

      countdownActive = false; // Reset for next round
=======
      pixels.show();

      // Volgende pixel
      currentPixel++;
      if (currentPixel >= numPixels) currentPixel = 0;

      // Reset countdown
      countdownActive = false;
>>>>>>> 22b6c9342c6c4a2b64494c2aec90478451b7b878
    }
  }
}
