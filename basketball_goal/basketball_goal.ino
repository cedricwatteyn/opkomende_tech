#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

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
  pinMode(irReceiverPin, INPUT);
  pinMode(ky032VCC, OUTPUT);
  pinMode(platePin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

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
}

void loop() {
  // ---------- Lees inputs ----------
  int beamState = digitalRead(irReceiverPin);
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
