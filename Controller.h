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

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Limiter.h"

#define LIMITL 22
#define LIMITR 10

class Controller
{
  private:
    int _x;      //Steps to make 

    int _stp;
    int _dir; 
    int _ms1; 
    int _ms2; 
    int _ms3; 
    int _en;

    void resetpins();
    void setpin(int pin, int state);
 
  public:

    Controller();
    void begin(int _stp, int _dir, int _ms1, int _ms2, int _ms3, int _en);
    Limiter *limiter[2];  

    void home();    
    void forward(int steps);
    void reverse(int steps);
    
};

Controller::Controller()
{

}

void Controller::begin(int stp, int dir, int ms1, int ms2, int ms3, int en)
{
  _stp = stp;
  _dir = dir; 
  _ms1 = ms1; 
  _ms2 = ms2; 
  _ms3 = ms3; 
  _en = en; 

  pinMode(_stp, OUTPUT);
  pinMode(_dir, OUTPUT);
  pinMode(_ms1, OUTPUT);
  pinMode(_ms2, OUTPUT);
  pinMode(_ms3, OUTPUT);
  pinMode(_en, OUTPUT);

  //Setup limit switches
  limiter[0] = new Limiter();
  limiter[0]->begin(LIMITR);
  
  resetpins();
}

void Controller::resetpins()
{
  digitalWrite(_stp, LOW);
  digitalWrite(_dir, LOW);
  digitalWrite(_ms1, LOW);
  digitalWrite(_ms2, LOW);
  digitalWrite(_ms3, LOW);
  digitalWrite(_en, HIGH);
}


void Controller::setpin(int pin, int state)
{
  if(pin == _stp) {
    digitalWrite(_stp, state);
  }
  else if (pin == _dir) {
    digitalWrite(_dir, state);
  }
  else if (pin == _ms1) {
    digitalWrite(_ms1, state);
  }
  else if(pin == _ms2) {
    digitalWrite(_ms2, state);
  }
  else if (pin == _ms3) {
    digitalWrite(_ms3, state);
  }
  else if (pin == _en) {
    digitalWrite(_en, state);
  }
  else {
    //DEBUG ERROR PRINT UNKNOW PIN      
  }
}



void Controller::forward(int steps)
{
  setpin(_en, LOW);
  setpin(_dir, LOW); 
  for(_x= 0; _x<steps; _x++)  
  {
    setpin(_stp,HIGH); 
    delay(1);
    setpin(_stp,LOW); 
    delay(1);
  }

  resetpins();
}


//Move tray to home position
void Controller::home()
{
  setpin(_en, LOW);
  setpin(_dir, HIGH); 
  
  do
  {
    setpin(_stp,HIGH); 
    delay(1);
    setpin(_stp,LOW); 
    delay(1);
  } while (limiter[0]->status() == LOW);

  resetpins();
}


void Controller::reverse(int steps)
{
  setpin(_en, LOW);
  setpin(_dir, HIGH); 
  for(_x= 0; _x<steps; _x++)
  {

    if(limiter[0]->status() == HIGH)
    {
      break;
    }
    else
    {
      setpin(_stp,HIGH); 
      delay(1);
      setpin(_stp,LOW); 
      delay(1);
    }
  }

  resetpins();
}

#endif
