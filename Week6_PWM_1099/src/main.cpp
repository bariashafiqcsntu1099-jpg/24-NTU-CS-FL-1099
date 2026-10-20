//Baria Shafiq
//24_NTU_CS_FL_1099

// Arduino ke bunyadi functions (delay, Serial, ledc wagera) ke liye
#include <Arduino.h>

// LED ka pin: GPIO 2 (esp32dev board ki apni built-in LED isi par hoti hai)
constexpr uint8_t LED_PIN = 2;

// PWM channel number (0 se 15 mein se koi). Core 2 mein channel alag dena parta hai
constexpr uint8_t PWM_CHANNEL = 0;

// PWM frequency: 5000 Hz yaani ek second mein 5000 baar ON/OFF ka chakkar
// (itni tez ke aankh ko flicker nazar nahi aata)
constexpr uint32_t PWM_FREQUENCY = 5000;

// Resolution: 8 bit yaani duty ke 256 darje (0 se 255)
constexpr uint8_t PWM_RESOLUTION = 8;

// Sab se zyada duty: 255 (100% roshni)
constexpr uint16_t PWM_MAX_DUTY = 255;

void setup()
{
    // Serial Monitor shuru karo (115200 speed), taake messages dekh sakein
    Serial.begin(115200);

    // Channel ki frequency aur resolution set karo.
    // ledcSetup kamyab ho to asal frequency wapas deta hai, fail ho to 0.
    // Is liye "!= 0" se true/false mil jata hai
    bool attached = ledcSetup(
        PWM_CHANNEL,        // kaun sa channel
        PWM_FREQUENCY,      // kitni frequency
        PWM_RESOLUTION      // kitni resolution
    ) != 0;                 // 0 nahi aaya to kamyab

    // Agar setup fail hua
    if (!attached)
    {
        // Error ka message Serial Monitor par dikhao
        Serial.println("ERROR: PWM attachment failed.");

        // Yahin hamesha ruk jao (har 1 second baad dobara intezar)
        while (true) { delay(1000); }
    }

    // LED ke pin ko is PWM channel se jodo.
    // Ab channel ka signal is pin par aayega
    ledcAttachPin(LED_PIN, PWM_CHANNEL);

    // Shuru mein duty 0 rakho yaani LED band
    ledcWrite(PWM_CHANNEL, 0);

    // Message dikhao ke program shuru ho gaya
    Serial.println("ESP32 PWM LED Fade Started");
}

void loop()
{
    // FADE IN (LED dheere dheere roshan hogi):
    // duty 0 se shuru ho kar 255 tak 1-1 barhti hai
    for (int duty = 0; duty <= PWM_MAX_DUTY; duty++)
    {
        // Is duty ko channel par lagao (duty barhne se LED tez hoti hai)
        ledcWrite(PWM_CHANNEL, duty);

        // 10 millisecond ruko, taake aankh ko farq nazar aaye
        delay(10);
    }

    // FADE OUT (LED dheere dheere band hogi):
    // duty 255 se ghat kar 0 tak 1-1 kam hoti hai
    for (int duty = PWM_MAX_DUTY; duty >= 0; duty--)
    {
        // Duty kam hone se LED dheemi hoti jati hai
        ledcWrite(PWM_CHANNEL, duty);

        // 10 millisecond ruko
        delay(10);
    }

    // loop() khatam hote hi dobara shuru ho jata hai, is liye fade chalta rehta hai
}