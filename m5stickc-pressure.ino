#include <Arduino.h>
#include <M5StickC.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#include "lcd.h"

namespace
{
    constexpr uint8_t sda = 0;
    constexpr uint8_t scl = 26;
    constexpr uint8_t bmp280_address = 0x76;
    constexpr uint8_t font_7seg = 7;
    constexpr uint8_t font_16px = 2;

    constexpr Vector2<int32_t> pressure_pos = Vector2<int32_t>(132, 40);
    constexpr Vector2<int32_t> unit_pos = Vector2<int32_t>(156, 64);
}

Adafruit_BMP280 bmp280;

void setup()
{
    M5.begin();

    LCD::SetRotation(LCDRotation::LandscapeLeft);
    LCD::FillScreen(Color16::Black());

    Wire.begin(sda, scl);

    if (!bmp280.begin(bmp280_address))
    {
        LCD::DrawString("BMP280 Not Found", Vector2<int32_t>::Zero());

        while (true)
        {

        }
    }

    LCD::SetTextFont(font_16px);
    LCD::SetTextDatum(TextDatum::BottomRight);
    LCD::DrawString("hPa", unit_pos);
}

void loop()
{
    M5.update();

    const uint16_t pressure_hpa = static_cast<uint16_t>(bmp280.readPressure() / 100);
    String str;

    if (pressure_hpa < 1000)
    {
        str = "0" + String(pressure_hpa, DEC);
    }
    else
    {
        str = String(pressure_hpa, DEC);
    }

    LCD::SetTextFont(font_7seg);
    LCD::SetTextDatum(TextDatum::MiddleRight);
    LCD::DrawString(str, pressure_pos);

    delay(100);
}