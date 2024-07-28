/***************************************************************************************
** Code for the GFX button UI element
** Grabbed from Adafruit_GFX library and enhanced to handle any label font
***************************************************************************************/
#include "Button_eSPI.h"

Button_eSPI::Button_eSPI(void)
{
  _gfx = nullptr;
  _xd = 0;
  _yd = 0;
  _textdatum = MC_DATUM;
  _label[9] = '\0';
  currstate = false;
  laststate = false;
}

// Classic initButton() function: pass center & size
void Button_eSPI::initButton(
    TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h,
    uint16_t outline, uint16_t fill, uint16_t pressedFill, uint16_t textcolor,
    char *label, const GFXfont *textfont)
{
  // Tweak arguments and pass to the newer initButtonUL() function...
  initButtonUL(gfx, x - (w / 2), y - (h / 2), w, h, outline, fill, pressedFill,
               textcolor, label, textfont);
}

// Newer function instead accepts upper-left corner & size
void Button_eSPI::initButtonUL(
    TFT_eSPI *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h,
    uint16_t outline, uint16_t fill, uint16_t pressedFill, uint16_t textcolor,
    char *label, const GFXfont *textfont)
{
  _x1 = x1;
  _y1 = y1;
  _w = w;
  _h = h;
  _outlinecolor = outline;
  _fillcolor = fill;
  _pressedFillColor = pressedFill;
  _textcolor = textcolor;
  _textfont = textfont;
  _gfx = gfx;
  strncpy(_label, label, 9);
}

// Adjust text datum and x, y deltas
void Button_eSPI::setLabelDatum(int16_t x_delta, int16_t y_delta, uint8_t datum)
{
  _xd = x_delta;
  _yd = y_delta;
  _textdatum = datum;
}

void Button_eSPI::drawButton(bool inverted, String long_name)
{
  uint16_t fill, outline, text;

  if (!inverted)
  {
    if (isPressed())
    {
      fill = _pressedFillColor;
    }
    else
    {
      fill = _fillcolor;
    }
    text = _textcolor;
  }
  else
  {
    fill = _textcolor;
    text = _fillcolor;
  }
  outline = _outlinecolor;

  uint8_t r = min(_w, _h) / 4; // Corner radius
  _gfx->fillRoundRect(_x1, _y1, _w, _h, r, fill);
  _gfx->drawRoundRect(_x1, _y1, _w, _h, r, outline);

  if (_gfx->textfont == 255)
  {
    _gfx->setCursor(_x1 + (_w / 8),
                    _y1 + (_h / 4));
    _gfx->setTextColor(text);
    //_gfx->setTextSize(_textsize);
    _gfx->setFreeFont(_textfont);
    _gfx->print(_label);
  }
  else
  {
    _gfx->setTextColor(text, fill);
    //_gfx->setTextSize(_textsize);
    _gfx->setFreeFont(_textfont);

    uint8_t tempdatum = _gfx->getTextDatum();
    _gfx->setTextDatum(_textdatum);
    uint16_t tempPadding = _gfx->getTextPadding();
    _gfx->setTextPadding(0);

    if (long_name == "")
      _gfx->drawString(_label, _x1 + (_w / 2) + _xd, _y1 + (_h / 2) - 4 + _yd);
    else
      _gfx->drawString(long_name, _x1 + (_w / 2) + _xd, _y1 + (_h / 2) - 4 + _yd);

    _gfx->setTextDatum(tempdatum);
    _gfx->setTextPadding(tempPadding);
  }
}

bool Button_eSPI::contains(int16_t x, int16_t y)
{
  return ((x >= _x1) && (x < (_x1 + _w)) &&
          (y >= _y1) && (y < (_y1 + _h)));
}

void Button_eSPI::press(bool p)
{
  laststate = currstate;
  currstate = p;
}

bool Button_eSPI::isPressed() { return currstate; }
bool Button_eSPI::justPressed() { return (currstate && !laststate); }
bool Button_eSPI::justReleased() { return (!currstate && laststate); }
