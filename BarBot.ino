/*
 * Copyright 2021 Vasoo Veerapen
 * vasoo.veerapen (at) gmail (dot) com
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

#include "Displayer.h"
#include "Controller.h"
#include "Dispenser.h"

//Arduino pin usage
//Big Easy Driver Pins
const int STP_PIN = 2;
const int DIR_PIN = 3;
const int MS1_PIN = 4;
const int MS2_PIN = 5;
const int MS3_PIN = 6;
const int EN_PIN  = 7;

//Servo pin
const int SERVO_PIN = 8;

//Servo parameters
const int SERVO_RELEASE_ANGLE = 130;
const int RELEASE_DELAY = 2500;
const int SERVO_PRESS_ANGLE = 80;
const int PRESS_DELAY = 2500;

//Bottle parameters
const int BOTTLE_OFFSET = 900;
const int BOTTLE_SPACING = 666;
const int NUM_BOTTLES = 6;

//LCD display parameters
const int LCD_DEVICE = 0x27;
const int LCD_ROWS = 4;
const int LCD_COLUMNS = 20;

const int KEYBOARD_MODE = 0;
const int BLUETOOTH_MODE = 1;

//Declare starting & ending markers for serial data packets
const char SMARKER = '<';
const char EMARKER = '>';

Dispenser   *dispenser;    //Servo motor controls
Displayer   *displayer;    // LCD display
Controller  *controller;   // Limit switch & stepper motor controls

//For Bluetooth communication
bool started = false;
bool ended = false;
char buf[80];
byte index;

//Declare variables for functions
char user_input;
int mode = KEYBOARD_MODE; //Set mode to read from Keyboard or from Bluetooth ( 0 = KB)

void setup() {

  randomSeed(analogRead(0));

  Serial.begin(9600); //Open Serial connection 
  Serial1.begin(9600);  //Open Bluetooth connection

  controller = new Controller();
  controller->begin(STP_PIN, DIR_PIN, MS1_PIN, MS2_PIN, MS3_PIN, EN_PIN);
  
  displayer = new Displayer();
  displayer->begin(LCD_DEVICE, LCD_COLUMNS, LCD_ROWS);
  
  dispenser = new Dispenser();
  dispenser->begin(SERVO_PIN, SERVO_PRESS_ANGLE, SERVO_RELEASE_ANGLE, PRESS_DELAY, RELEASE_DELAY);

  Serial.println();
  //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("0. Reset controller.");
  Serial.println("1. Calibrate default steps per mm.");
  Serial.println("2. Return tray to start.");
  Serial.println("3. Move to all bottles, dispense, return to start.");
  Serial.println("4. Press and release spirit dispenser.");
  Serial.println("5. Display limit switch status.");
  Serial.println("6. Move to random bottle, dispense, return to start.");
  Serial.println("7. Switch to bluetooth mode.");
  Serial.println();

  displayer->write("Ready.");
}

void loop()
{
  
  if (mode == KEYBOARD_MODE)
  {
    KeyboardMode();
  }
  else
  {
    BluetoothMode();
  }

}

//Reset Arduino
void(* resetFunc) (void) = 0; 

//Option 0.
void Reset()
{
  resetFunc();
}

//Option 1.
void Calibrate()
{
  Serial.println("Calibrating at default step mode.");
  controller->home();
  delay(500);
  controller->forward(1000);

}

//Option 2.
void Return()
{
  Serial.println("Moving back to starting position at default step mode.");
  controller->home();
}


//Option 3.
void Cycle()
{
  Serial.println("Performing a dry run.");
  int x;

  for(x= 0; x<NUM_BOTTLES; x++)
  {
    if(x==0)
    {
      controller->forward(BOTTLE_OFFSET);
    }
    else
    {
      controller->forward(BOTTLE_SPACING);
    }
    dispenser->Dispense(1);
  }

  delay(500);

  for(x= NUM_BOTTLES; x>0; x--)
  {
    dispenser->Dispense(1);
    
    if(x==1)
    {
      controller->reverse(BOTTLE_OFFSET);
    }
    else
    {
      controller->reverse(BOTTLE_SPACING);
    } 
  }

}


//Option 5.
void Status()
{
  Serial.println("Status of limit switch is :");
  Serial.println(controller->limiter[0]->status());
}

//Option 6.
void Select(int bottle)
{
  if(bottle==0)
  {
    controller->forward(BOTTLE_OFFSET);
  }
  else
  {
    controller->forward(BOTTLE_OFFSET + (BOTTLE_SPACING * bottle));
  }
}

//Option 7.
void Mode(int option)
{
  if(option == 0)
  {
    Serial.println("Switching to Keyboard mode");
    mode = KEYBOARD_MODE;
  }
  else
  {
     Serial.println("Switching to Bluetooth mode");
     mode = BLUETOOTH_MODE;
  }
}

//Keyboard mode
void KeyboardMode()
{
  while(Serial.available())
  {
      user_input = Serial.read(); //Read user input and trigger appropriate function
      if (user_input =='0')
      {
        displayer->write("Option 0 selected");
        Reset();
      }
      else if (user_input =='1')
      {
        displayer->write("Option 1 selected");
         Calibrate();
      }
      else if(user_input =='2')
      {
        displayer->write("Option 2 selected");
        Return();
      }
      else if(user_input =='3')
      {
        displayer->write("Option 3 selected");
        Cycle();
      }
      else if(user_input =='4')
      {
        displayer->write("Option 4 selected");
        dispenser->Dispense(1);
      }
      else if(user_input =='5')
      {
        displayer->write("Option 5 selected");
        Status();
      }
      else if(user_input =='6')
      {
         displayer->write("Option 6 selected");
         Select(random(0, NUM_BOTTLES));
         dispenser->Dispense(random(1,3));
         Return();
      }
      else if(user_input =='7')
      {
        displayer->write("Option 7 selected");
        Mode(BLUETOOTH_MODE);
      }      
      else
      {
        Serial.println("Invalid option entered.");
      }

      Serial.println("Enter new option");
      Serial.println();

      displayer->write("Enter new option.   ");

  } 
}

//Bluetooth mode
void BluetoothMode() {

  while(Serial1.available() > 0)
  {
    char inChar = Serial1.read();
    if(inChar == SMARKER)
    {
       index = 0;
       buf[index] = '\0';
       started = true;
       ended = false;
    }
    else if(inChar == EMARKER)
    {
       ended = true;
       break;
    }
    else
    {
      if(index < 79)
      {
        buf[index] = inChar;
        index++;
        buf[index] = '\0';
      }
    }
  }

  if(started && ended)
  {
    char *name = strtok(buf, "=");
    while(name)
    {
      char *valToken = strtok(NULL, ",");
      if(valToken)
      {
        int val = atoi(valToken);

        if(strcmp(name, "RESET") == 0)
        {
          Reset();
        }
        else if(strcmp(name, "CALIBRATE") == 0)
        {
          Calibrate();
        }
        else if(strcmp(name, "RETURN") == 0)
        {
          Return();
        }
        else if(strcmp(name, "CYCLE") == 0)
        {
          Cycle();       
        }
        else if(strcmp(name, "DISPENSE") == 0)
        {
          dispenser->Dispense(val);
        }
        else if(strcmp(name, "STATUS") == 0)
        {
          Status();
        }
        else if(strcmp(name, "SELECT") == 0)
        {
          Select(val);
        }
        else if(strcmp(name, "KEYBOARD") == 0)
        {
          Mode(KEYBOARD_MODE);
        }
        else
        {
          Serial.println("Unknown command.");
        }         
      }
      name = strtok(NULL, "=");
    }
    
    // Reset for the next packet
    started = false;
    ended = false;
    index = 0;
    buf[index] = '\0';

  }
    
}
