#ifndef __BED_H__
#define __BED_H__

#include "LimitSwitchObject.h"

#define LIMITL 22
#define LIMITR 10

class BEDObject
{
  private:
    int x;      //Steps to make 
 
  public:

    int stp;
    int dir; 
    int ms1; 
    int ms2; 
    int ms3; 
    int en;

  
    BEDObject();
    BEDObject(int _stp, int _dir, int _ms1, int _ms2, int _ms3, int _en);
    LimitSwitchObject *LimitSwitch[2];  

    void resetpins();
    void setpin(int pin, int state);
    void DefaultStepBackHome();    
    void DefaultStepForward(int steps);
    void DefaultStepReverse(int steps);
    void SmallStepForward(int steps);
    void SmallStepReverse(int steps);
    
};

BEDObject::BEDObject()
{

}

BEDObject::BEDObject(int _stp, int _dir, int _ms1, int _ms2, int _ms3, int _en)
{
  stp = _stp;
  dir = _dir; 
  ms1 = _ms1; 
  ms2 = _ms2; 
  ms3 = _ms3; 
  en = _en; 

  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(ms1, OUTPUT);
  pinMode(ms2, OUTPUT);
  pinMode(ms3, OUTPUT);
  pinMode(en, OUTPUT);

  //Setup limit switches
  //limitswitch[0] = LimitSwitchObject(LIMITL);
  LimitSwitch[0] = new LimitSwitchObject(LIMITR);
  
  resetpins();
}

void BEDObject::resetpins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(ms1, LOW);
  digitalWrite(ms2, LOW);
  digitalWrite(ms3, LOW);
  digitalWrite(en, HIGH);
}


void BEDObject::setpin(int pin, int state)
{
  if(pin == stp) {
    digitalWrite(stp, state);
  }
  else if (pin == dir) {
    digitalWrite(dir, state);
  }
  else if (pin == ms1) {
    digitalWrite(ms1, state);
  }
  else if(pin == ms2) {
    digitalWrite(ms2, state);
  }
  else if (pin == ms3) {
    digitalWrite(ms3, state);
  }
  else if (pin == en) {
    digitalWrite(en, state);
  }
  else {
    //DEBUG ERROR PRINT UNKNOW PIN      
  }
}



void BEDObject::DefaultStepForward(int steps)
{
  setpin(en, LOW);
  setpin(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<steps; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    setpin(stp,HIGH); //Trigger one step forward
    delay(1);
    setpin(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }

  resetpins();
}


//Move tray to home position
void BEDObject::DefaultStepBackHome()
{
  setpin(en, LOW);
  setpin(dir, HIGH); //Pull direction pin high to move in "reverse"
  
  do
  {
    setpin(stp,HIGH); 
    delay(1);
    setpin(stp,LOW); 
    delay(1);
  } while (LimitSwitch[0]->status() == LOW);

  resetpins();
}


void BEDObject::DefaultStepReverse(int steps)
{
  setpin(en, LOW);
  setpin(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 0; x<steps; x++)
  {

    if(LimitSwitch[0]->status() == HIGH)
    {
      break;
    }
    else
    {
      setpin(stp,HIGH); //Trigger one step
      delay(1);
      setpin(stp,LOW); //Pull step pin low so it can be triggered again
      delay(1);
    }
  }

  resetpins();
}

void BEDObject::SmallStepForward(int steps)
{
  setpin(en, LOW);
  setpin(dir, LOW); //Pull direction pin low to move "forward"
  setpin(ms1, HIGH); //Pull MS1,MS2, and MS3 high to set logic to 1/16th microstep resolution
  setpin(ms2, HIGH);
  setpin(ms3, HIGH);
  for(x= 0; x<steps; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    setpin(stp,HIGH); //Trigger one step forward
    delay(1);
    setpin(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }

  resetpins();
}

void BEDObject::SmallStepReverse(int steps)
{
  setpin(en, LOW);
  setpin(dir, HIGH); //Pull direction pin low to move "forward"
  setpin(ms1, HIGH); //Pull MS1,MS2, and MS3 high to set logic to 1/16th microstep resolution
  setpin(ms2, HIGH);
  setpin(ms3, HIGH);
  for(x= 0; x<steps; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    setpin(stp,HIGH); //Trigger one step forward
    delay(1);
    setpin(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }

  resetpins();  
}

#endif
