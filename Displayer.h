/*
 * Copyright 2021 Vasoo Veerapen
 * vasoo.veerapen (at) gmail (dot) com
 * 
 * This file is part of BarBot
 * 
 * BarBot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __DISPLAYER_H__
#define __DISPLAYER_H__

#include <LiquidCrystal_I2C.h>

class Displayer
{
  private:
    LiquidCrystal_I2C* _lcd;

  public:
    Displayer();
    void begin(int dev, int cols, int rows);
    void write(String text);
};

Displayer::Displayer()
{
}

void Displayer::begin(int dev, int cols, int rows)
{
  _lcd = new LiquidCrystal_I2C(dev, cols, rows);
  _lcd->init();
  _lcd->backlight();
  _lcd->setCursor(0, 0);
}

void Displayer::write(String text)
{
    _lcd->clear();
    _lcd->print(text);
}

#endif
