/**
 * @file      TFT_eSPI_Sprite.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-01-23
 * @note      Use TFT_eSPI Sprite made by framebuffer , unnecessary calling during use tft.xxxx function
 */


#include "esp_arduino_version.h"
#include "frames.h"
#if ESP_ARDUINO_VERSION < ESP_ARDUINO_VERSION_VAL(3,0,0)

#include <TFT_eSPI.h>   //https://github.com/Bodmer/TFT_eSPI
#include <LilyGo_RGBPanel.h>
#include <LV_Helper.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);
LilyGo_RGBPanel panel;

#define WIDTH  panel.width()
#define HEIGHT panel.height()

void setup()
{
    Serial.begin(115200);


    // Use TFT_eSPI Sprite made by framebuffer , unnecessary calling during use tft.xxxx function
    // Use TFT_eSPI sprites requires exchanging the color order
    if (!panel.begin(LILYGO_T_RGB_ORDER_BGR)) {
        while (1) {
            Serial.println("Error, failed to initialize T-RGB"); delay(1000);
        }
    }

    spr.createSprite(WIDTH, HEIGHT);

    spr.setColorDepth(16);

    spr.setSwapBytes(1);

    panel.setBrightness(16);

}

#if 0
uint16_t rgb(uint16_t r, uint16_t g, uint16_t b) {
    r = (r * 32 / 256) & 0x1f;
    b = (b * 64 / 256) & 0x3f;
    g = (g * 32 / 256) & 0x1f;

    return (r) + (b << 5) + (g << 11);
}
#else
uint16_t rgb(uint16_t r, uint16_t g, uint16_t b) {
    r &= 0x1f;
    b &= 0x3f;
    g &= 0x1f;

    return (r) + (b << 5) + (g << 11);
}
#endif

uint16_t rgb2(uint16_t r, uint16_t g, uint16_t b) {
    uint16_t val = rgb(r, g, b);
    return (val >> 8) | (val << 8);
}

void loop()
{
    /*
    int x = WIDTH / 2;
    spr.setTextDatum(MC_DATUM);

    spr.fillSprite(TFT_RED);
    spr.setTextColor(TFT_WHITE, TFT_RED);
    spr.drawString("Red Color", x, HEIGHT / 2, 4);
    panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    delay(2000);

    spr.fillSprite(TFT_GREEN);
    spr.setTextColor(TFT_WHITE, TFT_GREEN);
    spr.drawString("Green Color", x, HEIGHT / 2, 4);
    panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    delay(2000);

    spr.fillSprite(TFT_BLUE);
    spr.setTextColor(TFT_WHITE, TFT_BLUE);
    spr.drawString("Blue Color", x, HEIGHT / 2, 4);
    panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    delay(2000);

    spr.fillSprite(TFT_BLACK);
    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.drawString("Black Color", x - 20, HEIGHT / 2, 4);
    panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    delay(2000);

    uint16_t colors[6] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_MAGENTA};
    for (int i = 0; i < 6; ++i) {
        spr.fillSprite(TFT_BLACK);
        spr.setTextColor(colors[i], TFT_BLACK);
        spr.drawString("LilyGo.cc", WIDTH / 2, HEIGHT / 2, 4);
        spr.drawString("T-RGB", WIDTH / 2, HEIGHT / 2 + 30, 4);
        panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
        delay(200);
    }
    delay(2000);


    for (int pos = WIDTH; pos > 0; pos--) {
        int h = HEIGHT;
        while (h--) spr.drawFastHLine(0, h, WIDTH, rainbow(h * 4));
        spr.setTextSize(1);
        spr.setTextFont(4);
        spr.setTextColor(TFT_WHITE);
        spr.setTextWrap(false);
        spr.setCursor(pos, 100);
        spr.print("LilyGo LilyGo LilyGo");
        panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    }
    delay(2000);
    //*/

    memset(spr.getPointer(), 0, 2 * WIDTH * HEIGHT);
    panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());

    uint16_t *test = (uint16_t *)spr.getPointer();
    uint32_t w = 480;
    uint32_t h = 480;
    for (uint32_t y = 0; y < h; y++) {
        for (uint32_t x = w; x > 0; x--) {
            uint32_t idx = x - 1 + y * w;
            uint32_t x2 = x * 256 / w;
            uint32_t y2 = y * 256 / h;
            //test[idx] = rgb(x2, y2, 0);
            //spr.drawPixel(x, y, rgb2(x2, y2, 0));
            spr.drawPixel(x, y, rgb2(x, y, 0));
        }
        if (y % 2 == 0)
            panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    }
    panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    delay(2000);
    for (uint32_t j = 0; j < 50; j++) {
        for (uint32_t i = 0; i < numberOfFrames; i++) {
            memcpy(spr.getPointer(),frames[i],sizeof(frames[i]));
            panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
            delay(35);
        }
    }
    /*
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int idx = x + y * WIDTH;
            int r = 31;
            int b = 0;
            test[idx] = r + (b << 5);
        }
        panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    }
    delay(2000);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int idx = x + y * WIDTH;
            int r = 16;
            int b = 0;
            test[idx] = r + (b << 5);
        }
        panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    }
    delay(2000);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int idx = x + y * WIDTH;
            int r = 0;
            int b =31;
            test[idx] = r + (b << 5);
        }
        panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    }
    delay(2000);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int idx = x + y * WIDTH;
            int r = 0;
            int b = 16;
            test[idx] = r + (b << 5);
        }
        panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    }
    delay(2000);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int idx = x + y * WIDTH;
            test[idx] = (63 << 10);
        }
        panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    }
    delay(2000);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int idx = x + y * WIDTH;
            test[idx] = 32 << 10;
        }
        panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    }
    delay(2000);

    for (int i = 0; i < 16; i++) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int idx = x + y * WIDTH;
                test[idx] = 1 << i;
            }
        }
    }
    panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    delay(2000);
    //*/

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int idx = x + y * WIDTH;
            int i = y * 16 / HEIGHT;
            test[idx] = 1 << i;
        }
    }
    panel.pushColors(0, 0, WIDTH, HEIGHT, (uint16_t *)spr.getPointer());
    delay(10000);
}





// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int rainbow(uint8_t value)
{
    // Value is expected to be in range 0-127
    // The value is converted to a spectrum colour from 0 = red through to 127 = blue

    uint8_t red   = 0; // Red is the top 5 bits of a 16 bit colour value
    uint8_t green = 0;// Green is the middle 6 bits
    uint8_t blue  = 0; // Blue is the bottom 5 bits

    uint8_t sector = value >> 5;
    uint8_t amplit = value & 0x1F;

    switch (sector) {
    case 0:
        red   = 0x1F;
        green = amplit;
        blue  = 0;
        break;
    case 1:
        red   = 0x1F - amplit;
        green = 0x1F;
        blue  = 0;
        break;
    case 2:
        red   = 0;
        green = 0x1F;
        blue  = amplit;
        break;
    case 3:
        red   = 0;
        green = 0x1F - amplit;
        blue  = 0x1F;
        break;
    }

    return red << 11 | green << 6 | blue;
}

#else

#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    Serial.println("The current arduino version of TFT_eSPI does not support arduino 3.0, please change the version to below 3.0");
    delay(1000);
}

#endif