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
        analogWrite(LEFT_PWN, DUTY_L);
        analogWrite(RIGHT_PWN, DUTY_R);
        break;
        
      //drive BACKWARDS
      case('b'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 255- DUTY_L);
        analogWrite(RIGHT_PWN, 255 - DUTY_R);
        break;
        
      //rotate on-spot RIGHT
      case('r'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, DUTY_L);
        analogWrite(RIGHT_PWN, 255 -DUTY_R);
        break;
        
      //rotate on-spot LEFT
      case('l'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, 255- DUTY_L);
        analogWrite(RIGHT_PWN, DUTY_R);
        break;

      //gradual turn LEFT
      case('L'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, DUTY_L);
        analogWrite(RIGHT_PWN, DUTY_R);
        break;

      //gradual turn RIGHT
      case('R'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, DUTY_L);
        analogWrite(RIGHT_PWN, DUTY_R);
        break;
  
      //stop
      default:
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, DUTY_L);
        analogWrite(RIGHT_PWN, DUTY_R);
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
  Serial.print("\nLast direction: ");
  Serial.print(mov, DEC);
}


//main runtime
void loop()
{
  //update IR sensor readings
  readIR();
  
  if(irl)
  {
    drive('L');
    while(irl || !irc)
    {
      readIR();
    }
  }
  
  else if (irr)
  {
    drive('f');
    //Delay 200ms to see if this is a corner or a T.
    unsigned long time= millis();
    while(irc || millis()<time+200)
    {
      readIR();
    }
    //A right hand corner was detected. 
    if(!irc)
    {
      drive('r');
      while(irr || !irc)
      {
        readIR();
      }
    }
  }

  else if (!irl && !irc && !irr)
  {

    drive('f');
    //ugly, go forward to see if we are at a T but missed
     unsigned long time= millis();
    while(!irl && !irc && !irr && millis() <= time + 250)
    {
      readIR();
    }
    //If any sensor has gone high, deal with it at the start of the loop, else do the full turn
    if(!irl && !irc && !irr)
    {
      drive('r');
      while (!irc)
      {
        readIR();
      }
    }
  }
  else
  {
    drive('f');
  }
}











