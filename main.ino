/*
   Пример использования библиотеки GyverButton, все возможности в одном скетче.
*/
//led strip settings
#define STRIP_PIN 6 //led pin
#define NUMLEDS 12 //leds count
#define COLOR_DEBTH 3
#include <microLED.h>   // lib connect
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

#define PIN 2				// кнопка подключена сюда (PIN --- КНОПКА --- GND)

#include "GyverButton.h"
GButton butt1(PIN);
// GButton butt1(PIN, HIGH_PULL, NORM_OPEN); // можно инициализировать так

int value = 0;

void setup() {
  Serial.begin(9600);

  strip.setBrightness(150);
  strip.clear();
  strip.show(); // вывод изменений на ленту

  butt1.setDebounce(10);        // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(10);        // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setClickTimeout(100);   // настройка таймаута между кликами (по умолчанию 300 мс)

  butt1.setType(HIGH_PULL);
  butt1.setDirection(NORM_OPEN);
}

void loop() {
  butt1.tick();  
  if (butt1.isHold()||butt1.isClick()){
     //  Serial.println("Holding");        // проверка на удержание
      runningDots();      
      
  // Serial.println("Holded");  
   }    else   {
     breathing();
     m4Shprot();
   }

}
void m4Shprot() {
  strip.fill(mMagenta);
  strip.show();         // выводим изменения
  delay(30);

}

void rainbow() {
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++) {
    strip.set(i, mWheel8(counter + i * 255 / NUMLEDS));   // counter смещает цвет
  }
  counter += 3;   // counter имеет тип byte и при достижении 255 сбросится в 0
  strip.show();
  delay(20);
}

void runningDots() {
  strip.setBrightness(255);
  static byte counter = 0;
  // перемотка буфера со сдвигом (иллюзия движения пикселей)
  for (int i = 0; i < NUMLEDS - 1; i++) strip.leds[i] = strip.leds[i + 1];

  // каждый третий вызов - последний пиксель красным, иначе чёрным
  if (counter % 3 == 0) strip.leds[NUMLEDS - 2] = mRed;
  else strip.leds[NUMLEDS - 2] = mBlack;
  counter++;
  delay(40);  
  strip.show();

  
}
void breathing() {
  static int dir = 1;
  static int bright = 0;
  bright += dir * 4;    // 5 - множитель скорости изменения

  if (bright > 255) {
    bright = 255;
    dir = -1;
  }
  if (bright < 0) {
    bright = 0;
    dir = 1;
  }
  strip.setBrightness(bright);
}
