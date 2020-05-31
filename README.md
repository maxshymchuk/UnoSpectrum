# UnoSpectrum

Arduino-based sound spectrum visualizer

## Getting Started

1. Download [Arduino IDE](https://www.arduino.cc/en/Main/Software) (or use another arduino extension for your favourite IDE)
2. Follow this scheme to create a workable project

![Principal Scheme](../media/scheme.png?raw=true)

    - A1 = Arduino Uno ATMega328P
    - BM1 = KPCM-G97H45P-50DB-107В2 (4,5 V; 2,2 kOhm)
    - C1 = 0,1 uF
    - GB1 = 9 V
    - HG1 = OLED-0,96-4IIC-Y-128x64
    - R1 = 4,7 kOhm
    - R2 = R3 = 10 kOhm
    - R4 = 5,1 kOhm

3. To create custom image on startup use [this program](http://en.radzio.dxp.pl/bitmap_converter/), then paste generated code to _logo.h_ file