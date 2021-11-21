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

#ifndef __DISPENSER_H__
#define __DISPENSER_H__

#include <Servo.h>

class Dispenser : public Servo
{
  private:
   int    _pin;   //Servo is sttached this pin
   int    _dpress;  //Angle to rotate to press dispenser
   int    _drelease;  //Angle to rotate for release dispenser
   int    _dpdelay;
   int    _drdelay;
   Servo  _servo; //Servo object
   void   Press();
   void   Release();

  public:
    Dispenser();
    void begin(int pin, int dpress, int drelease, int dpdelay, int drdelay);
    void Dispense(int shots);
};


Dispenser::Dispenser()
{
  _pin, _dpress, _drelease, _dpdelay, _drdelay = 0;
}

void Dispenser::begin(int pin, int dpress, int drelease, int dpdelay, int drdelay)
{
  _pin = pin;
  _dpress = dpress;
  _drelease = drelease;
  _dpdelay = dpdelay;
  _drdelay = drdelay;
  
  _servo.write(_drelease);
  delay(15);
  _servo.attach(_pin);
  delay(1000);
  _servo.detach();  
}


void Dispenser::Press()
{
  _servo.write(_dpress); 
  delay(15);
  _servo.attach(_pin);
  delay(1000);
  _servo.detach();
}

void Dispenser::Release()
{
  _servo.write(_drelease); 
  delay(15);
  _servo.attach(_pin);
  delay(1000);
  _servo.detach();
}

void Dispenser::Dispense(int shots)
{
  int s=0;

  for(s=0; s< shots; s++)
  {
    this->Press();
    delay(_dpdelay);
    this->Release();
    delay(_drdelay);
  }
}



#endif
