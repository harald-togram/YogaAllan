#include <FABRIK2D.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Ramp.h>

rampInt rampHojreBagBenX;
rampInt rampHojreBagBenY;

rampInt rampVenstreBagBenX;
rampInt rampVenstreBagBenY;

rampInt rampVenstreForBenX;
rampInt rampVenstreForBenY;

rampInt rampHojreForBenX;
rampInt rampHojreForBenY;

int lengths[] = { 60, 65 };
Fabrik2D hojreBagBen(3, lengths);
Fabrik2D venstreBagBen(3, lengths);
Fabrik2D venstreForBen(3, lengths);
Fabrik2D hojreForBen(3, lengths);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN 150   // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600   // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 600      // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400     // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50  // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;

#define MULTIPLY 1
const int topPosX = 30;   //>90
const int bundPosX = 80;  //<20
int x;
int toggle_x;

const int topPosY = 35;
const int bundPosY = 35;
int y = 40;
int toggle_y;
//const int y = 20;
int xToggle;
int lastX;
#define METODE QUADRATIC_INOUT
#define TID 1200

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  Wire.begin(40, 41);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  pinMode(39, OUTPUT);
  digitalWrite(39, LOW);
  delay(10);

  hojreForBen.setTolerance(1);
  hojreBagBen.setTolerance(1);
  venstreBagBen.setTolerance(1);
  venstreForBen.setTolerance(1);
  delay(1000);
  for (int i = 0; i < 12; i++) {
    int servoGrader = 90;
    Serial.println(servoGrader);
    int servoPuls = map(servoGrader, 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(i, 0, servoPuls);
    delay(1000);
  }

  delay(10000);
  Serial.print("ang0");
  Serial.print("\t");
  Serial.print("ang1");
  Serial.print("\t");
  Serial.print("x0");
  Serial.print("\t");
  Serial.print("y0");
  Serial.print("\t");
  Serial.print("x1");
  Serial.print("\t");
  Serial.print("y1");
  Serial.print("\t");
  Serial.print("x2");
  Serial.print("\t");
  Serial.println("y2");

  Serial.begin(115200);

  rampVenstreBagBenX.setGrain(1);
  rampVenstreBagBenX.go(bundPosX);
  rampVenstreBagBenY.setGrain(1);
  rampVenstreBagBenY.go(bundPosX);


  rampHojreBagBenX.setGrain(1);
  rampHojreBagBenX.go(bundPosX);
  rampHojreBagBenY.setGrain(1);
  rampHojreBagBenY.go(bundPosX);

  rampHojreForBenX.setGrain(1);
  rampHojreForBenX.go(bundPosX);
  rampHojreForBenY.setGrain(1);
  rampHojreForBenY.go(bundPosX);

  rampVenstreForBenX.setGrain(1);
  rampVenstreForBenX.go(bundPosX);
  rampVenstreForBenY.setGrain(1);
  rampVenstreForBenY.go(bundPosX);
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;

  pulselength = 1000000;      // 1,000,000 us per second
  pulselength /= SERVO_FREQ;  // Analog servos run at ~60 Hz updates
  Serial.print(pulselength);
  Serial.println(" us per period");
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength);
  Serial.println(" us per bit");
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  Serial.println("start er klaret");
  position(0, 30, 35);
  Serial.println("position er klaret");
  while (opdater() == false) {}
  
  position(0, 80, 35);
  while (opdater() == false) {}
/*
  rampVenstreBagBenX.go(topPosX, TID, METODE, ONCEFORWARD);
  rampVenstreBagBenY.go(topPosY, TID, METODE, ONCEFORWARD);
  while (rampVenstreBagBenX.getCompletion() != 100 && rampVenstreBagBenY.getCompletion() != 100) {
    venstreBagBen.solve(rampVenstreBagBenX.update(), rampVenstreBagBenY.update(), lengths);
    int shoulderAngle = venstreBagBen.getAngle(0) * RAD_TO_DEG * MULTIPLY;
    int elbowAngle = venstreBagBen.getAngle(1) * RAD_TO_DEG * MULTIPLY;
    //setServoPulse(180 - shoulderAngle, 1);
    //setServoPulse(180 - (0 - elbowAngle), 2);
    setServoPulse(180 - (shoulderAngle), 4);
    setServoPulse(180 - (0 - elbowAngle), 5);
    serialSend();
  }
  rampVenstreBagBenX.go(bundPosX, TID, METODE, ONCEFORWARD);
  rampVenstreBagBenY.go(bundPosY, TID, METODE, ONCEFORWARD);
  while (rampVenstreBagBenX.getCompletion() != 100 && rampVenstreBagBenY.getCompletion() != 100) {
    venstreBagBen.solve(rampVenstreBagBenX.update(), rampVenstreBagBenY.update(), lengths);
    int shoulderAngle = venstreBagBen.getAngle(0) * RAD_TO_DEG * MULTIPLY;
    int elbowAngle = venstreBagBen.getAngle(1) * RAD_TO_DEG * MULTIPLY;
    //setServoPulse(180 - shoulderAngle, 1);
    //setServoPulse(180 - (0 - elbowAngle), 2);
    setServoPulse(180 - (shoulderAngle), 4);
    setServoPulse(180 - (0 - elbowAngle), 5);
    serialSend();
  }


  rampVenstreForBenX.go(topPosX, TID, METODE, ONCEFORWARD);
  rampVenstreForBenY.go(topPosY, TID, METODE, ONCEFORWARD);
  while (rampVenstreForBenX.getCompletion() != 100 && rampVenstreForBenY.getCompletion() != 100) {
    venstreForBen.solve(rampVenstreForBenX.update(), rampVenstreForBenY.update(), lengths);
    int shoulderAngle = venstreForBen.getAngle(0) * RAD_TO_DEG * MULTIPLY;
    int elbowAngle = venstreForBen.getAngle(1) * RAD_TO_DEG * MULTIPLY;
    //  setServoPulse(180 - shoulderAngle, 1);
    //  setServoPulse(180 - (0 - elbowAngle), 2);
    setServoPulse(shoulderAngle, 4 + 3);
    setServoPulse(0 - elbowAngle, 5 + 3);
    serialSend();
  }
  rampVenstreForBenX.go(bundPosX, TID, METODE, ONCEFORWARD);
  rampVenstreForBenY.go(bundPosY, TID, METODE, ONCEFORWARD);
  while (rampVenstreForBenX.getCompletion() != 100 && rampVenstreForBenY.getCompletion() != 100) {
    venstreForBen.solve(rampVenstreForBenX.update(), rampVenstreForBenY.update(), lengths);
    int shoulderAngle = venstreForBen.getAngle(0) * RAD_TO_DEG * MULTIPLY;
    int elbowAngle = venstreForBen.getAngle(1) * RAD_TO_DEG * MULTIPLY;
    //  setServoPulse(180 - shoulderAngle, 1);
    //  setServoPulse(180 - (0 - elbowAngle), 2);
    setServoPulse(shoulderAngle, 4 + 3);
    setServoPulse(0 - elbowAngle, 5 + 3);
    serialSend();
  }*/
   delay(3000);
}

