// // Week3-Lecture2
// // Timer Interrupt (Internal)
// // Embedded IoT System Fall-2026

// // Name: Baria                  Reg#: 1099


// #include <Arduino.h>

// #define LED 4

// hw_timer_t *My_timer = NULL; //pointer object

// void ARDUINO_ISR_ATTR onTimer() {           // ARDUINO_ISR_ATTR == IRAM_ATTR
//   digitalWrite(LED, !digitalRead(LED));     // safe in ISR on ESP32
// }

// void setup() {
//   pinMode(LED, OUTPUT);

//   // 1 MHz timer tick (1 tick = 1 µs)
//   My_timer = timerBegin(1000000);

//   // attach ISR (new signature: no edge/level arg)
//   timerAttachInterrupt(My_timer, &onTimer);

//   // call ISR every 1,000,000 µs; autoreload=true; unlimited reloads (0)
//   timerAlarm(My_timer, 1000000, true, 0);
// }
// void loop() {
//   // nothing needed, all handled by interrupts
// }
#include <Arduino.h>

#define LED 4

hw_timer_t *My_timer = NULL;

void ARDUINO_ISR_ATTR onTimer() {
  digitalWrite(LED, !digitalRead(LED));
}

void setup() {
  pinMode(LED, OUTPUT);

  // timer 0, prescaler 80 (80 MHz / 80 = 1 MHz, 1 tick = 1 µs), count up
  My_timer = timerBegin(0, 80, true);

  // ISR attach karo (true = edge interrupt)
  timerAttachInterrupt(My_timer, &onTimer, true);

  // har 1,000,000 µs (1 sec) baad alarm, auto-reload = true
  timerAlarmWrite(My_timer, 1000000, true);

  // alarm enable karo
  timerAlarmEnable(My_timer);
}

void loop() {
  // sab kuch interrupt handle kar raha hai
}