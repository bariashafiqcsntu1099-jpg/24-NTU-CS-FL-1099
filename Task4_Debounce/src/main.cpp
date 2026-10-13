// Week3-Lecture1
// Interrupt (External Button) with Debounce
// Embedded IoT System Fall-2026

// Name: Baria
// Reg#: 24-Ntu-Cs-Fl-1099

#include <Arduino.h>

const int buttonPin = 23;
const int ledPin = 4;

volatile bool ledState = LOW;

// Debounce Timer
hw_timer_t *debounceTimer = NULL;
volatile bool debounceActive = false;


// Timer Interrupt Service Routine
void IRAM_ATTR onDebounceTimer()
{
    debounceActive = false;
}


// Button Interrupt Service Routine
void IRAM_ATTR handleButton()
{
    // Ignore button press during debounce time
    if (debounceActive)
    {
        return;
    }

    // Start debounce
    debounceActive = true;

    // Toggle LED
    ledState = !ledState;
    digitalWrite(ledPin, ledState);

    // Reset timer
    timerWrite(debounceTimer, 0);

    // Start debounce timer
    timerAlarmEnable(debounceTimer);
}


void setup()
{
    // Button uses internal pull-up resistor
    pinMode(buttonPin, INPUT_PULLUP);

    // LED as output
    pinMode(ledPin, OUTPUT);

    // Create hardware timer
    // ESP32 timer clock = 80 MHz
    // Divider = 80
    // Result = 1 MHz
    debounceTimer = timerBegin(0, 80, true);

    // Attach timer interrupt
    timerAttachInterrupt(
        debounceTimer,
        &onDebounceTimer,
        true
    );

    // Set debounce time = 50 ms
    // 50,000 microseconds = 50 ms
    timerAlarmWrite(
        debounceTimer,
        50000,
        false
    );

    // Attach button interrupt
    attachInterrupt(
        digitalPinToInterrupt(buttonPin),
        handleButton,
        FALLING
    );
}


void loop()
{
    // Main loop is free for other tasks
}

// #include <Arduino.h>

// const int buttonPin = 23;
// const int ledPin = 4;
// bool ledOn = false;

// void setup() {
//   Serial.begin(9600);
//   pinMode(buttonPin, INPUT_PULLUP);
//   pinMode(ledPin, OUTPUT);
// }

// void loop() {
//   if (digitalRead(buttonPin) == LOW) {
//     Serial.println("Button: PRESSED");
//   } else {
//     Serial.println("Button: not pressed");
//   }
//   ledOn = !ledOn;
//   digitalWrite(ledPin, ledOn);
//   delay(500);
// }