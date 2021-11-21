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

#ifndef __Limiter_H__
#define __Limiter_H__

class Limiter
{
  private:
    int _pin = 0;

  public:
    Limiter();
    void begin(int pin);
    int status();

};

Limiter::Limiter()
{
  _pin = 0;
}

void Limiter::begin(int pin)
{
  _pin = pin;
  pinMode(pin, INPUT); 
}

int Limiter::status()
{
  byte b = 1;

  //Check for keypress
  if ( b == digitalRead(_pin) ) 
  {    
    delay(1); 
  }

  return digitalRead(_pin); 
}

#endif
