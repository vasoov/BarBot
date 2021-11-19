#ifndef __SERVOOBJECT_H__
#define __SERVOOBJECT_H__

#include <Servo.h>

class ServoObject
{
  private:
   int    _pin;   //Servo is sttached this pin
   int    _dpress;  //Angle to rotate to press dispenser
   int    _drelease;  //Angle to rotate for release dispenser
   Servo  _servo; //Servo object

  public:
    ServoObject();
    ServoObject(int pin, int dpress, int drelease);
    void Press();
    void Release();
};


ServoObject::ServoObject()
{
  _pin, _dpress, _drelease = 0;
}

ServoObject::ServoObject(int pin, int dpress, int drelease)
{
  _pin = pin;
  _dpress = dpress;
  _drelease = drelease;
  
  _servo.write(_drelease);
  delay(15);
  _servo.attach(_pin);
  delay(1000);
  _servo.detach();  
}


void ServoObject::Press()
{
  _servo.write(_dpress); 
  delay(15);
  _servo.attach(_pin);
  delay(1000);
  _servo.detach();
}

void ServoObject::Release()
{
  _servo.write(_drelease); 
  delay(15);
  _servo.attach(_pin);
  delay(1000);
  _servo.detach();
}

#endif
