/*
@file    EVE_HelloWorld.cpp
@brief   Main file for PlatformIO/Arduino EVE HelloWorld
@version 1.2
@date    2024-06-23
@author  Rudolph Riedel
*/

#include <Arduino.h>
#include <SPI.h>
#include "EVE.h"
#include "colores.h"
#include "imagenes_test.h"
#include "tft.h"

void setup()
{
    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);

#if defined (ESP32)
    #if defined (EVE_USE_ESP_IDF)
        /* not using the Arduino SPI class in order to use DMA */
        EVE_init_spi();
    #else
        /* using the Arduino SPI class to be compatible with other devices */
        SPI.begin(EVE_SCK, EVE_MISO, EVE_MOSI);
    #endif
#elif defined (ARDUINO_NUCLEO_F446RE) || defined (WIZIOPICO) || defined (PICOPI)
    /* not using the Arduino SPI class in order to use DMA */
    EVE_init_spi();
#else
    SPI.begin(); /* sets up the SPI to run in Mode 0 and 1 MHz */
    /* switch to 8MHz, note, init must be done with <11MHz */
    SPI.beginTransaction(SPISettings(8UL * 1000000UL, MSBFIRST, SPI_MODE0));
#endif
        TFT_init();
        EVE_memWrite8(REG_ROTATE, 1); /* bit0 = invert, bit2 = portrait, bit3 = mirrored */

}

void loop()
{
    uint32_t current_millis;
    static uint32_t previous_millis = 0;
    static uint8_t display_delay = 0;
    uint32_t micros_start, micros_end;
    current_millis = millis();

    if ((current_millis - previous_millis) > 4) /* execute the code every 5 milli-seconds */
    {
        previous_millis = current_millis;
        micros_start = micros();

        display_delay++;
        if (display_delay > 4) /* refresh the display every 25ms / 40 per second */
        {
            display_delay = 0;
            TFT_display();
            micros_end = micros();
            num_profile_a = (micros_end - micros_start); /* calculate the micro-seconds passed during the call to TFT_display */
        }
        else
        {
            TFT_touch();
            micros_end = micros();
            num_profile_b = (micros_end - micros_start); /* calculate the micro-seconds passed during the call to TFT_touch */
        }
    }
}
