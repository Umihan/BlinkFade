#include <Arduino.h>

#define Blinkintervall 300
#define FadeIntervall 30

//*** Ein- und Ausgänge
#define T1 3 // aktive LOW
#define L1 5 // PWM-fähig

//*** Zustände
#define BLINKEN 0
#define FADEN 1

int Zustand;
int fadeValue = 0, fadeDirection = 1;
unsigned long lastTimeT1 = 0, lastTimeBlink = 0, lastFadeTime = 0;
bool lastStateT1 = false, LedState = false;

bool steigendeFlankeT1()
{
  bool stFlanke;
  bool aktStateT1 = digitalRead(T1);
  if (aktStateT1 == LOW && lastStateT1 == HIGH) // weil active low
  {
    stFlanke = true;
    Serial.println("Flanke erkannt");
  }
  else
    stFlanke = false;
  lastStateT1 = aktStateT1;
  return stFlanke;
}
void Blinken()
{
  if (millis() - lastTimeBlink > Blinkintervall)
  {
    LedState = !LedState;
    lastTimeBlink = millis();
    digitalWrite(L1, LedState);
  }
}
void Faden()
{
  if (millis() - lastFadeTime > FadeIntervall)
  {
    fadeValue += fadeDirection;
    if (fadeValue > 255 || fadeValue < 0)
      fadeDirection = -fadeDirection;
    else
      analogWrite(L1, fadeValue);
    lastFadeTime = millis();
  }
}
void setup()
{
  pinMode(L1, OUTPUT);
  pinMode(T1, INPUT);
  Serial.begin(115200);
  Zustand = BLINKEN;
}

void loop()
{
  bool stFlankeErkannt = steigendeFlankeT1();
  //*** Bedingungen
  if (stFlankeErkannt)
  {
    if (Zustand == BLINKEN)
      Zustand = FADEN;
    else if (Zustand == FADEN)
      Zustand = BLINKEN;
  }
  //** AKTIONEN
  switch (Zustand)
  {
  case BLINKEN:
    Blinken();
    break;
  case FADEN:
    Faden();
    break;

  default:
    break;
  }
}