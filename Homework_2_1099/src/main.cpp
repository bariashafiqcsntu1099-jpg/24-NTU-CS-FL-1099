//Baria Shafiq 
//24_NTU_CS_FL_1099
// Arduino ke bunyadi functions (delay, Serial, ledc wagera) ke liye
#include <Arduino.h>
// [ADDED #8] OLED ke liye 3 libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// [CHANGED #1] LED 1 (red) -> GPIO 19, LED 2 (yellow) -> GPIO 18 (diagram.json ke mutabiq)
constexpr uint8_t LED_PIN_1 = 19;
constexpr uint8_t LED_PIN_2 = 18;

// PWM channel number (0 se 15 mein se koi). Core 2 mein channel alag dena parta hai
constexpr uint8_t PWM_CHANNEL_1 = 0;
// [CHANGED #2] 1 ki jagah 2, kyunke channel 0 aur 1 ek timer share karte hain (frequency same ho jati)
constexpr uint8_t PWM_CHANNEL_2 = 2;


// PWM frequency: 5000 Hz yaani ek second mein 5000 baar ON/OFF ka chakkar
// (itni tez ke aankh ko flicker nazar nahi aata)
constexpr uint32_t PWM_FREQUENCY_1 = 5000;
constexpr uint32_t PWM_FREQUENCY_2 = 1000;

// Resolution: 8 bit yaani duty ke 256 darje (0 se 255)
constexpr uint8_t PWM_RESOLUTION = 8;

// Sab se zyada duty: 255 (100% roshni)
constexpr uint16_t PWM_MAX_DUTY = 255;

// [ADDED #3] Duty ab global hai (pehle loop ke andar thi), taake Task 2 mein OLED par dikha sakein
int duty_1 = 0;
int duty_2 = 0;

// [ADDED #9] OLED ki settings: screen 128 x 64, address 0x3C
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// [ADDED #10] OLED par dono LEDs ki duty % aur frequency likhta hai
void updateOLED()
{
    // Purani screen saaf karo
    display.clearDisplay();

    display.setCursor(0, 10);
    display.println("PWM MONITOR");

    // LED 1: duty 0 se 255 hoti hai, 255 = 100%, is liye duty * 100 / 255 se percent milta hai
    display.setCursor(1, 25);
    display.print("LED1: ");
    display.print(duty_1 * 100 / PWM_MAX_DUTY);
    display.print("% F:");
    display.print(PWM_FREQUENCY_1);
    display.println("Hz");

    // LED 2: bilkul wahi
    display.setCursor(1, 40);
    display.print("LED2: ");
    display.print(duty_2 * 100 / PWM_MAX_DUTY);
    display.print("% F:");
    display.print(PWM_FREQUENCY_2);
    display.println("Hz");

    // Ab sab kuch asli screen par bhejo (iske baghair screen nahi badalti)
    display.display();
}

void setup()
{
    // Serial Monitor shuru karo (115200 speed), taake messages dekh sakein
    Serial.begin(115200);

    // Channel ki frequency aur resolution set karo.
    // ledcSetup kamyab ho to asal frequency wapas deta hai, fail ho to 0.
    // Is liye "!= 0" se true/false mil jata hai
    bool attached_1 = ledcSetup(
        PWM_CHANNEL_1,        // kaun sa channel
        PWM_FREQUENCY_1,      // kitni frequency
        PWM_RESOLUTION      // kitni resolution
    ) != 0;  
        // Agar setup fail hua
    if (!attached_1)
    {
        // [CHANGED #4] message mein LED ka number likha
        Serial.println("ERROR: PWM attachment failed for LED 1.");

        // Yahin hamesha ruk jao (har 1 second baad dobara intezar)
        while (true) { delay(1000); }
    }               // 0 nahi aaya to kamyab
    bool attached_2 = ledcSetup(
        PWM_CHANNEL_2,        // kaun sa channel
        PWM_FREQUENCY_2,      // kitni frequency
        PWM_RESOLUTION      // kitni resolution
    ) != 0;                 // 0 nahi aaya to kamyab

     if (!attached_2)
    {
        // [CHANGED #4] message mein LED ka number likha
        Serial.println("ERROR: PWM attachment failed for LED 2.");

        // Yahin hamesha ruk jao (har 1 second baad dobara intezar)
        while (true) { delay(1000); }
    }

    // LED ke pin ko is PWM channel se jodo.
    // Ab channel ka signal is pin par aayega
    ledcAttachPin(LED_PIN_1, PWM_CHANNEL_1);
    // Shuru mein duty 0 rakho yaani LED band
    ledcWrite(PWM_CHANNEL_1, 0);
    // For led_2
    ledcAttachPin(LED_PIN_2, PWM_CHANNEL_2);
    ledcWrite(PWM_CHANNEL_2, 0);
    // [REMOVED #5] ledcAttachPin aur "Started" message do do dafa tha, ek ek rakha
    // [REMOVED #12] yahan "Started" message dobara tha, neeche wala rakha hai
    Wire.begin(21, 22);
    Wire.setClock(400000);                     // I2C tez karo taake OLED jaldi update ho
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
    {
        Serial.println("ERROR: OLED not found.");
        while (true) { delay(1000); }
    }
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    updateOLED();                              // pehli screen foran dikhao

    Serial.println("ESP32 PWM LED Fade Started");
}

