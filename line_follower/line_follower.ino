//declaring constants
#define LEFT_PWN 10
#define LEFT_DIR 9
#define RIGHT_PWN 5
#define RIGHT_DIR 6

#define DUTY_R 140
#define DUTY_L 85

#define IR_RIGHT 7
#define IR_LEFT 8
#define IR_CENTER 11

//declaring global variables
char mov = 'h';
bool irr = false;
bool irl = false;
bool irc = false;




//init ports as I/O
void setup()
{
  //config serial for debug
  Serial.begin(9600);
  Serial.print("serial init\n");
  
  //set motor GPIO as output
  pinMode(LEFT_PWN , OUTPUT);
  pinMode(LEFT_DIR , OUTPUT);
  pinMode(RIGHT_PWN , OUTPUT);
  pinMode(RIGHT_DIR , OUTPUT);

  //set IR GPIO as inpurt
  pinMode(IR_RIGHT, INPUT);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);

  //set motors to known state
  drive('h');
}


/*
 * drive function
 * 
 * INPUT    MOTOR_R   MOTOR_L   EFFECT
 * f        fwd       rev       drive forward   
 * b        rev       rev       drive backwards
 * r        rev       fwd       rotate right, on-the-spot
 * l        fwd       rev       rotate left, on-the-spot
 * h        off       off       full stop
 * R        off       fwd       hang right while moving forward
 * L        fwd       off       hang left while moving forward
 * 
 */
void drive(char dir)
{
  //only change bridge outputs if direction differs from current movement
  if (mov != dir)
  {
    //overwrite current movement and change bridge outputs
    mov = dir;
    switch(dir)
    {
      //drive FORWARD
      case('f'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, 255-DUTY_L);
        analogWrite(RIGHT_PWN, 255-DUTY_R);
        break;
        
      //drive BACKWARDS
      case('b'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, DUTY_L);
        analogWrite(RIGHT_PWN, DUTY_R);
        break;

      //turn RIGHT
      case('R'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 255-DUTY_L);
        analogWrite(RIGHT_PWN, 0);

      //turn LEFT
      case('L'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, 0);
        analogWrite(RIGHT_PWN, 255-DUTY_R);
        
      //rotate on-spot RIGHT
      case('r'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 255-DUTY_L);
        analogWrite(RIGHT_PWN, DUTY_R);
        break;
        
      //rotate on-spot LEFT
      case('l'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, DUTY_L);
        analogWrite(RIGHT_PWN, 255-DUTY_R);
        break;
  
      //stop
      default:
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 0);
        analogWrite(RIGHT_PWN, 0);
        break;
    }
  }
}


//test motors
void testDrive()
{
  drive('h');
  delay(1000);
  drive('r');
  delay(1000);
  drive('l');
  delay(1000);
  drive('f');
  delay(1000);
  drive('b');
  delay(1000);
}


//update all IR sensors
void readIR()
{
  //poll sensors (true for tape, false for no tape)
  irr = (digitalRead(IR_RIGHT) == 0) ? false : true;
  irl = (digitalRead(IR_LEFT) == 0) ? false : true;
  irc = (digitalRead(IR_CENTER) == 0) ? false : true;
}


//print IR readings
void printCurrentIR()
{
  Serial.print("\n\nLEFT:   ");
  Serial.print(irl, DEC);
  Serial.print("\nCENTER: ");
  Serial.print(irc, DEC);
  Serial.print("\nRIGHT:  ");
  Serial.print(irr, DEC);
}


//main runtime
void loop()
{
  //update IR sensor readings
  readIR();
  
  //end block found
  if (mov == 'f' && irl && irc && irr)
  {
    //drive forward for 3 seconds to make sure the end is found
    unsigned long stamp = millis();
    drive('f');
    while (millis() <= stamp+3000)
    {
      //update sensor info
      readIR();
      
      //if sensors drop low during 3 seconds crawl break
      if (!irl || !irc || !irr)
      {
        drive('h');
        return;
      }
    }

    //final check at end of crawl if still on all-black
    if (irl && irc && irr)
    {
      while(true) {drive('h');}
    }
    else
    {
      drive('h');
      return;
    }
  }
  //right turn found
  else if (!irl && irr)
  {
    drive('r');
  }
  //left turn found
  else if (irl && !irr)
  {
    drive('l');
  }
  //otherwise forward
  else if (!irl && irc && !irr)
  {
    drive('f');
  }
  else
  {
    drive('b');
  }
}
