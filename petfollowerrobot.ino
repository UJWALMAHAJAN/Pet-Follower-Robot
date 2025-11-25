#include <Servo.h>
Servo Myservo;

#define trigPin 9
#define echoPin 8

#define MLa 4
#define MLb 5
#define MRa 6
#define MRb 7

long duration, distance;

// Distance thresholds (in cm)
int safeMin = 20;   // Too close - move back
int safeMax = 40;   // Too far - follow

void setup() {
  Serial.begin(9600);

  pinMode(MLa, OUTPUT);
  pinMode(MLb, OUTPUT);
  pinMode(MRa, OUTPUT);
  pinMode(MRb, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Myservo.attach(10);
  Myservo.write(90); // Always look forward
  delay(500);
}

// Motor control
void rightMotor(bool forward) {
  digitalWrite(MRa, forward ? LOW : HIGH);
  digitalWrite(MRb, forward ? HIGH : LOW);
}

void leftMotor(bool forward) {
  digitalWrite(MLa, forward ? LOW : HIGH);
  digitalWrite(MLb, forward ? HIGH : LOW);
}

void moveForward() {
  leftMotor(true);
  rightMotor(true);
}

void moveBackward() {
  leftMotor(false);
  rightMotor(false);
}

void stopMotors() {
  digitalWrite(MLa, LOW);
  digitalWrite(MLb, LOW);
  digitalWrite(MRa, LOW);
  digitalWrite(MRb, LOW);
}

long readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 20000);
  long dist = duration / 58.2;
  if (dist <= 0 || dist > 300) dist = 300;
  return dist;
}

void loop() {
  Myservo.write(90);
  delay(20);
  distance = readDistance();

  Serial.println(distance);

  if (distance > safeMax) {
    moveForward();     // Too far → Follow!
  }
  else if (distance < safeMin) {
    moveBackward();    // Too close → Step back
    delay(200);
  }
  else {
    stopMotors();      // Perfect distance → Stay like a pet beside you
  }

  delay(30);
}