void loop_1()
{
    // FADE IN (LED dheere dheere roshan hogi):
    // duty 0 se shuru ho kar 255 tak 1-1 barhti hai
    // [CHANGED #6] "int duty" ki jagah global duty_1 (baaki loop aap ka hi hai)
    for (duty_1 = 0; duty_1 <= PWM_MAX_DUTY; duty_1++)
    {
        // Is duty ko channel par lagao (duty barhne se LED tez hoti hai)
        ledcWrite(PWM_CHANNEL_1, duty_1);

        // [ADDED #11] har 5 step baad OLED par nayi duty dikhao
        if (duty_1 % 5 == 0) { updateOLED(); }

        // 10 millisecond ruko, taake aankh ko farq nazar aaye
        delay(10);
    }

    // FADE OUT (LED dheere dheere band hogi):
    // duty 255 se ghat kar 0 tak 1-1 kam hoti hai
    for (duty_1 = PWM_MAX_DUTY; duty_1 >= 0; duty_1--)
    {
        // Duty kam hone se LED dheemi hoti jati hai
        ledcWrite(PWM_CHANNEL_1, duty_1);

        // [ADDED #11] har 5 step baad OLED update
        if (duty_1 % 5 == 0) { updateOLED(); }

        // 10 millisecond ruko
        delay(10);
    }
    duty_1 = 0;   // [ADDED #6] loop khatam hone par duty_1 -1 reh jati hai, isay 0 kar diya
}


//For led2
void loop_2()
{
    // FADE IN (LED dheere dheere roshan hogi):
    // duty 0 se shuru ho kar 255 tak 1-1 barhti hai
    // [CHANGED #6] "int duty" ki jagah global duty_2
    for (duty_2 = 0; duty_2 <= PWM_MAX_DUTY; duty_2++)
    {
        // Is duty ko channel par lagao (duty barhne se LED tez hoti hai)
        ledcWrite(PWM_CHANNEL_2, duty_2);

        // [ADDED #11] har 5 step baad OLED update
        if (duty_2 % 5 == 0) { updateOLED(); }

        // 5 millisecond ruko, taake aankh ko farq nazar aaye
        delay(5);
    }

    // FADE OUT (LED dheere dheere band hogi):
    // duty 255 se ghat kar 0 tak 1-1 kam hoti hai
    for (duty_2 = PWM_MAX_DUTY; duty_2 >= 0; duty_2--)
    {
        // Duty kam hone se LED dheemi hoti jati hai
        ledcWrite(PWM_CHANNEL_2, duty_2);

        // [ADDED #11] har 5 step baad OLED update
        if (duty_2 % 5 == 0) { updateOLED(); }

        // 5 millisecond ruko
        delay(5);
    }
    duty_2 = 0;   // [ADDED #6] duty_2 ko 0 kar diya
}

// [ADDED #7] Arduino ko loop() chahiye. Pehle LED 1 fade hogi, phir LED 2
void loop()
{
    loop_1();
    loop_2();
}