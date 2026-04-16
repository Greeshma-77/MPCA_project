#include <Servo.h> 

#define TRIGGER_PIN  9
#define ECHO_PIN     10
#define SERVO_PIN    11
#define IR_SENSOR_PIN 2
#define LED_PIN 7

#define MAX_DISTANCE 20

Servo servo; 

unsigned long openTime = 0;
unsigned long lastMoveTime = 0;

bool isOpen = false;
bool isClosing = false;

int currentAngle = 0;

void setup() { 
  Serial.begin(9600); 
  
  pinMode(TRIGGER_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT); 
  
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  servo.attach(SERVO_PIN); 
  servo.write(0);
} 

void loop() { 
  long duration;
  float distance;

  // 🔵 Ultrasonic
  digitalWrite(TRIGGER_PIN, LOW); 
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER_PIN, HIGH); 
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER_PIN, LOW); 

  duration = pulseIn(ECHO_PIN, HIGH); 
  distance = duration * 0.034 / 2;

  // 🟢 OPEN lid
  if (distance > 0 && distance <= MAX_DISTANCE && !isOpen) { 
    currentAngle = 90;
    servo.write(currentAngle);
    isOpen = true;
    isClosing = false;
    openTime = millis();
    Serial.println("Lid Opened");
  }

  // ⏱ Start closing after 10 sec
  if (isOpen && (millis() - openTime >= 10000)) {
    isClosing = true;
    isOpen = false;
  }

  // 🔄 Smooth closing WITHOUT delay
  if (isClosing && (millis() - lastMoveTime >= 20)) {  
    lastMoveTime = millis();

    if (currentAngle > 0) {
      currentAngle--;        // move step
      servo.write(currentAngle);
    } else {
      isClosing = false;
      Serial.println("Lid Closed Smoothly");
    }
  }

  // 🔴 IR sensor
  int irState = digitalRead(IR_SENSOR_PIN);

  if (irState == LOW) {  
    digitalWrite(LED_PIN, HIGH);   
  } else {
    digitalWrite(LED_PIN, LOW);    
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(50);  // small delay (safe)
}