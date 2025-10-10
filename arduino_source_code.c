#include <Servo.h>

// Definisi Pin
#define TRIG_PANAS 2
#define ECHO_PANAS 3
#define SERVO_PANAS_PIN 9

#define TRIG_NORMAL 4
#define ECHO_NORMAL 5
#define SERVO_NORMAL_PIN 10

#define TRIG_DINGIN 6
#define ECHO_DINGIN 7
#define SERVO_DINGIN_PIN 11

// Inisialisasi Servo
Servo servoPanas;
Servo servoNormal;
Servo servoDingin;

// Variabel penanda status dispenser
bool keranNyala = false;

// Deklarasi Prototipe Fungsi
long ukurJarak(int trigPin, int echoPin);
void tekanServo(Servo &servo);

void setup() {
  pinMode(TRIG_PANAS, OUTPUT);
  pinMode(ECHO_PANAS, INPUT);
  pinMode(TRIG_NORMAL, OUTPUT);
  pinMode(ECHO_NORMAL, INPUT);
  pinMode(TRIG_DINGIN, OUTPUT);
  pinMode(ECHO_DINGIN, INPUT);

  servoPanas.attach(SERVO_PANAS_PIN);
  servoNormal.attach(SERVO_NORMAL_PIN);
  servoDingin.attach(SERVO_DINGIN_PIN);

  servoPanas.write(0);
  servoNormal.write(0);
  servoDingin.write(0);
}

void loop() {
  // Jalankan pengecekan sensor jarak jika dispenser menganggur
  if (!keranNyala) {
    long jarakPanas = ukurJarak(TRIG_PANAS, ECHO_PANAS);
    if (jarakPanas < 10 && jarakPanas > 0) {
      tekanServo(servoPanas);
    }

    else if (jarakNormal < 10 && jarakNormal > 0) {
      tekanServo(servoNormal);
    }

    else if (jarakDingin < 10 && jarakDingin > 0) {
      tekanServo(servoDingin);
    }
  }
  
  delay(100);
}

long ukurJarak(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long durasi = pulseIn(echoPin, HIGH);
  return durasi * 0.034 / 2;
}

void tekanServo(Servo &servo) {
  // Fokus 1 keran menyala
  keranNyala = true; 

  servo.write(90);
  delay(1000);
  servo.write(0);
  delay(2000); 
  
  // Keran selesai
  keranNyala = false;
}