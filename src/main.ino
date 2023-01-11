/*
  This sketch is the same as the Font_Demo_4 example, except the fonts in this
  example are in a FLASH (program memory) array. This means that processors
  such as the STM32 series that are not supported by a SPIFFS library can use
  smooth (anti-aliased) fonts.
*/

/*
  There are four different methods of plotting anti-aliased fonts to the screen.

  This sketch uses method 4, printing "String" or character array types only to screen,
  via a Sprite. The Sprite must NOT have been created already. The printToSprite()
  function automatically creates a sprite of a minimal size to contain the String,
  then plots to screen at the "tft" cursor position. Printing via a sprite draws the
  text faster on the screen. This method minimises flicker but uses RAM for the Sprite,
  the Sprite is automatically deleted after plotting to the TFT.

  Number and float types must be converted to strings to use printToSprite() e.g.:
  spr.printToSprite( (String) number );
  spr.printToSprite( (String) (number * 55 / 1.23) );  // Put calculations within brackets

  The key advantage of this method is that you do not need to calculate the size of sprite
  needed to contain the text, the library does that for you. The library also fills the
  the sprite with text background colour for you.

  printToSprite() has a second purpose, if the sprite has been created already the String
  will be printed into the Sprite at the "sprite" cursor position, which is
  different to the "tft" cursor position. In this case the Sprite is not deleted and
  you must use pushSprite() to plot on the screen. This method is not used in this sketch.
  because in general it is better to use drawString() in an already created sprite.
  printToSprite() will NOT move the tft cursor.
*/

// A processing sketch to create new fonts can be found in the Tools folder of TFT_eSPI
// https://github.com/Bodmer/TFT_eSPI/tree/master/Tools/Create_Smooth_Font/Create_font

// This sketch uses font files created from the Noto family of fonts:
// https://www.google.com/get/noto/

#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#include <OneWire.h>
#include <DallasTemperature.h>
// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
int DS18S20_Pin = D2; // DS18S20 Signal pin on digital 2

// Temperature chip i/o
OneWire oneWire(DS18S20_Pin); // on digital pin 2
DallasTemperature ds(&oneWire);
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft); // Sprite class needs to be invoked
float TemperatureSum = 0;

void setup(void)
{

    Serial.begin(9600);

    tft.begin();

    tft.setRotation(1);

    spr.setColorDepth(16); // 16 bit colour needed to show anti-aliased fonts

      // Start up the library
    ds.begin();
}

void loop()
{

    tft.fillScreen(TFT_BLACK);
    spr.loadFont(AA_FONT_LARGE);            // Load another different font
    tft.setTextColor(TFT_GREEN, TFT_BLACK); // Set the font colour and the background colour
    // Draw changing numbers - no flicker using this plot method!

    for (int i = 0; i <= 9999; i++)
    {
        //float temperature = getTemp();
        ds.requestTemperatures(); // Send the command to get temperatures
        float temperature = ds.getTempCByIndex(0);
        tft.setCursor(30, 25);
        // Number is converted to String type by (String) (number)
        spr.printToSprite(" " + (String)(temperature) + " "); // Space padding helps over-write old numbers
        delay(100);
    }
    spr.unloadFont(); // Remove the font to recover memory used

    delay(1000);
}




/*float getTemp()
{
    // returns the temperature from one DS18S20 in DEG Celsius

    byte data[12];
    byte addr[8];

    if (!ds.search(addr))
    {
        // no more sensors on chain, reset search
        ds.reset_search();
        return -1000;
    }

    if (OneWire::crc8(addr, 7) != addr[7])
    {
        Serial.println("CRC is not valid!");
        return -1000;
    }

    if (addr[0] != 0x10 && addr[0] != 0x28)
    {
        Serial.print("Device is not recognized");
        return -1000;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1); // start conversion, with parasite power on at the end

    byte present = ds.reset();
    ds.select(addr);
    ds.write(0xBE); // Read Scratchpad

    for (int i = 0; i < 9; i++)
    { // we need 9 bytes
        data[i] = ds.read();
    }

    ds.reset_search();

    byte MSB = data[1];
    byte LSB = data[0];

    float tempRead = ((MSB << 8) | LSB); // using two's compliment
    float TemperatureSum = tempRead / 16;

    return TemperatureSum;
}*/



// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);