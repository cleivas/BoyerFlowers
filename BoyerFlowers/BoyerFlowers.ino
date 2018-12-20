/* Boyer Flowers by Clara Leivas, 2018
 * licensed under CC BY-SA 4.0 
 * https://creativecommons.org/licenses/by-sa/4.0/ 
 */
 
 #define LED 13
#define POT A0

#include <CapacitiveSensor.h>

//Capsense
CapacitiveSensor   cs = CapacitiveSensor(4, 2); //sensor pin = 2
boolean touched = false;

//UV led
#define UV 9
int fade = 0;
long preTimeFade = 0;
long currentTime = 0;

//Average baseValue
const int numReadings = 300;
int readings[numReadings];
int readIndex = 0;
long total = 0;
long preTime = 0;
int interval = 200;
int baseVal = 0;

//Current aerage value
const int numReadingsC = 10;
int readingsC[numReadingsC];
int readIndexC = 0;
long totalC = 0;


void setup() {
  analogWrite(UV, 0);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  for (int i = 0; i < numReadingsC; i++) {
    readings[i] = 0;
  }
}

void loop() {
  currentTime = millis();

  int r = cs.capacitiveSensor(3);

  if (currentTime - preTime > interval) {
    baseVal = getBase(r);
    preTime = currentTime;
  }
  int capVal = getCurrent(r);

  int add = map(analogRead(POT), 0, 1023, 100, 2000);
  int threshold = baseVal + add;

  if (capVal > threshold) fadeUV('i', 5, 3);
  else fadeUV('o', 1, 50);

  /*
    Serial.print(add);
    Serial.print("  ");
    Serial.print(threshold);
    Serial.print("  ");
    Serial.print(baseVal);
    Serial.print("  ");
    Serial.println(capVal);
  */
}


void fadeUV(char _m, int _fs, int _i) {
  char m = _m;
  int fStep = _fs;
  int interval = _i;

  if (m == 'i') {
    if (currentTime - preTime > interval) {
      fade += fStep;
      preTime = currentTime;
    }
  }

  else if (m == 'o') {
    if (currentTime - preTime > interval) {
      fade -= fStep;
      preTime = currentTime;
    }
  }


  fade = constrain(fade, 0, 100);
  int linearFade = linearPWM(fade);
  analogWrite(UV, linearFade);

}

int linearPWM(int percentage) {
  // coefficients
  double a = 9.7758463166360387E-01;
  double b = 5.5498961535023345E-02;
  return floor((a * exp(b * percentage) + .5)) - 1;
}

int getBase(int _nv) {
  total = total - readings[readIndex];
  readings[readIndex] =  _nv;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) readIndex = 0;

  int a = total / numReadings;
  return (a);
}

int getCurrent(int _nv) {
  totalC = totalC - readingsC[readIndexC];
  readingsC[readIndexC] =  _nv;
  totalC = totalC + readingsC[readIndexC];
  readIndexC = readIndexC + 1;

  if (readIndexC >= numReadingsC) readIndexC = 0;

  int a = totalC / numReadingsC;
  return (a);
}
