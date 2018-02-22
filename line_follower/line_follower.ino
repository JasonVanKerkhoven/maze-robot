//declaring constants
#define LEFT_PWN 10
#define LEFT_DIR 9
#define RIGHT_PWN 5
#define RIGHT_DIR 6
#define DUTY 130

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


//drive function
void drive(char dir)
{
  //only change bridge outputs if direction differs from current movement
  if (mov != dir)
  {
    //overwrite current movement and change bridge outputs
    mov = dir;
    switch(dir)
    {
      //move forward
      case('f'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, 255-DUTY);
        analogWrite(RIGHT_PWN, 255-DUTY);
        break;
        
      //move backwards
      case('b'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, DUTY);
        analogWrite(RIGHT_PWN, DUTY);
        break;
        
      //turn right
      case('r'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 255-DUTY);
        analogWrite(RIGHT_PWN, DUTY);
        break;
        
      //turn left
      case('l'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, DUTY);
        analogWrite(RIGHT_PWN, 255-DUTY);
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

/*
*     LR/C
*          0   1
*     00   F   T
*     01   F   F
*     11   F   F
*     10   F   F
*/
//main
void loop()
{
  testDrive();
  //update IR sensor readings
  readIR();
  
  //straight path found
  if (irc && !irl && !irr)
  {
    drive('f');
  }
  //end block found
  else if (irc && irl && irr)
  {
    drive('h');
  }
  // right turn found
  else if (!irl && irr)
  {
    drive('r');
  }
  else if (irl && !irr)
  {
    drive('l');
  }

  /*
  //deadend found
  else if (!irl && !irc && !irr)
  {
    //turn until path is found
    drive('r');
    while (!(irc && !irl && !irr))
    {
      readIR();
    }
  }
  //right turn found
  else if (!irl && irr)
  {
    //turn until path is found
    drive('r');
    while (!(irc && !irl && !irr))
    {
      readIR();
    }
  }
  */



  /*
  //end reached
  if (tapeLeft && tapeCenter && tapeRight)
  {
    drive('h');
  }
  //turn right
  else if (!tapeLeft && tapeRight)
  {
    drive('r');
  }
  //turn left
  else if (tapeLeft && !tapeRight)
  {
    drive('l');
  }
  
  //straight T
  else if (tapeLeft && !tapeCenter && tapeRight)
  {
    drive('r');
  }
  //right T |--
  else if (!tapeLeft && tapeCenter && tapeRight)
  {
    drive('r');
  }
  //left T --|
  else if (tapeLeft && tapeCenter && !tapeRight)
  {
    drive('f');
  }
  
  //forward
  else
  {
    drive('f');
  }
  */
}
