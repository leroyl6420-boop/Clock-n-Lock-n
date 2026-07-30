//Servo is modified! 163 keeps it still because my SG90 Servo was modified while the encoder was at 163 degrees. Yours might be different!
//You can adapt an SG90 Servo motor by taking out the screws on its base, then clipping the part of the shaft that is feeding into
//the encoder. It should have a D-shape. If that is too difficult, you can alternatively cut the wires connecting the encoder to the control
//module, though that should be avoided due to being a waste of a perfectly good encoder. If you would not like to deal with the hassle of 
//finding the position your encoder was in when you disconected it, you may set your encoder to a specific angle prior to disabling it. 
//However, this is also discouraged, as it will require you to disable the encoder by cutting its wires. Cutting the shaft will cause the 
//encoder to shift. Your SG90 may also have a little mechanical catch. You may get rid of it however you like. 
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Stepper.h>
#include <U8g2lib.h>
#define WIDTH 128
#define HEIGHT 64
#define PR1 A0
#define PR2 A1
#define LED1 6
#define LED2 7
#define LED3 12
#define LED4 13
#define SERVO 5
#define OLED_RESET -1
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
int position = 0;
int hours = 0;
int seconds = 0;
int minutes = 0;
int time = 0;
bool count = false;
int pm = 0;
int ph = 0;
int md = 0;
int hd = 0;
int x = 0;
int xh = 0;
#define B1 3
#define B2 4
Servo myServo;
Adafruit_SSD1306 display1(WIDTH, HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(WIDTH, HEIGHT, &Wire, OLED_RESET);

void setup () {
    Serial.begin(9600);
    pinMode(PR1, INPUT);
    pinMode(PR2, INPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(B1, INPUT_PULLUP);
    pinMode(B2, INPUT_PULLUP);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    myServo.attach(SERVO);
    myStepper.setSpeed(5);
    if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed for Display 1"));
        for(;;);
    }
    if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
        Serial.println(F("SSD1306 allocation failed for Display 2"));
        for(;;);
    }
    myServo.write(163);
    display1.clearDisplay();
    display2.clearDisplay();
    display1.setTextColor(SSD1306_WHITE);
    display2.setTextColor(SSD1306_WHITE);
    display1.display();
    display2.display();
}    

void loop () {
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    int P1 = analogRead(PR1);
    int P2 = analogRead(PR2);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    
    if (position == 0){
        if (P2 < 990){
            do {
                myServo.write(150);
                P1 = analogRead(PR1);
                P2 = analogRead(PR2);
            } while (!(P1 < 990 && P2 < 990));
            do {
                myServo.write(150);
                P1 = analogRead(PR1);
                P2 = analogRead(PR2);
            } while (!(P1 > 990 || P2 > 990));
        }
        myServo.write(163);
        position = 1;
    }
    
    if (position == 9){
        if (hours >= 1){
            myStepper.step(2480);
            do {
                myServo.write(150);
                P1 = analogRead(PR1);
                P2 = analogRead(PR2);
            } while (!(P1 < 990 && P2 > 990));
            do {
                myServo.write(150);
                P1 = analogRead(PR1);
                P2 = analogRead(PR2);
            } while (!(P1 > 990 && P2 > 990));
            position ++;
            hours = 0;
            myServo.write(163);
        }
    }
    if (position >= 10){
        if (hours >= 1){
            myStepper.step(2480);
            unsigned long startTime = millis();
            while (millis() - startTime < 1000) {
                myServo.write(150);
            }
            myServo.write(163);
            seconds = 0;
            minutes = 0;
            time = 0;
            hours = 0;
            count = false;
            position = 0;
        }
    }

    if (hours >= 1 && position > 0 && position < 9){
            myStepper.step(2048);
            do {
                myServo.write(150);
                P1 = analogRead(PR1);
                P2 = analogRead(PR2);
            } while (!(P1 < 990 && P2 < 990));
            do {
                myServo.write(150);
                P1 = analogRead(PR1);
                P2 = analogRead(PR2);
            } while (!(P1 > 990 || P2 > 990));
            position ++;
            hours = 0;
            myServo.write(163);
    }
    if (digitalRead(B1) == 0 && position > 0){
        count = true;
        digitalWrite(LED3, HIGH);
    }
    if (digitalRead(B2) == 0 && count == true){
        count = false;
        time = 0;
        seconds = 0;
        minutes = 0;
        hours = 0;
        digitalWrite(LED4, HIGH);
    }
    if (count == true){
        time++;
    }
    if (time >= 100){
        seconds++;
        time = 0;
    }
    if (seconds >= 60){
        minutes++;
        seconds = 0;
    }
    if (minutes >= 60){
        hours++;
        minutes = 0;
    }

    pm = minutes / 3;
    md = pm * 6;
    x = md - 6;
    if (position == 0){
        ph = 0;
    } else {
        ph = position - 1;
    }
    hd = ph * 12;
    xh = hd - 12;
    display1.clearDisplay();
    display2.clearDisplay();
    if (pm < 11){
        display1.setTextSize(1);
        display1.setCursor(x, 0);
        display1.print(F("O"));
        do {
            x = x - 1;
            display1.setCursor(x, 0);
            display1.print(F("O"));
        } while (!(x <= 0));
    } else {
        display1.setTextSize(1);
        display2.setTextSize(1);
        display2.setCursor(x - 60, 0);
        display2.print(F("O"));
        do {
            x = x - 1;
            display2.setCursor(x - 60, 0);
            display2.print(F("O"));
        } while (!(x - 60 <= 0));
        x = 58;
        do {
            x = x - 1;
            display1.setCursor(x, 0);
            display1.print(F("O"));
        } while (!(x <= 0));
    }
    if (ph < 6){
        display1.setTextSize(2);
        display1.setCursor(xh, 48);
        display1.print(F("O"));
        do {
            xh = xh - 1;
            display1.setCursor(xh, 48);
            display1.print(F("O"));
        } while (!(xh <= 0));
    } else {
        display1.setTextSize(2);
        display2.setTextSize(2);
        display2.setCursor(xh - 60, 48);
        display2.print(F("O"));
        do {
            xh = xh - 1;
            display2.setCursor(xh, 48);
            display2.print(F("O"));
        } while (!(xh <= 0));
        xh = 52;
        do {
            xh = xh - 1;
            display1.setCursor(xh, 48);
            display1.print(F("O"));
        } while (!(xh <= 0));
    }
    display1.setTextSize(4);
    display1.setCursor(2 , 12);
    display1.print(ph);
    display1.setTextSize(3);
    display1.setCursor(26, 16);
    display1.print(pm);
    display2.setTextSize(5);
    display2.setCursor(7 , 11);
    display2.print(seconds);

    display1.display();
    display2.display();

    delay(10);
}
