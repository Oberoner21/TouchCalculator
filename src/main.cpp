/*
  TouchCalculator V 1.0
  Touchscreen Calculator auf Sunton ESP32-2432S028R

  Oberoner21, Version 1.0 vom 27.07.2024
*/

// ----------------------------
// Standard Libraries
// ----------------------------
#include <Arduino.h>
#include <SPI.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <XPT2046_Touchscreen.h>
// A library for interfacing with the touch screen
//
//https://github.com/PaulStoffregen/XPT2046_Touchscreen

#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
//
//https://github.com/Bodmer/TFT_eSPI

#include "7seg.h"
#include "orbitron.h"
#include "Button_eSPI.h"

// ----------------------------
// Touch Screen pins
// ----------------------------

// The CYD touch uses some non default SPI pins

const uint8_t XPT2046_IRQ = 36;
const uint8_t XPT2046_MOSI = 32;
const uint8_t XPT2046_MISO = 39;
const uint8_t XPT2046_CLK = 25;
const uint8_t XPT2046_CS = 33;

// Constants for touch calibration
const uint16_t TS_MINX = 200;
const uint16_t TS_MINY = 240;
const uint16_t TS_MAXX = 3850;
const uint16_t TS_MAXY = 3700;

// ----------------------------

// Objects
SPIClass mySpi = SPIClass(VSPI);                  // SPI for touch with non default SPI pins
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();

// PWM constants
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;
const uint32_t pwmDuty = 100;     // Set brightness, max 255

// Display constants
const uint16_t screenWidth = tft.getViewportWidth();
const uint16_t screenHeight = tft.getViewportHeight();

// Colors
const uint16_t TFT_GREY = 0x5AEB; 
const uint16_t DARKBLUE = 0x22AE;         // Fill color calculators result area
const uint16_t C1 = 0x02CD;               // Unpressed button fill color for number chars
const uint16_t C2 = 0xE280;               // Pressed button fill color for all buttons
const uint16_t C3 = 0x6005;               // Unpresses button fill color for operater chars
const uint16_t BACK_COLOR = TFT_WHITE;    // Calculators background color 

// Calculator
const uint8_t marginX = 12;
const uint8_t buttonWidth = 45;
const uint8_t buttonHeight = 45; 
const uint8_t bnRowsCount = 4;
const uint8_t bnColsCount = 4;

char buttonLabels[bnRowsCount][bnColsCount]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'0','.','=','+'}};
short buttonColors[bnRowsCount][bnColsCount]={{C1,C1,C1,C3},{C1,C1,C1,C3},{C1,C1,C1,C3},{C1,C3,C3,C3}};
Button_eSPI *buttons[bnRowsCount * bnColsCount] = {0};

uint8_t curOperation = 0;   // 0 = keine, 1 +, 2 -, 3 *, 4 /
float n1 = 0;
String num = "0";           // Calculators result string


void draw()
{
  tft.setFreeFont(&DSEG7_Classic_Bold_24);
  tft.setTextColor(TFT_WHITE, DARKBLUE);
  tft.fillRoundRect(marginX, 30, screenWidth - 2 * marginX, 40, 7, DARKBLUE);
  tft.setCursor(screenWidth - marginX - 5 - tft.textWidth(num), 62);
  tft.print(num);
}

