/*
    User_Setup for ESP32-4827S043R - Display only

    For touch functionality use extern library!!!
*/

#define ILI9341_DRIVER
#define TFT_WIDTH 240
#define TFT_HEIGHT 320
#define TFT_MISO 12                     // Matching T_DO
#define TFT_MOSI 13                     // Matching T_DIN
#define TFT_SCLK 14                     // Matching T_CLK
#define TFT_CS 15                       // Chip select control pin
#define TFT_DC 2                        // Data Command control pin
#define TFT_RST -1                      // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
#define TFT_BL 21                       // LED back-light (only for ST7789 with backlight control pin)
#define TOUCH_CS -1                     // Chip select pin (T_CS) of touch screen
#define SPI_FREQUENCY 40000000          // Maximum to use SPIFFS
#define SPI_READ_FREQUENCY 16000000
// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY 1000000

#define USE_HSPI_PORT

#define TFT_BACKLIGHT_ON HIGH
#define LOAD_GLCD  
#define LOAD_FONT2 
#define LOAD_FONT4 
#define LOAD_FONT6 
#define LOAD_FONT7
#define LOAD_FONT8 
#define LOAD_GFXFF 
#define SMOOTH_FONT
