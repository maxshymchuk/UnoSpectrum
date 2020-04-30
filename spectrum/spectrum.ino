#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define VOL_THR 35
#define LOW_PASS 30
#define DEF_GAIN 80

#define AUDIO_IN 0
#define OLED_RESET -1

#define LOG_OUT 1
#define FHT_N 256

#define printByte(args) write(args);
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#include <FHT.h>
#include "nano_gfx.h"
#include "ssd1306.h"

#include ".\logo.h"

byte gain = DEF_GAIN;
unsigned long gainTimer;
byte maxValue, maxValue_f;
float k = 0.1;

void setup() {
  Serial.begin(9600);

  // https://acdc.foxylab.com/node/49
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  analogReference(EXTERNAL);  
  
  ssd1306_128x64_i2c_init();
  ssd1306_drawBitmap(0, 0, Logo::width, Logo::height, Logo::bsuir);
  delay(500);
}

void analyzeAudio() {
  for (int i = 0; i < FHT_N; i++) {
    int sample = analogRead(AUDIO_IN);
    fht_input[i] = sample; // put real data into bins
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
}

void loop() {
  uint8_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];
  NanoCanvas canvas(SCREEN_WIDTH, SCREEN_HEIGHT, buffer);

  while (1) {
    analyzeAudio();
    canvas.clear();
    for (int curBin = 0; curBin < FHT_N / 2; curBin++) {
      const int value = fht_log_out[curBin];
    
      int posLevel = map(value, LOW_PASS, gain, 0, SCREEN_HEIGHT - 1);
      posLevel = constrain(posLevel, 0, SCREEN_HEIGHT - 1);
      
  //      Serial.print(value);
  //      Serial.print(" ");
      canvas.drawVLine(curBin, SCREEN_HEIGHT - posLevel, SCREEN_HEIGHT);
  
      if (value > maxValue) maxValue = value;
      maxValue_f = maxValue * k + maxValue_f * (1 - k);
      if (millis() - gainTimer > 1500) {      // каждые 1500 мс
        // если максимальное значение больше порога, взять его как максимум для отображения
        if (maxValue_f > VOL_THR) {
          gain = maxValue_f;
        } else {
          // если нет, то взять порог побольше, чтобы шумы вообще не проходили
          gain = 100;
        }
        gainTimer = millis();
      }
    }
    canvas.blt(0, 0);
    //Serial.println("");
  }
}