void initDraw()
{
  tft.fillRect(screenWidth - 53, 8, 8, 15, 0x52AA);
  tft.fillRect(screenWidth - 43, 8, 8, 15, 0x52AA);
  tft.fillRect(screenWidth - 33, 8, 8, 15, 0x52AA);
  tft.fillRect(screenWidth - 23, 8, 8, 15, 0x52AA);
  
  tft.setTextColor(TFT_BLACK, BACK_COLOR);
  tft.setFreeFont(&Orbitron_Bold_18);
  tft.setCursor(marginX + 3, 22);
  tft.print("CASIO");
  tft.setFreeFont(&FreeMonoBold9pt7b);
  tft.drawString("CALCULATOR", marginX + 3, 72, 2);
  tft.fillRoundRect(marginX, 30, screenWidth - 2 * marginX, 40, 7, DARKBLUE);

  // Button field
  const uint8_t fromTop = 96;
  uint8_t bnCount = 0;
  uint16_t posX, posY;
  char labelBuff[2];
  labelBuff[1] = 0;
  for(uint8_t i=0; i<bnRowsCount; i++)
  {
      posY = fromTop + (buttonHeight * i)+(marginX * i);
      for(int j =0; j<bnColsCount; j++)
      { 
          buttons[bnCount] = new Button_eSPI();
          posX = marginX + (buttonWidth * j) + (marginX * j);
          labelBuff[0] = buttonLabels[i][j];
          buttons[bnCount]->initButtonUL(&tft, posX, posY, buttonWidth, buttonHeight, BACK_COLOR, buttonColors[i][j], TFT_WHITE, labelBuff, &FreeMonoBold12pt7b);
          buttons[bnCount]->setLabelDatum(-1, 2, MC_DATUM);
          buttons[bnCount]->drawButton();
          bnCount++;
      }
  }
}

void keyHandler(char key) 
{

  float r;
  int p;

  switch(key) 
  {
    case '+':
      curOperation = 1;
      n1 = num.toFloat();
      num = "";
      break;
    case '-':
      curOperation = 2;
      n1 = num.toFloat();
      num = "";
      break;
    case '*':
      curOperation = 3;
      n1 = num.toFloat();
      num = "";
      break;
    case '/':
      curOperation = 4;
      n1 = num.toFloat();
      num = "";
      break;
    case '=':
      switch(curOperation) 
      {
        case 1:
          r = n1 + num.toFloat(); 
          break;
        case 2:
          r = n1 - num.toFloat(); 
          break;
        case 3:
          r = n1 * num.toFloat(); 
          break;
        case 4:
          r = n1 / num.toFloat(); 
          break;
      }
      num = String(r); 
      n1 = num.toFloat();
      p = r * 10.00;
      if(p % 10 == 0)
      num = String(p / 10);
      break;
    default:
      if(num.equals("0")) num = "";
      num = num + key;
  }

  draw();
}


void checkTouched(TS_Point p) 
{
  uint8_t row, col;
  uint16_t px = map(p.x, TS_MINX, TS_MAXX, 0, screenWidth);
  uint16_t py = map(p.y, TS_MINY, TS_MAXY, 0, screenHeight);

  for(uint8_t i=0; i<16; i++)
  {
    if(buttons[i]->contains(px, py))
    {
      if(!buttons[i]->isPressed()) 
      {
        buttons[i]->press(true);
        buttons[i]->drawButton(true);
        col = i % bnColsCount;
        row = i / bnRowsCount;
        Serial.print("pressed: ");
        Serial.println(buttonLabels[row][col]);
        keyHandler(buttonLabels[row][col]);
        break;
      }
    }
    else 
    {
      if(buttons[i]->isPressed()) 
      {
        buttons[i]->press(false);
        buttons[i]->drawButton(false);
        Serial.println("not pressed");
        break;
      }
    }
  }

  // for( auto & button : buttonArray) 
  // {
  //     if(px > button.xStart && 
  //        px < button.xStart + button.width &&
  //        py > button.yStart && 
  //        py < button.yStart + button.height) 
  //     {
  //        tft.drawString(String)
  //     }

  // }
}


void setup() 
{
  Serial.begin(115200);
  while(!Serial);

  // Start the SPI for the touch screen and init the TS library
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(0);

  // Initialize the TFT display
  tft.init();
  tft.setRotation(0); 
  tft.fillScreen(BACK_COLOR);

  // Initialize PWM brightness
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, pwmDuty);

  // Draw the calculator screen
  initDraw();
  // Draw the init result 0
  draw();
}


TS_Point nullPoint = TS_Point();
void loop() 
{

  if (ts.tirqTouched() && ts.touched()) {
     TS_Point p = ts.getPoint();
     checkTouched(p);
     delay(100);
  }
  else 
  {
    checkTouched(nullPoint);
  }
}