void setServoPulse(int angle, int servoNum) {
  int puls = map(angle, 0, 180 * MULTIPLY, SERVOMIN, SERVOMAX);
  pwm.setPWM(servoNum, 0, puls);
}

void serialSend() {
  Serial.print(venstreBagBen.getAngle(0) * 57296 / 1000);
  Serial.print("\t");
  Serial.print(venstreBagBen.getAngle(1) * 57296 / 1000);
  Serial.print("\t");
  Serial.print(venstreBagBen.getX(0));
  Serial.print("\t");
  Serial.print(venstreBagBen.getY(0));
  Serial.print("\t");
  Serial.print(venstreBagBen.getX(1));
  Serial.print("\t");
  Serial.print(venstreBagBen.getY(1));
  Serial.print("\t");
  Serial.print(venstreBagBen.getX(2));
  Serial.print("\t");
  Serial.println(venstreBagBen.getY(2));
}

void position(int ben, int x, int y) {
  switch (ben) {
    case 0:
      rampHojreBagBenX.go(x, TID, METODE, ONCEFORWARD);
      rampHojreBagBenY.go(y, TID, METODE, ONCEFORWARD);
      break;
    case 1:
      rampVenstreBagBenX.go(x, TID, METODE, ONCEFORWARD);
      rampVenstreBagBenY.go(y, TID, METODE, ONCEFORWARD);
      break;
    case 2:
      rampVenstreForBenX.go(x, TID, METODE, ONCEFORWARD);
      rampVenstreForBenY.go(y, TID, METODE, ONCEFORWARD);
      break;
    case 3:
      rampHojreForBenX.go(x, TID, METODE, ONCEFORWARD);
      rampHojreForBenY.go(y, TID, METODE, ONCEFORWARD);
      break;
  }
}

bool opdater() {
  venstreForBen.solve(rampVenstreForBenX.update(), rampVenstreForBenY.update(), lengths);
  venstreBagBen.solve(rampVenstreBagBenX.update(), rampVenstreBagBenY.update(), lengths);
  
  hojreForBen.solve(rampHojreForBenX.update(), rampHojreForBenY.update(), lengths);
  hojreBagBen.solve(rampHojreBagBenX.update(), rampHojreBagBenY.update(), lengths);
  
  int shoulderAngles[4] = {0,0,0,0};
  int elbowAngles[4] = {0,0,0,0};
  
  shoulderAngles[0] = hojreBagBen.getAngle(0) * RAD_TO_DEG * MULTIPLY;
  elbowAngles[0] = hojreBagBen.getAngle(1) * RAD_TO_DEG * MULTIPLY;
  
  shoulderAngles[1] = hojreForBen.getAngle(0) * RAD_TO_DEG * MULTIPLY;
  elbowAngles[1] = hojreForBen.getAngle(1) * RAD_TO_DEG * MULTIPLY;
  
  shoulderAngles[2] = venstreBagBen.getAngle(0) * RAD_TO_DEG * MULTIPLY;
  elbowAngles[2] = venstreBagBen.getAngle(1) * RAD_TO_DEG * MULTIPLY;
  
  shoulderAngles[3] = venstreForBen.getAngle(0) * RAD_TO_DEG * MULTIPLY;
  elbowAngles[3] = venstreForBen.getAngle(1) * RAD_TO_DEG * MULTIPLY;
  
  setServoPulse(shoulderAngles[0], 4 + 3);
  Serial.println(elbowAngles[0]);
  setServoPulse(0 - elbowAngles[0], 5 + 3);
  return (rampHojreBagBenX.getCompletion() != 100 && rampHojreBagBenY.getCompletion() != 100 && rampVenstreBagBenX.getCompletion() != 100 && rampVenstreBagBenY.getCompletion() != 100 && rampVenstreForBenX.getCompletion() != 100 && rampVenstreForBenY.getCompletion() != 100 && rampHojreForBenX.getCompletion() != 100 && rampHojreForBenY.getCompletion() != 100);
}
