#include "LCDObject.h"
#include "BEDObject.h"
#include "ServoObject.h"

//Arduino pin usage
const int STP_PIN = 2;
const int DIR_PIN = 3;
const int MS1_PIN = 4;
const int MS2_PIN = 5;
const int MS3_PIN = 6;
const int EN_PIN  = 7;
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

ServoObject       *servo;
LCDObject         *lcd;       // LCD display
BEDObject         *stepper;   // Stepper motor driver controls ()

//For Bluetooth communication
bool started = false;
bool ended = false;
char buf[80];
byte index;

//Declare variables for functions
char user_input;
int mode = KEYBOARD_MODE; //Set mode to read from Keyboard or from Bluetooth ( 0 = KB)

void setup() {

  Serial.begin(9600); //Open Serial connection 
  Serial1.begin(9600);  //Open Bluetooth connection

  randomSeed(analogRead(0));
  
  stepper = new BEDObject(STP_PIN, DIR_PIN, MS1_PIN, MS2_PIN, MS3_PIN, EN_PIN);
  servo = new ServoObject(SERVO_PIN, SERVO_PRESS_ANGLE, SERVO_RELEASE_ANGLE);
  lcd = new LCDObject(LCD_DEVICE, LCD_COLUMNS, LCD_ROWS);

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

  lcd->write("Ready.");
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
  stepper->DefaultStepBackHome();
  delay(500);
  stepper->DefaultStepForward(1000);

}

//Option 2.
void Return()
{
  Serial.println("Moving back to starting position at default step mode.");
  stepper->DefaultStepBackHome();
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
      stepper->DefaultStepForward(BOTTLE_OFFSET);
    }
    else
    {
      stepper->DefaultStepForward(BOTTLE_SPACING);
    }
    Dispense(1); 
  }

  delay(500);

  for(x= NUM_BOTTLES; x>0; x--)
  {
    Dispense(1);
    
    if(x==1)
    {
      stepper->DefaultStepReverse(BOTTLE_OFFSET);
    }
    else
    {
      stepper->DefaultStepReverse(BOTTLE_SPACING);
    } 
  }

}

//Option 4.
void Dispense(int shots)
{
  int s=0;
  Serial.println("Dispensing.");

  for(s=0; s< shots; s++)
  {
    servo->Press();
    delay(PRESS_DELAY);
    servo->Release();
    delay(RELEASE_DELAY);
  }
}

//Option 5.
void Status()
{
  Serial.println("Status of limit switch is :");
  Serial.println(stepper->LimitSwitch[0]->status());
}

//Option 6.
void Select(int bottle)
{
  if(bottle==0)
  {
    stepper->DefaultStepForward(BOTTLE_OFFSET);
  }
  else
  {
    stepper->DefaultStepForward(BOTTLE_OFFSET + (BOTTLE_SPACING * bottle));
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
        lcd->write("Option 0 selected");
        Reset();
      }
      else if (user_input =='1')
      {
        lcd->write("Option 1 selected");
         Calibrate();
      }
      else if(user_input =='2')
      {
        lcd->write("Option 2 selected");
        Return();
      }
      else if(user_input =='3')
      {
        lcd->write("Option 3 selected");
        Cycle();
      }
      else if(user_input =='4')
      {
        lcd->write("Option 4 selected");
        Dispense(random(1,3));
      }
      else if(user_input =='5')
      {
        lcd->write("Option 5 selected");
        Status();
      }
      else if(user_input =='6')
      {
         lcd->write("Option 6 selected");
         Select(random(0, NUM_BOTTLES));
         Dispense(random(1,3));
         Return();
      }
      else if(user_input =='7')
      {
        lcd->write("Option 7 selected");
        Mode(BLUETOOTH_MODE);
      }      
      else
      {
        Serial.println("Invalid option entered.");
      }

      Serial.println("Enter new option");
      Serial.println();

      lcd->write("Enter new option.   ");

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
          Dispense(val);
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
