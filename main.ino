//led strip settings
#define STRIP_R 2 //led pin
#define STRIP_L 3 //led pin

#define NUMLEDS 11 //leds count
#define COLOR_DEBTH 3
#include <microLED.h>   // lib connect
microLED<NUMLEDS, STRIP_R, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip_R;
microLED<NUMLEDS, STRIP_L, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip_L;

#define PIN_STOP 6				// кнопка подключена сюда (PIN --- КНОПКА --- GND)
#define PIN_TURN_LEFT 5
#define PIN_TURN_RIGHT 4

#include "GyverButton.h"
GButton butt1(PIN_STOP);
GButton butt2 (PIN_TURN_LEFT);
// GButton butt1(PIN, HIGH_PULL, NORM_OPEN); // можно инициализировать так

int value = 0;
float brake_input = 0.0;
float temp_V = 0.0;

void setup() {
  Serial.begin(9600);

  strip_R.setBrightness(250);
  strip_R.clear();
  strip_R.show(); // вывод изменений на ленту

  butt1.setDebounce(10);        // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(10);        // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setClickTimeout(100);   // настройка таймаута между кликами (по умолчанию 300 мс)

  butt1.setType(HIGH_PULL);
  butt1.setDirection(NORM_OPEN);

  
  butt2.setDebounce(80);        // настройка антидребезга (по умолчанию 80 мс)
  butt2.setTimeout(500);        // настройка таймаута на удержание (по умолчанию 500 мс)
  butt2.setClickTimeout(300);   // настройка таймаута между кликами (по умолчанию 300 мс)

  butt2.setType(HIGH_PULL);
  butt2.setDirection(NORM_OPEN);
}

void loop() {
  butt1.tick();  
  butt2.tick();  

  bool pressed = false;
  int analogvalue = analogRead(A0);
  temp_V = (analogvalue * 5.0) / 1024.0;
 if (butt2.isHold()) {
     //  Serial.println("Holding");        // проверка на удержание
      pressed = true;
           turnSignal_R();

  // Serial.println("Holded");  
    } else if (butt1.isHold()||butt1.isClick()|| temp_V > 1.0){
     //rainbow();
      //     runningDots();      
           turnSignal_L();    

    //turnRight();
   }
  
    else  {

      rainbow();
        strip_R.setBrightness(255);
    strip_L.setBrightness(255);

    // breathing();
     //m4Shprot();
   }

}
void m4Shprot() {
  strip_R.fill(mMagenta);
  strip_R.show();         // выводим изменения

delay(30);
  strip_L.fill(mMagenta);
  strip_L.show();
  delay(30);

}

void rainbow() {
  //strip.begin();
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++) {
    strip_R.set(i, mWheel8(counter + i * 255 / NUMLEDS));   // counter смещает цвет
        strip_L.set(i, mWheel8(counter + i * 255 / NUMLEDS));   // counter смещает цвет

  }
  counter += 2;   // counter имеет тип byte и при достижении 255 сбросится в 0
  strip_R.show();
    delay(15);

  strip_L.show();

  delay(15);
}

void runningDots() {
  strip_R.setBrightness(255);
    strip_L.setBrightness(255);

  static byte counter = 0;
  // перемотка буфера со сдвигом (иллюзия движения пикселей)
  for (int i = 0; i < NUMLEDS - 1; i++) strip_R.leds[i] = strip_R.leds[i + 1];
  for (int i = 0; i < NUMLEDS - 1; i++) strip_L.leds[i] = strip_L.leds[i + 1];

  // каждый третий вызов - последний пиксель красным, иначе чёрным
  if (counter % 3 == 0) strip_R.leds[NUMLEDS - 2] = mRed;
    else strip_R.leds[NUMLEDS - 2] = mBlack;

    if (counter % 3 == 0) strip_L.leds[NUMLEDS - 2] = mRed;

  else strip_L.leds[NUMLEDS - 2] = mBlack;

  counter++;
  delay(40);  
  strip_R.show();
    counter++;
  delay(40); 
  strip_L.show();

}

void breathing() {
  static int dir = 1;
  static int bright = 0;
  bright += dir * 2;    // 5 - множитель скорости изменения

  if (bright > 255) {
    bright = 255;
    dir = -1;
  }
  if (bright < 70) {
    bright = 70;
    dir = 1;
  }
  strip_R.setBrightness(bright);
    strip_L.setBrightness(bright);

}

void turnSignal_L() {
  for (int j = 0; j < 2; j++) {  // 3 раза зажигаем и гасим светодиоды
    // включаем светодиоды
    strip_L.setBrightness(255);
    for (int i = 0; i < NUMLEDS +1; i++) {       //count_led = количество светодиодов
      strip_L.leds[NUMLEDS - i] = mOrange;      //последовательно , от 0 до 11 зажигаем i-й светодиод желтым цветом
      delay(25);          //задержка между зажиганием светодиода
      strip_L.show(); //горим на ленте
    }
    for (int i = 0; i < NUMLEDS+1; i++) {   //послдовательно "гасим" светодиоды
      strip_L.leds[NUMLEDS - i] = mBlack;
      strip_L.show();
      delay(25);
    }
  }
}
void turnSignal_R() {
  for (int j = 0; j < 2; j++) {  // 3 раза зажигаем и гасим светодиоды
    // включаем светодиоды
    strip_R.setBrightness(255);
    for (int i = 0; i < NUMLEDS +1; i++) {       //count_led = количество светодиодов
      strip_R.leds[NUMLEDS - i] = mOrange;      //последовательно , от 0 до 11 зажигаем i-й светодиод желтым цветом
      delay(25);          //задержка между зажиганием светодиода
      strip_R.show(); //горим на ленте
    }
    for (int i = 0; i < NUMLEDS+1; i++) {   //послдовательно "гасим" светодиоды
      strip_R.leds[NUMLEDS - i] = mBlack;
      strip_R.show();
      delay(25);
    }
  }
}
