//declaring constants
#define LEFT_PWN 10
#define LEFT_DIR 9
#define RIGHT_PWN 5
#define RIGHT_DIR 6
#define DUTY 120

#define IR_RIGHT 7
#define IR_LEFT 8
#define IR_CENTER 11

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
}


//drive function
void drive(char dir)
{
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
      analogWrite(LEFT_PWN, DUTY);
      break;
      
    //turn right
    case('r'):
      digitalWrite(LEFT_DIR, HIGH);
      digitalWrite(RIGHT_DIR, LOW);
      analogWrite(LEFT_PWN, 255-DUTY);
      analogWrite(LEFT_PWN, DUTY);
      break;
      
    //turn left
    case('l'):
      digitalWrite(LEFT_DIR, LOW);
      digitalWrite(RIGHT_DIR, HIGH);
      analogWrite(LEFT_PWN, DUTY);
      analogWrite(LEFT_PWN, 255-DUTY);
      break;

    //stop
    default:
      digitalWrite(LEFT_DIR, LOW);
      digitalWrite(RIGHT_DIR, LOW);
      analogWrite(LEFT_PWN, 0);
      analogWrite(LEFT_PWN, 0);
      break;
  }
}


//main
void loop()
{
    //poll sensors (true for tape, false for no tape)
  bool tapeRight = (digitalRead(IR_RIGHT) == 0) ? false : true;
  bool tapeLeft = (digitalRead(IR_LEFT) == 0) ? false : true;
  bool tapeCenter = (digitalRead(IR_CENTER) == 0) ? false : true;

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
}
