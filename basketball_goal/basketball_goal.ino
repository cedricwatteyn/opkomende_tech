#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// -------------------- PIN DEFINITIES --------------------
const int irReceiverPin = 2;    // IR beam sensor
const int ledPin = 13;          // LED indicator
const int platePin = 3;         // EG STARTS 5X knop (start countdown)
const int resetPin = 4;         // Reset-knop
const int neoPin = 6;           // NeoPixel data pin

// -------------------- NEO PIXEL --------------------
const int numPixels = 8;
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

// -------------------- DFPLAYER --------------------
SoftwareSerial mySerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// -------------------- GAME STATE --------------------
bool countdownActive = false;
unsigned long countdownStartTime = 0;
bool beamBroken = false;

int currentPixel = 0;
int score = 0; // Score teller

// -------------------- SETUP --------------------
void setup() {
  // Pin modes
  pinMode(irReceiverPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(platePin, INPUT_PULLUP);  // Startknop
  pinMode(resetPin, INPUT_PULLUP);  // Resetknop

  // Serial monitor
  Serial.begin(9600);
  Serial.println("Basketball game initializing...");

  // NeoPixel initialisatie
  pixels.begin();
  pixels.clear();
  pixels.show();

  // DFPlayer initialisatie
  mySerial.begin(9600);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini not found!");
    while (true); // Stop hier als geen module gevonden
  }
  Serial.println("DFPlayer Mini online.");
  myDFPlayer.volume(20);  // Volume 0-30
}

// -------------------- LOOP --------------------
void loop() {
  int beamState = digitalRead(irReceiverPin);
  int startState = digitalRead(platePin);  // Startknop
  int resetState = digitalRead(resetPin);  // Reset-knop

  // -------- START COUNTDOWN --------
  if (!countdownActive && startState == LOW) {
    countdownActive = true;
    countdownStartTime = millis();
    beamBroken = false;
    Serial.println("Countdown started!");
  }

  // -------- RESET SCORE --------
  if (resetState == LOW) {
    score = 0;
    currentPixel = 0;
    pixels.clear();
    pixels.show();
    digitalWrite(ledPin, LOW);
    Serial.println("Score reset!");
    delay(200); // korte debounce
  }

  // -------- COUNTDOWN ACTIVE --------
  if (countdownActive) {
    // Controleer of beam onderbroken is
    if (beamState == HIGH) {
      beamBroken = true;
    }

    // 5 seconden timer
    if (millis() - countdownStartTime >= 5000) {
      if (beamBroken) {
        // SCORE!
        score++;
        Serial.print("Score! Total = ");
        Serial.println(score);

        digitalWrite(ledPin, HIGH); // LED aan
        pixels.setPixelColor(currentPixel, pixels.Color(0, 255, 0)); // Groen voor score
        pixels.show();

        myDFPlayer.play(1); // Score geluid
      } else {
        // MISS
        Serial.print("Miss! Total = ");
        Serial.println(score);

        digitalWrite(ledPin, LOW); // LED uit
        pixels.setPixelColor(currentPixel, pixels.Color(255, 0, 0)); // Rood voor miss
        pixels.show();

        myDFPlayer.play(2); // Miss geluid
      }

      // Volgende pixel klaarzetten
      currentPixel++;
      if (currentPixel >= numPixels) {
        currentPixel = 0;
      }

      // Countdown resetten
      countdownActive = false;
    }
  }
}
