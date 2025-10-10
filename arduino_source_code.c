#include <Servo.h>

// Definisi pin
#define TRIG_PANAS 2
#define ECHO_PANAS 3
#define SERVO_PANAS_PIN 9

#define TRIG_NORMAL 4
#define ECHO_NORMAL 5
#define SERVO_NORMAL_PIN 10

#define TRIG_DINGIN 6
#define ECHO_DINGIN 7
#define SERVO_DINGIN_PIN 11

// Batas jarak deteksi gelas (centimeter)
const int JARAK_THRESHOLD = 10;

// Inisialisasi servo
Servo servoPanas;
Servo servoNormal;
Servo servoDingin;

// 0 = tidak ada yang aktif (standby)
// 1 = panas aktif, 2 = normal aktif, 3 = dingin aktif
int servoAktif = 0;

// Deklarasi prototipe fungsi
long ukurJarak(int trigPin, int echoPin);

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
  // BAGIAN 1: Jika dispenser sedang standby (tidak ada keran yang menyala)
  if (servoAktif == 0) {
    long jarakPanas = ukurJarak(TRIG_PANAS, ECHO_PANAS);
    if (jarakPanas < JARAK_THRESHOLD && jarakPanas > 0) {
      servoPanas.write(90); // Buka keran panas
      servoAktif = 1;       // Keran panas aktif
    }
    else {
      long jarakNormal = ukurJarak(TRIG_NORMAL, ECHO_NORMAL);
      if (jarakNormal < JARAK_THRESHOLD && jarakNormal > 0) {
        servoNormal.write(90); // Buka keran normal
        servoAktif = 2;        // Keran normal aktif
      }
      else {
        long jarakDingin = ukurJarak(TRIG_DINGIN, ECHO_DINGIN);
        if (jarakDingin < JARAK_THRESHOLD && jarakDingin > 0) {
          servoDingin.write(90); // Buka keran dingin
          servoAktif = 3;        // Keran dingin sedang aktif
        }
      }
    }
  }
  // BAGIAN 2: Jika ada salah satu keran yang sedang menyala
  else {
    // Cek apakah gelas pada keran yang aktif sudah ditarik
    if (servoAktif == 1) { // Jika keran panas aktif
      long jarakPanas = ukurJarak(TRIG_PANAS, ECHO_PANAS);
      if (jarakPanas > JARAK_THRESHOLD || jarakPanas == 0) {
        servoPanas.write(0); // Tutup keran panas
        servoAktif = 0;      // Kembali ke mode standby
        delay(1000);
      }
    }
    else if (servoAktif == 2) { // Jika keran normal aktif
      long jarakNormal = ukurJarak(TRIG_NORMAL, ECHO_NORMAL);
      if (jarakNormal > JARAK_THRESHOLD || jarakNormal == 0) {
        servoNormal.write(0); // Tutup keran normal
        servoAktif = 0; // Kembali ke mode standby
        delay(1000);
      }
    }
    else if (servoAktif == 3) { // Jika keran dingin aktif
      long jarakDingin = ukurJarak(TRIG_DINGIN, ECHO_DINGIN);
      if (jarakDingin > JARAK_THRESHOLD || jarakDingin == 0) {
        servoDingin.write(0); // Tutup keran dingin
        servoAktif = 0; // Kembali ke mode standby
        delay(1000);
      }
    }
  }
  
  delay(50);
}

// Fungsi ukur jarak masing-masing sensor
long ukurJarak(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long durasi = pulseIn(echoPin, HIGH);
  return durasi * 0.034 / 2;
}