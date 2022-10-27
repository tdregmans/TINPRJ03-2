/*
 * ACM voor project 2 Technische Informatica
 * 
 * Bartholomeus Petrus
 * student 1034228
 * Thijs Dregmans
 * student 1024272
 * Periode 2, Ti1H
 * 
 * versie 2.1
 */

// include libs
#include <Wire.h>

// define pins
#define irLinks 2
#define irRechts 3
#define reed 4
#define linksMotorCW 5
#define linksMotorCCW 6
#define rechtsMotorCW 7
#define rechtsMotorCCW 8
#define ultraSoonTrig 9
#define ultraSoonEcho 10

#define besturingMetWifiPin 12

// Wire.h instellingen


// define motor constanten
#define motorVooruit 1
#define motorStil 0
#define motorAchteruit -1

// define Ultrasoon constant
const int aantalMinimale_mm = 20;
int UltrasoonWaardes[3] = {0, 0, 0};

void setup() {
  Serial.begin(9600);

  // define pinnnen
  pinMode(irLinks, INPUT);
  pinMode(irRechts, INPUT);
  
  pinMode(reed, INPUT);

  pinMode(linksMotorCW, OUTPUT);
  pinMode(linksMotorCCW, OUTPUT);
  pinMode(rechtsMotorCW, OUTPUT);
  pinMode(rechtsMotorCCW, OUTPUT);
  
  pinMode(ultraSoonTrig, OUTPUT);
  pinMode(ultraSoonEcho, INPUT);

  
  pinMode(besturingMetWifiPin, INPUT);
}

// besturingMetWifi = false --> ACM algortime
// besturingMetWifi = true --> ACM besturing met wifi


bool detecteerObstakel() {
  digitalWrite(ultraSoonTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraSoonTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultraSoonTrig, LOW);
  double duration = pulseIn(ultraSoonEcho, HIGH);
  // Calculating the distance
  double newDistance = duration * 0.034 / 2;
  /*
  UltrasoonWaardes[0] = UltrasoonWaardes[1];
  UltrasoonWaardes[1] = UltrasoonWaardes[2];
  UltrasoonWaardes[2] = newDistance;
  double distance = (UltrasoonWaardes[0] + UltrasoonWaardes[1] + UltrasoonWaardes[2]) / 3;
  */
  return newDistance <= aantalMinimale_mm;
}

bool detecteerAfgrondOfTapeLinks() {
  return digitalRead(irLinks);
}

bool detecteerAfgrondOfTapeRechts() {
  return digitalRead(irRechts);
}

bool detecteerMagneetstrip() {
  return digitalRead(reed);
}


void motorenAansturen(int links, int rechts) {
  // gebruik functie: motorenAansturen(motorVooruit,motorVooruit);
  if(links == -1) {
    // links motor achteruit
    Serial.println("links motor achteruit");
    digitalWrite(linksMotorCW, LOW);
    digitalWrite(linksMotorCCW, HIGH);
  }
  else if(links == 0) {
    // links motor stil
    Serial.println("links motor stil");
    digitalWrite(linksMotorCW, LOW);
    digitalWrite(linksMotorCCW, LOW);
  }
  else if(links == 1) {
    // links motor vooruit
    Serial.println("links motor vooruit");
    digitalWrite(linksMotorCW, HIGH);
    digitalWrite(linksMotorCCW, LOW);
  }
  if(rechts == -1) {
    // rechts motor achteruit
    Serial.println("rechts motor achteruit");
    digitalWrite(rechtsMotorCW, LOW);
    digitalWrite(rechtsMotorCCW, HIGH);
  }
  else if(rechts == 0) {
    // rechts motor stil
    Serial.println("rechts motor stil");
    digitalWrite(rechtsMotorCW, LOW);
    digitalWrite(rechtsMotorCCW, LOW);
  }
  else if(rechts == 1) {
    // rechts motor vooruit
    Serial.println("rechts motor vooruit");
    digitalWrite(rechtsMotorCW, HIGH);
    digitalWrite(rechtsMotorCCW, LOW);
  }
}

bool besturingMetWifi() {
  return digitalRead(besturingMetWifiPin);
}


void loop(){
  // haal gegevens op van ESP-afstandsbediening:
  // check of besturing met wifi is


    
  if(besturingMetWifi()) {
    Serial.println("besturing me wifi");
  }
  else {
    // besturing door ACM algoritme
    //Serial.println("besturing met algoritme");
    if(!(detecteerObstakel() || detecteerAfgrondOfTapeLinks() || detecteerAfgrondOfTapeRechts())) {
      Serial.println("niks aan de hand");
      motorenAansturen(motorVooruit,motorVooruit);
    }
    else if(detecteerObstakel()) {
      Serial.println("obstakel");
      // rij een stukje achteruit
      motorenAansturen(motorAchteruit,motorAchteruit);
      delay(2000);
      motorenAansturen(motorVooruit,motorAchteruit);
      delay(2000);
      motorenAansturen(motorStil,motorStil);
      }
    
    else if(detecteerAfgrondOfTapeLinks()) {
      Serial.println("afgrond of tape links");
      // rij een stukje achteruit
      motorenAansturen(motorAchteruit,motorAchteruit);
      delay(2000);
      // draai naar rechts
      motorenAansturen(motorVooruit,motorAchteruit);
      delay(2000);
      motorenAansturen(motorStil,motorStil);
    }
    else if(detecteerAfgrondOfTapeRechts()) {
      Serial.println("afgrond of tape rechts");
      // rij een stukje achteruit
      motorenAansturen(motorAchteruit,motorAchteruit);
      delay(2000);
      // draai naar links
      motorenAansturen(motorAchteruit,motorVooruit);
      delay(2000);
      motorenAansturen(motorStil,motorStil);
    }
  
    if(detecteerMagneetstrip()) {
      // reed ziet magneetstrip
      delay(2000);
    }
  } 
  

  Serial.print("links: ");
  Serial.println(detecteerAfgrondOfTapeLinks());

  Serial.print("rechts: ");
  Serial.println(detecteerAfgrondOfTapeRechts());

  Serial.print("obstakel: ");
  Serial.println(detecteerObstakel());
  delay(1000);
}
