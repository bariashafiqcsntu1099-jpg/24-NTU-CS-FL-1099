//Baria Shafiq
//24_NTU_CS_FL_1099
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C
#define DHT_PIN 19
#define DHT_TYPE DHT22

// Bar graph ki range: temperature 0 se 50 C, humidity 0 se 100 %
#define TEMP_MIN 0.0
#define TEMP_MAX 50.0
#define HUM_MIN 0.0
#define HUM_MAX 100.0

Adafruit_SSD1306 display(
    SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1
);
DHT dht(DHT_PIN, DHT_TYPE);

// Ek horizontal bar banata hai.
// y = bar ki upar wali position, value = reading, minVal/maxVal = range
void drawBar(int y, float value, float minVal, float maxVal)
{
    // Value ko range ke andar rakho
    value = constrain(value, minVal, maxVal);

    // Bar ki lambai: andar ki chaudai 124 pixel hai (128 minus 2+2 border)
    int fillWidth = (int)((value - minVal) / (maxVal - minVal) * 124);

    // Bahar ka dibba (khali frame)
    display.drawRect(0, y, SCREEN_WIDTH, 10, SSD1306_WHITE);

    // Andar ka bhara hua hissa
    display.fillRect(2, y + 2, fillWidth, 6, SSD1306_WHITE);
}

void setup() {
    Serial.begin(115200);
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED initialization failed!");
        while (true) { delay(100); }
    }

    dht.begin();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("ESP32 Environment");
    display.println("Initializing...");
    display.display();

    delay(2000);
}

void loop() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("DHT read failed!");

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Sensor Error");
        display.println("Check DHT sensor");
        display.display();

        delay(2000);
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(humidity, 1);
    Serial.print(" %   Temperature: ");
    Serial.print(temperature, 1);
    Serial.println(" C");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Title
    display.setCursor(0, 0);
    display.println("ENVIRONMENT MONITOR");
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    // Humidity: pehle likhawat, neeche uska bar
    display.setCursor(0, 14);
    display.print("Hum: ");
    display.print(humidity, 1);
    display.println(" %");
    drawBar(24, humidity, HUM_MIN, HUM_MAX);

    // Temperature: pehle likhawat, neeche uska bar
    display.setCursor(0, 40);
    display.print("Temp: ");
    display.print(temperature, 1);
    display.println(" C");
    drawBar(50, temperature, TEMP_MIN, TEMP_MAX);

    display.display();
    delay(2000);
}