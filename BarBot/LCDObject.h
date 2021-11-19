#ifndef __LCDOBJECT_H__
#define __LCDOBJECT_H__

#include <LiquidCrystal_I2C.h>

class LCDObject
{
  private:
    LiquidCrystal_I2C* _lcd;

  public:
    LCDObject();
    LCDObject(int dev, int cols, int rows);
    void write(String text);
};

LCDObject::LCDObject()
{
}

LCDObject::LCDObject(int dev, int cols, int rows)
{
  _lcd = new LiquidCrystal_I2C(dev, cols, rows);
  _lcd->init();
  _lcd->backlight();
  _lcd->setCursor(0, 0);
}

void LCDObject::write(String text)
{
    _lcd->clear();
    _lcd->print(text);
}

#endif
