#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// ---------- Pins ----------
const int ledPin = 13;
const int neoPin = 6;

// ---------- NeoPixel ----------
const int numPixels = 8;
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

// ---------- DFPlayer ----------
SoftwareSerial mySerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  // NeoPixel
  pixels.begin();
  pixels.clear();
  pixels.show();

  // DFPlayer
  mySerial.begin(9600);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini niet gevonden!");
    while (true);
  }
  myDFPlayer.volume(20);
}

void loop() {
  // LED test
  digitalWrite(ledPin, HIGH);
  Serial.println("LED aan");
  delay(1000);
  digitalWrite(ledPin, LOW);
  Serial.println("LED uit");
  delay(1000);

  // NeoPixel test
  for (int i = 0; i < numPixels; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // rood
    pixels.show();
    delay(200);
    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // groen
    pixels.show();
    delay(200);
    pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // blauw
    pixels.show();
    delay(200);
  }
  pixels.clear();
  pixels.show();

  // DFPlayer test
  Serial.println("Speel test geluid 1");
  myDFPlayer.play(1);
  delay(2000);
  Serial.println("Speel test geluid 2");
  myDFPlayer.play(2);
  delay(2000);
}
