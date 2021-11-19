#ifndef __LIMITSWITCHOBJECT_H__
#define __LIMITSWITCHOBJECT_H__

class LimitSwitchObject
{
  private:
    int _pin = 0;

  public:
    LimitSwitchObject();
    LimitSwitchObject(int pin);
    int status();

};

LimitSwitchObject::LimitSwitchObject()
{
  _pin = 0;
}

LimitSwitchObject::LimitSwitchObject(int pin)
{
  _pin = pin;
  pinMode(pin, INPUT); 
}

int LimitSwitchObject::status()
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
