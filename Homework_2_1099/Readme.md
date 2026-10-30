Name: Baria Shafiq
Reg No:24_NTU_CS_FL_1099

In this homework I used an ESP32 to fade two LEDs with PWM, and each LED has its own PWM frequency. In the second part I added an SSD1306 OLED that shows the live duty cycle (in percent) and the PWM frequency of both LEDs. I built and tested everything in Wokwi first, and then on a real ESP32 kit.

Task 1: two LEDs fading from 0% to 100% and back, with different PWM frequencies.
Task 2:OLED added on top of Task 1 to show the duty cycle and frequency of each LED.


ESP32 DevKit board
2 LEDs (LED 1 red, LED 2 yellow)
2 resistors, one for each LED 
SSD1306  OLED display
Breadboard and jumper wires

The OLED I²C address is 0x3C. Each LED has a separate resistor, so one LED never shares its current limiting resistor with the other one. I avoided GPIO 6 to 11 (flash pins) and the boot pins.

I chose 8-bit resolution because it gives 256 brightness steps, which is enough for a smooth fade. It is also safe for both frequencies. The frequency multiplied by the number of levels must stay below the 80 MHz timer clock, and here it is only 5000 × 256 = 1.28 MHz.

For task1
Each LED has a global variable (duty_1 and duty_2).
A for loop increases the duty one step at a time from 0 to 255 (fade in), and another loop decreases it from 255 back to 0 (fade out).
Every step is sent to the LED with ledcWrite(), and then the program waits a short time (10 ms for LED 1 and 5 ms for LED 2). So the two LEDs have different fade speeds as well.

In my code the LEDs fade one after the other: first LED 1 does a full fade in and out, then LED 2 does its fade. While one LED is fading, the other one is at 0%.

The frequency tells how many on/off cycles happen in one second. LED 1 does 5000 cycles per second and LED 2 does 1000 cycles per second. With the same duty cycle, both LEDs have the same average brightness, 

The ESP32 LEDC peripheral has 16 channels and 8 timers.
This was a real limitation for my project. If I use channel 0 for LED 1 and channel 1 for LED 2, both LEDs end up on the same timer, and the frequency that is set last (1000 Hz) is used for both. To get two different frequencies, I used channel 0 for LED 1 and channel 2 for LED 2, so they are on two different timers.

Task2
I added the SSD1306 OLED .
The function updateOLED() clears the screen, writes three lines of text, and then calls display.display() to send everything to the screen. 
Without display.display() nothing would change on the real screen.

The OLED shows:
PWM MONITOR
LED1: 94% F:5000Hz
LED2: 0% F:1000Hz

Hardware implementation
I built the same circuit on a breadboard with the ESP32 board, two LEDs with resistors, and the OLED. 
Frequencies shown on the OLED: LED 1=5000 Hz, LED 2=1000 Hz.

Problems I faced and how I fixed them:
Same frequency on both LEDs: my first choice of channels (0 and 1) shares one timer. I changed LED 2 to channel 2.
OLED not working in Wokwi:I had connected SCL to GPIO 21 (same as SDA) and VCC to 5V by mistake. I fixed it by moving SCL to GPIO 22 and VCC to 3V3.
Compile errors while adding the OLED:I forgot the OLED libraries and the updateOLED() function. After adding them the code compiled.
